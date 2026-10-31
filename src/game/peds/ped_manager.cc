#include <cmath>
#include <cstdlib>
#include <intrin.h>

#include "retail.hh"
#include "game/als/als.hh"
#include "game/app.hh"
#include "game/game.hh"
#include "game/peds/ped_jq.hh"
#include "game/peds/ped_manager.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"

using namespace treyarch;

// sub_75F070
void treyarch::ped_jq_callback(jq::batch* batch) {
    ped_manager::advance_ped((actor*)batch->payload[0],
                             &ped_manager::focus,
                             &ped_manager::camera_position,
                             &ped_manager::camera_facing);
}

// sub_763520
void ped_manager::frame_advance(f32 time_inc) {
    if (!ped_spawn_pos_search_jq::finished.read()) {
        retail::sub_A148D0(ped_spawn_pos_search_jq::group.read(), 0, 1); // jq_flush

        ped_spawn_pos_search_jq::finished.write(true);
    }

    if (!ped_spawn_quad_search_jq::finished.read()) {
        retail::sub_A148D0(ped_spawn_quad_search_jq::group.read(), 0, 1); // jq_flush

        ped_spawn_quad_search_jq::finished.write(true);
    }

    retail::sub_7437C0(time_inc);

    u32 frame = references::g_world_ptr.read()->time_mgr.frame_sequence;

    if (unk_01036e4c != frame) {
        unk_0102fee8 = 0;
        unk_0102feec = 0;
        unk_01036e4c = frame;
    }

    unk_0102fedc = references::unk_0102fee0.read();
    references::unk_0102fee0.write(0);
    unk_0102fee4 = 0;

    camera* view_camera = references::game.read()->get_current_view_camera();

    const vector3d &facing = view_camera->my_abs_po->get_z_facing();

    camera_facing.x = facing.x;
    camera_facing.y = facing.y;
    camera_facing.z = facing.z;
    camera_facing.w = 0.0f;

    const vector3d &position = view_camera->my_abs_po->get_position();

    camera_position.x = position.x;
    camera_position.y = position.y;
    camera_position.z = position.z;
    camera_position.w = 0.0f;

    focus = camera_position;

    if (entity* hero = references::g_world_ptr.read()->hero_ptr) {
        const vector3d &hero_position = hero->my_abs_po->get_position();

        focus.x = hero_position.x;
        focus.y = hero_position.y;
        focus.z = hero_position.z;
        focus.w = 0.0f;
    }

    references::unk_00e770d4.write(0.0f > camera_position.y ? 0.5f : 1.0f);

    references::unk_01030a40.write(camera_facing);
    references::unk_01030a50.write(camera_position);
    references::unk_01030a30.write(focus);

    if (retail::sub_740A10()) {
        i32 unk = references::unk_00e77104.read();

        unk_01030090 = 0;
        unk_01030094 = 0;
        unk_0102feec = unk;
        unk_0102fee8 = unk;
    }

    // ped_jq's start, but the workers only wake with peds enabled
    bool multi_threading = jq::references::in_multi_threaded_code.read() <= 0 && references::master_clock_is_up.read();

    if (multi_threading)
        jq::references::in_multi_threaded_code.write(jq::references::in_multi_threaded_code.read() + 1);

    ped_jq::multi_threading.write(multi_threading);
    ped_jq::profiling      .write(false);
    ped_jq::started        .write(true);

    vc8::vector<arch_base_vhandle> &peds = references::ped_entities.get();

    if (!references::peds_enabled.read()) {
        for (arch_base_vhandle* it = peds.begin(); it != peds.end();) {
            auto ped = (actor*)it->resolve();

            if (!ped) {
                it = peds.erase(it);
                continue;
            }

            if (ped->is_visible())
                unspawn_ped(ped);

            ++it;
        }
    } else {
        ped_jq::keep_workers_awake();
        ped_jq::lock_batch_pool();

        for (arch_base_vhandle* it = peds.begin(); it != peds.end();) {
            auto ped = (actor*)it->resolve();

            if (!ped) {
                it = peds.erase(it);
                continue;
            }

            if (ped->is_visible()) {
                ped_jq::static_batch.get().payload[0] = ped;
                ped_jq::add_batch(1);
            }

            ++it;
        }

        ped_jq::unlock_batch_pool();
    }

    ped_jq::finish(false);

    vendor_jq::start();
    vendor_jq::lock_batch_pool();

    vc8::vector<arch_base_vhandle> &vendors = references::vendor_entities.get();

    for (arch_base_vhandle* it = vendors.begin(); it != vendors.end();) {
        arch_base* vendor = it->resolve();

        if (!vendor) {
            it = vendors.erase(it);
            continue;
        }

        vendor_jq::static_batch.get().payload[0] = vendor;
        vendor_jq::add_batch(1);

        ++it;
    }

    vendor_jq::unlock_batch_pool();
    vendor_jq::finish(false);

    f32 dilation = references::g_world_ptr.read()->time_mgr.get_time_dilation_factor();

    retail::sub_7584F0((f32)((f64)dilation * (f64)time_inc),
                       (i32)&focus,
                       (i64*)&camera_position,
                       (i64*)&camera_facing);
}

