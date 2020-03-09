#include "evictor.hh"
#include <functional>
#include <unordered_map> // std::unordered_map

class LLNode {
    public:
        key_type key_;
        LLNode* front_;
        LLNode* rear_;
    LLNode(key_type key, LLNode* front, LLNode* rear) {
        key_ = key;
        front_ = front;
        rear_ = rear;
    }

};

using node_ptr = LLNode*;

// using node_type = whatever the type of the nodes of the list are
class LruEvictor:public Evictor {
    private:
        std::unordered_map<key_type, LLNode*> map_;
    public:
        LLNode* head_;
        LLNode* tail_;
        void touch_key(const key_type&);
        const key_type evict();
        ~LruEvictor();
        LruEvictor();
};
