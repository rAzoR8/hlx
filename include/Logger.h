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

#ifndef HLX_LOGGER_H
#define HLX_LOGGER_H

#include "StandardDefines.h"
#include "Singleton.h"
#include "StringTypes.h"
#include "Timer.h"

#include <string>
#include <fstream>
#include <mutex>
#include <functional>
#include <iostream>

namespace hlx
{
	enum MessageType
	{
		kMessageType_Info = 0, // None
		kMessageType_Warning = 1,
		kMessageType_Error = 2,
		kMessageType_Fatal = 3
	};

	//template <typename char_t>
	//std::basic_ostream<char_t, std::char_traits<char_t>>& GetCout();

	//template<>
	//std::basic_ostream<char, std::char_traits<char>>& GetCout<char>() { return std::cout; }

	//template<>
	//std::basic_ostream<wchar_t, std::char_traits<wchar_t>>& GetCout<wchar_t>() { return std::wcout; }

#ifndef _WIN32

#ifndef HWND
#define HWND int*
#endif

#include <signal.h>

#endif

	class Logger : public hlx::Singleton<Logger>
	{
		typedef std::function<void(const wchar_t* _pLogMsg, const MessageType _kErrorLevel)> TLogCallback;
	public:
		Logger(const MessageType _kErrorLevel = kMessageType_Info) noexcept;
		~Logger();

		void WriteToStream(std::wostream* _Stream);
		void WriteToFile(const wchar_t* pFilePath);
		void WriteToAdditionalConsole();

		//returns 0 when error level is below HERROR_FATAL, or IDABORT, IDIGNORE, IDRETRY depending on the MessageBox result (if ShowMessageBoxOnFatal is set)
		template <typename ...Args>
		int32_t Log(const MessageType _kErrorLevel, const wchar_t* pFuncName, const wchar_t* pSourceFile, uint32_t uLineNum, const wchar_t* pFormat, Args&& ...args);

		inline void SetLogLevel(const MessageType _kErrorLevel) noexcept { m_kLogLevel = _kErrorLevel; }
		inline void SetLogToFile(bool _bLogToFile) noexcept { m_bLogToFile = _bLogToFile; }
		inline void SetLogToStream(bool _bLogToStream) noexcept { m_bLogToStream = _bLogToStream; }
		inline void SetLogToOutputConsole(bool _bLogToOutputConsole) noexcept { m_bLogToOutputConsole = _bLogToOutputConsole; }
		inline void SetShowMessageBoxOnFatal(bool _bShowMessageBoxOnFatal) noexcept { m_bShowMessageBoxOnFatal = _bShowMessageBoxOnFatal; }

		inline void SetHWND(const HWND& _HWND) noexcept
		{
			#ifdef WIN32
			m_HWND = _HWND;
			#endif
		}

		inline void SetLogCallback(const TLogCallback& _LogCallback) { std::lock_guard<std::mutex> Lock(m_Mutex); m_LogFunction = _LogCallback; }
		inline void SetSuppressLoggerStartedStopped(bool _bSuppress) noexcept { m_bSuppressFirstLastLog = _bSuppress; }

	private:
		bool m_bSuppressFirstLastLog = false;

		bool m_bFirstLog = true;
		bool m_bLastLog = false;

		bool m_bLogToFile = false;
		bool m_bLogToStream = false;
		bool m_bLogToOutputConsole = true;
		bool m_bShowMessageBoxOnFatal = false;

		std::basic_fstream<wchar_t> m_FileStream;

#ifdef _WIN32
		HWND m_HWND = nullptr;
		FILE* m_pConsoleOut = nullptr; // Console Window output
#endif

		TLogCallback m_LogFunction;
		MessageType m_kLogLevel;
		std::mutex m_Mutex;
		std::wostream* m_Stream = nullptr;
	};

	// Implementation
	//---------------------------------------------------------------------------------------------------

	inline Logger::Logger(const MessageType _kErrorLevel) noexcept
	{
		//log all messages above or equal to HERROR_Info level
		m_kLogLevel = _kErrorLevel;

		m_bLogToOutputConsole = true;
		m_bLogToFile = false;
		m_bLogToStream = false;
		m_bShowMessageBoxOnFatal = false;

		m_bFirstLog = true;

		m_Stream = nullptr;
	}

	//---------------------------------------------------------------------------------------------------

