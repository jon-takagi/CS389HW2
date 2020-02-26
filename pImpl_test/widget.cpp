#include "widget.h"
#include <memory>
class widget::Impl {
    int id;
    public:
        Impl(int id) {
            set_id(id);
        }
        int get_id() {
            return id;
        }
        void set_id(int new_id) {
            id = new_id;
        }
};
widget::widget(int id) : pImpl(std::unique_ptr<Impl>()) {

}
widget::~widget() = default;// or =default
int widget::get_id() {
    return pImpl->get_id();
}
void widget::set_id(int new_id) {
    pImpl->set_id(new_id);
}



//non pImpl version
// widget::~widget() = default;
// widget::widget(int arg) {
//     set_id(arg);
// }
// int widget::get_id(){
//     return id;
// }
// void widget::set_id(int new_id) {
//     id=new_id;
// }
