#include "retail.hh"
#include "treyarch/game/wds/entity/entity.hh"

using namespace treyarch;

void entity::invoke_render_phase() {
    using render_phase_method = void (__thiscall*)(void* self);

    void* target = (void*)retail::sub_402CC0(render_object_table, 11);
    auto method = (render_phase_method)(*(void***)target)[90];

    method(target);
}
