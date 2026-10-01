#include "treyarch/game/post_process/blitter.hh"
#include "treyarch/game/post_process/filter.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_72F3A0
post_process::filter::filter(treyarch::blitter* owner) {
    vtable  = &references::filter_vtable.get();
    blitter = owner;

    if (owner)
        ++owner->reference_count;

    setup = nullptr;
}

void post_process::filter::destroy() {
    using deleting_destructor = void*(__thiscall*)(filter*, u8);

    ((deleting_destructor*)vtable)[0](this, 1);
}

// sub_72F3C0
post_process::down_sample_filter::down_sample_filter(treyarch::blitter* owner)
    : filter(owner) {

    vtable = &references::down_sample_filter_vtable.get();
}

// sub_745A70
post_process::filter_queue_node::~filter_queue_node() {}

// sub_73C670
void post_process::filter_queue::reset() {
    if (tail) {
        tail->next = free_nodes;
        free_nodes = head;
    }

    head  = nullptr;
    tail  = nullptr;
    count = 0;

    for (filter_queue_node* node = free_nodes; node; node = node->next) {
        node->sources.count = 0;
        node->targets.count = 0;
    }
}

// sub_757A20
void post_process::filter_queue::clear() {
    reset();

    for (filter_queue_node* node = free_nodes; node;) {
        filter_queue_node* released = node;

        node = node->next;

        released->~filter_queue_node();
        memory::heap::free(released);
    }

    free_nodes = nullptr;
}
