#include <unordered_map>
#include <tuple>
#include "cache.hh"

using entry_type = std::pair<val_type, size_type> //To better track size for entries

Cache::Impl{
    public:
        std::unordered_map<Cache::key_type, Cache::val_type, hash_func> dict;//might be an error here; might be able to use default hash type
        size_type size;
};

Cache::Cache(index_type maxmem, evictor_type evictor = [](){ return 0; }, hash_func hasher = std::hash<std::string>()){
    std::unordered_map<Cache::key_type, Cache::val_type, hash_func> dict(4, hasher);//set to 4 buckets at start
    pImpl->dict = dict;//hope this works..
    pImpl->dict.max_load_factor(0.75);
    pImpl->size = 0;
}

//Basic version: Allows setting of key only when there's enough size, otherwise rejects
//Then copies by iterating over val to make new entry to put into pImpl dict
void Cache::set(key_type key, val_type val, index_type size) {
    if(pImpl->size + size <= maxmem){
        val_type entry_val[size];
        int i = 0;
        while(val[i] != '\0'){ //Searching for null terminator
            entry_val[i] = val[i];
        }
        pImpl->dict.insert({key,entry_val});
        pImpl->size += size;
    }
}

Cache::val_type Cache::get(key_type key, index_type& val_size) const {
    size_type size = 0;
    //Iterate through or just grab the size?
    val_size = size;
    return pImpl->dict[key];
}

void Cache::del(key_type key) {
    val_type val=pImpl->dict[key];
    int size = 0;
    while(val[size] != '\0'){ //Searching for null terminator
        size += 1;
    }
    pImpl->size -= size;
    pImpl->dict.erase(key);
}

Cache::size_type Cache::space_used() const{
    /*
    Cache::size_type size = 0;
    for (std::pair<Cache::key_type, Cache::val_type> element : pImpl->dict)
    {
    	size += sizeof(element.second);
    }
    return size;
    */
    //faster
    return pImpl->size;
}

void Cache::reset() {
    pImpl->dict.erase(pImpl->dict.begin(), pImpl->dict.end());
    pImpl->size = 0;
}
