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

#ifndef HLX_BYTESTREAM_H
#define HLX_BYTESTREAM_H

#include "Bytes.h"

namespace hlx
{
	template <class BYTES>
	class basestream
	{
	public:
		typedef typename BYTES BaseType;

		//works on passed reference
		basestream(BYTES& b) : m_Bytes(b) {};

		// create new instance from moved bytes
		basestream(BYTES&& b) : m_pBytes(new BYTES(std::move(b))), m_Bytes(*m_pBytes) {};

		// creates a new instance of bytes
		basestream() : m_pBytes(new BYTES) , m_Bytes(*m_pBytes) {};

		// supress copying
		basestream(const basestream&) = delete;

		// supress copy assignment
		basestream& operator=(const basestream& _Other) = delete;

		virtual ~basestream()
		{ 
			if (m_pBytes != nullptr)
			{
				delete m_pBytes;
				m_pBytes = nullptr;
			}
		}

		template<typename T> T get() const;
		template<typename T> bool get(T& b) const;
		template<typename T> T get(const size_t& l) const;

		// overwrites the contents of b (instead of appending data)
		bool get(bytes& b, const size_t& l) const; // returns false if available data is insufficient

		void put(const BYTES& b);

		template<typename T> void put(const T& b);
		template<typename T> void put_front(const T& b);

		template <uint32_t size = 0>
		void put(const char(&pStr)[size], bool _bWriteTerminator = true);

		void put(const std::string& s, bool _bWriteTerminator = true);

		//Inserts additional characters into the string right before the character indicated by pos
		template<typename T> void insert(const size_type& pos, const T& b);
		void insert(const size_type& pos, const BYTES& b);

		template<typename T> void replace(const size_type& pos, const T& b);
		void replace(const size_type& pos, const BYTES& b);

		// erase from current offset length bytes
		bool erase(const size_type& length);
		// erase from pos length bytes
		bool erase(const size_type& pos, const size_type& length);

		size_t available() const;
		bool good() const;

		const size_t& get_offset() const;
		void set_offset(const std::streamoff& offset, std::ios_base::seekdir dir = std::ios_base::beg);

		void skip(const size_t& _uLength);

		BYTES& get_bytes() const;

		template<typename U = BYTES, typename std::enable_if<
			std::is_const<U>::value, const uint8_t*>::type = 0>
			const uint8_t* get_data(const size_t& pos = 0)
		{ 
			if (pos < m_Bytes.size())
			{
				return &m_Bytes.data()[pos];
			}

			return nullptr;
		}

		template<typename U = BYTES, typename std::enable_if<
			!std::is_const<U>::value, uint8_t*>::type = 0>
			uint8_t* get_data(const size_t& pos = 0)
		{
			if (pos < m_Bytes.size())
			{
				return &m_Bytes.at(pos);
			}

			return nullptr;
		}

		// swaps internal bytes with b, sets offset to 0
		void reset(BYTES& b);
		// same as reset
		void swap(BYTES& b);

		void clear();

#pragma region GetImpl
		// inline partial specialization
		template<>
		inline bytes get(const size_t& l) const
		{
			if (available() >= l)
			{
				bytes Ret(m_Bytes.substr(m_uOffset, l));
				m_uOffset += l;

				return Ret;
			}

			return bytes();
		}

		template<>
		inline const bytes get(const size_t& l) const
		{
			if (available() >= l)
			{
				const bytes Ret(m_Bytes.substr(m_uOffset, l));
				m_uOffset += l;

				return Ret;
			}

			return const bytes();
		}

		template<>
		inline std::string get(const size_t& l) const
		{
			if (available() >= l)
			{
				std::string Ret(reinterpret_cast<const char*>(m_Bytes.substr(m_uOffset, l).data()), l);
				m_uOffset += l;

				return Ret;
			}

			return std::string();
		}

		template<>
		inline std::string get() const
		{
			if (good())
			{
				std::string Ret(reinterpret_cast<const char*>(m_Bytes.data() + m_uOffset));

				m_uOffset += Ret.size();

				if (good() && m_Bytes.at(m_uOffset) == '\0')
				{
					++m_uOffset;
				}

				return Ret;
			}

			return std::string();
		}

		template<> // assigns data
		inline bool get(std::string& s) const
		{
			if (good())
			{
				s.assign(reinterpret_cast<const char*>(m_Bytes.data() + m_uOffset));

				m_uOffset += s.size();

				if (good() && m_Bytes.at(m_uOffset) == '\0')
				{
					++m_uOffset;
				}

				return true;
			}

			return false;
		}

#pragma endregion
	protected:

