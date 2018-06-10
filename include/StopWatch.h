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

#ifndef HLX_STOPWATCH_H
#define HLX_STOPWATCH_H

#include <chrono>
#include <iostream>
#include <string>

namespace hlx
{
	template <class ClockType = std::chrono::system_clock>
	class StopWatch
	{
	public:
		StopWatch(const std::string& _sInfo = {}) : m_Start(ClockType::now()), m_sInfo(_sInfo) {}
		~StopWatch()
		{ 
			if (m_sInfo.empty() == false)
			{
				const float fElapsed = Elapsed();
				std::cout << m_sInfo << ": " << fElapsed << "s" << std::endl;
			}
		}

		template <class Fp = float>
		inline Fp Elapsed() const { return std::chrono::duration<Fp>(ClockType::now() - m_Start).count(); }

	private:
		std::chrono::time_point<ClockType> m_Start;
		const std::string m_sInfo;
	};
};

#endif // !STOPWATCH_H
