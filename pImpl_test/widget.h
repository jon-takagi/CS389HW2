#include <memory>

class widget {
    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;
    public:
        widget(const int id=0);
        void set_id(int new_id);
        int get_id();
        ~widget();
};

//non pImpl version
// class widget {
//     private:
//         int id;
//     public:
//         widget(const int id=0);
//         void set_id(int new_id);
//         int get_id();
//         ~widget();
// };