		BYTES* m_pBytes = nullptr;
		BYTES& m_Bytes;
		mutable size_type m_uOffset = 0;
	};

	using const_bytestream = basestream<const bytes>;
	using bytestream = basestream<bytes>;

#pragma region Offset Functions
	//---------------------------------------------------------------------------------------------------
	// Offset manipulation functions
	//---------------------------------------------------------------------------------------------------

	template<class BYTES>
	inline size_t basestream<BYTES>::available() const
	{
		return m_Bytes.size() - m_uOffset;
	}

	template<class BYTES>
	inline bool basestream<BYTES>::good() const
	{
		return available() > 0;
	}

	template<class BYTES>
	inline const size_t& basestream<BYTES>::get_offset() const
	{
		return m_uOffset;
	}

	template<class BYTES>
	inline void basestream<BYTES>::set_offset(const std::streamoff& offset, std::ios_base::seekdir dir)
	{
		switch (dir)
		{
		case std::ios_base::beg:
			m_uOffset = static_cast<size_t>(offset);
			return;
		case std::ios_base::end:
			m_uOffset = m_Bytes.size() - offset;
			return;
		case std::ios_base::cur:
			m_uOffset += offset;
			return;
		}

		return;
	}

	template<class BYTES>
	inline void basestream<BYTES>::skip(const size_t& _uLength)
	{
		m_uOffset += _uLength;
	}

	template<class BYTES>
	inline BYTES& basestream<BYTES>::get_bytes() const
	{
		return m_Bytes;
	}

	template<class BYTES>
	inline void basestream<BYTES>::reset(BYTES& b)
	{
		m_Bytes.swap(b);
		m_uOffset = 0u;
	}

	template<class BYTES>
	inline void basestream<BYTES>::swap(BYTES & b)
	{
		m_Bytes.swap(b);
		m_uOffset = 0u;
	}

	template<class BYTES>
	inline void basestream<BYTES>::clear()
	{
		m_Bytes.resize(0u);
		m_uOffset = 0u;
	}

#pragma endregion

#pragma region GetPut Functions
	//---------------------------------------------------------------------------------------------------
	// Read and write functions
	//---------------------------------------------------------------------------------------------------

	// copys data
	template<class BYTES>
	template<typename T>
	inline T basestream<BYTES>::get() const
	{
		if (available() >= sizeof(T))
		{
			T Ret(*reinterpret_cast<const T*>(m_Bytes.data() + m_uOffset));
			m_uOffset += sizeof(T);

			return Ret;
		}

		return T();
	}

	// assignes
	template<class BYTES>
	template<typename T>
	inline bool basestream<BYTES>::get(T& b) const
	{
		if (available() >= sizeof(T))
		{
			b = *reinterpret_cast<const T*>(m_Bytes.data() + m_uOffset);
			m_uOffset += sizeof(T);
			return true;
		}

		return false;
	}

	// assignes
	template<class BYTES>
	inline bool basestream<BYTES>::get(bytes& b, const size_t& l) const
	{
		if (available() >= l)
		{
			b.resize(l);
			b.assign(m_Bytes.data() + m_uOffset, l);
			m_uOffset += l;
			return true;
		}

		return false;
	}

	template<class BYTES>
	inline void basestream<BYTES>::put(const BYTES& b)
	{
		m_Bytes += b;
		m_uOffset = m_Bytes.size();
	}

	template<class BYTES>
	template<typename T>
	inline void basestream<BYTES>::put(const T& b)
	{
		m_Bytes += BYTES(b);
		m_uOffset = m_Bytes.size();
	}

	template<class BYTES>
	template<typename T>
	inline void basestream<BYTES>::put_front(const T& b)
	{
		m_Bytes = BYTES(b) + m_Bytes;
		m_uOffset = m_Bytes.size();
	}

	template<class BYTES>
	template<uint32_t size>
	inline void basestream<BYTES>::put(const char(&pStr)[size], bool _bWriteTerminator)
	{
		m_Bytes += BYTES(pStr);
		m_uOffset += size;

		if (_bWriteTerminator && pStr[size-1u] != '\0')
		{
			m_Bytes += static_cast<uint8_t>(0u); // write string terminator
			m_uOffset += sizeof(uint8_t);
		}
	}

