#include "game/wds/entity/entity.hh"
#include "game/wds/render/far_away_render_list.hh"

using namespace treyarch;

// inlined into sub_9772D0
void far_away_render_list_entry::activate(f32 amount) {
    arch_base* target = vhandle.resolve();

    if (target)
        ((entity*)target)->unk_108(amount);
}
