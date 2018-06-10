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

#ifndef HLX_IPLUGIN_H
#define HLX_IPLUGIN_H

#include "StandardDefines.h"
#include <string>
#include <boost\dll\alias.hpp>

namespace hlx
{
	static constexpr uint32_t kPluginInterfaceVersion = 1u;

	class HABI IPlugin
	{
	public:
		IPlugin() {};
		virtual ~IPlugin() {};

		virtual uint32_t GetInterfaceVersion() const = 0;
		virtual uint32_t GetPluginVersion() const = 0;
		virtual std::wstring GetIdentifier() const = 0;
		virtual void Release() = 0;
	};

#define GETPLUGIN_ALIAS get_hlx_plugin
#define GETPLUGIN_ALIASNAME "get_hlx_plugin"

#define PLUGIN_INTERFACE(_type, _version)	static _type* GetPlugin(){ return new _type();} \
											uint32_t GetInterfaceVersion() const final { return hlx::kPluginInterfaceVersion; } \
											uint32_t GetPluginVersion() const final { return _version;} \
											std::wstring GetIdentifier() const final {return L#_type;}

	// GetPlugin with complete namespace: Tracy::PluginType::GetPlugin
#define PLUGIN_ALIAS(_funcname) BOOST_DLL_ALIAS(_funcname, GETPLUGIN_ALIAS);

} // hlx

#endif // !HLX_IPLUGIN_H
