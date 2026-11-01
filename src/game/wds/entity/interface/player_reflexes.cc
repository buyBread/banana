#include "retail.hh"
#include "game/wds/entity/interface/player_reflexes.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/time_interface.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"

using namespace treyarch;

// sub_7EFE40
void player_reflexes::enable() {
    if (enabled || state == player_reflexes_state_locked_off)
        return;

    enabled = true;

    ((void (__thiscall*)(wds_time_manager*, f32))retail::sub_68B960)(&references::g_world_ptr.read()->time_mgr, world_time_dilation); // the world's reflex dilation

    if (!owner->my_actor->has_ifc(entity_ifc_time))
        owner->my_actor->create_time_ifc();

    owner->my_actor->time_ifc()->set_time_dilation((f32)((f64)player_time_dilation / (f64)world_time_dilation));
    owner->my_actor->time_ifc()->set_mode(time_mode_relative);

    string_hash enter_reflexes;
    enter_reflexes.initialize(mash::ALLOCATED, "enter_reflexes");

    retail::sub_900D60(enter_reflexes.source_hash_code, 0x10000000, 0); // post the sound event on the global game object

    owner->my_actor->raise_event(references::reflexes_entered.read());
}

// sub_7EFF80
void player_reflexes::disable(bool force) {
    if (!enabled || (state == player_reflexes_state_locked_on && !force))
        return;

    enabled = false;

    ((void (__thiscall*)(wds_time_manager*, f32))retail::sub_68B960)(&references::g_world_ptr.read()->time_mgr, 1.0f); // the world's reflex dilation

    if (owner->my_actor->has_ifc(entity_ifc_time)) {
        owner->my_actor->time_ifc()->time_dilation = 1.0f;
        owner->my_actor->time_ifc()->set_mode(time_mode_relative);
    }

    string_hash exit_reflexes;
    exit_reflexes.initialize(mash::ALLOCATED, "exit_reflexes");

    retail::sub_900D60(exit_reflexes.source_hash_code, 0x10000000, 0); // post the sound event on the global game object

    owner->my_actor->raise_event(references::reflexes_exited.read());
}

// sub_7EA6F0
void player_reflexes::inc_reflex_percent(f32 percent) {
    reflex_pool = (f32)((f64)percent + (f64)reflex_pool);

    if (min_engage_percent > reflex_pool && owner->unk_073())
        reflex_pool = (f32)((f64)min_engage_percent + (f64)0.0001f);

    if (reflex_pool < 0.0f)
        reflex_pool = 0.0f;
    else if (reflex_pool > 1.0f)
        reflex_pool = 1.0f;
}

// sub_7EA770
void player_spider_senses::enable(bool unk) {
    if (state == player_reflexes_state_locked_off)
        return;

    if (unk)
        references::g_world_ptr.read()->get_hero_ptr()->spawn_effect(references::spidey_sense_shockwave.read(), 0);

    enabled = true;
    unk_04  = unk_08;
    unk_1c  = 0.0f;

    owner->unk_043(1);

    retail::sub_6D88E0((u32*)references::frontend.get().igo, unk); // ui_frontend's spidersense start
    retail::sub_906060();
}

// sub_7EA840
void player_spider_senses::disable(bool unk, bool force) {
    if (!enabled || (state == player_reflexes_state_locked_on && !force))
        return;

    enabled = false;
    unk_15  = false;
    unk_16  = false;
    unk_0c  = 0.0f;

    retail::sub_6D8A20((u32*)references::frontend.get().igo, unk); // ui_frontend's spidersense exit
}

// sub_7EA880
void player_spider_senses::lock_on(bool unk) {
    enable(unk);
    state = player_reflexes_state_locked_on;
}

// sub_7EA8A0
void player_spider_senses::lock_off(bool unk) {
    disable(unk, false);
    state = player_reflexes_state_locked_off;
}

// sub_7EA8E0
void player_spider_senses::unlock(bool unk) {
    disable(unk, false);
    state = player_reflexes_state_normal;
}
