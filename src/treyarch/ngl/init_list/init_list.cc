#include "treyarch/ngl/init_list/init_list.hh"

using namespace treyarch;

ngl::init_list::init_list() : next(references::init_list_head.read()) {
    references::init_list_head.write(this);
}

ngl::init_list_function::init_list_function(callback_type value) :
    callback(value) {}

void ngl::init_list_function::register_item() {
    callback();
}
