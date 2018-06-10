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

#ifndef HLX_TIMERWIN32_H
#define HLX_TIMERWIN32_H

#include "StandardDefines.h"
#include <stdint.h>
#include "String.h"

#include <time.h>
#include <iomanip>

namespace hlx
{
	class TimerWin32
	{
	public:
		TimerWin32();
		
		void Pause();
		void Unpause();
		void Reset();
		void FrameTick();

		double TotalTimeD();

		float ElapsedTimeF() const;
		double ElapsedTimeD() const;		

		template <typename char_t>
		static std::basic_string<char_t> GetLocalTimeString();
		static double GetSecondsPerTick();
		static float GetSecondsPerTickF();

		static double GetTicksPerSecond();
		static float GetTicksPerSecondF();

		static int64_t GetCurrentCount();
		static double GetGlobalTime();
		static float GetGlobalTimeF();

	private:
		double m_secPerCount;
		double m_elapsedTime;

		//per frame
		int64_t m_prevTime;
		int64_t m_curTime;

		int64_t m_startTime;//time the timer was started

		int64_t m_pausedTime;//time when timer was last paused
		int64_t m_unpausedTime;//time when tiemr was last unpaused
		int64_t m_totalPausedTime; //time interval the timer was paused

		bool m_paused;
	};
	//---------------------------------------------------------------------------------------------------

	inline double TimerWin32::GetSecondsPerTick()
	{
		int64_t iFreq;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&iFreq));
		return 1.0 / static_cast<double>(iFreq);
	}

	inline float TimerWin32::GetSecondsPerTickF()
	{
		int64_t iFreq;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&iFreq));
		return 1.f / static_cast<float>(iFreq);
	}

	inline double TimerWin32::GetTicksPerSecond()
	{
		int64_t iFreq;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&iFreq));
		return static_cast<double>(iFreq);
	}

	inline float TimerWin32::GetTicksPerSecondF()
	{
		int64_t iFreq;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&iFreq));
		return static_cast<float>(iFreq);
	}

	inline float TimerWin32::ElapsedTimeF() const { return static_cast<float>(m_elapsedTime); }
	inline double TimerWin32::ElapsedTimeD() const {return m_elapsedTime; }
	
	inline int64_t TimerWin32::GetCurrentCount()
	{
		int64_t cur;
		QueryPerformanceCounter((LARGE_INTEGER*)&cur);
		return cur;
	}

	inline double TimerWin32::GetGlobalTime()
	{
		return static_cast<double>(GetCurrentCount()) / GetTicksPerSecond();
	}

	inline float TimerWin32::GetGlobalTimeF()
	{
		return static_cast<float>(GetCurrentCount()) / GetTicksPerSecondF();
	}
	//---------------------------------------------------------------------------------------------------

	template <typename char_t>
	inline std::basic_string<char_t> TimerWin32::GetLocalTimeString()
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
	//---------------------------------------------------------------------------------------------------

	inline TimerWin32::TimerWin32()
	{
		m_paused = false;
		int64_t iFreq;
		QueryPerformanceFrequency((LARGE_INTEGER*)&iFreq);
		m_secPerCount = 1.0 / static_cast<double>(iFreq); //perform division once so we can use multiplication from now on.
		Reset();
	}
	//---------------------------------------------------------------------------------------------------

	//Call this function before using any other than the static functions above
	inline void TimerWin32::Reset()
	{
		//Restart the timer
		m_startTime = GetCurrentCount();
		m_prevTime = m_startTime;
		m_pausedTime = 0;
		m_totalPausedTime = 0;
		m_paused = false;
	}
	//---------------------------------------------------------------------------------------------------

	//call once per frame at start of computation
	inline void TimerWin32::FrameTick()
	{
		if (m_paused)
		{
			m_elapsedTime = 0.0; // no animation when game is paused
			return;
		}

		//aquire the current time
		QueryPerformanceCounter((LARGE_INTEGER*)&m_curTime);

		//compute elapsed time in seconds
		m_elapsedTime = (m_curTime - m_prevTime) * m_secPerCount;

		m_prevTime = m_curTime;

		//scheduler might produce negative timespan
		if (m_elapsedTime < 0.0)
			m_elapsedTime = 0.0;
	}
	//---------------------------------------------------------------------------------------------------

	inline void TimerWin32::Pause()
	{
		if (m_paused)
			return;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_pausedTime);
		m_paused = true;
	}
	//---------------------------------------------------------------------------------------------------

	inline void TimerWin32::Unpause()
	{
		if (m_paused == false)
			return;

		QueryPerformanceCounter((LARGE_INTEGER*)&m_prevTime);
		m_totalPausedTime += (m_prevTime - m_pausedTime);//accumulate the paused interval
		m_pausedTime = 0;
		m_paused = false;
	}
	//---------------------------------------------------------------------------------------------------

	inline double TimerWin32::TotalTimeD()
	{
		//(m_curTime - m_startTime) - m_totalPausedTime
		if (m_paused)
		{
			//don't count the time the game is currently paused so use m_pausedTime as curTime
			return ((m_pausedTime - m_startTime) - m_totalPausedTime)*m_secPerCount;
		}
		else
		{
			//aquire the current time
			QueryPerformanceCounter((LARGE_INTEGER*)&m_curTime);
			return ((m_curTime - m_startTime) - m_totalPausedTime)*m_secPerCount;
		}
	}
} // hlx

#endif // HLX_TIMERWIN32_H