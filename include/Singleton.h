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

#ifndef HLX_SINGLETON_H
#define HLX_SINGLETON_H

#include <atomic>
#include <memory>

namespace hlx
{
    //Usage:
    //class A : public hlx::Singleton<A> {
    //	A() {
    //		std::cout << "A constructed!\n";
    //	}
    //
    //	~A() {
    //		std::cout << "A destructed!\n";
    //	}
    //	int k;
    //};

    template<typename T>
    class Singleton
    {
        static std::shared_ptr<T> m_pSingletonInstance;

    public:
		virtual ~Singleton() { m_pSingletonInstance = nullptr; };

		template <class... Ts>
        static T* Instance(Ts&& ... args)
        {
			std::shared_ptr<T> pInstance(std::atomic_load_explicit(&m_pSingletonInstance, std::memory_order_acquire));

            if (pInstance == nullptr)
            {
                std::shared_ptr<T> pNewInstance(std::make_shared<T>(std::forward<Ts>(args)...));

                std::atomic_compare_exchange_strong_explicit(&m_pSingletonInstance, &pInstance, pNewInstance, std::memory_order_release, std::memory_order_acquire);

                return std::atomic_load_explicit(&m_pSingletonInstance, std::memory_order_acquire).get();
            }

            return pInstance.get();
        }
    };

    template<typename T>
    std::shared_ptr<T> Singleton<T>::m_pSingletonInstance = nullptr;
} //hlx

#endif // HLX_SINGLETON_H
