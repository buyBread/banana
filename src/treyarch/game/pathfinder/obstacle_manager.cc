#include <new>

#include "treyarch/game/pathfinder/obstacle_manager.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_42D4D0; IDA's DNameNode label is a library false positive
pathfinder::obstacle_manager::obstacle_manager() {
    vtable = &references::obstacle_manager_vtable.get();
    unk_04 = 0;
}

// sub_428FC0
void pathfinder::obstacle_manager::create_inst() {
    void* allocation = memory::heap::allocate(sizeof(obstacle_manager));

    obstacle_manager::instance() = allocation ? new (allocation) obstacle_manager() : nullptr;
}
