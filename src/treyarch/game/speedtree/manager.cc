#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/speedtree/manager.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/terrain.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/shared/mutex.hh"
#include "treyarch/shared/timing/hires_clock.hh"

using namespace treyarch;

// sub_7BCF40
void speed_tree_instance_pool::recycle() {
    speed_tree_instance* instance = releasing_instances;

    if (instance) {
        for (; instance->next; instance = instance->next)
            retail::sub_A42450((i32)instance->tree);

        retail::sub_A42450((i32)instance->tree);

        instance->next      = free_instances;
        free_instances      = releasing_instances;
        releasing_instances = nullptr;
    }

    releasing_instances = pending_instances;
    pending_instances   = nullptr;
}

// sub_7B4E10
void speed_tree_manager::update_camera_and_wind(const matrix4x4* camera) {
    vector4 position;
    vector4 direction;

    if (camera) {
        position  = camera->w;
        direction = camera->z;
    } else {
        ngl::scene* scene = ngl::references::current_scene.read();

        // retail leaves this inverse's fourth column unwritten, so position.w is stack garbage there
        matrix4x4 to_tree = speed_tree_wrapper::axis_basis().inverse_orthonormal();

        const vector4 &view_position = scene->view_position;

        position = vector4((f32)((f64)to_tree.x.x * view_position.x +
                                 (f64)to_tree.y.x * view_position.y +
                                 (f64)to_tree.z.x * view_position.z +
                                 to_tree.w.x),
                           (f32)((f64)to_tree.x.y * view_position.x +
                                 (f64)to_tree.y.y * view_position.y +
                                 (f64)to_tree.z.y * view_position.z +
                                 to_tree.w.y),
                           (f32)((f64)to_tree.x.z * view_position.x +
                                 (f64)to_tree.y.z * view_position.y +
                                 (f64)to_tree.z.z * view_position.z +
                                 to_tree.w.z),
                           (f32)((f64)to_tree.x.w * view_position.x +
                                 (f64)to_tree.y.w * view_position.y +
                                 (f64)to_tree.z.w * view_position.z +
                                 to_tree.w.w));

        const vector4 &view_direction = scene->view_direction;

        direction = vector4((f32)((f64)to_tree.x.x * view_direction.x +
                                  (f64)to_tree.y.x * view_direction.y +
                                  (f64)to_tree.z.x * view_direction.z),
                            (f32)((f64)to_tree.x.y * view_direction.x +
                                  (f64)to_tree.y.y * view_direction.y +
                                  (f64)to_tree.z.y * view_direction.z),
                            (f32)((f64)to_tree.x.z * view_direction.x +
                                  (f64)to_tree.y.z * view_direction.y +
                                  (f64)to_tree.z.z * view_direction.z),
                            0.0f);
    }

    retail::sub_A40870((f32*)&position, (f32*)&direction);

    f32 counts_per_millisecond = timing::references::performance_counts_per_millisecond.read();

    u32 &guard = references::speed_tree_wind_start_time_guard.get();

    if (!(guard & 1)) {
        guard |= 1;

        references::speed_tree_wind_start_time.write((f32)timing::get_cpu_cycle() / counts_per_millisecond);
    }

    f32 now     = (f32)timing::get_cpu_cycle() / counts_per_millisecond;
    f32 elapsed = (f32)((f64)(now - references::speed_tree_wind_start_time.read()) * 0.25);

    if (treyarch::references::game.read()->debug_stop_physics)
        return;

    auto* wind = (u32*)&references::speed_tree_wind.get();

    const vector3 &unk_00e80cf0 = references::speed_tree_wind_unk_00e80cf0.get();

    retail::sub_A43030(wind,
                       references::speed_tree_wind_unk_01086f18.read(),
                       unk_00e80cf0.x,
                       unk_00e80cf0.y,
                       unk_00e80cf0.z);

    retail::sub_A43880((i32)wind, (f32)((f64)elapsed / 1000.0), 1, 0, 0.0f, 0.0f, 1.0f);
}

// sub_7CB590
void speed_tree_manager::update() {
    {
        engine_lock_scope lock(&treyarch::references::subdivision_query_lock.get());

        references::speed_tree_fading_region_count.write(0);

        world_dynamics_system* world = treyarch::references::g_world_ptr.read();

        vector3 position = world->hero_ptr->my_abs_po->get_position();

        speed_tree_manager_visitor visitor;

        visitor.base.vtable  = &references::speed_tree_manager_visitor_vtable.get();
        visitor.base.visited = nullptr;
        visitor.position     = position;

        // the visitor's slot 1 adds each region within 10 units to the fading list
        retail::sub_779710((i32)&visitor);
        retail::sub_7846F0((i32)world->the_terrain->subdivision, (f32*)&position, 10.0f, (i32)&visitor);
        retail::sub_7797D0((i32)&visitor);
    }

    update_camera_and_wind(nullptr);

    for (i32 index = 0; index < entry_count; ++index) {
        for (u32 wrapper = 0; wrapper < 2; ++wrapper) {
            for (u32 region = 0; region < 16; ++region) {
                speed_tree_instance* instance = entries[index].wrappers[wrapper]->regions[region].instances;

                for (; instance; instance = instance->next)
                    retail::sub_A3C960((u32*)instance->tree);
            }
        }
    }

    instance_pool.recycle();
}

// sub_7D9D90
void speed_tree_manager::render(bool billboards_only) {
    for (i32 index = 0; index < entry_count; ++index) {
        speed_tree_manager_entry &entry = entries[index];

        if (!billboards_only) {
            for (u32 wrapper = 0; wrapper < 2; ++wrapper)
                entry.wrappers[wrapper]->submit_nodes();
        }

        // both rounds go to the first wrapper
        for (u32 round = 0; round < 2; ++round)
            entry.wrappers[0]->submit_billboards_only(billboards_only);
    }
}
