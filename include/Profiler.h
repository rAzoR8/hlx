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

#ifndef HLX_PROFILER_H
#define HLX_PROFILER_H

#include "Singleton.h"
#include "StandardDefines.h"
#include "Logger.h"
#include "TimerWin32.h"
#include <concurrent_unordered_map.h>

namespace hlx
{
	class Profiler : public hlx::Singleton<Profiler>
	{
		struct Stats
		{
			uint64_t m_uMin = ULLONG_MAX;
			uint64_t m_uMax = 0;
			uint64_t m_uTotal = 0;
			uint64_t m_uTotalAvg = 0; // average of all intervals
			uint64_t m_uIntervalStart = 0;
			uint64_t m_uIntervalSamples = 0;
			uint32_t m_uSamples = 0; // Number of measurements
			bool m_bTime = true;
		};

		using TTimeMap = concurrency::concurrent_unordered_map<std::string, Stats>;

	public:
		Profiler() {};
		~Profiler() {};

		void AddTiming(const std::string& _sFuncName, int64_t _iTickCount, uint32_t _uInterval = 60u, bool _bTime = true);

		//For maximum accuracy, call this function at the end of all profiling operations
		void PrintStatsToLog();

	private:
		TTimeMap m_Timings;
	};

	//---------------------------------------------------------------------------------------------------
	inline void Profiler::AddTiming(const std::string& _sFuncName, int64_t iTickCount, uint32_t _uInterval, bool _bTime)
	{
		TTimeMap::iterator it = m_Timings.find(_sFuncName);

		if (it != m_Timings.end())
		{
			Stats& Stats = it->second;
			Stats.m_uTotal += static_cast<uint64_t>(iTickCount);
			Stats.m_uMin = std::min(Stats.m_uMin, static_cast<uint64_t>(iTickCount));
			Stats.m_uMax = std::max(Stats.m_uMax, static_cast<uint64_t>(iTickCount));
			++Stats.m_uSamples;

			if (Stats.m_bTime && Stats.m_uSamples % _uInterval == 0)
			{
				++Stats.m_uIntervalSamples;
				Stats.m_uTotalAvg += static_cast<uint64_t>(std::round(static_cast<double>(Stats.m_uTotal - Stats.m_uIntervalStart) / static_cast<double>(_uInterval)));
				Stats.m_uIntervalStart = Stats.m_uTotal;
			}
		}
		else
		{
			Stats Stats;
			Stats.m_uSamples = 1;
			Stats.m_uTotalAvg = 0;
			Stats.m_uTotal = static_cast<uint64_t>(iTickCount);
			Stats.m_uMin = static_cast<uint64_t>(iTickCount);
			Stats.m_uMax = static_cast<uint64_t>(iTickCount);
			Stats.m_bTime = _bTime;
			Stats.m_uIntervalSamples = 0;
			m_Timings.insert(std::make_pair(_sFuncName, Stats));
		}
	}
	//---------------------------------------------------------------------------------------------------
	inline void Profiler::PrintStatsToLog()
	{
		HLOG("========== PERFORMANCE-STATS =======");

		double fSecPerTick = TimerWin32::GetSecondsPerTick();
		for (TTimeMap::iterator it = m_Timings.begin(); it != m_Timings.end(); ++it)
		{
			Stats& Stats = it->second;

			if (Stats.m_bTime)
			{
				double fTotalTime = Stats.m_uTotal * fSecPerTick;
				double fMinTime = Stats.m_uMin * fSecPerTick * 1000.f; //millisec
				double fMaxTime = Stats.m_uMax * fSecPerTick * 1000.f; //millisec

				uint64_t uTotalAvg = Stats.m_uTotalAvg == 0 ? Stats.m_uTotal : Stats.m_uTotalAvg;
				uint64_t uIntervalSamples = Stats.m_uIntervalSamples == 0 ? Stats.m_uSamples : Stats.m_uIntervalSamples;
				double fAvgIntervalTime = (static_cast<double>(uTotalAvg) / static_cast<double>(uIntervalSamples)) * 1000.f * fSecPerTick; //millisec

				HLOG("%s AvgFPS %f [%fms] Max %fms Min %fms Total %fs Calls %lu", CSTR(it->first), 1000.f / fAvgIntervalTime, fAvgIntervalTime, fMaxTime, fMinTime, fTotalTime, Stats.m_uSamples);
			}
			else
			{
				uint64_t uAvg = static_cast<uint64_t>(static_cast<double>(Stats.m_uTotal) / static_cast<double>(Stats.m_uSamples));
				HLOG("%s Avg %lu Max %lu Min %lu Total %lu Samples %lu", CSTR(it->first), uAvg, Stats.m_uMax, Stats.m_uMin, Stats.m_uSamples, Stats.m_uSamples);
			}
		}

		HLOG("========== PERFORMANCE-STATS =======");
	}


#ifndef _FINAL
	//No whitespaces and escape seqs allowed!
#define HPROFILE(_Name) int64_t iStartTime##_Name = hlx::Timer::GetCurrentCount();
#define HPROFILE_END(_Name) hlx::Profiler::Instance()->AddTiming(#_Name, hlx::Timer::GetCurrentCount() - iStartTime##_Name);
#define HPROFILE_COUNT(_Name, _uCount) hlx::Profiler::Instance()->AddTiming(#_Name, _uCount,0u, false);
#define HPROFILE_PRINT() hlx::Profiler::Instance()->PrintStatsToLog();

//#define HCVSERIES(_Name) concurrency::diagnostic::marker_series _Name##Series(TEXT(#_Name));
//#define HCVSPANEX(_Name, _Series) concurrency::diagnostic::span _NameSpan(_Series##Series, 1,TEXT(#_Name));
//
//#define HCVSPAN(_Name) concurrency::diagnostic::marker_series _Name##Series(TEXT(#_Name)); concurrency::diagnostic::span* _Name##Span = new concurrency::diagnostic::span(_Name##Series, 1,TEXT(#_Name));
//
//#define HPROFILEEX(_Name) int64_t iStartTime##_Name = hlx::Timer::GetCurrentCount(); HCVSPAN(_Name)
//#define HPROFILEEX_END(_Name) Helix::Profiler::Instance()->AddTiming(#_Name, hlx::Timer::GetCurrentCount() - iStartTime##_Name); HSAFE_DELETE(_Name##Span);

#elif

#define HPROFILE_COUNT(_Name, _uCount) 
#define HPROFILE(_Name)
#define HPROFILE_END(_Name)
#define HPROFILE_PRINT()

//#define HCVSERIES(_Name)
//#define HCVSPANEX(_Name, _Series)
//#define HCVSPAN(_Name)

#endif

} // hlx

#endif // HLX_PROFILER_H