#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/frame_lock.hh"

namespace treyarch { namespace ngl { namespace references {
    util::memory_reference<u32> frame_lock_immediate { 0x011170C0 };
}}} // treyarch::ngl::references

using namespace treyarch;

u32 ngl::apply_frame_lock(e_frame_lock frame_lock) {
    u32 presentation_interval = D3DPRESENT_INTERVAL_ONE;

    switch (frame_lock) {
        case frame_lock_none:
            presentation_interval = D3DPRESENT_INTERVAL_IMMEDIATE;
            references::frame_lock_immediate.write(0);
            break;
        case frame_lock_one:
        case frame_lock_one_or_immediate:
            references::frame_lock_immediate.write(0);
            break;
        case frame_lock_two:
        case frame_lock_two_or_immediate:
            presentation_interval = D3DPRESENT_INTERVAL_TWO;
            references::frame_lock_immediate.write(0);
            break;
        case frame_lock_three:
        case frame_lock_three_or_immediate:
            presentation_interval = D3DPRESENT_INTERVAL_THREE;
            references::frame_lock_immediate.write(0);
            break;
    }

    references::current_frame_lock.write(frame_lock);
    d3d9::references::presentation.get().PresentationInterval = presentation_interval;

    return presentation_interval;
}

void ngl::set_frame_lock(e_frame_lock frame_lock) {
    if (frame_lock == references::requested_frame_lock.read())
        return;

    apply_frame_lock(frame_lock);
    
    references::requested_frame_lock.write(frame_lock);
}
