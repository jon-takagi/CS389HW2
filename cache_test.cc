//Testing File for Cache
//Tests are written using the Catch2 Framework and will be described in the README

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "cache.hh"

TEST_CASE("Setting and Getting elements.") //basic rejection
{
    auto test_cache = Cache(16);
    
    //We want two elements to go in and last one to be blocked
    test_cache.set("key_one", "value_1", 8);
    test_cache.set("key_two", "value_2", 8);
    test_cache.set("key_three", "rejected", 9);
    
    //Confirm these were added or rejected as expected
    REQUIRE(test_cache.get("key_one", 8) == "value_1");
    REQUIRE(test_cache.get("key_two", 8) == "value_2");
    REQUIRE(test_cache.get("key_three", 9) == nullptr);
    
}