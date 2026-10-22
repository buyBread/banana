#include <algorithm>
#include <cmath>

#include "retail.hh"
#include "treyarch/amalga/resource_types.hh"
#include "treyarch/game/speedtree/render_node.hh"
#include "treyarch/game/speedtree/wrapper.hh"
#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/shared/hash/string_hash.hh"

using namespace treyarch;

// sub_7D8390
void speed_tree_wrapper::submit_nodes() {
    ngl::scene* scene = ngl::references::current_scene.read();

    ngl::validate_matrices(scene);

    for (i32 index = 0; index < 16; ++index) {
        speed_tree_region &entry = regions[index];

        if (!entry.instances)
            continue;

        speed_tree_render_params params;

        params.region = entry;
        params.light  = (light_source_data*)retail::sub_7D7340(entry.world_region);

        math::mat43 basis = axis_basis();

        params.prelight = entry.world_region->prelight;

        retail::sub_7BC640((u32*)params.prelight, index, 4, (i32)&params.horizon_projection_u);

        params.fades       = nullptr;
        params.translucent = false;

        bool fading = false;

        region** fading_regions = &references::speed_tree_fading_regions.get();

        for (u32 fading_index = 0; fading_index < references::speed_tree_fading_region_count.read(); ++fading_index) {
            if (fading_regions[fading_index] == entry.world_region) {
                fading = true;

                break;
            }
        }

        vector3d minimum;
        vector3d maximum;

        retail::sub_77EAC0((u32*)entry.world_region, (i32)&minimum, (f32*)&maximum);

        // the top of every region's bounds is pinned to 20
        maximum.y = 20.0f;

        vector3d center((f32)((f64)(f32)((f64)maximum.x + minimum.x) * 0.5),
                        (f32)((f64)(f32)((f64)minimum.y + 20.0) * 0.5),
                        (f32)((f64)(f32)((f64)maximum.z + minimum.z) * 0.5));

        f32 extent_x = (f32)((f64)maximum.x - minimum.x);
        f32 extent_y = (f32)(20.0 - (f64)minimum.y);
        f32 extent_z = (f32)((f64)maximum.z - minimum.z);

        f32 length = (f32)std::sqrt((f64)(f32)((f64)extent_y * extent_y +
                                               (f64)extent_x * extent_x +
                                               (f64)extent_z * extent_z));

        f32 radius = (f32)((f64)length * 0.5);

        if (retail::sub_95C1A0((f32*)&center, radius))
            continue;

        if (fading) {
            f32* fades = (f32*)ngl::list::allocate(entry.instance_count * sizeof(f32), 16);

            params.fades = fades;

            f32 nearest = 3.4028235e38f;

            for (speed_tree_instance* instance = entry.instances; instance; instance = instance->next, ++fades) {
                u32 &guard = references::speed_tree_fade_offset_guard.get();

                if (!(guard & 1)) {
                    guard |= 1;

                    references::speed_tree_fade_offset.write(math::vector4(0.0f, 0.0f, 3.0f, 0.0f));
                }

                const math::vector4 &offset = references::speed_tree_fade_offset.get();

                vector3d position((f32)((f64)instance->position.x + offset[0]),
                                  (f32)((f64)instance->position.y + offset[1]),
                                  (f32)((f64)instance->position.z + offset[2]));

                vector3d transformed;

                if (!sphere_in_view(basis, position, instance->radius, transformed))
                    continue;

                const math::rot_tran_mat43 &world_to_view = scene->world_to_view;

                f32 depth = (f32)((f64)world_to_view.get_x()[2] * transformed.x +
                                  (f64)world_to_view.get_y()[2] * transformed.y +
                                  (f64)world_to_view.get_z()[2] * transformed.z +
                                  world_to_view.get_w()[2]);

                f32 distance = (f32)std::fabs((f64)depth);

                if (10.0f > distance)
                    *fades = 0.25f;
                else if (25.0 > (f64)distance)
                    *fades = std::max(0.25f, (f32)(((f64)distance - 10.0) / 15.0));
                else {
                    *fades = 1.0f;

                    continue;
                }

                nearest = std::min(distance, nearest);
            }

            if (nearest != 3.4028235e38f) {
                params.translucent = true;

                auto* node = (speed_tree_leafs_node*)ngl::list::allocate(sizeof(speed_tree_leafs_node), 16);

                node->base.vtable = &references::speed_tree_leafs_node_vtable.get();
                node->wrapper     = this;
                node->params      = params;

                node->base.sort_key.floating = nearest;

                ngl::list_push_node(&node->base, ngl::sort_translucent);

                params.translucent = false;
            } else
                params.fades = nullptr;
        }

        auto* leafs = (speed_tree_leafs_node*)ngl::list::allocate(sizeof(speed_tree_leafs_node), 16);

        leafs->base.vtable = &references::speed_tree_leafs_node_vtable.get();
        leafs->wrapper     = this;
        leafs->params      = params;

        leafs->base.sort_key.integer = 0xF0000000;

        ngl::list_push_node(&leafs->base, ngl::sort_opaque);

        auto* billboards = (speed_tree_billboards_node*)ngl::list::allocate(sizeof(speed_tree_billboards_node), 16);

        billboards->base.vtable = &references::speed_tree_billboards_node_vtable.get();
        billboards->wrapper     = this;
        billboards->params      = params;

        billboards->base.sort_key.integer = 0xC0000000;

        ngl::list_push_node(&billboards->base, ngl::sort_opaque);

        auto* fronds = (speed_tree_fronds_node*)ngl::list::allocate(sizeof(speed_tree_fronds_node), 16);

        fronds->base.vtable = &references::speed_tree_fronds_node_vtable.get();
        fronds->wrapper     = this;
        fronds->params      = params;

        fronds->base.sort_key.integer = 0xE0000000;

        ngl::list_push_node(&fronds->base, ngl::sort_opaque);

        auto* branches = (speed_tree_branches_node*)ngl::list::allocate(sizeof(speed_tree_branches_node), 16);

        branches->base.vtable = &references::speed_tree_branches_node_vtable.get();
        branches->wrapper     = this;
        branches->params      = params;

        branches->base.sort_key.integer = 0xD0000000;

        ngl::list_push_node(&branches->base, ngl::sort_opaque);
    }
}

