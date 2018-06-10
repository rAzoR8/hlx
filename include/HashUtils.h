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

#ifndef HLX_HASHUTILS_H
#define HLX_HASHUTILS_H

#include <stdint.h>
#include <type_traits>
#include <string>

namespace hlx
{
	namespace detail
	{
		inline uint64_t fnv1a_hash_impl(const uint8_t* pData, const size_t _uSize) noexcept
		{	
			// FNV-1a hash function for bytes in [_First, _First + _Count)
			uint64_t uVal = 14695981039346656037ULL;
			if (pData != nullptr)
			{
				for (size_t _Next = 0; _Next < _uSize; ++_Next)
				{	// fold in another byte
					uVal ^= uint64_t( pData[_Next] );
					uVal *= 1099511628211ULL;
				}
			}
			return (uVal);
		}
	}

	inline uint64_t CombineHashes(const uint64_t& _uHash1, const uint64_t& _uHash2) noexcept
	{
		return _uHash1 ^ (_uHash1 + 0x9e3779b9ull + (_uHash2 << 6) + (_uHash2 >> 2));
	}

	template <typename char_t>
	inline uint64_t Hash(const std::basic_string<char_t>& _Str) noexcept
	{
		std::hash<std::basic_string<char_t>> func;
		return func(_Str);
	}

	template <typename T>
	inline uint64_t Hash(const T& _Val) noexcept
	{
		static_assert(std::is_pointer_v<T> == false, "Pointers can not be hashed");
		return detail::fnv1a_hash_impl(reinterpret_cast<const uint8_t*>(&_Val), sizeof(T));
	}

	template <typename U, typename... V>
	inline uint64_t Hash(const U& _Val1, const V&... _Tail) noexcept { return CombineHashes(Hash(_Val1), Hash(_Tail...)); }

	template <typename T>
	inline uint64_t AddHash(const uint64_t& _uHash, const T& _NewVal) noexcept
	{
		return CombineHashes(_uHash, Hash(_NewVal));
	}

	class Hasher
	{
	public:
		inline Hasher(const uint64_t& _uHash = 0u) noexcept : m_uHash(_uHash) {}
		inline operator uint64_t() const noexcept { return m_uHash; }

		template <class T>
		inline Hasher& operator<<(const T& _Val) noexcept
		{
			m_uHash = AddHash(m_uHash, _Val);
			return *this;
		}

		inline Hasher& operator+=(const uint64_t& _uHash) noexcept
		{
			m_uHash = CombineHashes(m_uHash, _uHash);
			return *this;
		}

		inline Hasher& operator=(const uint64_t& _uHash) noexcept { m_uHash = _uHash; }

		// override hash
		inline void SetHash(const uint64_t& _uHash) noexcept { m_uHash = _uHash; }

		inline Hasher& Combine(const uint64_t& _uHash) noexcept
		{
			m_uHash = CombineHashes(m_uHash, _uHash);
			return *this;
		}

		inline const uint64_t& GetHash() const noexcept { return m_uHash; }
	private:
		uint64_t m_uHash;
	};
} // !hlx

#endif // !HLX_HASHUTILS_H