//Testing File for Cache
//Tests are written using the Catch2 Framework and will be described in the README
#include <iostream>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "cache.hh"

TEST_CASE("Setting and Getting elements.") //basic rejection
{
    auto test_cache = Cache(16);

    Cache::val_type val1 = "val_one";

    //We want two elements to go in and last one to be blocked
    test_cache.set("key_one", val1, 8);
    test_cache.set("key_two", "val_two", 8);
    test_cache.set("key_three", "rejected", 9);

    //Confirm these were added or rejected as expected
    Cache::size_type size;

    REQUIRE(*test_cache.get("key_one", size) == *val1);
    test_cache.del("key_one");
    // Cache::val_type val2 = "value_2";
    // REQUIRE(*test_cache.get("key_two", size) == *val2);
    REQUIRE(test_cache.get("key_three", size) == nullptr);
    REQUIRE(test_cache.get("key_one", size) == nullptr);
    test_cache.reset();
}
