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

#ifndef HLX_ENUMOPS_H
#define HLX_ENUMOPS_H

#include <type_traits>
namespace hlx
{
	template <typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
	constexpr EnumType operator|(EnumType f0, EnumType f1)
	{
		return EnumType(std::underlying_type_t<EnumType>(f0) | std::underlying_type_t<EnumType>(f1));
	}

	template <typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
	constexpr EnumType operator&(EnumType f0, EnumType f1)
	{
		return EnumType(std::underlying_type_t<EnumType>(f0) & std::underlying_type_t<EnumType>(f1));
	}

	template <typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
	constexpr EnumType operator^(EnumType f0, EnumType f1)
	{
		return EnumType(std::underlying_type_t<EnumType>(f0) ^ std::underlying_type_t<EnumType>(f1));
	}
}; //hlx

#endif // !HLX_ENUMOPS_H
