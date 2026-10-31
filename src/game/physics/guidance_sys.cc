#include "retail.hh"
#include "game/physics/guidance_sys.hh"

using namespace treyarch;

void rocket_guidance_sys::set_target(const arch_base_vhandle &ent) {
    retail::sub_739A30((i32*)this, (i32*)&ent);
}
