//Testing File for Cache
//Tests are written using the Catch2 Framework and will be described in the README

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "cache.hh"

TEST_CASE("Testing Basic Cache Operations") //basic rejection
{
    
    SECTION("Setting and Getting elements."){
    
        //Make a cache with plenty of space for first test
        auto test_cache = Cache(100);
        
        //Loading in three entries
        test_cache.set("key_one", "value_1", 8);
        test_cache.set("key_two", "value_2", 8);
        test_cache.set("key_three", "value_3", 8);
        
        //We establish some values for these tests: the size variable to write to, and some char arrays to dereference
        Cache::size_type size; //Won't access this, but need it to pass as argument
        Cache::val_type val1 = "value_1";
        Cache::val_type val2 = "value_2";
        Cache::val_type val3 = "value_3";
        
        //Confirm these were added to the array
        REQUIRE(*test_cache.get("key_one", size) == *val1);
        REQUIRE(*test_cache.get("key_two", size) == *val2);
        REQUIRE(*test_cache.get("key_three", size) == *val3);
        
        //Now we overwrite two of them and confirm the values are as expected
        //Here we swap two and three, and keep one the same
        test_cache.set("key_two", "value_3", 8);
        test_cache.set("key_three", "value_2", 8);

        //Test these changes
        REQUIRE(*test_cache.get("key_one", size) == *val1);
        REQUIRE(*test_cache.get("key_two", size) == *val3);
        REQUIRE(*test_cache.get("key_three", size) == *val2);        
        
        
    }
    
    SECTION("Testing Size and Capacity."){
        
        //Again we will work with a spacious cache
        auto test_cache = Cache(100);
        
        //Confirm it is empty to begin with
        REQUIRE(test_cache.space_used() == 0);
        
        //Add some entries of variable sizes, and confirm that the size increases as expected
        test_cache.set("key_one", "value_1", 8);
        REQUIRE(test_cache.space_used() == 8);
        
        test_cache.set("key_two", "value_two", 10);
        REQUIRE(test_cache.space_used() == 18);
        
        test_cache.set("key_three", "value_threeeeeee", 17);
        REQUIRE(test_cache.space_used() == 35);
        
        //Confirm that calling "get" returns the expected size value
        Cache::size_type size; //Set up size to access
        
        test_cache.get("key_one", size);
        REQUIRE(size == 8);
        
        test_cache.get("key_two", size);
        REQUIRE(size == 10);
        
        test_cache.get("key_three", size);
        REQUIRE(size == 17);        
        
        
        //Overwrite some entries and cofirm size changes as expected
        test_cache.set("key_one", "size now at 42", 15);
        REQUIRE(test_cache.space_used() == 42);
        test_cache.get("key_one", size);
        REQUIRE(size == 15);  

        test_cache.set("key_two", "size now at 47", 15);
        REQUIRE(test_cache.space_used() == 47);
        test_cache.get("key_two", size);
        REQUIRE(size == 15);   
       
        
    }
    
    SECTION("Testing Delete and Reset."){
        
        auto test_cache = Cache(100);
        Cache::size_type size = 0; //Set up size to access; we expect this to remain unchanged since we are calling get for empty/non-existent entries
        
        //Add some values to begin with
        test_cache.set("key_one", "value_1", 8);
        test_cache.set("key_two", "value_2", 8);
        test_cache.set("key_three", "value_3", 8);
        
        //Remove values one at a time, checking that they now return nullptr
        test_cache.del("key_one");
        REQUIRE(test_cache.get("key_one", size) == nullptr);
        
        test_cache.del("key_two");
        REQUIRE(test_cache.get("key_two", size) == nullptr);
        
        test_cache.del("key_three");
        REQUIRE(test_cache.get("key_three", size) == nullptr);
        
        //confirm size zero
        REQUIRE(test_cache.space_used() == 0);
        
        //fill the cache again
        test_cache.set("key_one", "value_1", 8);
        test_cache.set("key_two", "value_2", 8);
        test_cache.set("key_three", "value_3", 8);
        
        //reset the cache, then confirm all entries nullptr and size is zero
        test_cache.reset();
        REQUIRE(test_cache.get("key_one", size) == nullptr);
        REQUIRE(test_cache.get("key_two", size) == nullptr);
        REQUIRE(test_cache.get("key_three", size) == nullptr);
        REQUIRE(test_cache.space_used() == 0);
        
    }
    
    
}


//Test Hash and load factor

//Test Eviction



    
    
    
    
    