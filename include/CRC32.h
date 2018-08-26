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

#ifndef HLX_CRC32_H
#define HLX_CRC32_H

#include <stdint.h>
#include <nmmintrin.h>

namespace hlx
{
	//CRC32
	inline uint32_t CRC32(const void* _pData, const size_t& _uLength)
	{
		size_t uCurSize = _uLength;
		uint64_t uCRC32 = 0u;

		const uint64_t* pData64 = reinterpret_cast<const uint64_t*>(_pData);
		const uint8_t* pData8 = reinterpret_cast<const uint8_t*>(_pData);

		// compute 64bit chunks
		while (uCurSize >= sizeof(uint64_t))
		{
			uCRC32 = _mm_crc32_u64(uCRC32, *pData64);
			uCurSize -= sizeof(uint64_t);
			++pData64;
		}

		//compute rest that does not fit into 64bit chunk
		for (; uCurSize > 0; --uCurSize)
		{
			uCRC32 = _mm_crc32_u8(static_cast<uint32_t>(uCRC32), pData8[_uLength - uCurSize]);
		}

		return static_cast<uint32_t>(uCRC32);
	}
} // hlx

#endif // !HLX_CRC32_H
