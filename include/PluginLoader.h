//Copyright(c) 2018
//Authors: Fabian Wahlster
//Website: https://twitter.com/singul4rity
//Contact: f.wahlster@tum.de

//////////////////////////////////////////////////////////////////////////////
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute copies of the Software, and
//to permit persons to whom the Software is furnished to do so, subject to the
//following conditions:
//
//- The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//- Naming the author(s) of this software in any of the following locations:
//	About page, README file, credits.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.
//////////////////////////////////////////////////////////////////////////////

#ifndef HLX_PLUGINLOADER_H
#define HLX_PLUGINLOADER_H

#ifdef STANDARD_DLL
#include <dll>
namespace dll = std::dll;
#else
#include <boost/dll.hpp>
namespace dll = boost::dll;
#endif

#ifdef STANDARD_FS
namespace filesystem = std::experimental::filesystem;
#else
namespace filesystem = boost::filesystem;
#endif

#include "IPlugin.h"
#include "Logger.h"
#include <unordered_map>

namespace hlx
{
	//forward delc
	template <class TPlugin>
	class PluginLoader;

	template <class TPlugin>
	class IPluginConsumer
	{
		using TLoader = PluginLoader<TPlugin>;
		friend class TLoader;

	public:
		IPluginConsumer(const filesystem::path& _Path) : m_LibPath(_Path)
		{
			m_pPlugin = TLoader::Instance().AddConsumer(this);
		};
		virtual ~IPluginConsumer() { TLoader::Instance().RemoveConsumer(this); };

		const filesystem::path& GetLibPath() const { return m_LibPath; }
		bool HasValidPlugin() const { return m_pPlugin != nullptr; }

	protected:
		// loaded or reloaded
		virtual void OnPluginReloaded(TPlugin* _pPlugin) {}
		virtual void OnPluginUnloaded() {}

		TPlugin* GetPlugin() const { return m_pPlugin; }

	private:
		const filesystem::path m_LibPath;
		TPlugin* m_pPlugin = nullptr;
	};
	//---------------------------------------------------------------------------------------------------

	template <class TPlugin>
	class PluginLoader
	{
	public:
		using TConsumer = IPluginConsumer<TPlugin>;
		friend class TConsumer;

		static PluginLoader& Instance()
		{
			static PluginLoader Loader;
			return Loader;
		}

		typedef TPlugin*(get_plugin_func)();
		using TPluginFunc = get_plugin_func;

		struct DynamicLib
		{
			DynamicLib() {}
			~DynamicLib();

			dll::shared_library Lib;
			TPlugin* pPlugin = nullptr;
			std::vector<TConsumer*> Consumers;
			filesystem::path Path;

			bool Load(const filesystem::path& _Path);
			void Unload(const bool _bKeepConsumers);
		};

		// plugin path -> libs
		using TLibMap = std::unordered_map<std::wstring, DynamicLib>;

		PluginLoader() {};
		virtual ~PluginLoader();

		// or reload
		TPlugin* Load(const filesystem::path& _Path, bool _bReload = false);

		// does not remove the lib!
		void Unload(const filesystem::path& _Path, const bool _bKeepConsumers);

	private:
		TPlugin* AddConsumer(TConsumer* _pConsumer);
		void RemoveConsumer(TConsumer* _pConsumer);

	private:

		// lib name -> dll
		TLibMap m_Plugins;
	};

	//---------------------------------------------------------------------------------------------------
	// DynamicLib
	//---------------------------------------------------------------------------------------------------

#pragma region DynamicLib
	template<class TPlugin>
	inline PluginLoader<TPlugin>::DynamicLib::~DynamicLib()
	{
		Unload(false);
	}
	//---------------------------------------------------------------------------------------------------

