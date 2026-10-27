#include "treyarch/ngl/frame_lock.hh"
#include "treyarch/ngl/gpu/d3d9/device.hh"

namespace treyarch { namespace ngl { namespace references {
    util::memory_reference<u32> frame_lock_immediate { 0x011170C0 };
}}} // treyarch::ngl::references

using namespace treyarch;

// sub_9DC920
u32 ngl::d3d9::set_frame_lock_params(e_frame_lock frame_lock) {
    u32 presentation_interval = D3DPRESENT_INTERVAL_ONE;

    switch (frame_lock) {
        case frame_lock_none:
            presentation_interval = D3DPRESENT_INTERVAL_IMMEDIATE;
            ngl::references::frame_lock_immediate.write(0);
            break;
        case frame_lock_one:
        case frame_lock_one_or_immediate:
            ngl::references::frame_lock_immediate.write(0);
            break;
        case frame_lock_two:
        case frame_lock_two_or_immediate:
            presentation_interval = D3DPRESENT_INTERVAL_TWO;
            ngl::references::frame_lock_immediate.write(0);
            break;
        case frame_lock_three:
        case frame_lock_three_or_immediate:
            presentation_interval = D3DPRESENT_INTERVAL_THREE;
            ngl::references::frame_lock_immediate.write(0);
            break;
    }

    ngl::references::frame_lock.write(frame_lock);
    references::present_params.get().PresentationInterval = presentation_interval;

    return presentation_interval;
}

// sub_9DC980
void ngl::set_frame_lock(e_frame_lock frame_lock) {
    if (frame_lock == references::prev_frame_lock.read())
        return;

    d3d9::set_frame_lock_params(frame_lock);
    
    references::prev_frame_lock.write(frame_lock);
}
