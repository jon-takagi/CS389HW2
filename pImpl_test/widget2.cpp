#include "widget.h"

class widget::Impl {
    public:
        int id_;
};
widget::widget(int id):pImpl_(new Impl()){
    pImpl_->id_=id;
}
int widget::get_id() {
    return pImpl_->id_;
}
void widget::set_id(int id) {
    pImpl_->id_=id;
}
widget::~widget() {
    
}
