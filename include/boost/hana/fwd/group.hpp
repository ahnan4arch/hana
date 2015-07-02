/*!
@file
Forward declares `boost::hana::group`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_GROUP_HPP
#define BOOST_HANA_FWD_GROUP_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/by_fwd.hpp>


namespace boost { namespace hana {
    //! Group adjacent elements of a sequence that all respect a binary
    //! predicate, by default equality.
    //! @relates Sequence
    //!
    //! Given a _finite_ Sequence and an optional predicate (by default
    //! `equal`), `group` returns a sequence of subsequences representing
    //! groups of adjacent elements that are "equal" with respect to the
    //! predicate. In other words, the groups are such that the predicate is
    //! satisfied when it is applied to any two adjacent elements in that
    //! group. The sequence returned by `group` is such that the concatenation
    //! of its elements is equal to the original sequence, which is equivalent
    //! to saying that the order of the elements is not changed.
    //!
    //! If no predicate is provided, adjacent elements in the sequence must
    //! all be compile-time `Comparable`.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Sequence `s` of data type `S(T)`, a Logical `Bool` and a
    //! predicate \f$ pred : T \times T \to Bool \f$, `group` has the
    //! following signatures. For the variant with a provided predicate,
    //! \f[
    //!     \mathtt{group} : S(T) \times (T \times T \to Bool) \to S(S(T))
    //! \f]
    //!
    //! for the variant without a custom predicate, the `T` data type is
    //! required to be Comparable. The signature is then
    //! \f[
    //!     \mathtt{group} : S(T) \to S(S(T))
    //! \f]
    //!
    //! @param xs
    //! The sequence to split into groups.
    //!
    //! @param predicate
    //! A binary function called as `predicate(x, y)`, where `x` and `y`
    //! are _adjacent_ elements in the sequence, and returning a `Logical`
    //! representing whether both elements should be in the same group
    //! (subsequence) of the result. The result returned by `predicate` must
    //! be a compile-time `Logical`. Also, `predicate` has to define an
    //! equivalence relation as defined by the `Comparable` concept.
    //! When this predicate is not provided, it defaults to `equal`.
    //!
    //! ### Example
    //! @snippet example/sequence.cpp group
    //!
    //!
    //! Syntactic sugar (`group.by`)
    //! ----------------------------
    //! `group` can be called in a third way, which provides a nice syntax
    //! especially when working with the `comparing` combinator:
    //! @code
    //!     group.by(predicate, xs) == group(xs, predicate)
    //!     group.by(predicate) == group(-, predicate)
    //! @endcode
    //!
    //! where `group(-, predicate)` denotes the partial application of
    //! `group` to `predicate`.
    //!
    //! ### Example
    //! @snippet example/sequence.cpp group.by
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto group = [](auto&& xs[, auto&& predicate]) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct group_impl : group_impl<S, when<true>> { };

    struct group_t : detail::by<group_t> {
        template <typename Xs>
        constexpr auto operator()(Xs&& xs) const;

        template <typename Xs, typename Predicate>
        constexpr auto operator()(Xs&& xs, Predicate&& pred) const;
    };

    constexpr group_t group{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_GROUP_HPP