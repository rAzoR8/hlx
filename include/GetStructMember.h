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

#ifndef HLX_GETSTRUCTMEMBER_H
#define HLX_GETSTRUCTMEMBER_H

#include <type_traits>

namespace hlx
{
#pragma region construct_arity
	struct anything {
		template<class T> operator T()const;
	};

	namespace details {
		template<class T, class Is, class = void>
		struct can_construct_with_N :std::false_type {};

		template<class T, std::size_t ...Is>
		struct can_construct_with_N<T, std::index_sequence<Is...>, std::void_t< decltype(T{ (void(Is),anything{})... }) >> :
			std::true_type
		{};
	}
	template<class T, std::size_t N>
	using can_construct_with_Nx = details::can_construct_with_N<T, std::make_index_sequence<N>>;

	namespace details {
		template<std::size_t Min, std::size_t Range, template<std::size_t N>class target>
		struct maximize :
			std::conditional_t<
			(maximize<Min, Range / 2, target>{}) == (Min + Range / 2) - 1,
			maximize<Min + Range / 2, (Range + 1) / 2, target>,
			maximize<Min, Range / 2, target>
			>
		{};
		template<std::size_t Min, template<std::size_t N>class target>
		struct maximize<Min, 1, target> :
			std::conditional_t<
			(target<Min>{}),
			std::integral_constant<std::size_t, Min>,
			std::integral_constant<std::size_t, Min - 1>
			>
		{};
		template<std::size_t Min, template<std::size_t N>class target>
		struct maximize<Min, 0, target> :
			std::integral_constant<std::size_t, Min - 1>
		{};

		template<class T>
		struct construct_searcher {
			template<std::size_t N>
			using result = can_construct_with_Nx<T, N>;
		};
	}

	template<class T, std::size_t Cap = 32>
	using construct_airity = details::maximize< 0, Cap, details::construct_searcher<T>::template result >;
#pragma endregion

	namespace detail
	{
		struct filler { template< typename type > operator type && (); };
		template< typename aggregate,
			typename index_sequence = std::index_sequence<>,
			typename = void >
			struct aggregate_arity : index_sequence
		{};

		template< typename aggregate,
			std::size_t ...indices >
			struct aggregate_arity < aggregate,
			std::index_sequence< indices... >,
			std::void_t< decltype(aggregate{ (indices, std::declval< filler >())..., std::declval< filler >() }) > >
			: aggregate_arity< aggregate, std::index_sequence< indices..., sizeof...(indices) > >
		{};
	}

	template <class T>
	constexpr size_t aggregate_arity = detail::aggregate_arity<std::remove_reference_t<std::remove_cv_t<T>>>::size();

	template< std::size_t index, typename type >
	constexpr decltype(auto) get(type& value) noexcept
	{
	constexpr std::size_t arity = aggregate_arity<type>;
	static_assert(index < arity, "Invalid Index");

	if constexpr (arity == 1) {
		if constexpr (index == 0) {
			auto &[p0] = value;
			return (p0);
		} else {
			return;
		}
	}

	if constexpr (arity == 2) {
		auto &[p0, p1] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else {
			return;
		}
	}

	if constexpr (arity == 3) {
		auto &[p0, p1, p2] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else {
			return;
		}
	}

	if constexpr (arity == 4) {
		auto &[p0, p1, p2, p3] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else {
			return;
		}
	}

	if constexpr (arity == 5) {
		auto &[p0, p1, p2, p3, p4] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else {
			return;
		}
	}

	if constexpr (arity == 6) {
		auto &[p0, p1, p2, p3, p4, p5] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else {
			return;
		}
	}

	if constexpr (arity == 7) {
		auto &[p0, p1, p2, p3, p4, p5, p6] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else {
			return;
		}
	}

	if constexpr (arity == 8) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else {
			return;
		}
	}

