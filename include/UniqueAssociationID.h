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

#ifndef HLX_UNIQUEASSOCIATIONID_H
#define HLX_UNIQUEASSOCIATIONID_H

#include "UniqueID.h"
#include <unordered_map>
#include <type_traits>

namespace hlx
{
	template <class T, class IDType = size_t, class HashFunc = std::hash<T>>
	class UniqueAssociationID
	{
		using HashType = std::invoke_result_t<HashFunc, T>;

		struct DefaultReduction
		{
			inline const HashType& operator()(const HashType& _ID) const { return _ID; }
		};

		using TAssociationContainer = std::unordered_map<HashType, IDType, DefaultReduction>;
	public:

		inline IDType GetAssociatedID(const T& _AssociatedType)
		{
			HashType Hash = m_HashFunc(_AssociatedType);

			auto it = m_Association.find(Hash);
			if (it != m_Association.end())
			{
				return it->second;
			}

			IDType ID = m_UniqueID.NextID();

			m_Association.insert({ Hash, ID });

			return ID;
		}

		inline IDType operator()(const T& _AssociatedType) { return GetAssociatedID(_AssociatedType); }

		inline size_t GetAssociationCount() const { return m_Association.size(); }

	private:
		UniqueID<IDType> m_UniqueID;
		HashFunc m_HashFunc;
		TAssociationContainer m_Association;
	};
} // hlx

#endif // !HLX_UNIQUEASSOCIATIONID_H