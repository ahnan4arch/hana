/*!
@file
Defines `boost::hana::count_if`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COUNT_IF_HPP
#define BOOST_HANA_COUNT_IF_HPP

#include <boost/hana/fwd/count_if.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/constexpr/algorithm.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/unpack.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename Pred>
    constexpr auto count_if_t::operator()(Xs&& xs, Pred&& pred) const {
        using S = typename datatype<Xs>::type;
        using CountIf = BOOST_HANA_DISPATCH_IF(count_if_impl<S>,
            _models<Foldable, S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Foldable, S>::value,
        "hana::count_if(xs, pred) requires 'xs' to be Foldable");
    #endif

        return CountIf::apply(static_cast<Xs&&>(xs),
                              static_cast<Pred&&>(pred));
    }
    //! @endcond

    namespace detail {
        template <bool ...b>
        struct fast_and
            : std::is_same<fast_and<b...>, fast_and<(b, true)...>>
        { };

        template <typename Pred>
        struct count_pred {
            Pred pred;
            template <typename ...Xs, typename = typename std::enable_if<
                fast_and<_models<Constant,
                    decltype((*pred)(std::declval<Xs&&>()))>::value...>::value
            >::type>
            constexpr auto operator()(Xs&& ...xs) const {
                constexpr bool results[] = {false, // <-- avoid empty array
                    hana::value<decltype((*pred)(static_cast<Xs&&>(xs)))>()...
                };
                constexpr std::size_t total = detail::constexpr_::count(
                    results, results + sizeof(results), true
                );
                return hana::size_t<total>;
            }

            template <typename ...Xs, typename = void, typename = typename std::enable_if<
                !fast_and<_models<Constant,
                    decltype((*pred)(std::declval<Xs&&>()))>::value...>::value
            >::type>
            constexpr auto operator()(Xs&& ...xs) const {
                std::size_t total = 0;
                using Swallow = std::size_t[];
                (void)Swallow{0, ((*pred)(static_cast<Xs&&>(xs)) ? ++total : 0)...};
                return total;
            }
        };
    }

    template <typename T, bool condition>
    struct count_if_impl<T, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            // We use a pointer instead of a reference to avoid a Clang ICE.
            return hana::unpack(static_cast<Xs&&>(xs),
                detail::count_pred<decltype(&pred)>{&pred}
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COUNT_IF_HPP