#include "retail.hh"
#include "game/physics/rb_character_controller.hh"

using namespace treyarch;

// sub_819260
void treyarch::rb_character_controller_pool_free(rb_character_controller* controller) {
    retail::sub_813710((u32*)&references::rb_character_controller_pool.get(), (u32*)controller);
}
