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

#ifndef HLX_STRING_H
#define HLX_STRING_H

#include <locale>
#include <codecvt>
#include <string>
#include <sstream>

namespace hlx
{
	inline std::string to_sstring(const char* _pStr)
	{
		return std::string(_pStr);
	}

	inline const std::string& to_sstring(const std::string& _sStr) noexcept
	{
		return _sStr;
	}

	inline std::string to_sstring(const std::wstring& _sWStr)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> Conv;
		return Conv.to_bytes(_sWStr);
	}

	inline std::string to_sstring(const wchar_t* _pStr)
	{
		return to_sstring(std::wstring(_pStr));
	}

	inline std::wstring to_wstring(const std::string& _UTF8Str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> Conv;
		return Conv.from_bytes(_UTF8Str);
	}

	inline const std::wstring& to_wstring(const std::wstring& _sStr) noexcept
	{
		return _sStr;
	}

	inline std::wstring to_wstring(const wchar_t* _pStr)
	{
		return std::wstring(_pStr);
	}

	inline std::wstring to_wstring(const char* _pStr)
	{
		return to_wstring(std::string(_pStr));
	}

	template <class char_t>
	const char_t* select_type(const char* _pStr, const wchar_t* _pWStr);

	template<>
	inline constexpr const char* select_type<char>(const char* _pStr, const wchar_t* _pWStr)
	{
		return _pStr;
	}

	template<>
	inline constexpr const wchar_t* select_type<wchar_t>(const char* _pStr, const wchar_t* _pWStr)
	{
		return _pWStr;
	}

#ifdef UNICODE // wchar_t
	static constexpr bool is_unicode = true;
	using base_char_t = wchar_t;
	using stringstream = std::wstringstream;

#ifndef S
#define S(str) L##str
#endif

	using string = std::wstring;

	inline string to_string(const char* _sUTF8Str)
	{
		return to_wstring(std::string(_sUTF8Str));
	}

	inline string to_string(const wchar_t* _pStr)
	{
		return std::wstring(_pStr);
	}

	inline string to_string(const std::string& _sUTF8Str)
	{
		return to_wstring(_sUTF8Str);
	}

	inline const string& to_string(const std::wstring& _sWString)
	{
		return _sWString;
	}
#else // char

#ifndef S
#define S(str) str
#endif

	static constexpr bool is_unicode = false;
	using base_char_t = char;
	using stringstream = std::stringstream;
	using string = std::string;

	inline string to_string(const char* _pStr)
	{
		return std::string(_pStr);
	}

	inline string to_string(const wchar_t* _pStr)
	{
		return to_sstring(_pStr);
	}

	inline string to_string(const std::wstring& _sWString)
	{
		return to_sstring(_sWString);
	}

	inline const string& to_string(const std::string& _sUTF8Str) noexcept
	{
		return _sUTF8Str;
	}
	
#endif
} // HLX

#ifndef STR
#define STR(_str) hlx::to_string(_str)
#endif

#ifndef CSTR
#define CSTR(_str) hlx::to_string(_str).c_str()
#endif

#ifndef SCSTR
#define SSCTR(_str) hlx::to_sstring(_str).c_str()
#endif

#ifndef WCSTR
#define WCSTR(_str) hlx::to_wstring(_str).c_str()
#endif

  // usage: tempalte<typename char_t> functOrStructDef() ... ST(char_t, "yourText") => returns either "yourText" or L"yourText" based on the template argument
#ifndef ST
#define ST(_type, _str) hlx::select_type<_type>(_str, L##_str)
#endif

#ifndef SW
#define SW(str) L##str
#endif

#ifndef SW2
#define SW2(x) SW(x)
#endif

#ifndef WFILE
#define WFILE SW2(__FILE__)
#endif

#ifndef WFUNC
#define WFUNC SW2(__FUNCTION__)
#endif

#endif // HLX_STRING_H
