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

#ifndef HLX_FILESTREAM_H
#define HLX_FILESTREAM_H

#include "ByteStream.h"
#include "StringTypes.h"
#include <memory>

namespace hlx
{
	class fbytestream : public std::fstream
	{
	public:
		fbytestream(void) {}

		fbytestream(const std::string& fn, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) : std::fstream(fn.c_str(), mode), m_sFileName(hlx::to_wstring(fn)) {}
		fbytestream(const std::wstring& fn, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) : std::fstream(fn.c_str(), mode), m_sFileName(hlx::to_wstring(fn)) {}
		
		void open(const std::string& fn, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
		void open(const std::wstring& fn, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

		// read all remaining data
		void getAll(bytes& _OutData);

		template<typename T> T get(void);
		template<typename T> T get(const size_t& l);

		template<typename T> void put(const T& b);
		template<typename T> void put(const T& b, const size_t& l);

		template <typename BYTES = bytes>
		// if l = 0 then a stream of the available data from pos to end will be returned
		std::unique_ptr<basestream<BYTES>> make_bytestream(const size_t& pos = 0, const size_t& l = 0);

		std::unique_ptr<const_bytestream> make_const_bytestream(const size_t& pos = 0, const size_t& l = 0);

		size_t size(void);
		size_t available(void);
		void skip(const size_t& _uLength);

		inline const std::wstring& GetFileName() const { return m_sFileName; }

	private:
		std::wstring m_sFileName;
	};

	inline void fbytestream::open(const std::string& fn, std::ios_base::openmode mode)
	{
		m_sFileName = hlx::to_wstring(fn);
		std::fstream::open(fn.c_str(), mode);
	}

	inline void fbytestream::open(const std::wstring& fn, std::ios_base::openmode mode)
	{
		m_sFileName = fn;
		std::fstream::open(fn.c_str(), mode);
	}

	inline void fbytestream::getAll(bytes& _OutData)
	{
		const size_t size = available();
		if (size > 0u)
		{
			_OutData.resize(size);
			read(reinterpret_cast<char*>(&_OutData.front()), size);
		}
	}

	template<typename T> inline T fbytestream::get(void)
	{
		if (available() >= sizeof(T))
		{
			T ret{};
			read(reinterpret_cast<char*>(&ret), sizeof(T));
			return ret;
		}

		return T();
	}

	template<> inline std::string fbytestream::get<std::string>(void)
	{
		std::string ret;
		char c = 0;

		while (good() && read(&c, sizeof(char)) && c != 0)
		{
			ret += c;
		}

		return ret;
	}

	template<> inline std::wstring fbytestream::get<std::wstring>(void)
	{
		std::wstring ret;
		wchar_t c = 0;

		while (good() && read(reinterpret_cast<char*>(&c), sizeof(wchar_t)) && c != 0)
		{
			ret += c;
		}

		return ret;
	}

	template<> inline std::string fbytestream::get<std::string>(const size_type& l)
	{
		if (available() >= l && l > 0u)
		{
			std::string ret; ret.resize(l);
			read(&ret[0], l);
			return ret;
		}

		return std::string();
	}

	template<> inline bytes fbytestream::get<bytes>(const size_type& l)
	{
		if (available() >= l && l > 0u)
		{
			bytes ret; ret.resize(l);
			read(reinterpret_cast<char*>(&ret[0]), l);
			return ret;
		}

		return bytes();
	}

	template<typename T> inline void fbytestream::put(const T& b)
	{
		write(reinterpret_cast<const char*>(&b), sizeof(T));
	}

	template<typename BYTES>
	inline std::unique_ptr<basestream<BYTES>> fbytestream::make_bytestream(const size_t& pos, const size_t& l)
	{	
		size_t prev_pos = tellg();

		seekg(0, std::fstream::end);
		size_t end = tellg();

		if (pos + l <= end)
		{
			std::streamsize read_length = l == 0u ? end - pos : l;
			seekg(pos, std::fstream::beg);

			bytes temp; temp.resize(read_length);
			read(reinterpret_cast<char*>(&temp[0]), read_length);

			seekg(prev_pos, std::fstream::beg);

			if (fail())
			{
				return nullptr;
			}

			return std::make_unique<basestream<BYTES>>(std::move(temp));
		}

		seekg(prev_pos, std::fstream::beg);
		return nullptr;
	}

	inline std::unique_ptr<const_bytestream> fbytestream::make_const_bytestream(const size_type& pos, const size_type& l)
	{
		return make_bytestream<const bytes>(pos, l);
	}

	//template<typename T>
	//inline void fbytestream::put(const T & b, const size_type & l)
	//{
	//	put<T>(b);
	//}

	template<> inline void fbytestream::put<std::string>(const std::string& b)
	{
		write(b.data(), b.size());
	}

	template<> inline void fbytestream::put<bytes>(const bytes& b)
	{
		write(reinterpret_cast<const char*>(b.data()), b.size());
	}

	template<> inline void fbytestream::put<std::string>(const std::string& b, const size_type& l)
	{
		write(b.c_str(), l);
	}

	template<> inline void fbytestream::put<bytes>(const bytes& b, const size_type& l)
	{
		write(reinterpret_cast<const char*>(b.data()), l);
	}

	template<typename T>
	inline fbytestream& operator>>(fbytestream& stream, T& b)
	{
		b = stream.get<T>();
		return stream;
	}

	template<typename T>
	inline fbytestream& operator<<(fbytestream& stream, const T& b)
	{
		stream.put<T>(b);
		return stream;
	}

	inline size_t fbytestream::available(void)
	{
		std::streamoff p = tellg();
		seekg(0, std::fstream::end);
		std::streamoff l = tellg();
		seekg(p);
		return l - p;
	}

	inline void fbytestream::skip(const size_t& _uLength)
	{
		seekg(_uLength, std::fstream::cur);
	}

	inline size_t fbytestream::size(void)
	{
		std::streamoff p = tellg();
		seekg(0, std::fstream::end);
		std::streamoff l = tellg();
		seekg(p);
		return l;
	}
} // hlx

#endif // HLX_FILESTREAM_H