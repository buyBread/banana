#include <new>

#include "treyarch/chuck/script_library/script_library_class.hh"
#include "treyarch/chuck/script_library/slc_manager.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;
using namespace treyarch::chuck::script_library;

// sub_A21710
script_library_class::script_library_class(const char*,
                                                 i32   requested_size,
                                           const char* requested_parent_name,
                                                 bool  is_dummy) : size(requested_size),
                                                                   functions(nullptr) {

    parent_name = requested_parent_name && *requested_parent_name ?
        requested_parent_name : nullptr;

    if (is_dummy)
        return;

    void* allocation = memory::heap::allocate(sizeof(dinkumware::vector<script_library_function*>));

    functions = allocation ?
        new (allocation) dinkumware::vector<script_library_function*>() : nullptr;

    functions->reserve(10);

    slc_manager::inst()->add(this);
}

// sub_A21010
// sub_83DAC0
script_library_class::~script_library_class() {
    if (!functions)
        return;

    i32 count = (i32)functions->size();

    for (i32 index = 0; index < count; ++index) {
        script_library_function* &function = (*functions)[index];

        if (function) {
            delete function;

            function = nullptr;
        }
    }

    functions->erase(functions->begin(), functions->end());

    functions->~vector();
    memory::heap::free(functions);
}

// sub_A90D90
u32 script_library_class::find_instance(const mash::string&) const {
    return 0;
}

// sub_A20F80
script_library_function* script_library_class::get_function(i32 index) const {
    return (*functions)[index];
}

// sub_A217B0
slc_num_t::slc_num_t(const char* name, i32 requested_size, const char* requested_parent_name)
    : script_library_class(name, requested_size, requested_parent_name, false) {}

// sub_A217E0
slc_uint_t::slc_uint_t(const char* name, i32 requested_size, const char* requested_parent_name)
    : script_library_class(name, requested_size, requested_parent_name, false) {}

// sub_A21810
slc_str_t::slc_str_t(const char* name, i32 requested_size, const char* requested_parent_name)
    : script_library_class(name, requested_size, requested_parent_name, false) {}

// sub_A214E0
slc_str_t::~slc_str_t() {}

// sub_A215E0
script_library_function::script_library_function(script_library_class* slc) {
    slc->functions->push_back(this);
}

// sub_A21650
script_library_function::script_library_function() {
    references::slc_global.read()->functions->push_back(this);
}

// sub_823B70
bool script_library_function::operator()(vm::vm_stack&) {
    return true;
}
