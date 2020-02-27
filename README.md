# Initial Commit

## Part 1: Basic operations
Our cache uses an `unordered_map` as the underlying data type. Rather than directly storing key-value pairs in the map, we store key-(value-size) pairs.

The `get` method is required to set the `val_size` argument to be the size of the value associated with the given key, so rather than iterate through the value searching for the string terminator, we simply store the size alongside the value and return it more quickly.

However, in order to deep-copy the value into the cache when we `set` it, we must iterate over each character in the value and store it in the new value. Because variable sized arrays are illegal in C++, we instead create a pointer and ask for a variable number of spaces on the heap. Since we allocated heap memory for the deep copy, we must free it - when that value is removed from the cache.

The `del` method deletes the memory we allocated with new, helping us to prevent memory leaks

To be compliant with the pimpl idiom, we store our member variables in the implementation class.
## Part 2: Testing
## Part 3: Performance
## Part 4: Collision Resolution
## Part 5: Dynamic Resizing
## Part 6: Eviction Policy
## Part 7: Extra Credit
