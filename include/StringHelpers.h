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

#ifndef HLX_STRINGHELPERS_H
#define HLX_STRINGHELPERS_H

#include "ByteStream.h"
#include "StringTypes.h"
#include <vector>
#include <algorithm>
#include <locale>

namespace hlx
{
#pragma region find_first_of
    template <typename char_t = base_char_t>
    inline size_t find_first_of(const std::basic_string<char_t>& _sInput, const std::vector<std::basic_string<char_t>>& _sFind, _Out_ size_t& _uIndex, const size_t& _uPos = 0u)
    {
        size_t uFirst = std::string::npos;
        size_t uCur = std::string::npos;

        const size_t uSize = _sFind.size();
        for (size_t i = 0u; i < uSize; ++i)
        {
            uCur = _sInput.find(_sFind.at(i), _uPos);
            if (uCur < uFirst)
            {
                uFirst = uCur;
                _uIndex = i;
            }
        }

        return uFirst;
    }
#pragma endregion


#pragma region to_lower
    template <typename char_t = base_char_t>
    inline char_t to_lower(const char_t c)
    {
        return ::tolower(c);
    }

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> to_lower(const std::basic_string<char_t>& str)
	{
		std::basic_string<char_t> out = str;
		std::transform(out.begin(), out.end(), out.begin(), ::tolower);
		return out;
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> to_lower(const char_t* str)
	{
		return to_lower(std::basic_string<char_t>(str));
	}

	template <typename char_t = base_char_t>
	inline void to_lower(std::basic_string<char_t>& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	}
#pragma endregion

#pragma region to_upper
    template <typename char_t = base_char_t>
    inline char_t to_upper(const char_t c)
    {
        return ::toupper(c);
    }

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> to_upper(const std::basic_string<char_t>& str)
	{
		std::basic_string<char_t> out(str);
		std::transform(out.begin(), out.end(), out.begin(), ::toupper);
		return out;
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> to_upper(const char_t* str)
	{
		return to_upper(std::basic_string<char_t>(str));
	}

	template <typename char_t = base_char_t>
	inline void to_upper(std::basic_string<char_t>& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	}
#pragma endregion

#pragma region trim
	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> trim_right(const std::basic_string<char_t>& str)
	{
		int64_t iPos = str.size() - 1;
		for (; iPos > 0 && std::isspace(str[size_t(iPos)], std::locale::classic()); --iPos) {};
		return str.substr(0u, size_t(iPos+1));
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> trim_left(const std::basic_string<char_t>& str)
	{
		size_t uPos = 0;
		for (; uPos < str.size() && std::isspace(str[uPos], std::locale::classic()); ++uPos) {};
		return str.substr(uPos);
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> trim(const std::basic_string<char_t>& str)
	{
		return trim_left(trim_right(str));
	}
#pragma endregion

#pragma region from_string
	template <typename T, typename char_t = base_char_t>
	T from_string(const std::basic_string<char_t>& str)
	{
		T ret{};
		std::basic_stringstream<char_t> stream(str, std::ios_base::in);
		stream >> ret;
		return ret;
	}
#pragma endregion

#pragma region skip_until
	template<typename T, typename stream_t = const bytes>
	inline void skip_until(basestream<stream_t>& stream, const T& b)
	{
		while (stream.good())
		{
			if (stream.get<T>() == b) break;
		}
	}
#pragma endregion
	
#pragma region read_until
	template <typename char_t = base_char_t, typename stream_t = const bytes>
	inline std::basic_string<char_t> read_until(basestream<stream_t>& stream, char_t separator)
	{		
		char_t c = {};
		std::basic_string<char_t> ret;
		while (stream.good() && (c = stream.get<char_t>()) != separator) { ret += c; }
		return ret;
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> read_until(std::basic_stringstream<char_t>& stream, char_t separator)
	{
		char_t c = {};
		std::basic_string<char_t> ret;
		while (stream.good() && (c = stream.get()) != separator) { ret += c; }
		return ret;
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> read_until(std::basic_stringstream<char_t>& stream, const std::basic_string<char_t>& delimiter)
	{
		std::basic_string<char_t> sOut;
		if (delimiter.empty())
		{
			return sOut;
		}
		
		size_t uAlloc = delimiter.size();
		sOut.resize(delimiter.size());
		stream.read(&sOut[0], uAlloc);

		size_t uOff = 0;

		while (sOut.find(delimiter, uOff) == std::string::npos && stream.good())
		{
			uOff = sOut.size();

			sOut.resize(uOff + uAlloc);

			stream.read(&sOut[uOff], uAlloc);

			if (uOff > 0)
			{
				--uOff;
			}
		}

		return sOut;
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> read_until(std::basic_stringstream<char_t>& stream, const char_t* delimiter)
	{
		return read_until(stream, std::basic_string<char_t>(delimiter));
	}
#pragma endregion
	
#pragma region split
	template <typename char_t = base_char_t>
	inline std::vector<std::basic_string<char_t>> split(const std::basic_string<char_t>& str, char_t separator, const size_t offset = 0u)
	{
		std::vector<std::basic_string<char_t>> tokens;
		std::basic_stringstream<char_t> stream(str);
        stream.seekg(offset);

		std::basic_string<char_t> token;
		while (std::getline(stream, token, separator))
		{
			tokens.push_back(token);
		}

		return tokens;
	}

	template <typename char_t = base_char_t>
	inline std::vector<std::basic_string<char_t>> split(const char_t* str, char_t separator, const size_t offset = 0u)
	{
		return split(std::basic_string<char_t>(str), offset);
	}

	template <typename char_t = base_char_t>
	inline std::vector<std::basic_string<char_t>> split(const std::basic_string<char_t>& str, const std::basic_string<char_t>& separator, const size_t offset = 0u, const size_t end = std::string::npos)
	{
		std::vector<std::basic_string<char_t>> tokens;

		size_t uPos = offset;
		size_t uNext = str.find(separator, offset);

		while (uNext >= uPos)
		{
            if (uNext > uPos && uNext <= end)
			    tokens.push_back(str.substr(uPos, uNext - uPos));

            if (uNext >= end)
                break;

			uPos = uNext + separator.size();
			uNext = str.find(separator, uPos);
		}

		return tokens;
	}

    template <typename char_t = base_char_t>
    inline std::vector<std::basic_string<char_t>> split(const std::basic_string<char_t>& str, const std::vector<std::basic_string<char_t>>& separators, const size_t offset = 0u, const size_t end = std::string::npos)
    {
        std::vector<std::basic_string<char_t>> tokens;

        size_t uSeparatorIndex = 0;
        size_t uPos = offset;
        size_t uNext = find_first_of(str, separators, uSeparatorIndex, uPos);

        while (uNext >= uPos)
        {
            if(uNext > uPos && uNext <= end)
                tokens.push_back(str.substr(uPos, uNext - uPos));

            if (uNext >= end)
                break;

            uPos = uNext + separators[uSeparatorIndex].size();
            uNext = find_first_of(str, separators, uSeparatorIndex, uPos);
        }

        return tokens;
    }

	template <typename char_t = base_char_t>
	inline std::vector<std::basic_string<char_t>> split(const std::basic_string<char_t>& str, const char_t* separator, const size_t offset = 0u, const size_t end = std::string::npos)
	{
		return split(str, std::basic_string<char_t>(separator), offset, end);
	}
#pragma endregion


#pragma region get_body
	// set _uStart = 0 to search from the beginning of the string
	template <typename char_t = base_char_t>
	inline bool get_body(const std::basic_string<char_t>& _sInput, _Inout_ size_t& _uStart, _Out_ size_t& _uEnd, const std::basic_string<char_t>& _sOpen, const std::basic_string<char_t>& _sClose)
	{
		const size_t uMinOpen = _sInput.find(_sOpen, _uStart);

		if (uMinOpen == std::string::npos)
		{
			_uStart = std::string::npos;
			_uEnd = std::string::npos;
			return false;
		}

        size_t uMaxClose = uMinOpen;

        if (_sOpen == _sClose)
        {
            uMaxClose = _sInput.find(_sClose, uMinOpen + _sClose.size());
        }
        else
        {
            const std::vector<std::basic_string<char_t>> Tags = { _sOpen, _sClose };

            size_t uCount = 1u;

            for (size_t uCurPos = uMinOpen, uIndex = 0u;
                uCount > 0u && uCurPos != std::string::npos;)
            {
                uCurPos = find_first_of<char_t>(_sInput, Tags, uIndex, uCurPos + 1);

                if (uCurPos == std::string::npos)
                    break;

                if (uIndex == 0u) // open tag
                {
                    ++uCount;
                }
                else if (uIndex == 1u) // close tag
                {
                    uMaxClose = uCurPos;
                    --uCount;
                }
            }
        }

		_uStart = uMinOpen;
		_uEnd = uMaxClose;

		return uMinOpen != std::string::npos && uMaxClose != std::string::npos;
	}

	template <typename char_t = base_char_t>
	inline bool get_body(const std::basic_string<char_t>& _sInput, _Inout_ size_t& _uStart, _Out_ size_t& _uEnd, const char_t* _sOpen = ST(char_t, "{"), const char_t* _sClose = ST(char_t,"}"))
	{
		return get_body(_sInput, _uStart, _uEnd, std::basic_string<char_t>(_sOpen), std::basic_string<char_t>(_sClose));
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> get_body(const std::basic_string<char_t>& _sInput, const std::basic_string<char_t>& _sOpen, const std::basic_string<char_t>& _sClose)
	{
		size_t uStart = 0;
		size_t uEnd = 0;

		if (get_body(_sInput, uStart, uEnd, _sOpen, _sClose))
		{
			return _sInput.substr(uStart + _sOpen.size(), uEnd - uStart - _sOpen.size());
		}
		
		return std::basic_string<char_t>();
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> get_body(const std::basic_string<char_t>& _sInput, const char_t* _sOpen = ST(char_t, "{"), const char_t* _sClose = ST(char_t, "}"))
	{
		return get_body(_sInput, std::basic_string<char_t>(_sOpen), std::basic_string<char_t>(_sClose));
	}
#pragma endregion
	
#pragma region get_right_of
	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> get_right_of(const std::basic_string<char_t>& str, const std::basic_string<char_t>& find, bool _bCaseSensitive = true)
	{
		size_t uPos = _bCaseSensitive ? str.rfind(find) : to_lower(str).rfind(to_lower(find));
		return str.substr(uPos != std::string::npos ? uPos + find.length() : 0u, std::string::npos);
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> get_right_of(const std::basic_string<char_t>& str, const char_t* find, bool _bCaseSensitive = true)
	{
		return get_right_of(str, std::basic_string<char_t>(find), _bCaseSensitive);
	}
#pragma endregion
	
#pragma region get_left_of
	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> get_left_of(const std::basic_string<char_t>& str, const std::basic_string<char_t>& find, bool _bCaseSensitive = true, bool _bReverse = false)
	{
		if (_bCaseSensitive)
		{
			return str.substr(0, _bReverse ? str.rfind(find) : str.find(find));
		}
		else
		{
			return str.substr(0, _bReverse ? to_lower(str).rfind(to_lower(find)) : to_lower(str).find(to_lower(find)));
		}
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> get_left_of(const std::basic_string<char_t>& str, const char_t* find, bool _bCaseSensitive = true, bool _bReverse = false)
	{
		return get_left_of(str, std::basic_string<char_t>(find),_bCaseSensitive, _bReverse);
	}
#pragma endregion

#pragma region contains
    template <typename char_t = base_char_t>
    inline bool contains(const std::basic_string<char_t>& str, const char_t find, bool _bCaseSensitive = true)
    {
        if (_bCaseSensitive)
        {
            return str.find(find, 0u) != string::npos;
        }
        else
        {
            return to_lower(str).find(to_lower(find), 0u) != string::npos;
        }
    }

	template <typename char_t = base_char_t>
	inline bool contains(const std::basic_string<char_t>& str, const std::basic_string<char_t>& find, bool _bCaseSensitive = true)
	{
		if (_bCaseSensitive)
		{
			return str.find(find, 0u) != string::npos;
		}
		else
		{
			return to_lower(str).find(to_lower(find), 0u) != string::npos;
		}
	}

	template <typename char_t = base_char_t>
	inline bool contains(const std::basic_string<char_t>& str, const char_t* find, bool _bCaseSensitive = true)
	{
		return contains(str, std::basic_string<char_t>(find), _bCaseSensitive);
	}
#pragma endregion
	
#pragma region starts_with
    template <typename char_t = base_char_t>
    inline bool starts_with(const std::basic_string<char_t>& str, const char_t& find, bool _bCaseSensitive = true)
    {
        if (_bCaseSensitive)
        {
            return str.find(find, 0u) == 0u;
        }
        else
        {
            return to_lower(str).find(to_lower(find), 0u) == 0u;
        }
    }

	template <typename char_t = base_char_t>
	inline bool starts_with(const std::basic_string<char_t>& str, const std::basic_string<char_t>& find, bool _bCaseSensitive = true)
	{
		if (_bCaseSensitive)
		{
			return str.find(find, 0) == 0;
		}
		else
		{
			return to_lower(str).find(to_lower(find), 0) == 0;
		}		
	}

	template <typename char_t = base_char_t>
	inline bool starts_with(const std::basic_string<char_t>& str, const char_t* find, bool _bCaseSensitive = true)
	{
		return starts_with(str, std::basic_string<char_t>(find), _bCaseSensitive);
	}
#pragma endregion
	
#pragma region ends_with
    template <typename char_t = base_char_t>
    inline bool ends_with(const std::basic_string<char_t>& str, const char_t& find, bool _bCaseSensitive = true)
    {
        if (_bCaseSensitive)
        {
            return str.rfind(find) == str.size() - find.size();
        }
        else
        {
            return to_lower(str).rfind(to_lower(find)) == str.size() - find.size();
        }
    }

	template <typename char_t = base_char_t>
	inline bool ends_with(const std::basic_string<char_t>& str, const std::basic_string<char_t>& find, bool _bCaseSensitive = true)
	{
		if (_bCaseSensitive)
		{
			return str.rfind(find) == str.size() - find.size();
		}
		else
		{
			return to_lower(str).rfind(to_lower(find)) == str.size() - find.size();
		}
	}

	template <typename char_t = base_char_t>
	inline bool ends_with(const std::basic_string<char_t>& str, const char_t* find, bool _bCaseSensitive = true)
	{
		return ends_with(str, std::basic_string<char_t>(find), _bCaseSensitive);
	}
#pragma endregion
	
#pragma region remove
	template <typename char_t = base_char_t>
	inline void remove(std::basic_string<char_t>& str, char_t rm)
	{
		str.erase(std::remove(str.begin(), str.end(), rm), str.end());
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> remove(const std::basic_string<char_t>& str, char_t rm)
	{
		std::basic_string<char_t> ret(str);
		remove(ret, rm);
		return ret;
	}

	template <typename char_t = base_char_t>
	inline void remove(std::basic_string<char_t>& str, const std::basic_string<char_t>& rm)
	{
		size_t uFound = str.find(rm, 0);
		while (uFound != std::string::npos)
		{
			str.erase(uFound, rm.size());
			uFound = str.find(rm, 0);
		}		
	}

	template <typename char_t = base_char_t>
	inline void remove(std::basic_string<char_t>& str, const char_t* rm)
	{
		remove(str, std::basic_string<char_t>(rm));
	}
	
	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> remove(const std::basic_string<char_t>& str, const std::basic_string<char_t>& rm)
	{
		std::basic_string<char_t> ret(str);
		remove(ret, rm);
		return ret;
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> remove(const std::basic_string<char_t>& str, const char_t* rm)
	{
		std::basic_string<char_t> ret(str);
		remove(ret, rm);
		return ret;
	}
#pragma endregion

#pragma region remove_any
	template <typename char_t = base_char_t>
	inline void remove_any(std::basic_string<char_t>& str, const std::basic_string<char_t>& rm)
	{
		size_t uFound = str.find_first_of(rm, 0);
		while (uFound != std::string::npos)
		{
			str.erase(uFound, 1);
			uFound = str.find_first_of(rm, 0);
		}
	}

	template <typename char_t = base_char_t>
	inline void remove_any(std::basic_string<char_t>& str, const char_t* rm)
	{
		return remove_any(str, std::basic_string<char_t>(rm));
	}
#pragma endregion

#pragma region remove_whitespaces
	template <typename char_t = base_char_t>
	inline void remove_whitespaces(std::basic_string<char_t>& str)
	{
		str.erase(std::remove_if(str.begin(), str.end(), [](const char_t& ch) -> bool { return std::isspace<char_t>(ch, std::locale::classic()); }), str.end());
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> remove_whitespaces(const std::basic_string<char_t>& str)
	{
		std::basic_string<char_t> sRet(str);
		remove_whitespaces(sRet);
		return sRet;
	}
#pragma endregion
	
#pragma region replace_extension
	template <typename char_t = base_char_t>
	inline void replace_extension(_Inout_ std::basic_string<char_t>& _sFilePath, const std::basic_string<char_t>& _sNewExtension)
	{
		size_t pos = _sFilePath.rfind(ST(char_t, "."));

		if (pos == string::npos)
		{
			_sFilePath.append(ST(char_t, ".") + _sNewExtension);
		}
		else
		{
			_sFilePath = _sFilePath.substr(0, pos + 1) + _sNewExtension;
		}
	}

	template <typename char_t = base_char_t>
	inline void replace_extension(_Inout_ std::basic_string<char_t>& _sFilePath, const char_t* _sNewExtension)
	{
		replace_extension(_sFilePath, std::basic_string<char_t>(_sNewExtension));
	}
#pragma endregion

#pragma region replace
	template <typename char_t = base_char_t>
    inline void replace(std::basic_string<char_t>& str, const std::basic_string<char_t>& find, const std::basic_string<char_t>& replace, const size_t offset = 0u, const size_t end = std::string::npos)
	{
		size_t uPos = str.find(find, offset);

		while (uPos < end)
		{
			str.replace(uPos, find.size(), replace);

			uPos = str.find(find, uPos + 1u);
		}
	}

	template <typename char_t = base_char_t>
	inline void replace(std::basic_string<char_t>& str, const char_t* pFind, const char_t* pReplace, const size_t offset = 0u, const size_t end = std::string::npos)
	{
		replace(str, std::basic_string<char_t>(pFind), std::basic_string<char_t>(pReplace), offset, end);
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> replace(const std::basic_string<char_t>& str, const std::basic_string<char_t>& sFind, const std::basic_string<char_t>& sReplace, const size_t offset = 0u, const size_t end = std::string::npos)
	{
		std::basic_string<char_t> ret(str);
		replace(ret, sFind, sReplace, offset, end);
		return ret;
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> replace(const std::basic_string<char_t>& str, const char_t* pFind, const char_t* pReplace, const size_t offset = 0u, const size_t end = std::string::npos)
	{
		std::basic_string<char_t> ret(str);
		replace(ret, std::basic_string<char_t>(pFind), std::basic_string<char_t>(pReplace), offset, end);
		return ret;
	}
#pragma endregion
	
#pragma region skip_whitespaces
	template <typename char_t = base_char_t>
	inline void skip_whitespaces(std::basic_stringstream<char_t>& stream)
	{
		int c = 0;
		bool b = true;
		while (stream.good() && b)
		{
			c = stream.get();
			if (std::isspace<char_t>(static_cast<char_t>(c), std::locale::classic()))
			{
				b = true;
				continue;
			}
			else
			{
				b = false;
				stream.unget();
				break;
			}
		}
	}
#pragma endregion

#pragma region read_whitespaces
	template <typename char_t = base_char_t, typename stream_t = const bytes>
	inline char_t read_whitespaces(basestream<stream_t>& stream)
	{
		char_t c = 0;
		bool b = true;
		while (stream.good() && b)
		{
			c = stream.get<char_t>();
			if (std::isspace<char_t>(c, std::locale::classic()))
			{
				b = true;
				continue;
			}
			else
			{
				b = false;
			}
		}
		return c;
	}
#pragma endregion
	
#pragma region read_token
	template <typename char_t = base_char_t, typename stream_t = const bytes>
	inline std::basic_string<char_t> read_token(basestream<stream_t>& stream)
	{
		char_t c = 0;
		bool token = false;
		bool b = true;
		std::basic_string<char_t> ret;
		while (stream.good() && b)
		{
			c = stream.get<char_t>();
			if (std::isspace<char_t>(c, std::locale::classic()))
			{
				if (token == true) b = false;
				continue;
			}
			else
			{
				ret += c;
				token = true;
			}
		}
		return ret;
	}

	template <typename char_t = base_char_t>
	inline std::basic_string<char_t> read_token(std::basic_stringstream<char_t>& stream)
	{
		int c = 0;
		bool token = false;
		bool b = true;
		std::basic_string<char_t> ret;
		while (stream.good() && b)
		{
			c = stream.get();
			if (c == -1 || std::isspace<char_t>(c, std::locale::classic()))
			{
				if (token == true) b = false;
				continue;
			}
			else
			{
				ret += c;
				token = true;
			}
		}
		return ret;
	}
#pragma endregion
	
#pragma region format
	template <typename ...Args>
	inline std::string format(const std::string& _sFormat, Args&& ...args)
	{
		int iSize = std::snprintf(nullptr, 0, _sFormat.c_str(), std::forward<Args>(args)...);

		std::string sOutput(iSize + 1, '\0');
		std::sprintf(&sOutput[0], _sFormat.c_str(), std::forward<Args>(args)...);

		return sOutput;
	}

	template <typename ...Args>
	inline void format(std::string& _sOutput, const std::string& _sFormat, Args&& ...args)
	{
		int iSize = std::snprintf(nullptr, 0, _sFormat.c_str(), std::forward<Args>(args)...);

		_sOutput.resize(iSize + 1, '\0');
		std::sprintf(&_sOutput[0], _sFormat.c_str(), std::forward<Args>(args)...);
	}

	template <typename ...Args>
	inline std::wstring format(const std::wstring& _sFormat, Args&& ...args)
	{
		int iSize = std::swprintf(nullptr, 0, _sFormat.c_str(), std::forward<Args>(args)...);

		std::wstring sOutput(iSize + 1, L'\0');
		std::swprintf(&sOutput[0], _sFormat.c_str(), std::forward<Args>(args)...);

		return sOutput;
	}

	template <typename ...Args>
	inline void format(std::wstring& _sOutput, const std::wstring& _sFormat, Args&& ...args)
	{
		int iSize = std::swprintf(nullptr, 0, _sFormat.c_str(), std::forward<Args>(args)...);

		_sOutput.resize(iSize + 1, L'\0');
		std::swprintf(&_sOutput[0], _sFormat.c_str(), std::forward<Args>(args)...);
	}
#pragma endregion

} // hlx

#endif //HLX_STRINGHELPERS_H