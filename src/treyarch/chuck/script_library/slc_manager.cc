#include <new>

#include "treyarch/chuck/script_library/slc_manager.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "game/script/chuck_register_script_libs.hh"
#include "game/script/client_script_libs.hh"
#include "treyarch/mm/heap.hh"

using namespace treyarch;
using namespace treyarch::chuck::script_library;

// sub_A20FA0
slc_manager::slc_manager() : classes(nullptr) {
    void* allocation = memory::heap::allocate(sizeof(vc8::vector<script_library_class*>));

    classes = allocation ?
        new (allocation) vc8::vector<script_library_class*>() : nullptr;
}

// sub_A21450
slc_manager::~slc_manager() {
    destruct_client_script_libs();

    vm::vm_dynamic_array_manager::destroy();

    if (!classes)
        return;

    for (u32 index = 0; index < classes->size(); ++index) {
        script_library_class* slc = (*classes)[index];

        if (slc)
            delete slc;
    }

    classes->~vector();
    memory::heap::free(classes);

    classes = nullptr;
}

// sub_97AAB0
void slc_manager::setup() {
    void* allocation = memory::heap::allocate(sizeof(slc_manager));

    instance() = allocation ? new (allocation) slc_manager() : nullptr;

    instance()->init();
}

// sub_A20E70
void slc_manager::init() {
    vm::vm_dynamic_array_manager::setup();

    chuck_register_script_libs();

    construct_client_script_libs();
}

// sub_A216C0
void slc_manager::add(script_library_class* slc) {
    classes->push_back(slc);
}

// sub_A20F90
script_library_class* slc_manager::get_class(i32 index) const {
    return (*classes)[index];
}

// inlined @ sub_97CEA0
void slc_manager::destroy() {
    slc_manager* manager = inst();

    if (manager) {
        manager->~slc_manager();
        memory::heap::free(manager);
    }

    instance() = nullptr;
}