// sub_758B90
bool ped_manager::advance_ped(actor*          ped,
                              const vector4d* the_focus,
                              const vector4d* the_camera_position,
                              const vector4d* the_camera_facing) {

    auto wander = (ai_wander_inode*)ped->get_ai_core()->get_info_node(info_node_type_ai_wander);

    if (!ped->is_visible()) {
        if (wander->flags & ai_wander_flag_unk_02000000) {
            if (!(wander->flags & ai_wander_flag_available)) {
                unspawn_ped(ped);
                return false;
            }

            return true;
        }

        if (std::rand() % 4 >= 1 || references::unk_0102fee0.read() >= references::max_peds.read() - 10)
            return true;

        _InterlockedIncrement((volatile long*)&unk_0102feec);
        _InterlockedIncrement((volatile long*)&references::unk_0102fee0.get());

        if (!retail::sub_753200((i32)ped, (i32*)the_camera_position, (i32*)the_camera_facing, true, true)) {
            _InterlockedDecrement((volatile long*)&unk_0102feec);
            _InterlockedDecrement((volatile long*)&references::unk_0102fee0.get());
        }

        return unk_0102feec >= references::unk_00e77104.read();
    }

    if (!(wander->flags & ai_wander_flag_unk_02000000))
        retail::sub_753200((i32)ped, (i32*)the_camera_position, (i32*)the_camera_facing, false, false);

    const vector3d &position = ped->my_abs_po->get_position();

    f32 camera_dx          = (f32)((f64)the_camera_position->x - (f64)position.x);
    f32 camera_dz          = (f32)((f64)the_camera_position->z - (f64)position.z);
    f32 camera_distance_sq = (f32)((f64)camera_dx * camera_dx + (f64)camera_dz * camera_dz); // across the ground

    f32 focus_dx          = (f32)((f64)the_focus->x - (f64)position.x);
    f32 focus_dy          = (f32)((f64)the_focus->y - (f64)position.y);
    f32 focus_dz          = (f32)((f64)the_focus->z - (f64)position.z);
    f32 focus_distance_sq = (f32)((f64)focus_dx * focus_dx + (f64)focus_dy * focus_dy + (f64)focus_dz * focus_dz);

    f32 height = std::fabs((f32)((f64)the_camera_position->y - (f64)position.y));

    auto car = (ai_car_inode*)ped->get_ai_core()->get_info_node(info_node_type_ai_car);

    if (car && car->is_in_car)
        _InterlockedIncrement((volatile long*)&unk_0102fee4);
    else
        _InterlockedIncrement((volatile long*)&references::unk_0102fee0.get());

    bool unk_00100000 = wander->flags & ai_wander_flag_unk_00100000;

    if (ped->has_ifc(entity_ifc_card))
        ped->card_ifc()->unk_00c &= ~1u;

    wander->flags &= ~ai_wander_flag_unk_00200000;

    bool unk_00400000 = wander->flags & ai_wander_flag_unk_00400000;

    if (!(wander->flags & ai_wander_flag_available)) {
        if (wander->unk_03c && (wander->unk_03c[8] & 2)) {
            if (animation_controller* anim_ctrl = ped->anim_ctrl) {
                retail::sub_5AD2E0((i32*)anim_ctrl, false);
                retail::sub_5ADB70((i32*)anim_ctrl, false);
            }

            if (als::animation_logic_system_interface* als = ped->get_als())
                als->suspend_logic_system(false);

            retail::sub_73EDE0((u32*)ped, true, true); // ped_manager::enable_ped_collisions

            return false;
        }

        f64 anim_move_radius    = references::ped_anim_move_radius.read();
        f64 anim_move_radius_sq = anim_move_radius * anim_move_radius;

        if (camera_distance_sq > anim_move_radius_sq && focus_distance_sq > anim_move_radius_sq) {
            f64 spawn_scale  = references::unk_00e770d4.read();
            f64 spawn_radius = references::ped_spawn_radius.read();

            bool wants_unspawn = references::unk_0102fee0.read() > references::max_peds.read() ||
                                 camera_distance_sq > spawn_scale * spawn_scale * (spawn_radius * spawn_radius) ||
                                 (0.0f > the_camera_position->y && height > references::unk_00e770d0.read());

            if (wants_unspawn && retail::sub_73ED40((u32*)ped)) {
                _InterlockedDecrement((volatile long*)&references::unk_0102fee0.get());

                unspawn_ped(ped);

                return true;
            }

            retail::sub_73EDE0((u32*)ped, unk_00400000, unk_00400000); // ped_manager::enable_ped_collisions
            retail::sub_73EE60((i32)ped, !unk_00100000, !unk_00100000);

            wander->flags |= ai_wander_flag_unk_00200000;

            if (ped->has_card_ifc())
                ped->card_ifc()->unk_00c |= 1u;

            return false;
        }

        if (ped->was_occluded_last_frame()) {
            f64 spawn_scale  = references::unk_00e770d4.read();
            f64 spawn_radius = references::ped_spawn_radius.read();

            bool wants_unspawn = references::unk_0102fee0.read() > references::max_peds.read() - 5 ||
                                 camera_distance_sq > spawn_scale * spawn_scale * (spawn_radius * spawn_radius) ||
                                 (0.0f > the_camera_position->y && height > references::unk_00e770d0.read());

            if (wants_unspawn && retail::sub_73ED40((u32*)ped)) {
                _InterlockedDecrement((volatile long*)&references::unk_0102fee0.get());

                unspawn_ped(ped);

                return true;
            }

            retail::sub_73EDE0((u32*)ped, unk_00400000, unk_00400000); // ped_manager::enable_ped_collisions
            retail::sub_73EE60((i32)ped, !unk_00100000, !unk_00100000);

            return false;
        }

        f64 animate_radius    = references::ped_animate_radius.read();
        f64 animate_radius_sq = animate_radius * animate_radius;

        if (camera_distance_sq > animate_radius_sq && focus_distance_sq > animate_radius_sq) {
            retail::sub_73EDE0((u32*)ped, unk_00400000, unk_00400000); // ped_manager::enable_ped_collisions

            if (animation_controller* anim_ctrl = ped->anim_ctrl) {
                retail::sub_5AD2E0((i32*)anim_ctrl, false);
                retail::sub_5ADB70((i32*)anim_ctrl, unk_00100000);
            }

            if (als::animation_logic_system_interface* als = ped->get_als())
                als->suspend_logic_system(false);

            return false;
        }

        f64 collide_radius    = references::ped_collide_radius.read();
        f64 collide_radius_sq = collide_radius * collide_radius;

        if (camera_distance_sq > collide_radius_sq && focus_distance_sq > collide_radius_sq)
            retail::sub_73EDE0((u32*)ped, unk_00400000, unk_00400000); // ped_manager::enable_ped_collisions
        else
            retail::sub_73EDE0((u32*)ped, true, !(wander->flags & ai_wander_flag_unk_00080000)); // ped_manager::enable_ped_collisions
    }

    if (animation_controller* anim_ctrl = ped->anim_ctrl) {
        retail::sub_5AD2E0((i32*)anim_ctrl, false);
        retail::sub_5ADB70((i32*)anim_ctrl, false);
    }

    if (als::animation_logic_system_interface* als = ped->get_als())
        als->suspend_logic_system(false);

    return false;
}

