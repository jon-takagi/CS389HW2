#include <unordered_map>
#include <tuple>
#include "cache.hh"
#include "evictor.hh"

using entry_type = std::pair<Cache::val_type, Cache::size_type> //To better track size for entries

class Cache::Impl{
    private:
        std::unordered_map<key_type, Cache::val_type, hash_func> dict;//might be an error here; might be able to use default hash type
        size_type size;
        size_type maxmem;
    public:
        Impl( size_type maxmem,
              float max_load_factor = 0.75,
              Evictor* evictor = nullptr,
              hash_func hasher = std::hash<key_type>()
            ){
                std::unordered_map<key_type, Cache::val_type, hash_func> dict(4, hasher);//set to 4 buckets at start
                dict.max_load_factor(max_load_factor);
                size = 0;
                maxmem = maxmem;
        }
        void set(key_type key, val_type val, size_type a_size){
            if(size + a_size <= maxmem){
                val_type *entry_val=new val_type[a_size];
                int i = 0;
                while(val[i] != '\0'){ //Searching for null terminator
                    entry_val[i] = val[i];
                }
                dict.insert({key,entry_val});
                size += a_size;
                delete entry_val;
            }
        }
        val_type get(key_type key, size_type& val_size) const {
            size_type size = 0;
            //Iterate through or just grab the size?
            val_size = size;
            return dict[key];
        }
        bool del(key_type key){
            val_type val=dict[key];
            int size = 0;
            while(val[size] != '\0'){ //Searching for null terminator
                size += 1;
            }
            size -= size;
            dict.erase(key);
        }
        size_type space_used() const {
            return size;
        }
        void reset() {
            dict.erase(dict.begin(), dict.end());
            size = 0;
        }
}

Cache::Cache(size_type maxmem, float max_load_factor = 0.75, Evictor* evictor = nullptr, hash_func hasher = std::hash<evictor::key_type>())
: pImpl(std::unique_ptr<Impl>()){
}

// widget::widget(int id) : pImpl(std::unique_ptr<Impl>()) {
//
// }
//Basic version: Allows setting of key only when there's enough size, otherwise rejects
//Then copies by iterating over val to make new entry to put into pImpl dict
void Cache::set(key_type key, val_type val, Cache::size_type size) {
    pImpl->set(key, val, size);
}

Cache::val_type Cache::get(key_type key, Cache::size_type& val_size) const {
    return pImpl->get(key, val_size);
}

void Cache::del(key_type key) {
    pImpl->del(key);
}

Cache::size_type Cache::space_used() const{
    /*
    Cache::size_type size = 0;
    for (std::pair<key_type, Cache::val_type> element : pImpl->dict)
    {
    	size += sizeof(element.second);
    }
    return size;
    */
    //faster
    return pImpl->space_used();
}

void Cache::reset() {
    pImpl->reset();
}
