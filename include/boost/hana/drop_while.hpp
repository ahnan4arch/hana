/*!
@file
Defines `boost::hana::drop_while`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DROP_WHILE_HPP
#define BOOST_HANA_DROP_WHILE_HPP

#include <boost/hana/fwd/drop_while.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/eval_if.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/tail.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename Pred>
    constexpr auto drop_while_t::operator()(Xs&& xs, Pred&& pred) const {
        using It = typename datatype<Xs>::type;
        using DropWhile = BOOST_HANA_DISPATCH_IF(drop_while_impl<It>,
            _models<Iterable, It>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Iterable, It>::value,
        "hana::drop_while(xs, pred) requires 'xs' to be an Iterable");
    #endif

        return DropWhile::apply(static_cast<Xs&&>(xs), static_cast<Pred&&>(pred));
    }
    //! @endcond

    namespace iterable_detail {
        struct drop_while_helper {
            struct next {
                template <typename Xs, typename Pred>
                constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
                    return hana::drop_while(
                        hana::tail(static_cast<Xs&&>(xs)),
                        static_cast<Pred&&>(pred)
                    );
                }
            };

            template <typename Xs, typename Pred>
            constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
                return hana::eval_if(pred(hana::front(xs)),
                    hana::lazy(next{})(xs, pred),
                    hana::lazy(xs)
                );
            }
        };
    }

    template <typename It, bool condition>
    struct drop_while_impl<It, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            return hana::eval_if(hana::is_empty(xs),
                hana::lazy(xs),
                hana::lazy(iterable_detail::drop_while_helper{})(
                                            xs, static_cast<Pred&&>(pred))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DROP_WHILE_HPP