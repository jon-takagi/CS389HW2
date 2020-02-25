#include <unordered_map>
#include "cache.hh"

Cache::Impl{
    public:
        std::unordered_map<Cache::key_type, Cache::val_type> dict = {};
        size_type size = 0;

        Impl::Impl()
        {
           dict = {};
           size = 0;
        }
};

Cache::Cache(index_type maxmem, evictor_type evictor = [](){ return 0; }, hash_func hasher = std::hash<std::string>()){
return null;
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
return null;
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
    Cache::size_type size = 0;
    for (std::pair<Cache::key_type, Cache::val_type> element : pImpl->dict)
    {
    	size += sizeof(element.second);
    }
    return size;
}

void Cache::reset() {

}
