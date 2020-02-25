#include <cache.hh>

Cache::Cache(index_type maxmem, evictor_type evictor = [](){ return 0; }, hash_func hasher = std::hash<std::string>()){
return null;
}

void Cache::set(key_type key, val_type val, index_type size) {

}

Cache::val_type Cache::get(key_type key, index_type& val_size) const {
return null;
}

void Cache::del(key_type key) {

}
Cache::index_type Cache::space_used() const{
return null;
}
