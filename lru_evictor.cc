#include "lru_evictor.h"
#include "evictor.hh"
#include <list>           // std::list
#include <queue>
#include <iostream>
#include <memory>


LruEvictor::LruEvictor() {

}
LruEvictor::~LruEvictor() {

}
void LruEvictor::touch_key(const key_type& key) {
    // age of a key in contents is its index
    // front_ is age 0
    // back is age n
    // dont need to compute age of an actual thing
    if(contents_.find(key) == contents_.end()) {
        // add key to contents w/ age = 0
        contents_.push_front(key);
    } else {
        // set age to 0
        // gotta do it in O(1)...
    }
}
const key_type LruEvictor::evict() {
    /*
    return key with age 0
    */
    key_type last = contents_.front();
    contents_.pop();
    return last;
}

class LLNode {
    private:
        int age_;
        LLNode* front_;
        LLNode* rear_;
    LLNode(int age) {
        age_ = age;
    }
    LLNode(int age, LLNode* front, LLNode* rear) {
        age_ = age;
        front_ = front;
        rear_ = rear;
    }
public:
    int get_age() {
        return age_;
    }
    LLNode* get_front() {
        return front_;
    }
    LLNode* get_rear() {
        return rear_;
    }
    void set_age(int age) {
        age_ = age;
    }
    void set_front(LLNode* front) {
        front_ = front;
    }
    void set_rear(LLNode* rear) {
        rear_ = rear;
    }
};
