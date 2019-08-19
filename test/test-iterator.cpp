#include <catch2/catch.hpp>
#include <basic_string_view.hpp>
#include <iterator.hpp>

TEST_CASE("iterator operations for random access iterator", "[iterator.rai]") {
    kstd::string_view s{"hello", sizeof("hello")};

    REQUIRE(kstd::distance(s.data(), s.data() + sizeof("hello")) ==
            sizeof("hello"));

    const char *non_owning_p = s.data();

    kstd::advance(non_owning_p, 3);

    REQUIRE(kstd::distance(s.data(), non_owning_p) == 3);
    REQUIRE(*non_owning_p == 'l');

    REQUIRE(*kstd::next(non_owning_p) == 'o');
    REQUIRE(*kstd::prev(non_owning_p) == 'l');
}
