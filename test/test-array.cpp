#include <catch2/catch.hpp>

#include <algorithm.hpp>
#include <array.hpp>

TEST_CASE("array forms a valid range and works w/ range functions", "[array]") {
    using namespace kstd;

    array<int, 5> arr = {1, 2, 3, 4, 5};

    REQUIRE(*arr.begin() == 1);
    REQUIRE(arr.size() == 5);
    REQUIRE(distance(arr.begin(), arr.end()) == 5);
    REQUIRE(*next(arr.begin()) == 2);
    REQUIRE(!arr.empty());
    REQUIRE(*arr.data() == 1);

    array<int, 10> arr2;
    copy(arr.begin(), arr.end(), arr2.begin());
    REQUIRE(*arr2.begin() == 1);

    copy(arr.begin(), arr.end(), next(arr2.begin(), 5));
    REQUIRE(*(arr2.begin() + 5) == 1);

    REQUIRE(arr[0] == 1);
    REQUIRE(arr2[5] == 1);
}
