#include "treyarch/game/arch_base.hh"
#include "treyarch/game/wds/render/far_away_render_list.hh"

using namespace treyarch;

// inlined into sub_9772D0; virtual slot 108 of the resolved target
void far_away_render_list_entry::activate(f32 amount) {
    using activate_method = void (__thiscall*)(arch_base* self, f32 amount);

    arch_base* target = vhandle.resolve();

    if (target) {
        auto method = (activate_method)target->vtable[108];

        method(target, amount);
    }
}