	if constexpr (arity == 9) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else {
			return;
		}
	}

	if constexpr (arity == 10) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else {
			return;
		}
	}

	if constexpr (arity == 11) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else {
			return;
		}
	}

	if constexpr (arity == 12) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else {
			return;
		}
	}

	if constexpr (arity == 13) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else {
			return;
		}
	}

	if constexpr (arity == 14) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else {
			return;
		}
	}

	if constexpr (arity == 15) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else {
			return;
		}
	}

	if constexpr (arity == 16) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else {
			return;
		}
	}

	if constexpr (arity == 17) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else {
			return;
		}
	}

	if constexpr (arity == 18) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else {
			return;
		}
	}

	if constexpr (arity == 19) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else {
			return;
		}
	}

	if constexpr (arity == 20) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else 	if constexpr (index == 19) {
			return (p19);
		} else {
			return;
		}
	}

	if constexpr (arity == 21) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else 	if constexpr (index == 19) {
			return (p19);
		} else 	if constexpr (index == 20) {
			return (p20);
		} else {
			return;
		}
	}

	if constexpr (arity == 22) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else 	if constexpr (index == 19) {
			return (p19);
		} else 	if constexpr (index == 20) {
			return (p20);
		} else 	if constexpr (index == 21) {
			return (p21);
		} else {
			return;
		}
	}

	if constexpr (arity == 23) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else 	if constexpr (index == 19) {
			return (p19);
		} else 	if constexpr (index == 20) {
			return (p20);
		} else 	if constexpr (index == 21) {
			return (p21);
		} else 	if constexpr (index == 22) {
			return (p22);
		} else {
			return;
		}
	}

	if constexpr (arity == 24) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else 	if constexpr (index == 19) {
			return (p19);
		} else 	if constexpr (index == 20) {
			return (p20);
		} else 	if constexpr (index == 21) {
			return (p21);
		} else 	if constexpr (index == 22) {
			return (p22);
		} else 	if constexpr (index == 23) {
			return (p23);
		} else {
			return;
		}
	}

	if constexpr (arity == 25) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else 	if constexpr (index == 19) {
			return (p19);
		} else 	if constexpr (index == 20) {
			return (p20);
		} else 	if constexpr (index == 21) {
			return (p21);
		} else 	if constexpr (index == 22) {
			return (p22);
		} else 	if constexpr (index == 23) {
			return (p23);
		} else 	if constexpr (index == 24) {
			return (p24);
		} else {
			return;
		}
	}

	if constexpr (arity == 26) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else 	if constexpr (index == 19) {
			return (p19);
		} else 	if constexpr (index == 20) {
			return (p20);
		} else 	if constexpr (index == 21) {
			return (p21);
		} else 	if constexpr (index == 22) {
			return (p22);
		} else 	if constexpr (index == 23) {
			return (p23);
		} else 	if constexpr (index == 24) {
			return (p24);
		} else 	if constexpr (index == 25) {
			return (p25);
		} else {
			return;
		}
	}

	if constexpr (arity == 27) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else 	if constexpr (index == 19) {
			return (p19);
		} else 	if constexpr (index == 20) {
			return (p20);
		} else 	if constexpr (index == 21) {
			return (p21);
		} else 	if constexpr (index == 22) {
			return (p22);
		} else 	if constexpr (index == 23) {
			return (p23);
		} else 	if constexpr (index == 24) {
			return (p24);
		} else 	if constexpr (index == 25) {
			return (p25);
		} else 	if constexpr (index == 26) {
			return (p26);
		} else {
			return;
		}
	}

	if constexpr (arity == 28) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else 	if constexpr (index == 19) {
			return (p19);
		} else 	if constexpr (index == 20) {
			return (p20);
		} else 	if constexpr (index == 21) {
			return (p21);
		} else 	if constexpr (index == 22) {
			return (p22);
		} else 	if constexpr (index == 23) {
			return (p23);
		} else 	if constexpr (index == 24) {
			return (p24);
		} else 	if constexpr (index == 25) {
			return (p25);
		} else 	if constexpr (index == 26) {
			return (p26);
		} else 	if constexpr (index == 27) {
			return (p27);
		} else {
			return;
		}
	}

	if constexpr (arity == 29) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else 	if constexpr (index == 19) {
			return (p19);
		} else 	if constexpr (index == 20) {
			return (p20);
		} else 	if constexpr (index == 21) {
			return (p21);
		} else 	if constexpr (index == 22) {
			return (p22);
		} else 	if constexpr (index == 23) {
			return (p23);
		} else 	if constexpr (index == 24) {
			return (p24);
		} else 	if constexpr (index == 25) {
			return (p25);
		} else 	if constexpr (index == 26) {
			return (p26);
		} else 	if constexpr (index == 27) {
			return (p27);
		} else 	if constexpr (index == 28) {
			return (p28);
		} else {
			return;
		}
	}

	if constexpr (arity == 30) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else 	if constexpr (index == 19) {
			return (p19);
		} else 	if constexpr (index == 20) {
			return (p20);
		} else 	if constexpr (index == 21) {
			return (p21);
		} else 	if constexpr (index == 22) {
			return (p22);
		} else 	if constexpr (index == 23) {
			return (p23);
		} else 	if constexpr (index == 24) {
			return (p24);
		} else 	if constexpr (index == 25) {
			return (p25);
		} else 	if constexpr (index == 26) {
			return (p26);
		} else 	if constexpr (index == 27) {
			return (p27);
		} else 	if constexpr (index == 28) {
			return (p28);
		} else 	if constexpr (index == 29) {
			return (p29);
		} else {
			return;
		}
	}

	if constexpr (arity == 31) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else 	if constexpr (index == 19) {
			return (p19);
		} else 	if constexpr (index == 20) {
			return (p20);
		} else 	if constexpr (index == 21) {
			return (p21);
		} else 	if constexpr (index == 22) {
			return (p22);
		} else 	if constexpr (index == 23) {
			return (p23);
		} else 	if constexpr (index == 24) {
			return (p24);
		} else 	if constexpr (index == 25) {
			return (p25);
		} else 	if constexpr (index == 26) {
			return (p26);
		} else 	if constexpr (index == 27) {
			return (p27);
		} else 	if constexpr (index == 28) {
			return (p28);
		} else 	if constexpr (index == 29) {
			return (p29);
		} else 	if constexpr (index == 30) {
			return (p30);
		} else {
			return;
		}
	}

	if constexpr (arity == 32) {
		auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31] = value;
		if constexpr (index == 0) {
			return (p0);
		} else 	if constexpr (index == 1) {
			return (p1);
		} else 	if constexpr (index == 2) {
			return (p2);
		} else 	if constexpr (index == 3) {
			return (p3);
		} else 	if constexpr (index == 4) {
			return (p4);
		} else 	if constexpr (index == 5) {
			return (p5);
		} else 	if constexpr (index == 6) {
			return (p6);
		} else 	if constexpr (index == 7) {
			return (p7);
		} else 	if constexpr (index == 8) {
			return (p8);
		} else 	if constexpr (index == 9) {
			return (p9);
		} else 	if constexpr (index == 10) {
			return (p10);
		} else 	if constexpr (index == 11) {
			return (p11);
		} else 	if constexpr (index == 12) {
			return (p12);
		} else 	if constexpr (index == 13) {
			return (p13);
		} else 	if constexpr (index == 14) {
			return (p14);
		} else 	if constexpr (index == 15) {
			return (p15);
		} else 	if constexpr (index == 16) {
			return (p16);
		} else 	if constexpr (index == 17) {
			return (p17);
		} else 	if constexpr (index == 18) {
			return (p18);
		} else 	if constexpr (index == 19) {
			return (p19);
		} else 	if constexpr (index == 20) {
			return (p20);
		} else 	if constexpr (index == 21) {
			return (p21);
		} else 	if constexpr (index == 22) {
			return (p22);
		} else 	if constexpr (index == 23) {
			return (p23);
		} else 	if constexpr (index == 24) {
			return (p24);
		} else 	if constexpr (index == 25) {
			return (p25);
		} else 	if constexpr (index == 26) {
			return (p26);
		} else 	if constexpr (index == 27) {
			return (p27);
		} else 	if constexpr (index == 28) {
			return (p28);
		} else 	if constexpr (index == 29) {
			return (p29);
		} else 	if constexpr (index == 30) {
			return (p30);
		} else 	if constexpr (index == 31) {
			return (p31);
		} else {
			return;
		}
	}

	}
} // !hlx

#endif //!HLX_GETSTRUCTMEMBER_H