// sub_73F990
void ped_manager::unspawn_ped(actor* ped) {
    ped->set_visible(false, false);
    retail::sub_65AE40((u32*)ped, 0, nullptr);
    ped->set_collisions_active(false, false);
    ped->raise_event(references::unspawned.read());

    if (ped->has_ifc(entity_ifc_standing))
        ped->standing_ifc()->manage_standing(0.01f, true, -1.0f);

    if (ai_core* core = ped->get_ai_core()) {
        if (auto civilian = (ai_civilian_inode*)core->get_info_node(info_node_type_ai_civilian))
            civilian->unk_05c = false;

        auto wander = (ai_wander_inode*)core->get_info_node(info_node_type_ai_wander);

        // the reset leaves these two as they were
        bool unk_00000400 = false;
        bool unk_01000000 = false;

        if (wander) {
            unk_01000000 = wander->flags & ai_wander_flag_unk_01000000;
            unk_00000400 = wander->flags & ai_wander_flag_unk_00000400;

            wander->flags &= ~ai_wander_flag_available;
            retail::sub_48FBE0((u32*)wander);
            wander->flags &= ~ai_wander_flag_unk_02000000;
        }

        core->reset();

        if (wander) {
            if (unk_00000400)
                wander->flags |= ai_wander_flag_unk_00000400;
            else
                wander->flags &= ~ai_wander_flag_unk_00000400;

            if (unk_01000000)
                wander->flags |= ai_wander_flag_unk_01000000;
            else
                wander->flags &= ~ai_wander_flag_unk_01000000;
        }
    }

    if (!(ped->flags & entity_flag_suspended))
        ped->suspend(true);
}

