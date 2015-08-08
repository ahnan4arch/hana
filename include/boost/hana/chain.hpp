/*!
@file
Defines `boost::hana::chain`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CHAIN_HPP
#define BOOST_HANA_CHAIN_HPP

#include <boost/hana/fwd/chain.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>

#include <boost/hana/flatten.hpp>
#include <boost/hana/transform.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename F>
    constexpr decltype(auto) chain_t::operator()(Xs&& xs, F&& f) const {
        using M = typename datatype<Xs>::type;
        using Chain = BOOST_HANA_DISPATCH_IF(chain_impl<M>,
            _models<Monad, M>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Monad, M>::value,
        "hana::chain(xs, f) requires 'xs' to be a Monad");
    #endif

        return Chain::apply(static_cast<Xs&&>(xs), static_cast<F&&>(f));
    }
    //! @endcond

    template <typename M, bool condition>
    struct chain_impl<M, when<condition>> : default_ {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs&& xs, F&& f) {
            return hana::flatten(hana::transform(static_cast<Xs&&>(xs),
                                                 static_cast<F&&>(f)));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CHAIN_HPP