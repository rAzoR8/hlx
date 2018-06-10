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

#ifndef HLX_UNIQUEID_H
#define HLX_UNIQUEID_H

//usage:
//global application context:
//uint32_t gid = UniqueID<uint32_t>::NextGlobalID();

//local class context:
//UniqueID<uint64_t> uID;
//uint64_t id = uID.NextID();

#include <atomic>
namespace hlx
{
	template <class T>
	class UniqueID
	{
	private:
		std::atomic<T> ID = T(0);
		inline static std::atomic<T> GlobalID = ATOMIC_VAR_INIT(0);
	public:
		inline T NextID()
		{
			return ID++;
		}
		inline T GetID()
		{
			return ID.load();
		}
		static inline T NextGlobalID()
		{
			return GlobalID++;
		}
		static inline T GetGlobalID()
		{
			return GlobalID.load();
		}
	};

//template<class T> T UniqueID<T>::GlobalID(0);
} // hlx

#endif // HLX_UNIQUEID_H