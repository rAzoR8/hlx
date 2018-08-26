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

#ifndef HLX_TIMER_H
#define HLX_TIMER_H

#include <stdint.h>

#ifndef __STDC_WANT_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1
#endif

#include <time.h>
#include <iomanip>
#include <chrono>

namespace hlx
{
	class Timer
	{
	public:
		using TClock = std::chrono::system_clock;
		using TTimePoint = std::chrono::time_point<TClock>;

		Timer() noexcept;
		
		void Reset() noexcept;

		float ElapsedTimeF() const noexcept;
		double ElapsedTimeD() const noexcept;

		template <typename char_t>
		static std::basic_string<char_t> GetLocalTimeString();

		template <class Fp = float>
		inline Fp Elapsed() const noexcept;

		static int64_t GetCurrentCount() noexcept;
		static double GetGlobalTime() noexcept;
		static float GetGlobalTimeF() noexcept;

	private:
		TTimePoint m_startTime; //time the timer was started
	};

	//---------------------------------------------------------------------------------------------------
	// constructor
	inline Timer::Timer() noexcept : m_startTime(TClock::now())
	{
	}

	//---------------------------------------------------------------------------------------------------

	//Call this function before using any other than the static functions above
	inline void Timer::Reset() noexcept
	{
		//Restart the timer
		m_startTime = TClock::now();
	}

	//---------------------------------------------------------------------------------------------------

	template <class Fp>
	inline Fp Timer::Elapsed() const  noexcept { return std::chrono::duration<Fp>(TClock::now() - m_startTime).count(); }

	inline float Timer::ElapsedTimeF() const noexcept { return Elapsed<float>(); }
	inline double Timer::ElapsedTimeD() const noexcept {return Elapsed<double>(); }
	
	inline int64_t Timer::GetCurrentCount() noexcept
	{
		return clock();
	}

	inline double Timer::GetGlobalTime() noexcept
	{
		return static_cast<double>(GetCurrentCount()) / static_cast<double>(CLOCKS_PER_SEC);
	}

	inline float Timer::GetGlobalTimeF() noexcept
	{
		return static_cast<float>(GetCurrentCount()) / static_cast<float>(CLOCKS_PER_SEC);
	}
	//---------------------------------------------------------------------------------------------------

	template <typename char_t>
	inline std::basic_string<char_t> Timer::GetLocalTimeString()
	{
		time_t rawtime;
		time(&rawtime);
		struct tm timeinfo;

		if (localtime_s(&timeinfo, &rawtime) == 0)
		{
			std::basic_stringstream<char_t, std::char_traits<char_t>, std::allocator<char_t>> sstream;
			sstream << std::put_time(&timeinfo, ST(char_t, "%H::%M::%S"));
			return sstream.str();
		}
		else
		{
			return{};
		}		
	}
} // hlx

#endif // HLX_TIMER_H