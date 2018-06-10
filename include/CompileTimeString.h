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

#ifndef HLX_COMPILETIMESTRING_H
#define HLX_COMPILETIMESTRING_H

// this header is only needed because the vsc lib didnt offer all to c++17 features yet

#include <string_view>
#include <string>

namespace hlx
{
	template <typename char_t, size_t Size>
	inline constexpr std::basic_string_view<char_t> make_string_view(const char_t(&str)[Size]) { return std::basic_string_view<char_t>(str, Size); }

	template <typename char_t>
	inline std::basic_string<char_t> to_string(std::basic_string_view<char_t> _sView) { return std::basic_string<char_t>(_sView.data(), _sView.size()); }

	template<typename char_t>
	inline constexpr size_t const_string_hash(const std::basic_string_view<char_t> _View)
	{	// FNV-1a hash function for bytes in [_First, _First + _Count)
#if defined(_WIN64)
		static_assert(sizeof(size_t) == 8, "This code is for 64-bit size_t.");
		const size_t _FNV_offset_basis = 14695981039346656037ULL;
		const size_t _FNV_prime = 1099511628211ULL;

#else /* defined(_WIN64) */
		static_assert(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;
#endif /* defined(_WIN64) */

		size_t _Val = _FNV_offset_basis;
		for (size_t _Next = 0; _Next < _View.size(); ++_Next)
		{	// fold in another byte
			_Val ^= (size_t)_View[_Next];
			_Val *= _FNV_prime;
		}
		return (_Val);
	}

	template <typename char_t, size_t Size>
	inline constexpr size_t const_string_hash(const char_t(&str)[Size]) { return const_string_hash(make_string_view(str)); }

	inline constexpr std::string_view operator "" _sv(const char *_Str, size_t _Len) { return std::string_view(_Str, _Len); }
	inline constexpr std::wstring_view operator "" _sv(const wchar_t *_Str, size_t _Len) { return std::wstring_view(_Str, _Len); }
	inline constexpr std::u16string_view operator "" _sv(const char16_t *_Str, size_t _Len) { return std::u16string_view(_Str, _Len); }
	inline constexpr std::u32string_view operator "" _sv(const char32_t *_Str, size_t _Len) { return std::u32string_view(_Str, _Len); }

	inline constexpr size_t operator "" _hash(const char *_Str, size_t _Len) { return const_string_hash(std::string_view(_Str, _Len)); }
	inline constexpr size_t operator "" _hash(const wchar_t *_Str, size_t _Len) { return const_string_hash(std::wstring_view(_Str, _Len)); }
	inline constexpr size_t operator "" _hash(const char16_t *_Str, size_t _Len) { return const_string_hash(std::u16string_view(_Str, _Len));}
	inline constexpr size_t operator "" _hash(const char32_t *_Str, size_t _Len) { return const_string_hash(std::u32string_view(_Str, _Len));}

	// compare operators should be there since c++17
	//http://en.cppreference.com/w/cpp/string/basic_string_view/operator_cmp

	template <typename char_t>
	constexpr bool operator==(const std::basic_string_view<char_t> l, const std::basic_string_view<char_t> r)
	{
		if (l.size() != r.size())
		{
			return false;
		}

		for (size_t i = 0; i < l.size(); ++i)
		{
			if (l[i] != r[i])
			{
				return false;
			}
		}

		return true;
	}

	template <typename char_t>
	constexpr bool operator!=(const std::basic_string_view<char_t> l, const std::basic_string_view<char_t> r)
	{
		return !(l == r);
	}
}; // hlx

#endif // !HLX_COMPILETIMESTRING_H
