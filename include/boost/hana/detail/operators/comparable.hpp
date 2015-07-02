/*!
@file
Defines operators for Comparables.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_OPERATORS_COMPARABLE_HPP
#define BOOST_HANA_DETAIL_OPERATORS_COMPARABLE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/fwd/concept/comparable.hpp>

#include <type_traits>


namespace boost { namespace hana {
    namespace detail {
        template <typename Tag>
        struct comparable_operators {
            static constexpr bool value = false;
        };
    }

    namespace operators {
        template <typename X, typename Y, typename = typename std::enable_if<
            detail::comparable_operators<typename datatype<X>::type>::value ||
            detail::comparable_operators<typename datatype<Y>::type>::value
        >::type>
        constexpr auto operator==(X&& x, Y&& y)
        { return hana::equal(static_cast<X&&>(x), static_cast<Y&&>(y)); }

        template <typename X, typename Y, typename = typename std::enable_if<
            detail::comparable_operators<typename datatype<X>::type>::value ||
            detail::comparable_operators<typename datatype<Y>::type>::value
        >::type>
        constexpr auto operator!=(X&& x, Y&& y)
        { return hana::not_equal(static_cast<X&&>(x), static_cast<Y&&>(y)); }
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_OPERATORS_COMPARABLE_HPP