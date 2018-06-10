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

// All defines of this header are in the global scope outside the hlx namespace

#ifndef HLX_STANDARDDEFINES_H
#define HLX_STANDARDDEFINES_H

//---------------------------------------------------------------------------------------------------
// Mem Debugging

#ifdef _DEBUG

#if !defined(NO_CRT_DEBUG) && defined(_WIN32)

//#ifndef _CRTDBG_MAP_ALLOC
//#define _CRTDBG_MAP_ALLOC
//#endif

#include <stdlib.h>
#include <crtdbg.h>

inline void HInitDebug(void) noexcept
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF /*| _CRTDBG_CHECK_ALWAYS_DF*/);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
}

inline void HBreakOnAlloc(long iAllocId) noexcept
{
	_CrtSetBreakAlloc(iAllocId);
}

#else //NO_CRT_DEBUG

#define HInitDebug()
#define HBreakOnAlloc(x)

#endif //!NO_CRT_DEBUG


#ifndef HNEW
#define HNEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#endif

#if defined(USE_DEBUG_HNEW) && defined (HNEW)
#define new HNEW 
#endif

#define HDEBUGNAME(_name)

//inline compile only if in debug mode
#define HDBG(x) x
#endif  // _DEBUG

#if defined(_NDEBUG) && defined(_WIN32)

#ifndef NDEBUG
#define NDEBUG 1//suppress assert()
#endif

#ifndef HNEW
#define HNEW new
#endif

#include <assert.h> //assert

#define HInitDebug()
#define HDEBUGNAME(_name)
#define HDBG(x)

#endif // _NDEBUG

//---------------------------------------------------------------------------------------------------
// DLL 

#ifdef HDYNAMIC_LINKAGE

#ifdef HDLL_EXPORT
#define HABI __declspec(dllexport)
#else
#define HABI __declspec(dllimport)
#endif

#else

#define HABI

#endif

//---------------------------------------------------------------------------------------------------
// Windows
#ifdef _WIN32

// if windows.h has been included before this header, undef existing macro
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define	WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <ppl.h>

#endif //!_WIN32

//---------------------------------------------------------------------------------------------------
// Standard includes

#include <algorithm>

template <class Container, class Pred, class Iterator = Container::iterator>
inline void RemoveIf(Container& _Container, const Pred& _Pred)
{
	Iterator it = std::remove_if(_Container.begin(), _Container.end(), _Pred);
	if (it != _Container.end())
	{
		_Container.erase(it);
	}
}

template <class Container, class Value = Container::value_type>
inline void RemoveIfValue(Container& _Container, const Value& _ValueToErase)
{
	RemoveIf(_Container, [&_ValueToErase](const Value& _Other) {return _ValueToErase == _Other; });
}

//---------------------------------------------------------------------------------------------------
// misc types & functions

template <class Type, class Element, size_t N = sizeof(Type) / sizeof(Element)>
union UnionCastType
{
	struct
	{
		Element Values[N];
	};

	Type Value;
};

template <class Type, class Element, size_t N = sizeof(Type) / sizeof(Element)>
Element& AccessUnionElement(Type& _Union, const size_t _Index)
{
	return (reinterpret_cast<UnionCastType<Type, Element, N>&>(_Union)).Values[_Index];
}

template <class Type, class Element, size_t N = sizeof(Type) / sizeof(Element)>
const Element& AccessUnionElementConst(const Type& _Union, const size_t _Index)
{
	return (reinterpret_cast<const UnionCastType<Type, Element, N>&>(_Union)).Values[_Index];
}

using union_u32_u16 = UnionCastType<uint32_t, uint16_t>;
using union_u32_u8 = UnionCastType<uint32_t, uint8_t>;

using union_i32_i16 = UnionCastType<int32_t, int16_t>;
using union_i32_i8 = UnionCastType<int32_t, int8_t>;

using union_u64_u32 = UnionCastType<uint64_t, uint32_t>;
using union_u64_u16 = UnionCastType<uint64_t, uint16_t>;
using union_u64_u8 = UnionCastType<uint64_t, uint8_t>;

using union_i64_i32 = UnionCastType<int64_t, int32_t>;
using union_i64_i16 = UnionCastType<int64_t, int16_t>;
using union_i64_i8 = UnionCastType<int64_t, int8_t>;

struct DefaultInitializerType {};
static constexpr DefaultInitializerType DefaultInit;

#ifndef HNULLSTR
#define HNULLSTR(ptr) (((ptr) != nullptr) ? (ptr) : "" )
#endif

#ifndef HNULLSTRW
#define HNULLSTRW(ptr) (((ptr) != nullptr) ? (ptr) : L"" )
#endif

