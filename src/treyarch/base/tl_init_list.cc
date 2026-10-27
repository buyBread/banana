#include "treyarch/base/tl_init_list.hh"

using namespace treyarch;

tl_init_list::tl_init_list() : next(references::tl_init_list_head.read()) {
    references::tl_init_list_head.write(this);
}

tl_init_list_function::tl_init_list_function(callback_type value) :
    callback(value) {}

// sub_8842A0
void tl_init_list_function::register_item() {
    callback();
}
