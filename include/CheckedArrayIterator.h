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

#ifndef HLX_CHECKEDARRAYITERATOR_H
#define HLX_CHECKEDARRAYITERATOR_H

// http://en.cppreference.com/w/cpp/concept/RandomAccessIterator
#include <iterator>

namespace hlx
{
	// https://stackoverflow.com/questions/12092448/code-for-a-basic-random-access-iterator-based-on-pointers
	template <class T>
	class CheckedArrayIterator // : public std::iterator<std::random_access_iterator_tag, Type>
	{
	public:
		using value_type = typename std::remove_pointer<T>::type;
		using difference_type = std::ptrdiff_t; //  using difference_type = typename std::iterator<std::random_access_iterator_tag, Type>::difference_type;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator_category = std::random_access_iterator_tag;
		using _Unchecked_type = std::true_type; // just fuck with microsofts sdl checks

		template <size_t SIZE>
		constexpr CheckedArrayIterator(T(&_Iter)[SIZE], size_t _uOffset = 0ull) : 
			m_pStart(_Iter), m_pEnd(_Iter + SIZE-1), m_pCurrent(_Iter + _uOffset) {} //todo: assert / throw if out of bounds
		
		//unsave internal constructor
		CheckedArrayIterator(pointer _pStart, pointer _pCurrent, pointer _pEnd) :
		m_pStart(_pStart), m_pCurrent(_pCurrent), m_pEnd(_pEnd-1) {}

		constexpr CheckedArrayIterator(const CheckedArrayIterator& _Other) :
			m_pStart(_Other.m_pStart), m_pCurrent(_Other.m_pCurrent), m_pEnd(_Other.m_pEnd) {}
		~CheckedArrayIterator() {};

		CheckedArrayIterator& operator=(const CheckedArrayIterator& _Other)
		{
			m_pCurrent = _Other.m_pCurrent;
			m_pStart = _Other.m_pStart;
			m_pEnd = _Other.m_pEnd;
			return *this;
		}

		CheckedArrayIterator& operator++() { CheckAndSet(m_pCurrent+1); return *this; }
		CheckedArrayIterator operator++(int) { CheckedArrayIterator Ret(*this); CheckAndSet(m_pCurrent+1); return Ret; }
		CheckedArrayIterator& operator--() { CheckAndSet(m_pCurrent - 1); return *this; }
		CheckedArrayIterator operator--(int) { CheckedArrayIterator Ret(*this); CheckAndSet(m_pCurrent-1); return Ret; }
		
		CheckedArrayIterator operator-(difference_type _uOffset) const { CheckedArrayIterator Ret(*this); Ret -= _uOffset; return Ret; }
		CheckedArrayIterator operator+(difference_type _uOffset) const { CheckedArrayIterator Ret(*this); Ret += _uOffset; return Ret; }

		CheckedArrayIterator& operator+=(difference_type _uOffset) { CheckAndSet(m_pCurrent + _uOffset); return *this; }
		CheckedArrayIterator& operator-=(difference_type _uOffset) { CheckAndSet(m_pCurrent - _uOffset); return *this; }

		difference_type operator-(const CheckedArrayIterator& _Other) { return(m_pCurrent - _Other.m_pCurrent); }

		constexpr bool operator==(const CheckedArrayIterator& _other) const { return m_pStart == _other.m_pStart && m_pCurrent == _other.m_pCurrent; }
		constexpr bool operator!=(const CheckedArrayIterator& _other) const { assert(m_pStart == _other.m_pStart); return m_pStart == _other.m_pStart && m_pCurrent != _other.m_pCurrent; }
		constexpr bool operator<(const CheckedArrayIterator& _other) const { assert(m_pStart == _other.m_pStart); return m_pCurrent < _other.m_pCurrent; }
		constexpr bool operator>(const CheckedArrayIterator& _other) const { assert(m_pStart == _other.m_pStart); return m_pCurrent > _other.m_pCurrent; }
		constexpr bool operator<=(const CheckedArrayIterator& _other) const { assert(m_pStart == _other.m_pStart); return m_pCurrent <= _other.m_pCurrent; }
		constexpr bool operator>=(const CheckedArrayIterator& _other) const { assert(m_pStart == _other.m_pStart); return m_pCurrent >= _other.m_pCurrent; }
		
		reference operator*() { return *m_pCurrent; }
		pointer operator->() { return m_pCurrent; }
		//reference operator[](difference_type _uOffset) { return m_pCurrent[_uOffset]; }

	private:
		void CheckAndSet(pointer _pPtr)
		{
			if (_pPtr >= m_pStart && _pPtr <= m_pEnd)
			{
				m_pCurrent = _pPtr;
			}
		}

	private:
		pointer m_pStart;
		pointer m_pCurrent;
		pointer m_pEnd; // exclusive
	};

	template <typename T, size_t SIZE>
	inline constexpr CheckedArrayIterator<T> make_checked(T(&_Iter)[SIZE], size_t _uOffset = 0ull)
	{
		return CheckedArrayIterator<T>(_Iter, _uOffset);
	}
} //! hlx

#endif // !HLX_CHECKEDARRAYITERATOR_H
