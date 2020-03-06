#include "evictor.hh"
#include <queue>        // std::queue
#include <list>         // std::list

// using node_type = whatever the type of the nodes of the list are
class LruEvictor:public Evictor {
    private:
        std::list<key_type> contents_;
    public:
        void touch_key(const key_type&);
        const key_type evict();
        ~LruEvictor();
};
