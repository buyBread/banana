#include "retail.hh"
#include "treyarch/chuck/script_library/slc_manager.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;
using namespace treyarch::chuck::script_library;

// inlined @ sub_97CEA0
void slc_manager::destroy() {
    slc_manager* manager = inst();

    if (manager) {
        retail::sub_A21450((u32**)manager); // ~slc_manager
        memory::heap::free(manager);
    }

    instance() = nullptr;
}
