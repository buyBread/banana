#include <cstddef>
#include "retail.hh"
#include "treyarch/game/light/deferred_lights.hh"
#include "treyarch/game/light/light_source.hh"
#include "treyarch/game/light/render_submission.hh"
#include "treyarch/game/wds/region.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/game/wds/render/references.hh"
#include "treyarch/game/wds/render/wds_render_manager.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"

using namespace treyarch;

void wds_render_manager::submit_light(render_region_info* entry,
                                      light_source*       source,
                                      entity*             selected_render_entity,
                                      i32                 worker_index) {

    if (!source || !source->properties ||
        source->render_generations[worker_index] == (&references::worker_generations.get())[worker_index]) {

        return;
    }

    switch (source->properties->flavor) {
        case light_flavor_point:
            light_render_submission::submit_point_light(source);
            
            break;

        case light_flavor_spot:
            light_render_submission::submit_spot_light(source);

            break;

        case light_flavor_directional:
            if ((region*)retail::sub_612D30((u32*)selected_render_entity) != entry->reg)
                return;

            light_render_submission::submit_directional_light(source);

            break;

        default:
            return;
    }

    source->render_generations[worker_index] =
        (&references::worker_generations.get())[worker_index];
}

void wds_render_manager::submit_region_renderables(render_data* frame_data) {
    for (u32 index = 0; index < frame_data->regions.size; ++index) {
        region* reg = frame_data->regions.entries[index].reg;

        for (u32* renderable = reg->renderables_begin; renderable != reg->renderables_end; ++renderable)
            retail::sub_798590(*renderable, 0, (i32)reg);
    }
}

world_dynamics_system* wds_render_manager::get_world() {
    return (world_dynamics_system*)((u8*)this - offsetof(world_dynamics_system, render_mgr));
}

void wds_render_manager::submit_world(world_dynamics_system* world) {
    using namespace references;

    if (auxiliary_producer_enabled.read() &&
        ngl::d3d9::references::particle_depth_texture_requested.read()) {

        retail::sub_A37670((i32)ngl::d3d9::references::framebuffers.get().linear_depth_buffer);
        retail::sub_A37680(auxiliary_callback_enabled.read() ?
            (void*)retail::sub_95D470 : nullptr);

        if (auxiliary_render_enabled.read())
            retail::sub_A378B0();
    }

    world->hero_ptr->invoke_render_phase();

    i32 worker_index = retail::sub_6020A0(0);
    entity* selected_render_entity = world->hero_ptr;

    if (world->camera_mgr.marky_camera_enabled &&
        world->camera_mgr.marky_camera) {

        selected_render_entity = world->camera_mgr.marky_camera;
    }

    deferred_lights::drain();

    treyarch::render_data* frame_data = references::render_data.read();

    for (u32 index = 0; index < frame_data->regions.size; ++index) {
        render_region_info* entry = &frame_data->regions.entries[index];
        region*             reg   = entry->reg;

        if (!reg)
            continue;

        for (light_source** source = reg->lights_begin; source != reg->lights_end; ++source)
            submit_light(entry, *source, selected_render_entity, worker_index);
    }

    submit_region_renderables(frame_data);
    retail::sub_602210(worker_index, 0);
}

void wds_render_manager::submit_blocked_world(world_dynamics_system* world) {
    world->hero_ptr->invoke_render_phase();

    for (far_away_render_list_entry* entry = references::far_away_render_list.read(); entry; entry = entry->next)
        entry->activate(1.0f);
}
