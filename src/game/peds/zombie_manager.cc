#include <cmath>

#include "retail.hh"
#include "game/als/als.hh"
#include "game/app.hh"
#include "game/frontend/entity_tracker.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"
#include "game/frontend/igo/igo_3d_zoom_map.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/mission/mission_manager.hh"
#include "game/peds/zombie_manager.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "game/math/references.hh"

using namespace treyarch;

// sub_76C610
void zombie_manager::frame_advance(f32 time_inc) {
    if (references::game.read()->game_paused)
        return;

    if (references::frontend.get().igo->scrapbook->is_active())
        return;

    if (references::frontend.get().igo->zoom_map->is_open_or_zooming())
        return;

    if (!zombies_enabled || mission_manager::inst()->is_act_transitioning()) {
        if (fake_zombie_count)
            retail::sub_732040((u32*)this); // frees the fake zombies

        if (zombies.size())
            retail::sub_763E20((u32*)this); // destroys the real ones

        return;
    }

    const po* view = references::game.read()->get_current_view_camera()->my_abs_po;

    const vector3d &view_facing = view->get_z_facing();

    // the camera's facing flattened to the ground; straight up or down takes the hero's
    vector3d facing(view_facing.x, 0.0f, view_facing.z);
    vector3d flat  (view_facing.x, 0.0f, view_facing.z);
    vector3d zero = references::unk_0102ff40.read();

    if (retail::sub_A27ED0((f32*)&flat, (f32*)&zero)) // vector3d ==
        facing = references::g_world_ptr.read()->hero_ptr->my_abs_po->get_z_facing();

    facing.normalize();

    const vector3d &view_position = view->get_position();

    const vector4d camera(view_position.x, view_position.y, view_position.z, 0.0f);

    atlas_frame_timer = (f32)((f64)atlas_frame_timer + (f64)time_inc);

    if (atlas_frame_timer >= 0.13333334f) {
        if (++atlas_frame >= atlas_frame_count[1] * atlas_frame_count[0])
            atlas_frame = 0;

        atlas_frame_timer = 0.0f;
    }

    if (fake_zombie_count != 25)
        retail::sub_7493E0((u32*)this, 25); // reallocates the fake zombies

    if (zombies.size() != 9)
        retail::sub_76B000((u32*)this); // spawns real ones up to nine

    u32 near_count = 0;
    u32 held_back  = 0;
    u32 retired    = 0;

    spawns_this_frame = 0;

    // real zombies out of sight or in a kind 7 point of interest go back to being sprites
    for (arch_base_vhandle* it = zombies.begin(); it != zombies.end(); ++it) {
        auto zombie = (actor*)it->resolve();

        if (!zombie || !zombie->entity::is_still_visible())
            continue;

        vector3d position = zombie->my_abs_po->get_position();

        if (retail::sub_742CE0((f32*)&position, 7)) {
            retail::sub_7407E0((u32*)this, (u32*)zombie, false); // parks it
            continue;
        }

        f32 dx = (f32)((f64)position.x - (f64)camera.x);
        f32 dz = (f32)((f64)position.z - (f64)camera.z);

        if ((f32)((f64)dz * dz + (f64)dx * dx) > 4225.0f)
            retail::sub_7407E0((u32*)this, (u32*)zombie, true); // parks it, a sprite taking its place
        else
            ++near_count;
    }

    for (u32 index = 0; index < fake_zombie_count; ++index) {
        fake_zombie* fake = &fake_zombies[index];

        if (!fake->alpha) {
            if (spawns_this_frame < 2 && !suspended) {
                if (held_back > near_count) {
                    const vector4d ahead(facing.x, facing.y, facing.z, 0.0f);

                    if (retail::sub_73FF60((i32)fake, (f32*)&camera, (f32*)&ahead)) // places it on a road ahead
                        ++spawns_this_frame;
                } else
                    ++held_back;
            }

            continue;
        }

        retail::sub_732000((u8*)fake, time_inc); // steps its fade

        vector3d position = fake->abs_po.get_position();

        if (retail::sub_742CE0((f32*)&position, 7) ||
            retail::sub_742CE0((f32*)&position, 8)) {

            fake->fade_step = -0x20;
            continue;
        }

        vector3d delta((f32)((f64)position.x - (f64)camera.x),
                       (f32)((f64)position.y - (f64)camera.y),
                       (f32)((f64)position.z - (f64)camera.z));

        f32 distance_sq = (f32)((f64)delta.z * delta.z + (f64)delta.x * delta.x); // across the ground

        delta.normalize();

        f32 cos_near = (f32)std::cos((f64)0.61086524f);
        f32 cos_far  = (f32)std::cos((f64)3.14159274f);

        f32 dot = delta * facing;

        if (distance_sq > 40000.0f)
            fake->fade_step = -0x20;
        else {
            f32 height = std::fabs((f32)((f64)camera.y - (f64)position.y));

            if (90.0f > height && 3600.0 > (f64)distance_sq) {
                if (fake->fade_step || (f64)cos_near >= (f64)dot)
                    fake->fade_step = -0x20;
                else {
                    const vector4d ahead(facing.x, facing.y, facing.z, 0.0f);

                    replace_fake_zombie(fake, &camera, &ahead);
                }
            }
        }

        retail::sub_731FC0((u8*)fake); // steps its fade

        if ((f64)dot > (f64)cos_near) {
            fake->in_view = 1;

            retail::sub_732000((u8*)fake, time_inc);

            const vector4d direction(delta.x, delta.y, delta.z, 0.0f);

            for (i32 bit = 0; bit < 2; ++bit) {
                const vector4d &row = fake->abs_po.get_matrix()[cone_rows[bit]];

                f32 side = (f32)std::fabs(retail::sub_5FC020((f32*)&row, (f32*)&direction));

                if (0.7f > side)
                    fake->render_flags |= 1 << bit;
            }

            const vector4d direction_up(delta.x, delta.y, delta.z, 0.0f);

            f32 up = (f32)std::fabs(retail::sub_5FC020((f32*)&fake->abs_po.get_matrix().y, (f32*)&direction_up));

            if (up > 0.7f)
                fake->render_flags |= 4;
        } else if ((f64)dot > (f64)cos_far && retired < 3) {
            ++retired;

            fake->fade_step = -0x20;
        }
    }
}