	template<class TPlugin>
	inline bool PluginLoader<TPlugin>::DynamicLib::Load(const filesystem::path& _Path)
	{
		Unload(true);

		Path = _Path;

		try
		{
			Lib.load(Path, dll::load_mode::append_decorations);

			if (Lib.is_loaded() == false)
				return false;

			if (Lib.has(GETPLUGIN_ALIASNAME) == false)
			{
				HERROR("symbol [%s] not found in %s", GETPLUGIN_ALIASNAME, Path.c_str());
				// no such symbol
				return false;
			}

			pPlugin = Lib.get_alias<get_plugin_func>(GETPLUGIN_ALIASNAME)();

			if (pPlugin != nullptr)
			{
				const std::wstring sPluginName = pPlugin->GetIdentifier();
				const uint32_t uInterFaceVersion = pPlugin->GetInterfaceVersion();
				const uint32_t uPluginVersion = pPlugin->GetPluginVersion();

				if (uInterFaceVersion != kPluginInterfaceVersion)
				{
					HERROR("Invalid plugin interface version %s reports %u but loaded expected %u", sPluginName.c_str(), uInterFaceVersion, kPluginInterfaceVersion);
					Unload(false);
					return false;
				}

				HLOG("Loaded %s::%s Version %u [Interface version %u]", Path.c_str(), sPluginName.c_str(), uPluginVersion, uInterFaceVersion);

				for (TConsumer* pConsumer : Consumers)
				{
					pConsumer->OnPluginReloaded(pPlugin);
				}
			}
			else
			{
				HERROR("Failed to create plugin from library %s", Path.c_str());
			}
		}
		catch (const boost::system::system_error& e)
		{
			HERROR("%s: %s", Path.c_str(), WCSTR(e.what()));
			return false;
		}

		return pPlugin != nullptr;
	}
	//---------------------------------------------------------------------------------------------------

	template<class TPlugin>
	inline void PluginLoader<TPlugin>::DynamicLib::Unload(const bool _bKeepConsumers)
	{
		for (TConsumer* pConsumer : Consumers)
		{
			pConsumer->OnPluginUnloaded();
		}

		if (_bKeepConsumers == false)
		{
			Consumers.clear();
		}

		HSAFE_RELEASE(pPlugin);

		while (Lib.is_loaded())
		{
			Lib.unload();
		}
	}
#pragma endregion

	//---------------------------------------------------------------------------------------------------
	// PluginLoader
	//---------------------------------------------------------------------------------------------------

	template<class TPlugin>
	inline PluginLoader<TPlugin>::~PluginLoader()
	{
		m_Plugins.clear();
	}
	//---------------------------------------------------------------------------------------------------

	template<class TPlugin>
	inline TPlugin* PluginLoader<TPlugin>::Load(const filesystem::path& _Path, bool _bReload)
	{
		std::wstring sPath(_Path.c_str());
		auto it = m_Plugins.find(sPath);

		if (it != m_Plugins.end())
		{
			if (_bReload)
			{
				it->second.Load(_Path);
			}
	
			return it->second.pPlugin;
		}
		else 
		{
			it = m_Plugins.emplace(sPath, DynamicLib{}).first;
			if (it->second.Load(_Path))
			{
				return it->second.pPlugin;
			}
			else
			{
				m_Plugins.erase(it);
			}
		}

		return nullptr;
	}
	//---------------------------------------------------------------------------------------------------

	template<class TPlugin>
	inline void PluginLoader<TPlugin>::Unload(const filesystem::path& _Path, const bool _bKeepConsumers)
	{
		std::wstring sPath(_pConsumer->GetLibPath().c_str());
		auto it = m_Plugins.find(sPath);

		if (it != m_Plugins.end())
		{
			it->second.Unload(_bKeepConsumers);
		}
	}
	//---------------------------------------------------------------------------------------------------

	template<class TPlugin>
	inline TPlugin* PluginLoader<TPlugin>::AddConsumer(TConsumer* _pConsumer)
	{
		std::wstring sPath(_pConsumer->GetLibPath().c_str());
		auto it = m_Plugins.find(sPath);

		if (it != m_Plugins.end())
		{			
			it->second.Consumers.push_back(_pConsumer);
			return it->second.pPlugin;
		}
		else
		{
			return Load(_pConsumer->GetLibPath(), false);
		}
	}
	//---------------------------------------------------------------------------------------------------

	template<class TPlugin>
	inline void PluginLoader<TPlugin>::RemoveConsumer(TConsumer* _pConsumer)
	{
		std::wstring sPath(_pConsumer->GetLibPath().c_str());
		auto it = m_Plugins.find(sPath);

		if (it != m_Plugins.end())
		{
			RemoveIfValue(it->second.Consumers, _pConsumer);
		}
	}

	//---------------------------------------------------------------------------------------------------

} // hlx

#endif // !HLX_PLUGINLOADER_H
