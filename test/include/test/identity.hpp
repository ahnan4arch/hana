/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_IDENTITY_HPP
#define BOOST_HANA_TEST_TEST_IDENTITY_HPP

#include <boost/hana/detail/create.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/concept/logical.hpp>

// models
#include <boost/hana/concept/applicative.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/concept/functor.hpp>
#include <boost/hana/concept/monad.hpp>
#include <boost/hana/concept/orderable.hpp>


namespace boost { namespace hana {
    namespace test {
        struct Identity;

        template <typename T>
        struct _identity {
            T value;
            struct hana { using datatype = Identity; };
        };

        constexpr detail::create<_identity> identity{};
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<test::Identity, test::Identity> {
        template <typename Id1, typename Id2>
        static constexpr auto apply(Id1 x, Id2 y)
        { return equal(x.value, y.value); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct less_impl<test::Identity, test::Identity> {
        template <typename Id1, typename Id2>
        static constexpr auto apply(Id1 x, Id2 y)
        { return less(x.value, y.value); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //
    // Define either one to select which MCD is used:
    //  BOOST_HANA_TEST_FUNCTOR_TRANSFORM_MCD
    //  BOOST_HANA_TEST_FUNCTOR_ADJUST_MCD_MCD
    //
    // If neither is defined, the MCD used is unspecified.
    //////////////////////////////////////////////////////////////////////////
#ifdef BOOST_HANA_TEST_FUNCTOR_TRANSFORM_MCD
    template <>
    struct transform_impl<test::Identity> {
        template <typename Id, typename F>
        static constexpr auto apply(Id self, F f)
        { return test::identity(f(self.value)); }
    };
#else
    template <>
    struct adjust_if_impl<test::Identity> {
        struct get_value {
            template <typename T>
            constexpr auto operator()(T t) const { return t.value; }
        };

        template <typename Id, typename P, typename F>
        static constexpr auto apply(Id self, P p, F f) {
            auto x = eval_if(p(self.value),
                lazy(compose(f, get_value{}))(self),
                lazy(get_value{})(self)
            );
            return test::identity(x);
        }
    };
#endif

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //
    // Define either one to select which MCD is used:
    //  BOOST_HANA_TEST_APPLICATIVE_FULL_MCD
    //  BOOST_HANA_TEST_APPLICATIVE_MONAD_MCD
    //
    // If neither is defined, the MCD used is unspecified.
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct lift_impl<test::Identity> {
        template <typename X>
        static constexpr auto apply(X x)
        { return test::identity(x); }
    };
#ifdef BOOST_HANA_TEST_APPLICATIVE_FULL_MCD
    template <>
    struct ap_impl<test::Identity> {
        template <typename F, typename X>
        static constexpr auto apply(F f, X x)
        { return test::identity(f.value(x.value)); }
    };
#else
    template <>
    struct ap_impl<test::Identity>
        : Monad::ap_impl<test::Identity>
    { };
#endif

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //
    // Define either one to select which MCD is used:
    //  BOOST_HANA_TEST_MONAD_FLATTEN_MCD
    //  BOOST_HANA_TEST_MONAD_CHAIN_MCD
    //
    // If neither is defined, the MCD used is unspecified.
    //////////////////////////////////////////////////////////////////////////
#ifdef BOOST_HANA_TEST_MONAD_FLATTEN_MCD
    template <>
    struct flatten_impl<test::Identity> {
        template <typename Id>
        static constexpr auto apply(Id self)
        { return self.value; }
    };
#else
    template <>
    struct chain_impl<test::Identity> {
        template <typename X, typename F>
        static constexpr auto apply(X x, F f)
        { return f(x.value); }
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_TEST_IDENTITY_HPP