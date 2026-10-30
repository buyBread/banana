#include "game/wds/entity/interface/tentacle_interface.hh"
#include "game/polytube.hh"

using namespace treyarch;

// sub_61F5A0
void tentacle_interface::set_tentacle_end_target(u32 which, arch_base_vhandle target) {
    end_targets.array[which] = target;
}

// sub_61FBF0
void tentacle_interface::set_tentacle_default_pct(u32 which, f32 pct) {
    tentacle_infos.array[which]->unk_01c     = pct;
    tentacle_infos.array[which]->unk_024     = pct;
    tentacle_infos.array[which]->unk_028     = 0.0f;
    tentacle_infos.array[which]->default_pct = pct;
}

// sub_61FC30
void tentacle_interface::set_tentacle_effect(u32 which, const string_hash &effect) {
    if (looping_effects.array[which]) {
        polytubes.array[which]->stop_looping_effect(looping_effects.array[which]);
        looping_effects.array[which] = 0;
    }

    tentacle_infos.array[which]->effect = effect;
}

// sub_61FCF0
u32 tentacle_interface::play_tentacle_effect(u32 which, const string_hash &effect) {
    return polytubes.array[which]->spawn_effect(effect, 0);
}
