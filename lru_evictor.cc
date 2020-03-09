#include "lru_evictor.h"
#include "evictor.hh"
#include <queue>
#include <iostream>
#include <memory>


LruEvictor::LruEvictor() {

}
LruEvictor::~LruEvictor() {

}

//Adds/updates a the position of a key in the evictor
void LruEvictor::touch_key(const key_type& k) {
    key_type key = k; //unordered_map won't work with a ref as a key, so we need this change
    //If the key isn't in the evictor already, we simply add it to the back (most recent)
    if(map_[key] == nullptr){
        LLNode* entry;
        entry->key_ = key;
        entry->front_ = tail_;
        tail_->rear_ = entry;
        tail_ = entry;
        map_.insert(key, entry);
    //Also, if this was the first node in, we need to set the head_ of the list
        if(head_ == nullptr){
            head_ = entry;
        }
    //Otherwise, we pull the entry out of its place, stitch together its neighbors, and shove it at the back
    } else {
        LLNode* entry = map_[key];
        if(entry != tail_){ //If it is tail, its already at the back, and the work is done
            if(entry != head_){ //Now we can assume that entry has a valid front and rear (not nullptrs)
                entry->front_->rear_ = entry->rear_;
            }else{
                head_ = entry->rear_;
            }
            entry->rear_->front_ = entry->front_; //Entry has rear regardless; this will work even if entry->front is nullptr
            //And now make it the tail
            entry->front_ = tail_;
            tail_->rear_ = entry;
            tail_ = entry;
        }
    }
}
const key_type LruEvictor::evict() {
    LLNode* last = tail_;
    key_type last_key= nullptr;
    if(last != nullptr){//If evictor not empty
        tail_ = last->front_;
        if(last != head_){//If last not the only element, then change head_
            last->front_->rear_ = nullptr;
        }    
        else{//Will need to update head if the evictor is now empty
            head_ = nullptr;
        }
        last_key = last->key_;
    }
    //remove from the map
    return last_key;

}

