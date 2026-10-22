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
