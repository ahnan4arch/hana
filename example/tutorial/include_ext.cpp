/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

//! [main]
#include <boost/hana/ext/std/tuple.hpp>
#include <tuple> // still required to create a tuple
using namespace boost::hana;


int main() {
  constexpr std::tuple<int, char, float> xs{1, '2', 3.0f};
  static_assert(front(xs) == 1, "");
}
//! [main]