# Systems Homework 2
Jon Takagi and Eli Poppele

## Part 1: Basic operations

### Constructor
Our cache uses an `unordered_map` as the underlying data type. Rather than directly storing key-value pairs in the map, we store key-(value-size) pairs. The `maxmem`, `size`, and `evictor` are stored as member variables, and the `max_load_factor` and `hasher` are members of the `unordered_map`

### Deconstructor
Because of the way we create the `char*` in the `set` method, the memory must be freed using `delete[]`. The `reset` method also frees the memory allocated to the array, so to avoid duplicating code we call it in the destructor. There are no other members with non-trivial destructors, so no other code needs to be called.

### Set
In order to deep-copy the value into the cache when we `set` it, we must iterate over each character in the value and store it in the new value. Because variable sized arrays are illegal in C++, we instead create a pointer to a variable number of spaces on the heap, a very different construction. Since we allocated heap memory for the deep copy, we must free it but not until the key is `del`-ed.
In order to handle the case where a key is overwritten, we simply call the `del` method to remove the key and then proceed with the insertion.
### Get
Using the `unordered_map` structure from the standard library enables us to return the value associated with a given key easily. However, the `get` method also requires that we store the size of value being returned in the pointer provided. Instead of recalculating this value (by iterating through the value, searching for the null character) each time we access a value, we store it alongside the value itself.

### Del
The `del` method removes a key-value pair (and the associated size) from the cache. If the key is not in the cache, the function returns `false` and halts. Otherwise, it deincrements the space used, frees the memory allocated to store the value, and removes it from the cache, then returns `true`. This ensures no memory is leaked.

### Space Used
We maintain a running total of the size of all values stored in the cache. This adds a constant factor to the `set` and `del` methods, but allows us to query the `space_used` in constant time.

### Reset
The `reset` method iterates over all items in the map, then frees the memory allocated to their values. We use a standard library function to remove all elements from the unordered_map. Finally, we update our running total of the size of the values stored to 0.

