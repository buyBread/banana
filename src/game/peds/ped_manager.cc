#include <cmath>
#include <cstdlib>
#include <intrin.h>

#include "retail.hh"
#include "game/als/als.hh"
#include "game/app.hh"
#include "game/game.hh"
#include "game/gen_block.hh"
#include "game/peds/ped_jq.hh"
#include "game/peds/ped_manager.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "game/fixed_string.hh"
#include "game/math/references.hh"
#include "treyarch/jq/jq.hh"

using namespace treyarch;

// sub_75F070
void treyarch::ped_jq_callback(jq::batch* batch) {
    ped_manager::advance_ped((actor*)batch->payload[0],
                             &ped_manager::focus,
                             &ped_manager::camera_position,
                             &ped_manager::camera_facing);
}

// sub_746B90
void treyarch::vendor_jq_callback(jq::batch* batch) {
    ped_manager::advance_vendor((actor*)batch->payload[0],
                                &references::unk_01030a30.get(),
                                &references::unk_01030a50.get(),
                                &references::unk_01030a40.get());
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

        if (!spawn_ped(ped, the_camera_position, the_camera_facing, true, true)) {
            _InterlockedDecrement((volatile long*)&unk_0102feec);
            _InterlockedDecrement((volatile long*)&references::unk_0102fee0.get());
        }

        return unk_0102feec >= references::unk_00e77104.read();
    }

    if (!(wander->flags & ai_wander_flag_unk_02000000))
        spawn_ped(ped, the_camera_position, the_camera_facing, false, false);

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
        if (wander->unk_03c && (wander->unk_03c->flags & 2)) {
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

// sub_73EEB0
bool ped_manager::advance_vendor(actor*          vendor,
                                 const vector4d* the_focus,
                                 const vector4d* the_camera_position,
                                 const vector4d* the_camera_facing) {

    const vector3d &position = vendor->my_abs_po->get_position();

    f32 camera_dx          = (f32)((f64)the_camera_position->x - (f64)position.x);
    f32 camera_dz          = (f32)((f64)the_camera_position->z - (f64)position.z);
    f32 camera_distance_sq = (f32)((f64)camera_dx * camera_dx + (f64)camera_dz * camera_dz); // across the ground

    f32 focus_dx          = (f32)((f64)the_focus->x - (f64)position.x);
    f32 focus_dy          = (f32)((f64)the_focus->y - (f64)position.y);
    f32 focus_dz          = (f32)((f64)the_focus->z - (f64)position.z);
    f32 focus_distance_sq = (f32)((f64)focus_dx * focus_dx + (f64)focus_dy * focus_dy + (f64)focus_dz * focus_dz);

    f32 height = std::fabs((f32)((f64)the_camera_position->y - (f64)position.y));

    if (!vendor->is_visible()) {
        f64 spawn_scale  = references::unk_00e770d4.read();
        f64 spawn_radius = references::ped_spawn_radius.read();

        if (spawn_scale * spawn_scale * (spawn_radius * spawn_radius) >= camera_distance_sq &&
            (the_camera_position->y >= 0.0f || (f64)references::unk_00e770d0.read() >= (f64)height))

            wake_vendor(vendor);
    } else {
        bool unk_00400000 = false;

        if (auto wander = (ai_wander_inode*)vendor->get_ai_core()->get_info_node(info_node_type_ai_wander)) {
            wander->flags &= ~ai_wander_flag_unk_00200000;

            unk_00400000 = wander->flags & ai_wander_flag_unk_00400000;
        }

        f64 anim_move_radius    = references::ped_anim_move_radius.read();
        f64 anim_move_radius_sq = anim_move_radius * anim_move_radius;

        if (camera_distance_sq > anim_move_radius_sq && focus_distance_sq > anim_move_radius_sq) {
            retail::sub_73EDE0((u32*)vendor, unk_00400000, unk_00400000); // ped_manager::enable_ped_collisions

            if (animation_controller* anim_ctrl = vendor->anim_ctrl) {
                retail::sub_5AD2E0((i32*)anim_ctrl, true);
                retail::sub_5ADB70((i32*)anim_ctrl, true);
            }

            if (als::animation_logic_system_interface* als = vendor->get_als())
                als->suspend_logic_system(true);

            f64 spawn_scale  = references::unk_00e770d4.read();
            f64 spawn_radius = references::ped_spawn_radius.read();

            if (camera_distance_sq > spawn_scale * spawn_scale * (spawn_radius * spawn_radius) ||
                (0.0f > the_camera_position->y && (f64)height > (f64)references::unk_00e770d0.read())) {

                sleep_vendor(vendor);

                return true;
            }

            return false;
        }

        if (vendor->was_occluded_last_frame())
            retail::sub_73EDE0((u32*)vendor, unk_00400000, unk_00400000); // ped_manager::enable_ped_collisions
        else {
            f64 animate_radius    = references::ped_animate_radius.read();
            f64 animate_radius_sq = animate_radius * animate_radius;

            if (camera_distance_sq > animate_radius_sq && focus_distance_sq > animate_radius_sq)
                retail::sub_73EDE0((u32*)vendor, unk_00400000, unk_00400000); // ped_manager::enable_ped_collisions
            else {
                f64 collide_radius    = references::ped_collide_radius.read();
                f64 collide_radius_sq = collide_radius * collide_radius;

                if (camera_distance_sq > collide_radius_sq && focus_distance_sq > collide_radius_sq)
                    retail::sub_73EDE0((u32*)vendor, unk_00400000, unk_00400000); // ped_manager::enable_ped_collisions
                else
                    retail::sub_73EDE0((u32*)vendor, true, true); // ped_manager::enable_ped_collisions

                if (animation_controller* anim_ctrl = vendor->anim_ctrl) {
                    retail::sub_5AD2E0((i32*)anim_ctrl, false);
                    retail::sub_5ADB70((i32*)anim_ctrl, false);
                }

                if (als::animation_logic_system_interface* als = vendor->get_als())
                    als->suspend_logic_system(false);

                return false;
            }
        }
    }

    if (animation_controller* anim_ctrl = vendor->anim_ctrl) {
        retail::sub_5AD2E0((i32*)anim_ctrl, true);
        retail::sub_5ADB70((i32*)anim_ctrl, true);
    }

    if (als::animation_logic_system_interface* als = vendor->get_als())
        als->suspend_logic_system(true);

    return false;
}

// sub_731730
void ped_manager::wake_vendor(entity* vendor) {
    if (vendor->flags & entity_flag_suspended)
        vendor->unsuspend(true);

    vendor->set_visible(true, false);
    vendor->set_collisions_active(false, false);
    vendor->raise_event(references::vendor_awake.read());
}

// sub_731790
void ped_manager::sleep_vendor(entity* vendor) {
    vendor->set_visible(false, false);
    vendor->set_collisions_active(false, false);
    vendor->raise_event(references::vendor_asleep.read());

    if (!(vendor->flags & entity_flag_suspended))
        vendor->suspend(true);
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

// sub_753200
bool ped_manager::spawn_ped(actor*          ped,
                            const vector4d* the_camera_position,
                            const vector4d* the_camera_facing,
                            bool            find_spot,
                            bool            check_distance) {

    auto wander = (ai_wander_inode*)ped->get_ai_core()->get_info_node(info_node_type_ai_wander);

    if (!find_spot) {
        if (ped->flags & entity_flag_suspended)
            ped->entity::unsuspend(true);

        if (ped->has_ifc(entity_ifc_damage))
            ped->damage_ifc()->set_hit_points(ped->damage_ifc()->get_max_hit_points());

        ped->set_visible(true, false);
        ped->set_collisions_active(true, false);
        ped->unk_03a = 0;

        wander->flags |= ai_wander_flag_unk_02000000;

        ped->raise_event(references::spawned.read());

        if (animation_controller* anim_ctrl = ped->anim_ctrl) {
            retail::sub_5AD2E0((i32*)anim_ctrl, false);
            retail::sub_5ADB70((i32*)anim_ctrl, false);
        }

        if (als::animation_logic_system_interface* als = ped->get_als())
            als->suspend_logic_system(false);

        return false;
    }

    if (ped->get_ai_core()) {
        auto car = (ai_car_inode*)ped->get_ai_core()->get_info_node(info_node_type_ai_car);

        if (car && car->is_in_car)
            return false;
    }

    if (ped->flags & entity_flag_suspended)
        ped->entity::unsuspend(true);

    ped_path path;
    path.flags  = 0;
    path.unk_00 = 0xFFFF;
    path.unk_02 = 0xFFFF;
    path.road   = 0xFFFF;
    path.unk_06 = 0xFF;

    i32 quad_path     = 0;
    i32 quad_path_unk = 0;

    f32 roll        = (f32)((f64)(f32)std::rand() * 3.0518509447574615e-05 * 1.5);
    f32 path_offset = (f32)((f64)roll - 0.75);

    if (wander) {
        path_offset = (f32)((f64)wander->unk_050 * (f64)path_offset);

        retail::sub_48FB60((i32)wander, false);
    }

    bool on_quad_path = std::rand() % 2 != 0;

    vector3d spot;
    vector3d facing;
    vector3d destination;

    if (wander && !(wander->flags & ai_wander_flag_unk_00000080))
        on_quad_path = false;

    if (on_quad_path) {
        const vector3d forward (the_camera_facing->x,   the_camera_facing->y,   the_camera_facing->z);
        const vector3d position(the_camera_position->x, the_camera_position->y, the_camera_position->z);

        std::array<ped_spawn_quad_search_buffer, 2> &buffers = references::unk_0102ff90.get();

        quad_path = retail::sub_7471C0((i32)&buffers[buffers[0].unk_28 <= buffers[1].unk_28],
                                       (f32*)&spot,
                                       (f32*)&facing,
                                       (f32*)&destination,
                                       (f32*)&position,
                                       (f32*)&forward,
                                       &quad_path_unk);

        if (!quad_path)
            on_quad_path = false;
    }

    if (!on_quad_path) {
        const vector3d forward (the_camera_facing->x,   the_camera_facing->y,   the_camera_facing->z);
        const vector3d position(the_camera_position->x, the_camera_position->y, the_camera_position->z);

        ped_path found;

        i32 result = retail::sub_7515E0((i32)&found,
                                        0,
                                        (f32*)&spot,
                                        (f32*)&facing,
                                        (f32*)&destination,
                                        (f32*)&position,
                                        (f32*)&forward,
                                        path_offset);

        retail::sub_48FA80((u16*)&path, result); // ped_path::operator=
    }

    if (check_distance) {
        f32 dx = (f32)((f64)spot.x - (f64)the_camera_position->x);
        f32 dz = (f32)((f64)spot.z - (f64)the_camera_position->z);

        f64 min_radius = references::min_fake_ped_radius.read();

        if (min_radius * min_radius > (f64)(f32)((f64)dz * dz + (f64)dx * dx))
            return false;

        f32 height = std::fabs((f32)((f64)spot.y - (f64)the_camera_position->y));

        if ((f64)height > (f64)references::unk_00e770d0.read() && 0.0f > the_camera_position->y)
            return false;
    }

    if (on_quad_path) {
        if (!quad_path)
            return false;
    } else if (path.unk_00 == 0xFFFF || path.unk_02 == 0xFFFF || path.road == 0xFFFF)
        return false;

    f32 dx          = (f32)((f64)the_camera_position->x - (f64)spot.x);
    f32 dz          = (f32)((f64)the_camera_position->z - (f64)spot.z);
    f32 distance_sq = (f32)((f64)dz * dz + (f64)dx * dx);

    f64 spawn_scale  = references::unk_00e770d4.read();
    f64 spawn_radius = references::ped_spawn_radius.read();

    // a NaN distance fails it too
    if (!(spawn_scale * spawn_scale * (spawn_radius * spawn_radius) >= distance_sq))
        return false;

    if (retail::sub_742CE0((f32*)&spot, 7))
        return false;

    return place_ped(ped,
                     the_camera_position,
                     the_camera_facing,
                     on_quad_path,
                     quad_path,
                     quad_path_unk,
                     &path,
                     path_offset,
                     &spot,
                     &facing,
                     &destination);
}

// sub_748750
bool ped_manager::place_ped(actor*          ped,
                            const vector4d* the_camera_position,
                            const vector4d* the_camera_facing,
                            bool            on_quad_path,
                            i32             quad_path,
                            i32             quad_path_unk,
                            const ped_path* path,
                            f32             path_offset,
                            const vector3d* position,
                            vector3d*       facing,
                            const vector3d* destination) {

    if (on_quad_path) {
        if (!quad_path)
            return false;
    } else if (path->unk_00 == 0xFFFF || path->unk_02 == 0xFFFF || path->road == 0xFFFF)
        return false;

    if (retail::sub_742CE0((f32*)position, 7))
        return false;

    if (ped->has_ifc(entity_ifc_standing))
        ped->standing_ifc()->manage_standing(0.01f, true, -1.0f);

    ai_core* core = ped->get_ai_core();

    ped->get_als()->reset_animation_player();
    core->restart_base_machine();

    facing->y = 0.0f;

    const vector3d &up = references::unk_00e771dc.get();

    const math::vector4 forward(-0.0f - facing->x, -0.0f - facing->y, -0.0f - facing->z, -0.0f);
    const math::vector4 normal (up.x, up.y, up.z, 0.0f);
    const math::vector4 at     (position->x, position->y, position->z, 0.0f);

    po placed;
    placed.set_po(forward, normal, at);

    retail::sub_627180((i32)ped, (f32*)&placed); // entity_base::set_abs_po
    ped->save_last_collision_free_state();

    if (animation_controller* anim_ctrl = ped->anim_ctrl) {
        retail::sub_5AD2E0((i32*)anim_ctrl, false);
        retail::sub_5ADB70((i32*)anim_ctrl, false);
    }

    if (als::animation_logic_system_interface* als = ped->get_als())
        als->suspend_logic_system(false);

    ped->set_visible(true, false);
    retail::sub_602060((u32*)ped, false);
    ped->set_collisions_active(true, false);
    ped->unk_03a = 0;

    if (ped->has_ifc(entity_ifc_damage))
        ped->damage_ifc()->set_hit_points(ped->damage_ifc()->get_max_hit_points());

    if (auto fear = (std_fear_inode*)core->get_info_node(info_node_type_std_fear))
        fear->reset_morale();

    ped->raise_event(references::spawned.read());

    if (ped->my_ifc_storage) {
        auto standing = (standing_interface*)ped->my_ifc_storage->find_ifc(entity_ifc_standing);

        if (standing && standing->is_enabled())
            standing->manage_standing(0.01f, true, 0.5f);
    }

    auto wander = (ai_wander_inode*)core->get_info_node(info_node_type_ai_wander);

    core->walk_run_cycle = std::rand() % 6;
    core->idle_type      = std::rand() % 6;

    if (auto civilian = (ai_civilian_inode*)core->get_info_node(info_node_type_ai_civilian))
        retail::sub_4951E0((u32*)civilian, (i32)ped->my_handle.value, 1);

    if (wander) {
        wander->flags |= ai_wander_flag_unk_00000001;

        if (auto civilian = (ai_civilian_inode*)core->get_info_node(info_node_type_ai_civilian))
            civilian->unk_05c = true;

        if (on_quad_path) {
            i32  node      = 0;
            u16* node_path = nullptr;

            retail::sub_48FD70((i32)wander, quad_path, quad_path_unk);

            if (retail::sub_43D7B0((f32*)position,
                                   2.0f,
                                   (f32*)&at,
                                   &node,
                                   &node_path,
                                   (f32*)&references::unk_00e771dc.get(),
                                   0,
                                   0,
                                   0.0f) &&
                node_path &&
                retail::sub_439F60(node_path, (f32*)position, 2.0f, nullptr, (f32*)&references::unk_00e771dc.get(), 0))

                retail::sub_48FAE0((u32*)wander, node);
            else
                retail::sub_48FAE0((u32*)wander, 0);
        } else
            retail::sub_48FD10((i32)wander, (i32)path, path_offset);

        wander->unk_040 = *destination;
        wander->flags  |= ai_wander_flag_unk_00000004;

        retail::sub_48FBF0((i32)wander);
    } else {
        auto goto_position = (bool (__thiscall*)(ai_core*, const vector3d*, f32, f32, bool, bool, bool, bool))retail::sub_4CC850; // ai_core::goto_position

        f32 speed = (f32)(retail::sub_408AE0(0.25f) + 0.25);

        goto_position(core, destination, speed, 2.0f, true, true, false, false);
    }

    if (ped->flags & entity_flag_suspended)
        ped->entity::unsuspend(true);

    return true;
}

// sub_751C10
void ped_manager::replace_fake_ped(fake_ped*       fake,
                                   const vector4d* the_camera_position,
                                   const vector4d* the_camera_facing) {

    if (fake->fade_step < 0)
        return;

    fake->fade_step = -0x20;

    u32 frame = references::g_world_ptr.read()->time_mgr.frame_sequence;

    references::unk_0102ff80.get().acquire();

    if (unk_00e7adcc != frame) {
        unk_01032084 = 0;
        unk_00e7adcc = frame;
    }

    references::unk_0102ff80.get().release();

    if (unk_01032084 >= 40 || references::unk_0102fee0.read() >= references::max_peds.read())
        return;

    _InterlockedIncrement((volatile long*)&unk_01032084);

    vc8::vector<arch_base_vhandle> &peds = references::ped_entities.get();

    actor* ped   = nullptr;
    i32    count = (i32)peds.size();

    {
        engine_lock_scope lock(&references::unk_0102ff80.get());

        for (i32 index = 0; index < count; ++index) {
            auto candidate = (actor*)peds.begin()[index].resolve();

            if (!candidate || candidate->is_visible())
                continue;

            bool female = candidate->unk_111()->get_text()[6] == 'f';

            if ((fake->unk_56 || !female) &&
                (fake->unk_56 != 1 || !female) &&
                (fake->unk_56 != 2 || female)) {

                ped = candidate;
                ped->set_visible(true, false);

                break;
            }
        }
    }

    if (!ped)
        return;

    ped_path path;
    retail::sub_73E280((u16*)&path, (i32)fake->path); // ped_path::ped_path

    const vector3d &z = fake->abs_po.get_z_facing();

    vector3d position    = fake->abs_po.get_position();
    vector3d facing      = vector3d(-0.0f - z.x, -0.0f - z.y, -0.0f - z.z);
    vector3d destination = fake->destination;

    if (!place_ped(ped,
                   the_camera_position,
                   the_camera_facing,
                   false,
                   0,
                   0,
                   &path,
                   0.0f,
                   &position,
                   &facing,
                   &destination)) {

        ped->set_visible(false, false);

        _InterlockedDecrement((volatile long*)&unk_01032084);
    }
}

// sub_7588C0
i32 ped_manager::find_peds_near(const vector3d* center,
                                f32             radius,
                                i32             max_count,
                                i32             flags,
                                u32*            found) {

    vc8::vector<arch_base_vhandle> &peds = references::ped_entities.get();

    i32 start = 0;

    if (peds.begin() && (i32)peds.size() > 0)
        start = std::rand() % (i32)peds.size();

    i32 count     = 0;
    i32 ped_count = (i32)peds.size();

    f32 radius_sq = (f32)((f64)radius * radius);

    if (0.0f > radius)
        radius_sq = 3.40282347e+38f;

    if (max_count < 0) {
        if (!peds.begin())
            return count;

        max_count = (i32)peds.size();
    }

    if (!peds.begin() || !peds.size() || ped_count <= 0)
        return count;

    for (i32 index = 0; count < max_count;) {
        arch_base_vhandle handle = peds.begin()[(u32)(start + index) % peds.size()];

        if (auto ped = (actor*)handle.resolve()) {
            bool in_range = 0.0f > radius;

            if (!in_range) {
                const vector3d &position = ped->my_abs_po->get_position();

                f32 dx = (f32)((f64)position.x - (f64)center->x);
                f32 dy = (f32)((f64)position.y - (f64)center->y);
                f32 dz = (f32)((f64)position.z - (f64)center->z);

                in_range = (f64)radius_sq > (f64)(f32)((f64)dx * dx + (f64)dy * dy + (f64)dz * dz);
            }

            if (in_range && retail::sub_73E7F0((u32*)ped, (i16)flags)) {
                auto civilian = (ai_civilian_inode*)ped->get_ai_core()->get_info_node(info_node_type_ai_civilian);

                if (retail::sub_4952D0((u32*)civilian)) {
                    if ((flags & 0x500) && !ped->is_visible()) {
                        const vector3d &zero = references::unk_0102ff40.get();

                        const vector4d facing  (zero.x, zero.y, zero.z, 0.0f);
                        const vector4d position(zero.x, zero.y, zero.z, 0.0f);

                        spawn_ped(ped, &position, &facing, false, false);
                    }

                    found[count++] = handle.value;
                }
            }
        }

        if (++index >= ped_count)
            break;
    }

    return count;
}

// sub_757C70
void treyarch::fake_ped_jq_callback(jq::batch* batch) {
    auto fake  = (fake_ped*)batch->payload[0];
    i32  count = batch->count;

    if (!references::peds_enabled.read()) {
        for (; count > 0; --count, ++fake) {
            if (fake->alpha) {
                retail::sub_730F20((i32)fake);
                continue;
            }

            if (fake->fade_step >= 0)
                fake->fade_step = -0x20;

            if (fake->fade_step)
                fake->step_fade();
        }

        return;
    }

    const vector4d &camera_position = references::unk_01030070.get();
    const vector4d &camera_facing   = references::unk_01030060.get();

    for (; count > 0; --count, ++fake) {
        if (!fake->alpha) {
            if (references::unk_0102fde8.read() < references::unk_0102fdec.read()) {
                _InterlockedIncrement((volatile long*)&references::unk_0102fde8.get());

                jq::references::unk_01126d10.get().acquire();

                if (retail::sub_7516A0((i32)fake, (i32)&camera_position, (i32*)&camera_facing)) { // places it near the camera
                    ped_path* placed = fake->path;

                    fake->unk_55 = 0;

                    ped_path path;
                    retail::sub_73E280((u16*)&path, (i32)placed); // ped_path::ped_path

                    if (path.road != 0xFFFF && references::g_gb_map.read()) {
                        if (gb_road* road = references::g_gb_map.read()->roads.array[path.road])
                            ++road->unk_33;
                    }
                }

                jq::references::unk_01126d10.get().release();
            }

            continue;
        }

        vector3d position = fake->abs_po.get_position();

        f32 camera_dx          = (f32)((f64)camera_position.x - (f64)position.x);
        f32 camera_dz          = (f32)((f64)camera_position.z - (f64)position.z);
        f32 camera_distance_sq = (f32)((f64)camera_dz * camera_dz + (f64)camera_dx * camera_dx);
        f32 camera_dy          = (f32)((f64)camera_position.y - (f64)position.y);

        f64 show_radius = references::unk_00e770b4.read();

        if ((f64)camera_distance_sq > show_radius * show_radius) {
            fake->fade_step = -0x20;
            fake->step_fade();
        } else {
            f64 min_radius = references::min_fake_ped_radius.read();

            if (90.0f > std::fabs(camera_dy) && min_radius * min_radius > (f64)camera_distance_sq && fake->fade_step >= 0)
                ped_manager::replace_fake_ped(fake, &camera_position, &camera_facing);

            if (fake->fade_step)
                fake->step_fade();
        }

        position = fake->abs_po.get_position();

        f32 dx        = (f32)((f64)position.x - (f64)camera_position.x);
        f32 dy        = (f32)((f64)position.y - (f64)camera_position.y);
        f32 dz        = (f32)((f64)position.z - (f64)camera_position.z);
        f32 length_sq = (f32)((f64)dy * dy + (f64)dx * dx + (f64)dz * dz);

        vector4d direction;

        if (length_sq > 0.0f) {
            f32 inverse = (f32)(1.0 / (f64)(f32)std::sqrt((f64)length_sq));

            direction = vector4d((f32)((f64)inverse * dx),
                                 (f32)((f64)inverse * dy),
                                 (f32)((f64)inverse * dz),
                                 (f32)((f64)inverse * 0.0));
        } else
            direction = references::unk_0102ff10.read();

        f32 dot = direction.y * camera_facing.y + direction.x * camera_facing.x + direction.z * camera_facing.z;

        if (dot > (f32)std::cos((f64)0.61086524f)) {
            fake->in_view = 1;

            retail::sub_750E70((u8*)fake, references::unk_0102fde0.read()); // advances its animation

            const matrix4x4 &rows = fake->abs_po.get_matrix();

            f32 z_side = std::fabs((f32)((f64)rows.z.x * direction.x + (f64)rows.z.y * direction.y + (f64)direction.z * rows.z.z + (f64)rows.z.w * 0.0));

            if (0.7f > z_side)
                fake->render_flags |= 1;

            f32 x_side = std::fabs((f32)((f64)rows.x.y * direction.y + (f64)rows.x.x * direction.x + (f64)rows.x.z * direction.z + (f64)rows.x.w * 0.0));

            if (0.7f > x_side)
                fake->render_flags |= 2;

            f32 up = std::fabs((f32)((f64)rows.y.x * direction.x + (f64)rows.y.y * direction.y + (f64)rows.y.z * direction.z + (f64)rows.y.w * 0.0));

            if (up > 0.7f)
                fake->render_flags |= 4;
        }

        if (fake->alpha)
            continue;

        // it faded out: one fewer on its road
        u16 road_index = fake->path->road;

        if (road_index == 0xFFFF || !references::g_gb_map.read())
            continue;

        gb_road* road = references::g_gb_map.read()->roads.array[road_index];

        jq::references::unk_01126d10.get().acquire();

        if (road && road->unk_33)
            --road->unk_33;

        jq::references::unk_01126d10.get().release();
    }
}
