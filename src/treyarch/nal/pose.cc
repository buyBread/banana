#include <cstring>

#include "treyarch/nal/pose.hh"
#include "treyarch/shared/memory/memory.hh"
#include "treyarch/shared/tl_defs.hh"

using namespace treyarch;

// sub_9CEB30
nal::pose::pose(nal::pose_layout* layout, void* preallocated_memory) {
    init(layout, preallocated_memory);
}

// sub_9CD1D0
nal::pose::~pose() {
    if (alloced_data)
        memory::free(pose_data);
}

// sub_9CEAB0
void nal::pose::init(nal::pose_layout* layout, void* preallocated_memory) {
    pose_layout  = layout;
    pose_data    = nullptr;
    alloced_data = 0;

    present.set_all_zeroes();

    if (preallocated_memory) {
        pose_data    = (u8*)preallocated_memory;
        alloced_data = 0;
    } else {
        pose_data    = (u8*)memory::allocate(tl_align(pose_layout->pose_size, 16), 16, 0);
        alloced_data = 1;
    }

    std::memset(pose_data, 0, pose_layout->pose_size);
}

// sub_9CEB80
void nal::pose::copy(const pose &pose) {
    std::memcpy(pose_data, pose.pose_data, pose_layout->pose_size);

    present = pose.present;
}

// inlined @ sub_9CD5B0
// inlined @ sub_9CDB30
// inlined @ sub_9CDE70
// inlined @ sub_9D06E0
// inlined @ sub_9CFDB0
// inlined @ sub_9CFF00
// inlined @ sub_9CFF70
// inlined @ sub_9D0640
// inlined @ sub_9D0780
// inlined @ sub_9D0440
// inlined @ sub_9CF290
f32* nal::pose::get_field_data_ptr(u32 field_type, i32 n) {
    return (f32*)(pose_data + pose_layout->field_offsets[field_type]) + values_per_field[field_type] * n;
}
