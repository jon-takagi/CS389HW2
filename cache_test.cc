//Testing File for Cache
//Tests are written using the Catch2 Framework and will be described in the README

#define CATCH_CONFIG_MAIN
#include <iostream>
#include "catch.hpp"
#include "cache.hh"
#include "fifo_evictor.h"

using key_type = std::string;

TEST_CASE("Testing Basic Cache Operations") //Tests the basic parts of the cache
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
        test_cache.set("key_one", "val_one", 8);
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


        //Overwrite some entries and confirm size changes as expected
        test_cache.set("key_one", "123456789", 10);
        test_cache.get("key_one", size);
        REQUIRE(size == 10);
        REQUIRE(test_cache.space_used() == 37);

        test_cache.set("key_two", "12345678901234", 15);
        REQUIRE(test_cache.space_used() == 42);
        test_cache.get("key_two", size);
        REQUIRE(size == 15);
    }

    SECTION("Testing del"){
        auto test_cache = Cache(100);
        Cache::size_type size = 0;
        //Set up size to access; we expect this to remain unchanged since we are calling get for empty/non-existent entries

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

    }
    SECTION("Testing Reset") {
        auto test_cache = Cache(100);
        Cache::size_type size = 0;
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

TEST_CASE("Hash Function")//Making two hash functions and checking that the cache is actually calling them when set is used
{
    
    std::vector<key_type> in_vec;
    std::vector<std::size_t> out_vec;

    //First test, making a bad hash function that hashes everything to the same value
    std::function<std::size_t(key_type)> bad_hash = [&](key_type k) {
        in_vec.push_back(k); 
        out_vec.push_back(0);
        return 0;
    };
    //auto bad_hash = HashFunctor(lam1, in_vec, out_vec);
    auto test_cache = Cache(100, 0.75, nullptr, bad_hash);
    
    
    //Then set a few times and confirm that our functor was called on these
    test_cache.set("key_one", "value_1", 8);
    REQUIRE(in_vec.back() == "key_one");
    REQUIRE(out_vec.back() == 0);
    test_cache.set("key_two", "value_2", 8);
    REQUIRE(in_vec.back() == "key_two");
    REQUIRE(out_vec.back() == 0);
    test_cache.set("key_three", "value_3", 8);
    REQUIRE(in_vec.back() == "key_three");
    REQUIRE(out_vec.back() == 0);
    
    
    //make a "better" hash and repeat
    std::function<std::size_t(key_type)> slightly_better_hash = [&](key_type k) {
        in_vec.push_back(k); 
        std::size_t ret = k[0]-0;
        out_vec.push_back(ret);
        return ret;
    };
    auto test_cache2 = Cache(100, 0.75, nullptr, slightly_better_hash);
    test_cache2.set("one_key", "value_1", 8);
    REQUIRE(in_vec.back() == "one_key");
    REQUIRE(out_vec.back() == 111);
    test_cache2.set("two_key", "value_2", 8);
    REQUIRE(in_vec.back() == "two_key");
    REQUIRE(out_vec.back() == 116);
    test_cache2.set("hree_key", "value_3", 8);
    REQUIRE(in_vec.back() == "hree_key");
    REQUIRE(out_vec.back() == 104);
    
    
}

TEST_CASE("Eviction")
{
    

    //In this section, we use several entries, all of the same size, and check that adding a new one at capacity will kick out the oldest one 
    SECTION("Testing eviction in proper order for 8 total entries of the same size.") {
        //Start with a smaller cache this time, so we can hit the limit more easily
        FifoEvictor evictor_obj = FifoEvictor();
        FifoEvictor *evictor_ptr;
        evictor_ptr = &evictor_obj;
        auto test_cache = Cache(40, 0.75,evictor_ptr, std::hash<key_type>());
        //Fill the cache
        test_cache.set("key_one", "value_one", 10);
        test_cache.set("key_two", "value_two", 10);
        test_cache.set("key_three", "value_tre", 10);
        test_cache.set("key_four", "value_for", 10);
        //No need to check the values are currently in since it works by the above tests; 
        //Now we start evicting, check the expected entry is evicted while others are not
        Cache::size_type size; //Won't access this, but need it to pass as argument
        
        
        Cache::val_type val2 = "value_two";
        Cache::val_type val5 = "value_fiv";
        test_cache.set("key_five", "value_fiv", 10);
        REQUIRE(test_cache.get("key_one", size) == nullptr);
        REQUIRE(*test_cache.get("key_two", size) == *val2);
        REQUIRE(*test_cache.get("key_five", size) == *val5);
        
        Cache::val_type val3 = "value_tre";
        Cache::val_type val6 = "value_six";
        test_cache.set("key_six", "value_six", 10);
        REQUIRE(test_cache.get("key_two", size) == nullptr);
        REQUIRE(*test_cache.get("key_three", size) == *val3);
        REQUIRE(*test_cache.get("key_six", size) == *val6);
        
        Cache::val_type val4 = "value_for";
        Cache::val_type val7 = "value_sev";
        test_cache.set("key_seven", "value_sev", 10);
        REQUIRE(test_cache.get("key_three", size) == nullptr);
        REQUIRE(*test_cache.get("key_four", size) == *val4);
        REQUIRE(*test_cache.get("key_seven", size) == *val7);
      
        //Cache::val_type val5 = "value_fiv";
        Cache::val_type val8 = "value_ate";
        test_cache.set("key_eight", "value_ate", 10);
        REQUIRE(test_cache.get("key_four", size) == nullptr);
        REQUIRE(*test_cache.get("key_five", size) == *val5);
        REQUIRE(*test_cache.get("key_eight", size) == *val8);
        
    }
    
    //For the second test, we use entries of increasing and decreasing size to ensure the evictor will properly remove multiple/oversized entries to creat space
    SECTION("Testing eviction with entries of differing sizes") {
        //Even smaller and more basic cache this time
        FifoEvictor evictor_obj = FifoEvictor();
        FifoEvictor *evictor_ptr;
        evictor_ptr = &evictor_obj;
        auto test_cache = Cache(10, 0.75,evictor_ptr, std::hash<key_type>());
        
        //add 4 ones
        //add 1 two;  1 one kicked
        //add 1 four, all kicked
        //add 1 two, four kicked
        //add 4 ones, two kicked at end
        //Add four entries of size 2
        test_cache.set("key_one", "1", 2);
        test_cache.set("key_two", "2", 2);
        test_cache.set("key_three", "3", 2);
        test_cache.set("key_four", "4", 2);
        REQUIRE(test_cache.space_used() == 8);
        //Add one entry of size 4; confirm this kicks the first entry
        test_cache.set("key_five", "555", 4);
        Cache::val_type val5 = "555";
        Cache::size_type size;
        REQUIRE(test_cache.get("key_one", size) == nullptr);
        REQUIRE(*test_cache.get("key_five", size) == *val5);
        REQUIRE(test_cache.space_used() == 10);
        //Now we can add one entry of size 8, which should kick everything
        test_cache.set("key_six", "value_6", 8);
        Cache::val_type val6 = "value_6";
        REQUIRE(test_cache.get("key_two", size) == nullptr);
        REQUIRE(test_cache.get("key_three", size) == nullptr);
        REQUIRE(test_cache.get("key_four", size) == nullptr);
        REQUIRE(test_cache.get("key_five", size) == nullptr);
        REQUIRE(*test_cache.get("key_six", size) == *val6);
        REQUIRE(test_cache.space_used() == 8);
        //Now we add back the size 4 entry, and check that it kicks out the size 8
        test_cache.set("key_five", "555", 4);
        REQUIRE(test_cache.get("key_six", size) == nullptr);
        REQUIRE(*test_cache.get("key_five", size) == *val5);
        REQUIRE(test_cache.space_used() == 4);
        //Then we add back in the four size 2 originals, and check that the size 4 is gone
        test_cache.set("key_one", "1", 2);
        test_cache.set("key_two", "2", 2);
        test_cache.set("key_three", "3", 2);
        test_cache.set("key_four", "4", 2);
        REQUIRE(test_cache.get("key_five", size) == nullptr);
        REQUIRE(test_cache.space_used() == 8);
    }
    
}
