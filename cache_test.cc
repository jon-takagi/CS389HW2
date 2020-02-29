//Testing File for Cache
//Tests are written using the Catch2 Framework and will be described in the README

#define CATCH_CONFIG_MAIN
#include <iostream>
#include "catch.hpp"
#include "cache.hh"
#include "fifo_evictor.h"

using key_type = std::string;

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
    SECTION("evictor") {
        FifoEvictor evictor_obj = FifoEvictor();
        FifoEvictor *evictor_ptr;
        evictor_ptr = &evictor_obj;
        auto test_cache = Cache(24, 0.75,evictor_ptr, std::hash<key_type>());
        //
        test_cache.set("key_one", "value_1", 8);
        test_cache.set("key_two", "value_2", 8);
        test_cache.set("key_three", "value_3", 8);
        // std::cout << test_cache.space_used() << std::endl;
        REQUIRE(test_cache.space_used() == 24);
        test_cache.set("key_one", "value_4", 8);

        test_cache.set("key_five", "value_5", 8);
        Cache::size_type size = 0;
        REQUIRE(test_cache.get("key_one", size) == nullptr);
        test_cache.set("key_six", "value_6", 8);
        REQUIRE(test_cache.get("key_two", size) == nullptr);
        // test_cache.set("key_seven", "value_7", 8);
    }
}

TEST_CASE("Hash Function")
{
    
    std::vector<key_type> in_vec;
    in_vec.push_back("keytest");
    std::vector<std::size_t> out_vec;
    
    //We define a Functor to pass as the hash function, so that it can record when and what it is called with
    //It keeps track of the inputs and outputs in arrays, so we can check that it was called, with the expected
    //key and getting the expected result
    class HashFunctor {
        private:
            Cache::hash_func function_;
            std::vector<key_type> inputs_;
            std::vector<std::size_t> outputs_;
        public:
            HashFunctor(Cache::hash_func func, std::vector<key_type>& inputs, std::vector<std::size_t>& outputs){
                function_ = func;
                inputs_ = inputs;
                outputs_ = outputs;
            }
        
            Cache::size_type operator () (key_type key){
                auto ret = function_(key);
                outputs_.push_back(ret);
                in_vec.push_back(key);
                auto k = inputs_.back();
                std::cout << k<< std::endl;
                return ret;
            }
            
            //Get the last entry in outputs_, which should be the return of the most recent call
            std::pair<key_type, std::size_t> most_recent(){
                auto k = inputs_.front();
                return std::make_pair(k, 1);
                //return std::make_pair(inputs_.back(), outputs_.back());
            }
            
            //test if either array is empty
            bool is_empty(){
                return (outputs_.empty() or inputs_.empty());
            }
    };
    
    //First test, making a bad hash function that hashes everything to the same value
    std::function<std::size_t(key_type)> lam1 = [](key_type k) {return (k == "key_0");};//avoid compiler warnings while "always" returning 0
    auto bad_hash = HashFunctor(lam1, in_vec, out_vec);
    auto test_cache = Cache(100, 0.75, nullptr, bad_hash);
    
    
    //Then set a few times and confirm that our functor was called on these
    test_cache.set("key_one", "value_1", 8);
    std::cout << in_vec.size() << std::endl;    
    //REQUIRE((bad_hash.most_recent()).first == "key_one"); 
    //REQUIRE(bad_hash.most_recent().second == 1); 
    test_cache.set("key_two", "value_2", 8);
  //  REQUIRE(bad_hash.most_recent().first == "key_two"); 
   // REQUIRE(bad_hash.most_recent().second == 1); 
    test_cache.set("key_three", "value_3", 8);
  //  REQUIRE(bad_hash.most_recent().first == "key_three"); 
   // REQUIRE(bad_hash.most_recent().second == 1); 
    
    
    //make a better hash and repeat
}
    
    
    //create cache with 1 functor
    //call set 3 times
    //confirm functor has expected stuff
    //repeated for 1-2 other functions
    
    
//Test Hash and load factor

//Test Eviction