#ifndef HUNDEFINED
#define HUNDEFINED -1
#define HUNDEFINED32 0xffffffffu
#define HUNDEFINED64 0xffffffffffffffffull
#endif

static constexpr size_t kUndefinedSizeT = std::numeric_limits<size_t>::max();

// platform abstraction handle
using THandle = size_t;

#ifdef HNUCLEO
#define HNCL(x) x
#else
#define HNCL(x)
#endif

// alternative to _countof
template<typename T, size_t size>
inline constexpr size_t GetArrayLength(const T(&)[size]){return size;}

//---------------------------------------------------------------------------------------------------
// for each

// HFOREACH usage :
//std::vector<SomeClass> vec;
//HFOREACH(it,end,vec)
//{
//	it->Member++
//}

// why use this instead of this ?
//for (auto &val : container)
//{
//}

//our macro enforces the convention to get the end iterator once before iterating, not in every iteration.
//it also forces the end iterator to be const, this also means that the vector size should not be changed during iteration.

#ifndef HFOREACH
#define HFOREACH(_it,_end,_container) { auto _it = _container.begin();auto _end = _container.end(); for(;_it != _end; ++_it){
#endif

#ifndef HFOREACH_END
#define HFOREACH_END }}
#endif

#ifndef HFOREACH_TYPE
#define HFOREACH_TYPE(_it,_end,_container,_type) { _type::iterator _it = _container.begin(); _type::iterator _end = _container.end(); for(;_it != _end; ++_it){
#endif

#ifndef HFOREACH_TYPE_END
#define HFOREACH_TYPE_END }}
#endif

#ifndef HFOREACH_CONST
#define HFOREACH_CONST(_it,_end,_container) { auto _it = _container.begin();const auto _end = _container.cend(); for(;_it != _end; ++_it){
#endif

#ifndef HFOREACH_CONST_END
#define HFOREACH_CONST_END }}
#endif

#ifndef HFOREACH_CONST_TYPE
#define HFOREACH_CONST_TYPE(_it,_end,_container,_type) { _type::iterator _it = _container.begin();const _type::iterator _end = _container.cend(); for(;_it != _end; ++_it){
#endif

#ifndef HFOREACH_CONST_TYPE_END
#define HFOREACH_CONST_TYPE_END }}
#endif

// linux?
//#ifndef HFOREACH_PARALLEL
//#define HFOREACH_PARALLEL(_val, _begin, _end, _container_type)\
//	Concurrency::parallel_for(_begin,_end, [&](const _container_type::value_type& _val) {
//#endif
//
//#ifndef HFOREACH_PARALLEL_END
//#define HFOREACH_PARALLEL_END });
//#endif

//---------------------------------------------------------------------------------------------------
// safe delete & release

#ifndef HSAFE_DELETE
#define HSAFE_DELETE(x) if((x) != nullptr) {delete (x); (x) = nullptr;}
#endif

#ifndef HSAFE_FUNC_RELEASE
#define HSAFE_FUNC_RELEASE(x, _func) if((x) != nullptr) {(x)->_func(); (x) = nullptr;}
#endif

#ifndef HSAFE_DELETE_ARRAY
#define HSAFE_DELETE_ARRAY(x) for(uint32_t i = 0; i < GetArrayLength(x); ++i) { if((x)[i] != nullptr) {delete (x)[i];} (x)[i] = nullptr;}
#endif

#ifndef HSAFE_RELEASE
#define HSAFE_RELEASE(x) if((x) != nullptr){ (x)->Release(); (x) = nullptr;}
#endif

#ifndef HSAFE_MAP_RELEASE
#define HSAFE_MAP_RELEASE(x) \
HFOREACH(it, end, x) \
HSAFE_RELEASE(it->second); \
HFOREACH_END
#endif

#ifndef HSAFE_MAP_DELETE
#define HSAFE_MAP_DELETE(x) \
HFOREACH(it, end, x) \
HSAFE_DELETE(it->second); \
HFOREACH_END
#endif

#ifndef HSAFE_VECTOR_RELEASE
#define HSAFE_VECTOR_RELEASE(x) \
HFOREACH(it, end, x) \
HSAFE_RELEASE(*it); \
HFOREACH_END
#endif

#ifndef HSAFE_VECTOR_DELETE
#define HSAFE_VECTOR_DELETE(x) \
HFOREACH(it, end, x) \
HSAFE_DELETE(*it); \
HFOREACH_END
#endif

#ifndef HSAFE_VECTOR_FUNC_DELETE
#define HSAFE_VECTOR_FUNC_DELETE(x, func) \
HFOREACH(it, end, x) \
HSAFE_FUNC_RELEASE(*it, func); \
HFOREACH_END
#endif

#endif //HLX_STANDARDDEFINES_H