// sub_7D8160
void speed_tree_wrapper::submit_billboards_only(bool all_regions) {
    void* environment = references::speed_tree_global_environment.read();

    if (!environment) {
        string_hash name;
        name.initialize(mash::ALLOCATED, "global");

        environment = (void*)retail::sub_687470((i32)amalga::e_resource_type::ENVIRONMENT, name.source_hash_code);

        references::speed_tree_global_environment.write(environment);
    }

    auto* light = (light_source_data*)retail::sub_7D6D60(environment, 1);

    ngl::scene* scene = ngl::references::current_scene.read();

    ngl::validate_matrices(scene);

    region* pool = &treyarch::references::region_pool.get();

    for (i32 index = 0; index < 800; ++index) {
        if (region_vertex_counts[index] <= 0)
            continue;

        region &world_region = pool[index];

        if (!all_regions && retail::sub_77EC90((i32)&world_region))
            continue;

        f32 offset_x = (f32)((f64)world_region.center.x - scene->view_position[0]);
        f32 offset_y = (f32)(0.0 - 0.0);
        f32 offset_z = (f32)((f64)world_region.center.z - scene->view_position[2]);

        f32 facing = (f32)((f64)scene->view_direction[0] * offset_x +
                           (f64)offset_y * 0.0 +
                           (f64)scene->view_direction[2] * offset_z);

        // behind the camera only within 200 units
        if (!(facing > 0.0f)) {
            f32 distance_squared = (f32)((f64)offset_x * offset_x +
                                         (f64)offset_y * offset_y +
                                         (f64)offset_z * offset_z);

            if (!(40000.0f > distance_squared))
                continue;
        }

        auto* node = (speed_tree_billboards_only_node*)ngl::list::allocate(sizeof(speed_tree_billboards_only_node), 16);

        node->base.vtable   = &references::speed_tree_billboards_only_node_vtable.get();
        node->wrapper       = this;
        node->params.region = index;
        node->params.light  = light;

        node->base.sort_key.integer = 0xC0000000;

        ngl::list_push_node(&node->base, ngl::sort_opaque);
    }
}