	inline Logger::~Logger()
	{
		if (m_bSuppressFirstLastLog == false && m_bFirstLog == false)
		{
			// avoid call to a deleted function
			SetLogCallback(nullptr);
			Log(kMessageType_Info, nullptr, nullptr, 0u, L"========== LOGGER STOPPED ==========");
		}

		std::unique_lock<std::mutex> Lock(m_Mutex);
		if (m_FileStream.is_open())
		{
			m_FileStream.close();
		}

#ifdef _WIN32
		if (m_pConsoleOut != nullptr)
		{
			fclose(m_pConsoleOut);
			FreeConsole();
		}
#endif
	}

	//---------------------------------------------------------------------------------------------------

	inline void Logger::WriteToFile(const wchar_t* pFilePath)
	{
		std::lock_guard<std::mutex> Lock(m_Mutex);
		m_bLogToFile = true;

		if (m_FileStream.is_open() == false)
		{
			m_FileStream.open(pFilePath, std::ios_base::out | std::ios_base::trunc);
		}
	}

	//---------------------------------------------------------------------------------------------------

	inline void Logger::WriteToStream(std::wostream* _Stream)
	{
		std::lock_guard<std::mutex> Lock(m_Mutex);
		m_bLogToStream = true;
		m_Stream = _Stream;
	}

	//---------------------------------------------------------------------------------------------------

	inline void Logger::WriteToAdditionalConsole()
	{
#ifdef _WIN32
		std::lock_guard<std::mutex> Lock(m_Mutex);

		if (m_pConsoleOut != nullptr)
		{
			fclose(m_pConsoleOut);
			FreeConsole();
		}

		AllocConsole();

		freopen_s(&m_pConsoleOut, "CONOUT$", "w", stdout);

		m_bLogToStream = true;
		m_Stream = &std::wcout;
#endif
	}

	//---------------------------------------------------------------------------------------------------
	template<typename ...Args>
	inline int32_t Logger::Log(const MessageType _kErrorLevel, const wchar_t* pFuncName, const wchar_t* pSourceFile, uint32_t uLineNum, const wchar_t* pFormat, Args&& ...args)
	{
		//if this is the first time Log(...) was called print logger started msg
		if (m_bFirstLog && m_bSuppressFirstLastLog == false)
		{
			m_bFirstLog = false;
			Log(kMessageType_Info, nullptr, nullptr, 0, L"========== LOGGER STARTED ==========");
		}

		//only log message if type level is equal or above m_logLevel (HERROR_NONE is default log level)
		if (_kErrorLevel < m_kLogLevel)
			return 0;

		//Error Type to print
		std::wstring sErrorType;

		switch (_kErrorLevel)
		{
			//case HERROR_NONE: nothing to print
		case kMessageType_Warning:
			sErrorType = L"[WARNING]";
			break;
		case kMessageType_Error:
			sErrorType = L"[ERROR]";
			break;
		case kMessageType_Fatal:
			sErrorType = L"[FATAL]";
			break;
		}

		const int iSize = std::swprintf(nullptr, 0, pFormat, std::forward<Args>(args)...);
		std::wstring sUserMsg(iSize + 1, L'\0');
		std::swprintf(&sUserMsg[0], sUserMsg.size(), pFormat, std::forward<Args>(args)...);

		std::wstring sOutputMsg(Timer::GetLocalTimeString<wchar_t>());

		if (_kErrorLevel > kMessageType_Info)
		{
			//time errType MSG dbgName funcName sourceFile lineNum
			sOutputMsg += L" " + sErrorType + L" " + sUserMsg + L" in " + HNULLSTRW(pFuncName) + L" " + HNULLSTRW(pSourceFile) + L":" + std::to_wstring(uLineNum) + L"\n";
		}
		else
		{
			//(time) MSG
			sOutputMsg += L" " + sUserMsg + L"\n";
		}

		//Enter critical section - threadsafety of buffer and stream
		std::lock_guard<std::mutex> Lock(m_Mutex);

		//Print on debug console if debugger is present
		if (m_bLogToOutputConsole)
		{
#ifdef _WIN32
			OutputDebugStringW(sOutputMsg.c_str());
			OutputDebugStringW(L"\n");
#endif
		}

		// call log function
		if (m_LogFunction)
		{
			m_LogFunction(sOutputMsg.c_str(), _kErrorLevel);
		}

		//Log to file
		if (m_bLogToFile && m_FileStream.is_open() && m_FileStream.good())
		{
			m_FileStream << sOutputMsg;
		}

		//Log to output string
		if (m_bLogToStream && m_Stream != nullptr)
		{
			*m_Stream << sOutputMsg;
		}

		//Only display MessageBox on fatal errors - close game
		if (_kErrorLevel == kMessageType_Fatal)
		{
#ifdef _WIN32
			if (m_bShowMessageBoxOnFatal)
			{
				const int iResult = MessageBoxW(m_HWND, sOutputMsg.c_str(), sErrorType.c_str(), MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_DEFBUTTON3);
				switch (iResult)
				{
				case IDABORT: DebugBreak(); break;  // break into the debugger
				case IDIGNORE:  break;
				case IDRETRY:	break;
				default:		break;
				}

				return iResult;
			}
			else
			{
				DebugBreak();
			}
#else // unix
			raise(SIGTRAP); // __builtin_trap()
#endif
		}

		return 0;
	}

