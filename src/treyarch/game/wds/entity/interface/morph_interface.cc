#include "retail.hh"
#include "treyarch/game/wds/entity/interface/morph_interface.hh"

using namespace treyarch;

// sub_63CCE0
void morph_interface::frame_advance_all_morph_ifcs(f32 t) {
    for (morph_interface* ifc : references::all_morph_ifcs.get())
        retail::sub_6351D0((i32)ifc, t); // morph_interface::frame_advance
}
