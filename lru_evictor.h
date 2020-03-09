#include "evictor.hh"
#include <unordered_map> // std::unordered_map

class LLNode {
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

};

// using node_type = whatever the type of the nodes of the list are
class LruEvictor:public Evictor {
    private:
        std::unordered_map<const key_type&, *LLNode> map_;
        LLNode* head_;
        LLNode* tail_;
    public:
        void touch_key(const key_type&);
        const key_type evict();
        ~LruEvictor();
};
