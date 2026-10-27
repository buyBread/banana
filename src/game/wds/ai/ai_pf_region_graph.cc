#include <new>

#include "retail.hh"
#include "game/wds/ai/ai_pf_region_graph.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// the cell lists these two keep belong to ped spawning
ai_quad_path_cell::~ai_quad_path_cell() {
    retail::sub_767F80((i32)this);
}

// inlined @ sub_4421F0
void ai_quad_path_cell::construct_mashed_class() {
    unk_28 = 0;
    unk_24 = 0;

    retail::sub_767F30((i32)this);
}

void ai_quad_path_cell::destruct_mashed_class() {
    retail::sub_767F80((i32)this);
}

// sub_4435C0
void ai_quad_path::construct_mashed_class() {
    connection_table.construct_mashed_class();
    vertices        .construct_mashed_class();
    cell_table      .construct_mashed_class();
    unk_6c          .construct_mashed_class();
    unk_7c          .construct_mashed_class();

    void* allocation = mm::allocate(sizeof(vc8::vector<void*>));

    obstacles = allocation ?
        new (allocation) vc8::vector<void*>() : nullptr;
}

// sub_443690
void ai_quad_path::destruct_mashed_class() {
    finalize(mash::FROM_MASH);

    connection_table.destruct_mashed_class();
    vertices        .destruct_mashed_class();
    cell_table      .destruct_mashed_class();
    unk_6c          .destruct_mashed_class();
    unk_7c          .destruct_mashed_class();
}

// sub_444710
void ai_quad_path::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    connection_table.unmash(mash_info, this, buffer);
    vertices        .unmash(mash_info, this, buffer);
    cell_table      .unmash(mash_info, this, buffer);
    unk_6c          .unmash(mash_info, this, buffer);
    unk_7c          .unmash(mash_info, this, buffer);
}

// sub_440C20
void ai_quad_path::finalize(mash::allocation_scope) {
    obstacles->clear();

    if (obstacles) {
        obstacles->~vector();

        mm::free(obstacles);
    }
}

// sub_449870
void ai_pf_region_graph::construct_mashed_class() {
    self = nullptr;

    // sub_58CA60
    lock.owner = 0;
    lock.state = 0;
    lock.depth = 0;

    quad_path_table.construct_mashed_class();
    region_points  .construct_mashed_class();

    void* allocation = mm::allocate(sizeof(vc8::vector<ai_quad_path_connection*>));

    unk_18 = 0;
    unk_64 = allocation ?
        new (allocation) vc8::vector<ai_quad_path_connection*>() : nullptr;
}

// sub_447C70
void ai_pf_region_graph::destruct_mashed_class() {
    if (unk_64) {
        unk_64->~vector();

        mm::free(unk_64);
    }

    unk_64 = nullptr;

    quad_path_table.destruct_mashed_class();
    region_points  .destruct_mashed_class();
}

// sub_449FF0
void ai_pf_region_graph::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    quad_path_table.unmash(mash_info, this, buffer);
    region_points  .unmash(mash_info, this, buffer);
}
