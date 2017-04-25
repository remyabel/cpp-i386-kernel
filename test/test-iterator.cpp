#include "catch.hpp"

#include <iterator.hpp>

#include <basic_string_view.hpp>

TEST_CASE("iterator operations for random access iterator", "[iterator.rai]") {
    using namespace kstd;

    string_view s{"hello", sizeof("hello")};

    REQUIRE(distance(s.data(), s.data() + sizeof("hello")) == sizeof("hello"));

    const char *non_owning_p = s.data();

    advance(non_owning_p, 3);

    REQUIRE(distance(s.data(), non_owning_p) == 3);
    REQUIRE(*non_owning_p == 'l');

    REQUIRE(*next(non_owning_p) == 'o');
    REQUIRE(*prev(non_owning_p) == 'l');
}
