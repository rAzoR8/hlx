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

#ifndef HLX_VECTOR_H
#define HLX_VECTOR_H

#include <cstdint>
#include <type_traits>
#include <array>

namespace hlx
{
	template <typename T, size_t Dim = 2>
	struct VecType
	{
		VecType() {};

		template <typename... Tail>
		VecType(typename std::enable_if<sizeof...(Tail)+1 == Dim, T>::type&& head, Tail&&... tail)
			: Data{ std::forward<T>(head), std::forward<Tail>(tail)... } {}

		template <size_t size>
		VecType(const T(&_Other)[size])
		{
			std::copy(std::cbegin(_Other), std::cend(_Other), Data);
		}

		template <size_t size>
		VecType(const std::array<T, size>& _Other)
		{
			std::copy(std::cbegin(_Other), std::cend(_Other), Data);
		}

		template <size_t size>
		VecType& operator=(const T(&_Other)[size])
		{
			std::copy(std::cbegin(_Other), std::cend(_Other), Data);
			return *this;
		}

		template <size_t size>
		VecType& operator=(const std::array<T, size>& _Other)
		{
			std::copy(std::cbegin(_Other), std::cend(_Other), Data);
			return *this;
		}

		T Data[Dim] = {};
		inline T& operator[](const size_t n)
		{
			return Data[n];
		}
		inline const T& operator[](const size_t n) const
		{
			return Data[n];
		}
		static constexpr int Dimension = Dim;
	};

	using VecUInt2 = VecType<uint32_t, 2>;
	using VecUInt3 = VecType<uint32_t, 3>;
	using VecUInt4 = VecType<uint32_t, 4>;

	using VecInt2 = VecType<int32_t, 2>;
	using VecInt3 = VecType<int32_t, 3>;
	using VecInt4 = VecType<int32_t, 4>;

	using VecFloat2 = VecType<float, 2>;
	using VecFloat3 = VecType<float, 3>;
	using VecFloat4 = VecType<float, 4>;
} // hlx

#endif // HLX_VECTOR_H