// sub_763C50
void zombie_manager::replace_fake_zombie(fake_zombie*    fake,
                                         const vector4d* camera_position,
                                         const vector4d* camera_facing) {

    fake->fade_step = -0x20;

    arch_base_vhandle* farthest             = zombies.end();
    f32                farthest_distance_sq = -1.0f;
    bool               erased               = false;

    arch_base_vhandle* it = zombies.begin();

    while (it != zombies.end()) {
        auto zombie = (actor*)it->resolve();

        if (!zombie) {
            it     = zombies.erase(it);
            erased = true;

            continue;
        }

        if (!zombie->is_visible()) {
            place_zombie(zombie, fake);
            return;
        }

        const vector3d &position = zombie->my_abs_po->get_position();

        f32 dx = position.x - camera_position->x;
        f32 dy = position.y - camera_position->y;
        f32 dz = position.z - camera_position->z;

        f32 dot = camera_facing->y * dy + camera_facing->x * dx + camera_facing->z * dz;

        if ((f32)std::cos((f64)0.61086524f) > dot) {
            f32 distance_sq = (f32)((f64)dz * dz + (f64)dx * dx);

            if ((f64)distance_sq > (f64)farthest_distance_sq) {
                farthest             = it;
                farthest_distance_sq = distance_sq;
            }
        }

        ++it;
    }

    if (erased || farthest == zombies.end())
        return;

    if (auto zombie = (actor*)farthest->resolve())
        place_zombie(zombie, fake);
}

// sub_740690
void zombie_manager::place_zombie(actor* zombie, const fake_zombie* fake) {
    if (zombie->has_ifc(entity_ifc_damage))
        zombie->damage_ifc()->set_hit_points(zombie->damage_ifc()->get_max_hit_points());

    if (zombie->has_ifc(entity_ifc_standing))
        zombie->standing_ifc()->manage_standing(0.01f, true, -1.0f);

    ai_core* core = zombie->get_ai_core();

    zombie->get_als()->reset_animation_player();
    core->restart_base_machine();

    retail::sub_627180((i32)zombie, (f32*)&fake->abs_po); // entity_base::set_abs_po
    zombie->save_last_collision_free_state();

    if (zombie->has_ifc(entity_ifc_physical))
        zombie->physical_ifc()->enable(true);

    if (zombie->unk_102())
        zombie->unk_101(false);

    if (zombie->has_ifc(entity_ifc_ise))
        zombie->ise_ifc()->unk_020();

    zombie->set_visible(true, false);

    entity_tracker_id id = references::frontend.get().igo->entity_tracker_mgr->get_tracker_by_entity(zombie->my_handle);

    if (entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(id))
        tracker->set_mini_map_active(true);
}
