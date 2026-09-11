#include "treyarch/game/game.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace frame_locking_references {
    util::memory_reference<i32> target_fps { 0x00BDE1C0 };
}} // treyarch::frame_locking_references

using namespace treyarch;

void game::handle_frame_locking(f32* time_inc) {
    i32 target_fps = frame_locking_references::target_fps.read();

    if (target_fps > 0)
        *time_inc = (f32)(1.0 / (f64)(f32)target_fps);
}
