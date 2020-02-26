#include <unordered_map>
#include <tuple>
#include "cache.hh"


using entry_type = std::pair<Cache::val_type, Cache::size_type>; //To better track size for entries

class Cache::Impl{
    public:
        std::unordered_map<key_type, Cache::val_type, hash_func> dict_;//might be an error here; might be able to use default hash type
        size_type size_;
        size_type maxmem_;
        Evictor* evictor_;
};

Cache::Cache(size_type maxmem,float max_load_factor, Evictor* evictor, hash_func hasher)
 : pImpl_(new Impl()) {
    std::unordered_map<key_type, Cache::val_type, hash_func> dict(4,hasher);//set to 4 buckets at start
    pImpl_->dict_ = dict;//hope this works..
    pImpl_->dict_.max_load_factor(max_load_factor);
    pImpl_->size_ = 0;
    pImpl_->maxmem_ = maxmem;
    pImpl_->evictor_ = evictor;
}

Cache::~Cache() {

}
// Basic version: Allows setting of key only when there's enough size, otherwise rejects
// Then copies by iterating over val to make new entry to put into pImpl_ dict
// void Cache::set(key_type key, val_type val, size_type size) {
    // if(pImpl_->size_ + size <= pImpl_->maxmem_){
    //     val_type entry_val[size];
    //     int i = 0;
    //     while(val[i] != '\0'){ //Searching for null terminator
    //         entry_val[i] = val[i];
    //     }
    //     std::pair<Cache::val_type, Cache::size_type> entry = std::make_pair(key,entry_val);
    //     pImpl_->dict_.insert(entry);
    //     pImpl_->size_ += size;
    // }
// }

Cache::val_type Cache::get(key_type key, size_type& val_size) const {
    size_type size = 0;
    //Iterate through or just grab the size?
    val_size = size;
    return pImpl_->dict_[key];
}

bool Cache::del(key_type key) {
    val_type val=pImpl_->dict_[key];
    int size = 0;
    while(val[size] != '\0'){ //Searching for null terminator
        size += 1;
    }
    pImpl_->size_ -= size;
    pImpl_->dict_.erase(key);
    return true;
}

Cache::size_type Cache::space_used() const{
    /*
    Cache::size_type size = 0;
    for (std::pair<Cache::key_type, Cache::val_type> element : pImpl_->dict_)
    {
    	size += sizeof(element.second);
    }
    return size;
    */
    //faster
    return pImpl_->size_;
}

void Cache::reset() {
    pImpl_->dict_.erase(pImpl_->dict_.begin(), pImpl_->dict_.end());
    pImpl_->size_ = 0;
}
