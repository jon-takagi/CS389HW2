# Initial Commit

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

### Compilation
Our makefile allows us to compile all necessary files for our tests. Using `make clean` also removes binary and object files, cleaning up our code before publishing to github. Examining our commit history, we were not always consistent about doing so, however, it is good practice to do so.
The makefile we use is fairly boilerplate, however, due to time issues with my virtual machine, I include the command to first touch all our source files. This updates the timestamp on these files to ensure they are seen as "in the past" by make. However, it does lead to recompilation of files that may not have been changed. A better solution might be to use NTP to fix the underlying issue with time drift in my VM, but...
## Part 3: Performance
## Part 4: Collision Resolution
## Part 5: Dynamic Resizing
## Part 6: Eviction Policy
The cache calls `touch` in `set` when it successfully adds an item to the queue. If `set` is used to overwrite a key, the key is duplicated inside of the evictor.
If calling `set` would cause the cache to exceed the maximum size set, the evictor chooses keys to `del` until there is room for the new key. Since `del` simply returns false if the chosen key is not in the cache, there is no error if the evictor's chosen key was already removed. Instead, `del` returns false, and the loop continues until a sufficient number of keys have been deleted. Note that a cache with a maxmem of 0 will loop infinitely.

### FIFO Evictor
The `FifoEvictor` extends the given evictor class. Since we use the standard library `queue` object (with an underlying `list`) object, we don't need to do anything in the constructor or destructor.
By using a FIFO queue as the underlying data structure, the logic is very straightforwards. Note that as `pop` is a void method to remove the front object from the queue, rather than returning the removed value, the `evict` method is longer than it would be otherwise.
#### Asymptotic analysis
The `std::list` underlying our evictor's queue is a doubly linked list. Insertions and access from the end are both constant time operations, and we never search the list. Thus, cache operations remain O(1)
### LRU Evictor