	template<class BYTES>
	inline void basestream<BYTES>::put(const std::string& s, bool _bWriteTerminator)
	{
		if (_bWriteTerminator && s.empty())
		{
			m_Bytes += static_cast<uint8_t>(0u); // write string terminator
			m_uOffset += sizeof(uint8_t);
		}
		else
		{
			m_Bytes += BYTES(s);
			m_uOffset += s.size();

			if (_bWriteTerminator && s.back() != '\0')
			{
				m_Bytes += static_cast<uint8_t>(0); // write string terminator
				m_uOffset += sizeof(uint8_t);
			}
		}
	}
#pragma endregion

	//---------------------------------------------------------------------------------------------------
	// insert, replace & erase
	//---------------------------------------------------------------------------------------------------
	template<class BYTES>
	template<typename T>
	inline void basestream<BYTES>::insert(const size_type & pos, const T& b)
	{
		m_Bytes.insert(pos, reinterpret_cast<const uint8_t*>(&b), sizeof(T));
	}

	template<class BYTES>
	inline void basestream<BYTES>::insert(const size_type & pos, const BYTES& b)
	{
		m_Bytes.insert(pos, b);
	}

	template<class BYTES>
	template<typename T>
	inline void basestream<BYTES>::replace(const size_type& pos, const T &b)
	{
		m_Bytes.replace(pos, sizeof(T), reinterpret_cast<const uint8_t*>(&b), sizeof(T));
	}

	template<class BYTES>
	inline void basestream<BYTES>::replace(const size_type & pos, const BYTES & b)
	{
		m_Bytes.replace(pos, b.size(), b);
	}

	template<class BYTES>
	inline bool basestream<BYTES>::erase(const size_type& length)
	{
		if (m_uOffset + length <= m_Bytes.size())
		{
			m_Bytes.erase(m_uOffset, length);
			return true;
		}

		return false;
	}

	template<class BYTES>
	inline bool basestream<BYTES>::erase(const size_type& pos, const size_type& length)
	{
		if (pos + length <= m_Bytes.size())
		{
			m_Bytes.erase(pos, length);

			if (m_uOffset >= m_Bytes.size())
			{
				m_uOffset = m_Bytes.size();
			}

			return true;
		}

		return false;
	}

#pragma region Stream Operators

#pragma endregion

	//---------------------------------------------------------------------------------------------------
	// global stream operators
	//---------------------------------------------------------------------------------------------------

	// READ:
	template<typename T>
	inline bytestream& operator>>(bytestream& stream, T& b)
	{
		stream.get<T>(b);
		return stream;
	}

	// const stream read
	template<typename T>
	inline const_bytestream& operator>>(const_bytestream& stream, T& b)
	{
		stream.get<T>(b);
		return stream;
	}

	// CONST const stream read
	template<typename T>
	inline const const_bytestream& operator>>(const const_bytestream& stream, T& b)
	{
		stream.get<T>(b);
		return stream;
	}

	// WRITE
	template<typename T>
	inline bytestream& operator<<(bytestream& stream, const T& b)
	{
		stream.put(b); // omit <T> template type for sfinae on put(const std::string& s, bool _bWriteTerminator = true)
		return stream;
	}

#ifndef STREAMOP_READ
#define STREAMOP_READ(_type,_var, _impl)\
	inline hlx::bytestream& operator >> (hlx::bytestream& stream, _type& _var) {	_impl; return stream;}
#endif

#ifndef STREAMOP_CONSTREAD
#define STREAMOP_CONSTREAD(_type,_var, _impl)\
	inline hlx::const_bytestream& operator >> (hlx::const_bytestream& stream, _type& _var) {	_impl; return stream;}
#endif

#ifndef CONSTSTREAMOP_CONSTREAD
#define CONSTSTREAMOP_CONSTREAD(_type,_var, _impl)\
	inline const hlx::const_bytestream& operator >> (const hlx::const_bytestream& stream, _type& _var) {	_impl; return stream;}
#endif

#ifndef STREAMOP_WRITE
#define STREAMOP_WRITE(_type,_var, _impl)\
	inline hlx::bytestream& operator << (hlx::bytestream& stream, _type& _var) {	_impl; return stream;}
#endif

#ifndef GLOBALSTREAMOPS
#define GLOBALSTREAMOPS(_type, _var, _impl_read, _impl_write) \
		STREAMOP_READ(_type, _var, _impl_read) \
		STREAMOP_CONSTREAD(_type, _var, _impl_read) \
		CONSTSTREAMOP_CONSTREAD(_type, _var, _impl_read) \
		STREAMOP_WRITE(_type, _var, _impl_write)
#endif
}
#endif // HLX_BYTESTREAM_H