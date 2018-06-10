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

#ifndef HLX_FUNCTIONALUTILS_H
#define	HLX_FUNCTIONALUTILS_H

#include <type_traits>

namespace hlx
{
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#pragma region is_same_type
	template <class T, class... Ts>
	constexpr bool is_same_type(){return std::conjunction_v<std::is_same<T, Ts>...>;}

#pragma endregion

#pragma region is_of_type
	template <class T, class... Ts>
	constexpr bool is_of_type() { return std::disjunction_v<std::is_same<T, Ts>...>; }
#pragma endregion

#pragma region to_ref
	// this construct will always return a reference of type T, even from pointer types
	// example: to_ref(T* t).member or to_ref(T& t).member (-> is omitted when pointer type is unknown)

	template<class T>
	inline typename std::enable_if<std::is_pointer<T>::value, typename std::remove_pointer<T>::type& >::type to_ref(T& t) { return *t; }

	template<class T>
	inline std::enable_if_t<!std::is_pointer<T>::value, T&>	to_ref(T& t) { return t; }
#pragma endregion

#pragma region minmax

	template <class U, class V>
	inline constexpr auto max(const U& l, const V& r) { return l > r ? l : r; }

	template<class U, class V, typename... Args>
	inline constexpr auto max(const U& l, const V& r, const Args&... args){	return max(l > r ? l : r, args...);	}

	template <class U, class V>
	inline constexpr auto min(const U& l, const V& r) { return l < r ? l : r; }

	template<class U, class V, typename... Args>
	inline constexpr auto min(const U& l, const V& r, const Args&... args){	return min(l < r ? l : r, args...);	}
#pragma endregion

#ifndef HFUNCTOREX
#define HFUNCTOREX(_name, _args, _impl) struct _name { inline auto operator()(_args) { _impl;} };
#endif

#ifndef HFUNCTOR
#define HFUNCTOR(_name, _impl) struct _name { inline auto operator() _impl };
#endif

} // !hlx

#endif // !HLX_FUNCTIONALUTILS_H
