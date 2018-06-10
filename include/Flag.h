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

#ifndef HLX_FLAG_H
#define HLX_FLAG_H

#include <type_traits>
namespace hlx
{
	template <class TEnum, class BaseType = std::underlying_type_t<TEnum>>
	class Flag
	{
	public:
		using TFlagType = Flag<TEnum, BaseType>;

		constexpr Flag(const Flag& _kFlag) noexcept : m_kFlag(_kFlag.m_kFlag) {}
		constexpr Flag(const BaseType _kFlag = BaseType(0)) noexcept : m_kFlag(_kFlag) {}
		constexpr Flag(const TEnum _kFlag) noexcept : m_kFlag(_kFlag) {}

		virtual ~Flag() {};

		inline void ResetFlag() noexcept { m_kFlag = BaseType(0); }

		inline constexpr bool None() const noexcept { return m_kFlag == BaseType(0); }

		inline void SetFlag(const Flag _kFlag) noexcept { m_kFlag |= _kFlag.m_kFlag; }
		inline void SetFlag(const TEnum _kFlag) noexcept { m_kFlag |= _kFlag; }
		inline void SetFlag(const BaseType _kFlag) noexcept { m_kFlag |= _kFlag; }

		inline void ClearFlag(const Flag _kFlag) noexcept { m_kFlag &= ~_kFlag.m_kFlag; }
		inline void ClearFlag(const TEnum _kFlag) noexcept { m_kFlag &= ~BaseType(_kFlag); }
		inline void ClearFlag(const BaseType _kFlag) noexcept { m_kFlag &= ~_kFlag; }

		inline constexpr bool CheckFlag(const Flag _kFlag) const noexcept { return m_kFlag & _kFlag.m_kFlag; }
		inline constexpr bool CheckFlag(const TEnum _kFlag) const noexcept { return m_kFlag & _kFlag; }
		inline constexpr bool CheckFlag(const BaseType _kFlag) const noexcept { return m_kFlag & _kFlag; }

		inline operator BaseType() const noexcept { return m_kFlag; }
		inline constexpr BaseType GetBase() const noexcept { return m_kFlag; }
		inline constexpr TEnum GetFlag() const noexcept { return static_cast<TEnum>(m_kFlag); }

	private:
		BaseType m_kFlag;
	};
} // hlx

#endif // !HLX_FLAG_H
