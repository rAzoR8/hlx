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

#ifndef HLX_ITERATORRANGE_H
#define HLX_ITERATORRANGE_H

#include <iterator> // for std::distance

// http://en.cppreference.com/w/cpp/concept/Container

namespace hlx
{
	template <typename Iterator>
	struct IteratorRange
	{
		IteratorRange(const Iterator& _start, const Iterator& _end) :
			m_start(_start), m_end(_end) {}

		IteratorRange(const IteratorRange& _other) : m_start(_other.m_start), m_end(_other.m_end) {}
        IteratorRange(IteratorRange&& _other) : m_start(std::move(_other.m_start)), m_end(std::move(_other.m_end)) {}

        IteratorRange& operator=(const IteratorRange& _other)
        {
            m_start = _other.m_start;
            m_end = _other.m_end;
            return *this;
        }

        IteratorRange& operator=(IteratorRange&& _other)
        {
            m_start = std::move(_other.m_start);
            m_end = std::move(_other.m_end);
            return *this;
        }

		inline auto begin() const { return m_start; }
		inline auto end() const { return m_end; }

		inline auto cbegin() const { return m_start; }
		inline auto cend() const { return m_end; }

		inline auto size() const { return std::distance(m_end, m_start); }
		inline bool empty() const { return m_start == m_end; }

	private:
		Iterator m_start;
		Iterator m_end;
	};

	template <typename Iterator>
	inline IteratorRange<Iterator> make_range(Iterator&& _start, Iterator&& _end)
	{
		return IteratorRange<Iterator>(std::forward<Iterator>(_start), std::forward<Iterator>(_end));
	}

	// usage: for(auto& i : make_range(start, end){i++;}
} // hlx

#endif // !HLX_ITERATORRANGE_H