// sub_76AEF0
void ped_manager::add_ped(arch_base_vhandle* ped, bool at_front) {
    auto ped_entity = (actor*)ped->resolve();

    if (!ped_entity)
        return;

    vc8::vector<arch_base_vhandle> &peds = references::ped_entities.get();

    for (arch_base_vhandle* it = peds.begin(); it != peds.end(); ++it) {
        if (*it == *ped)
            return;
    }

    unspawn_ped(ped_entity);

    if (at_front) {
        arch_base_vhandle* inserted;
        retail::sub_598810((u32*)&peds, (u32*)&inserted, (i32)peds.begin(), (u32*)ped); // vector::insert
    } else
        retail::sub_59BF30((u32*)&peds, (u32*)ped); // vector::push_back

    if (ped_entity->is_a_conglomerate())
        retail::sub_603C10((u32*)ped_entity, references::unk_00e770d8.read());

    if (!ped_entity->has_ifc(entity_ifc_card))
        retail::sub_666BF0((i32*)ped_entity);
}

// sub_763360
void ped_manager::remove_ped_entity(arch_base_vhandle* ped) {
    if (auto ped_entity = (actor*)ped->resolve()) {
        if (ped_entity->is_a_conglomerate())
            ((conglomerate*)ped_entity)->conglomerate_flags &= ~conglomerate_flag_unk_00000100;

        unspawn_ped(ped_entity);
        retail::sub_6391D0((u32*)ped_entity);
    }

    vc8::vector<arch_base_vhandle> &peds = references::ped_entities.get();

    for (arch_base_vhandle* it = peds.begin(); it != peds.end();) {
        if (!it->resolve() || *it == *ped)
            it = peds.erase(it);
        else
            ++it;
    }
}

// sub_748160
void ped_manager::clear_in_radius(const vector3d* center, f32 radius) {
    vc8::vector<arch_base_vhandle> &peds = references::ped_entities.get();

    i32 count     = (i32)peds.size();
    f32 radius_sq = (f32)((f64)radius * radius);

    for (i32 index = 0; index < count; ++index) {
        auto ped = (actor*)peds[index].resolve();

        if (!ped || !ped->is_visible())
            continue;

        const vector3d &position = ped->my_abs_po->get_position();

        f32 dx = (f32)((f64)position.x - (f64)center->x);
        f32 dy = (f32)((f64)position.y - (f64)center->y);
        f32 dz = (f32)((f64)position.z - (f64)center->z);

        if (radius_sq > (f32)((f64)dx * dx + (f64)dy * dy + (f64)dz * dz))
            unspawn_ped(ped);
    }
}
