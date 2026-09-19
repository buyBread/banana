#include "treyarch/game/wds/render/far_away_render_list.hh"

using namespace treyarch;

void far_away_render_list_entry::activate(f32 amount) {
    using activate_method = void (__thiscall*)(void* self, f32 amount);

    void* target = vhandle.resolve();

    if (target) {
        auto method = (activate_method)(*(void***)target)[108];

        method(target, amount);
    }
}