## Part 2: Testing
We used [`Catch2`](https://github.com/catchorg/Catch2) to create tests. Each function is separated into its own section, and can be run independently by calling `./main.bin -c "Section Name"`. While Catch2 is straightforwards and easy to use, it does significantly increase the compile time after changing both implementation and tests to nearly 13 seconds on my MacBook. How barbaric!

We also used [Valgrind](https://valgrind.org) to verify that no memory leaks occured. By running `valgrind ./main.bin` we confirm that all heap blocks were freed when running all our tests.

The testing is divided into three main sections. The first tests the basic features of the cache, confirming that you can set, retrieve, and overwrite entries. It also checks that the sizes are entered and returned as expected, and that the delete and reset capabilities are functional. An important edge case covered here is that we need to adjust the size of an entry and of the cache when a pre-existing key is set with a new value.
The second section in the tests is for the hash function, and was originally a longer section when we used a functor instead of a lambda with variable captures (see below). In this latest iteration, the test simply creates two simple hash lambdas, then creates some test caches with those lambdas as their hash functions. After calling `set` a few times, we confirm that the lambdas were called with the expected inputs (and outputs).
For the third section, we test the evictor. With currently just the basic evictor, we do this by first testing that entries are evicted in the order expected, by filling a cache with entries and then adding new ones, all of the same size, and confirming that the earliest added entry is the first one to be evicted in each case until only new entries remain. The second case works with entries of various sizes, confirming that the evictor will always take the earliest added entry out, regardless of size efficiency, and that it will evict multiple entries as necessary to fit in the new entry.

### Compilation
Our makefile allows us to compile all necessary files for our tests. Using `make clean` also removes binary and object files, cleaning up our code before publishing to github. Examining our commit history, we were not always consistent about doing so, however, it is good practice to do so.
The makefile we use is fairly boilerplate, however, due to time issues with my virtual machine, I include the command to first touch all our source files. This updates the timestamp on these files to ensure they are seen as "in the past" by make. However, it does lead to recompilation of files that may not have been changed. A better solution might be to use NTP to fix the underlying issue with time drift in my VM, but...
## Part 3: Performance
The use of has functions is largely handled through the unordered map, which can be inititialized with a hash function. In this case, all we had to do was write tests confirming that our cache was getting the has function and using it in its unordered map to has entries. To do this, we wrote hash functions as lambdas in the test files, adding in variable captures of vectors for inputs and outputs. We had first attempted to use a functor class for the hash, but this resulted in an accessible copy of the functor actually being used in the cache, so we couldn't access any of its data members. With the lambdas, we simply pushed inputs and outputs through a set of local vectors as the functions were called, then checked that those vectors were updated as expected when we called `set` for a cache made using one of those lambda hash functions. 
## Part 4: Collision Resolution
Since we used `unordered_map` from the STL, the collision resolution was already handled for us. The map uses collision chaining, and keeps entries that collide in the same bucket, although the exact manner of doing so is unclear (but allegedly runs in time linear to the bucket size). I also prefer bucket chaining for collisions in a hashtable, especially since the max load factor in this case is set to 0.75, and it is unlikely that buckets will ever have more than a few entries, and thus not much time is lost in this linear operation. Our choice of template to use was clearly influenced by the implementation of this method of collision resolution and not at all just because Eitan recommended it.
## Part 5: Dynamic Resizing
The C++ standard library again handles this part of the project for us, as `unordered_map` simply has a method `max_load_factor()` which can be called to set the maximum load factor of the hashtable. The documentation says that the map will automatically rehash when it exceeds the load factor. The one issue is that some implementations of `unordered_map` have a maximum bucket count, which will cause the maximum load factor to be ignored if it is exceeded.
We call `max_load_factor()` on our map in the constructor of the cache to set it to the appropriate value.
## Part 6: Eviction Policy
The cache calls `touch` in `set` when it successfully adds an item to the queue. If `set` is used to overwrite a key, the key is duplicated inside of the evictor.
If calling `set` would cause the cache to exceed the maximum size set, the evictor chooses keys to `del` until there is room for the new key. Since `del` simply returns false if the chosen key is not in the cache, there is no error if the evictor's chosen key was already removed. Instead, `del` returns false, and the loop continues until a sufficient number of keys have been deleted. Note that a cache with a maxmem of 0 will loop infinitely.

### FIFO Evictor
The `FifoEvictor` extends the given evictor class. Since we use the standard library `queue` object (with an underlying `list`) object, we don't need to do anything in the constructor or destructor.
By using a FIFO queue as the underlying data structure, the logic is very straightforwards. Note that as `pop` is a void method to remove the front object from the queue, rather than returning the removed value, the `evict` method is longer than it would be otherwise.
We only added an entry to the evictor when `set` was called with it, rather than with `get` as well; assuming that something is first `set` before an user calls `get` on an entry, the `set` will put the first instance of that key into the evictor, and then it will be removed first by that instance (we don't check if an entry has later instances in the evictor stack before just kickig it out).
#### Asymptotic analysis
The `std::list` underlying our evictor's queue is a doubly linked list. Insertions and access from the end are both constant time operations, and we never search the list. Thus, cache operations remain O(1)
### LRU Evictor
We have attempted the extra credit LRU evictor, implementing it with a double-linked list and a map. The list serves as a queue, with the first item being the least recently used. When a key is touched, the evictor moves that node of the list to the back, using the double-linked properties to connect the adjacent nodes together and remove the hole where that node used to be. The map (again using std unordered map) contains keys and pointers to that key's respective node in the list, and is then used to find an arbitrary node in the list in constant time. 
The first issue that we ran into was that touch used a `const key&`, and the unordered map could not be created with a reference as a key. We have tried to fix this by making the LRU map take a non-ref key, and then just set the entry with `map_[key] = entry`, as opposed to using `insert`, but at present we have segfault issues. The tests and all the rest of the code for the evictor is written and in theory is functional, but until the tests will actually run, we can't guarantee any correctness.
