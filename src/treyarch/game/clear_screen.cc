#include "treyarch/game/game.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace clear_screen_references {
    util::memory_reference<f32> red   { 0x01110C80 };
    util::memory_reference<f32> green { 0x01110C84 };
    util::memory_reference<f32> blue  { 0x01110C88 };
}} // treyarch::clear_screen_references

using namespace treyarch;

void game::clear_screen() {
    for (i32 i = 0; i != 2; ++i) {
        ngl::set_clear_flags(7);
        ngl::set_clear_color(clear_screen_references::red  .read(),
                             clear_screen_references::green.read(),
                             clear_screen_references::blue .read(),
                             0.0f);

        ngl::present();
    }
}