	// Macro definitions
	//---------------------------------------------------------------------------------------------------

#define HLOG(format,...) hlx::Logger::Instance()->Log(hlx::kMessageType_Info, nullptr, nullptr, 0 ,SW(format),__VA_ARGS__)
#define HWARNING(format,...) hlx::Logger::Instance()->Log(hlx::kMessageType_Warning, WFUNC, WFILE, __LINE__, SW(format),__VA_ARGS__)
#define HERROR(format,...) hlx::Logger::Instance()->Log(hlx::kMessageType_Error, WFUNC, WFILE, __LINE__, SW(format),__VA_ARGS__)
#define HFATAL(format,...) hlx::Logger::Instance()->Log(hlx::kMessageType_Fatal, WFUNC, WFILE, __LINE__, SW(format),__VA_ARGS__)
#define HASSERT(predicate, format, ...) {if ((predicate) == false) HFATAL(format, __VA_ARGS__);}

#ifdef _DEBUG
//Only log when in debug build
#define HLOGD(format,...) HLOG(format,__VA_ARGS__)
#define HWARNINGD(format,...) HWARNING(format,__VA_ARGS__)
#define HERRORD(format,...) HERROR(format,__VA_ARGS__)
#define HFATALD(format,...) HFATAL(format,__VA_ARGS__)

#define HASSERTD(predicate, format, ...) {if ((predicate) == false) HFATALD(format, __VA_ARGS__);}

#else //Release

#define HLOGD(format,...)
#define HWARNINGD(format,...)
#define HERRORD(format,...)
#define HFATALD(format,...)

#define HASSERTD(predicate, format, ...)

#endif //!_DEBUG

	//---------------------------------------------------------------------------------------------------
	// Breaks on error -> print formated message

#ifndef HR

#ifdef _WIN32
#define HR(x) \
	{\
		HRESULT res = (x); \
		if(FAILED(res)){\
			LPWSTR msg; \
			if(FormatMessageW((FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS), NULL, res, 0, (LPWSTR)&msg, 0, NULL) != 0){ \
				HFATAL("%s in %s", msg, SW(#x));\
				LocalFree(msg);\
			}\
		}\
	}

#else // unix
#define HR(x) { HRESULT res = (x); if(FAILED(res)){HFATAL("API-Call %s failed with code %X", SW(#x), res);}}
#endif //!WIN32

#endif // !HR

	//---------------------------------------------------------------------------------------------------

#ifndef HRWARNING

#ifdef _WIN32
#define HRWARNING(x) \
	{\
		HRESULT res = (x); \
		if(FAILED(res)){\
			LPWSTR msg; \
			if(FormatMessageW((FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS), NULL, res, 0, (LPWSTR)&msg, 0, NULL) != 0){ \
				HWARNING("%s in %s", msg, SW(#x));\
				LocalFree(msg);\
			}\
		}\
	}
#else // unix
#define HRWARNING(x) { HRESULT res = (x); if(FAILED(res)){HWARNING("API-Call %s failed with code %X", SW(#x), res);}}
#endif

#endif // !HRWARNING

	//---------------------------------------------------------------------------------------------------

#ifndef HRERROR
#ifdef _WIN32
#define HRERROR(x) \
	{\
		HRESULT res = (x); \
		if(FAILED(res)){\
			LPWSTR msg; \
			if(FormatMessageW((FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS), NULL, res, 0, (LPWSTR)&msg, 0, NULL) != 0){ \
				HERROR("%s in %s", msg, SW(#x));\
				LocalFree(msg);\
			}\
		}\
	}
#else // unix
#define HRERROR(x) { HRESULT res = (x); if(FAILED(res)){HERROR("API-Call %s failed with code %X", SW(#x), res);}}
#endif //!WIN32

#endif // !HRERROR

	//---------------------------------------------------------------------------------------------------

#ifndef HR2
// Displays a error
#define HR2(x) HRERROR(x)
#endif

#ifndef HRIF
#define HRIF(x) HRERROR(x) if(FAILED(x)) 
#endif

}; // hlx

#endif //HLX_LOGGER_H
