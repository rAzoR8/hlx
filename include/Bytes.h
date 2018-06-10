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

#ifndef HLX_BYTES_H
#define HLX_BYTES_H

#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <assert.h>

namespace hlx
{
	template<typename T> inline T switchByteOrder(const T& t)
	{
		T ret;
		const uint8_t *p = reinterpret_cast<const uint8_t*>(&t);
		uint8_t *r = reinterpret_cast<uint8_t*>(&ret);

		for (size_t i = 0; i < sizeof(T); ++i)
		{
			r[i] = p[sizeof(T) - i];
		}

		return ret;
	}

	template<> inline std::string switchByteOrder(const std::string& t)
	{
		size_t size = t.size();
		std::string ret; ret.resize(size);

		for (size_t i = 0; i < size; ++i)
		{
			ret[i] = t[size - i];
		}

		return ret;
	}

	template<typename T> inline void switchByteOrderMutable(T &t)
	{
		uint8_t tmp;
		uint8_t *p = reinterpret_cast<uint8_t*>(&t);

		for (size_t i = 0; i < sizeof(T); ++i)
		{
			tmp = p[i];
			p[i] = p[sizeof(T) - i];
			p[sizeof(T) - i] = tmp;
		}
	}

	template<> inline void switchByteOrderMutable(std::string &t)
	{
		size_t size = t.size();
		char tmp;

		for (size_t i = 0; i < size; ++i)
		{
			tmp = t[i];
			t[i] = t[size - i];
			t[size - i] = tmp;
		}
	}

	//Bytes <<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
	typedef std::basic_string<uint8_t> basic_bytes;
	typedef basic_bytes::size_type size_type;

	class bytes : public basic_bytes {
	public:
		typedef uint8_t base_type;

		bytes(void) : basic_bytes() {}
		explicit bytes(basic_bytes::size_type l, uint8_t c = 0u) : basic_bytes(l, c) {}
		virtual ~bytes(void) {}

		template<typename T> bytes(const T& b) : basic_bytes(convert_to_basic_bytes(b)) {}
		template<typename T> bytes(const T& b, const basic_bytes::size_type& l) : basic_bytes(convert_to_basic_bytes(b, l)) {}

		template<typename T> T as(void) const;
		bool operator==(const bytes& b);

	private:
		template<typename T> basic_bytes convert_to_basic_bytes(const T& b);
		basic_bytes convert_to_basic_bytes(const std::string& b, const basic_bytes::size_type& l);
		basic_bytes convert_to_basic_bytes(const bytes& b, const basic_bytes::size_type& l);
		basic_bytes convert_to_basic_bytes(const void* b, const basic_bytes::size_type& l);
	};

	template<typename T> inline basic_bytes bytes::convert_to_basic_bytes(const T& b)
	{
		return basic_bytes(reinterpret_cast<const uint8_t*>(&b), sizeof(T));
	}

	template<> inline basic_bytes bytes::convert_to_basic_bytes<bytes>(const bytes& b)
	{
		return b;
	}

	template<> inline basic_bytes bytes::convert_to_basic_bytes<basic_bytes>(const basic_bytes& b)
	{
		return b;
	}

	template<> inline basic_bytes bytes::convert_to_basic_bytes<std::string>(const std::string& b)
	{
		return basic_bytes(reinterpret_cast<const uint8_t*>(b.data()), b.size());
	}

	inline basic_bytes bytes::convert_to_basic_bytes(const std::string& b, const size_type& l)
	{
		return basic_bytes(reinterpret_cast<const uint8_t*>(b.data()), l);
	}

	inline basic_bytes bytes::convert_to_basic_bytes(const bytes& b, const size_type& l)
	{
		return b.substr(0, l);
	}

	inline basic_bytes bytes::convert_to_basic_bytes(const void* b, const size_type& l)
	{
		return basic_bytes(reinterpret_cast<const uint8_t*>(b), l);
	}

	template<typename T> inline T bytes::as(void) const
	{
		assert(size() >= sizeof(T));

		if (size() >= sizeof(T))
		{
			return *reinterpret_cast<const T*>(data());
		}

		return T();
	}

	template<> inline std::string bytes::as<std::string>(void) const
	{
		return std::string(reinterpret_cast<const char*>(data()), size());
	}

	template<> inline bytes bytes::as<bytes>(void) const
	{
		return *this;
	}

	inline bool bytes::operator==(const bytes& b){
		if (size() != b.size())
		{
			return false;
		}

		return memcmp(data(),b.data(),size()) == 0;
	}

	template<> inline bytes switchByteOrder(const bytes& b)
	{
		size_t size = b.size();
		bytes ret; ret.resize(size);

		for (size_t i = 0; i < size; ++i)
		{
			ret[i] = b[size - i];
		}

		return ret;
	}

	template<> inline void switchByteOrderMutable(bytes &b)
	{
		size_t size = b.size();
		uint8_t tmp = 0;

		for (size_t i = 0; i < size; ++i)
		{
			tmp = b[i];
			b[i] = b[size - i];
			b[size - i] = tmp;
		}
	}
} // hlx

#endif //HLX_BYTES_H
