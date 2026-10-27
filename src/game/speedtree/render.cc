#include <bit>
#include <cmath>
#include <cstring>

#include "retail.hh"
#include "game/pre_light.hh"
#include "game/speedtree/wrapper.hh"
#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/mesh_submission.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/fx/pass.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/scene.hh"

using namespace treyarch;

// sub_7CB940
void speed_tree_wrapper::render_billboards_only(const speed_tree_billboards_only_params* params) {
    if (!effect)
        return;

    ngl::scene* scene = ngl::references::current_scene.read();

    if (!scene->color_target || scene->depth_bias_enabled)
        return;

    find_technique(references::speed_tree_billboards_only_guard.get(),
                   1,
                   references::speed_tree_billboards_only_technique.get(),
                   "billboardsonly");

    ngl::fx::pass* pass = references::speed_tree_billboards_only_technique.read()->passes;

    if (scene->color_target) {
        upload_lighting(params->light);
        upload_color(region_colors[params->region]);
    }

    upload_transforms(axis_basis());

    f32 special_azimuth = upload_billboard_images();

    ngl::fx::set_float(parameters.billboard_fade, 1.0f);
    ngl::fx::set_float(parameters.tree_scale, 1.0f);
    ngl::fx::set_float(parameters.special_azimuth, special_azimuth);

    pass->states.alpha_blend_enabled = FALSE;
    pass->states.alpha_test_enabled  = TRUE;

    ngl::fx::begin_pass(effect, pass);

    ngl::d3d9::enable_alpha_test(D3DCMP_GREATER, 84);
    ngl::d3d9::draw_primitive(D3DPT_TRIANGLELIST,
                              region_vertex_counts[params->region],
                              0,
                              &region_vertex_buffers[params->region],
                              &references::speed_tree_region_billboard_format.get());

    ngl::fx::end_pass(effect, pass);
}

// sub_7CC710
void speed_tree_wrapper::render_billboards(const speed_tree_render_params* params) {
    ngl::scene* scene = ngl::references::current_scene.read();

    if (!scene->color_target || scene->depth_bias_enabled)
        return;

    speedtree::lod_values lods;
    retail::sub_A3C7D0((u32*)&lods);

    find_technique(references::speed_tree_billboards_guard.get(),
                   1,
                   references::speed_tree_billboards_technique.get(),
                   "billboards");

    ngl::fx::pass* pass = references::speed_tree_billboards_technique.read()->passes;

    if (scene->color_target)
        upload_lighting(params->light);

    math::mat43 basis = axis_basis();

    upload_transforms(basis);

    ngl::fx::set_vector(parameters.dof, math::vector4(1.0f, 1.0f, 1.0f, 0.0f));

    f32 special_azimuth = upload_billboard_images();

    i32 index = 0;

    for (speed_tree_instance* instance = params->region.instances; instance; instance = instance->next) {
        if (index >= params->region.instance_count)
            break;

        ++index;

        if (!instance->in_view(basis))
            continue;

        retail::sub_A3CE30((f32*)instance->tree, (f32*)&lods, -1.0f);

        if (0.0f >= lods.billboard_fade)
            continue;

        if (retail::sub_A3C9E0((u32*)instance->tree) > 0.10000000149011612)
            continue;

        if (scene->color_target) {
            u32 color = instance->color_source->get_color();

            upload_color((const u8*)&color);
        }

        const vector3d &position = instance->position;

        f32 scale = instance->scale;

        math::vector4 vertices[4] { math::vector4(position.x, (f32)((f64)position.z + scale), (f32)((f64)position.y - scale), 0.0f),
                                    math::vector4(position.x, (f32)((f64)position.z + 0.0),   (f32)((f64)position.y - scale), 0.0f),
                                    math::vector4(position.x, (f32)((f64)position.z + 0.0),   (f32)((f64)scale + position.y), 0.0f),
                                    math::vector4(position.x, (f32)((f64)scale + position.z), (f32)((f64)scale + position.y), 0.0f) };

        std::memcpy((u8*)parameters.billboard_vertex_table->data + 16, vertices, sizeof(vertices));

        ngl::fx::set_float(parameters.billboard_fade, lods.billboard_fade);
        ngl::fx::set_float(parameters.tree_scale, instance->scale);
        ngl::fx::set_float(parameters.special_azimuth, special_azimuth);

        pass->states.alpha_blend_enabled = FALSE;
        pass->states.alpha_test_enabled  = TRUE;

        // the pass is never ended
        ngl::fx::begin_pass(effect, pass);

        ngl::d3d9::enable_alpha_test(D3DCMP_GREATER, 84);
        ngl::d3d9::set_vertex_buffer(&billboard_vertex_buffer, &references::speed_tree_billboard_format.get(), 0, 0);
        ngl::d3d9::set_index_buffer(&billboard_index_buffer);

        ngl::d3d9::references::device.get()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
    }
}

// sub_7CD8C0
void speed_tree_wrapper::render_branches(const speed_tree_render_params* params) {
    speedtree::lod_values lods;
    retail::sub_A3C7D0((u32*)&lods);

    ngl::scene* scene = ngl::references::current_scene.read();

    bool depth_only = !scene->color_target || scene->depth_bias_enabled;

    find_technique(references::speed_tree_branches_guard.get(),
                   1,
                   references::speed_tree_branches_technique.get(),
                   "branches");

    find_technique(references::speed_tree_branches_guard.get(),
                   2,
                   references::speed_tree_branches_depth_technique.get(),
                   "branchesdepth");

    ngl::fx::technique* technique = depth_only ?
        references::speed_tree_branches_depth_technique.read() :
        references::speed_tree_branches_technique.read();

    ngl::fx::pass* pass = technique->passes;

    upload_wind_matrices();

    if (!depth_only)
        upload_lighting(params->light);

    math::mat43 basis = axis_basis();

    upload_transforms(basis);

    ngl::fx::set_vector(parameters.dof, math::vector4(1.0f, 1.0f, 1.0f, 0.0f));

    if (!depth_only) {
        ngl::fx::set_texture(parameters.self_shadow_map, textures[7]);
        ngl::fx::set_texture(parameters.branch_normal_map, textures[2]);
        ngl::fx::set_texture(parameters.branch_diffuse_map, textures[0]);
    }

    bool drawn = false;
    i32 index  = 0;

    for (speed_tree_instance* instance = params->region.instances; instance; instance = instance->next) {
        if (index >= params->region.instance_count)
            break;

        ++index;

        if (!instance->in_view(basis))
            continue;

        f32 lod_level = (f32)retail::sub_A3C9E0((u32*)instance->tree);

        if (!scene->color_target && !scene->depth_target)
            retail::sub_A3CA10((u32*)instance->tree, 0.0f);

        retail::sub_A3CE30((f32*)instance->tree, (f32*)&lods, -1.0f);

        i32 lod = lods.branch_lod;

        if (branch_strip_lengths && lod > -1 && branch_strip_lengths[lod]) {
            if (!depth_only) {
                u32 color = instance->color_source->get_color();

                upload_color((const u8*)&color);
            }

            ngl::fx::set_float(parameters.wind_matrix_offset, (f32)instance->wind_matrix_offset);
            ngl::fx::set_vector(parameters.tree_position,
                                vector4d(instance->position, (f32)((f64)instance->rotation / 57.295780181884766)));
            ngl::fx::set_float(parameters.tree_scale, instance->scale);
            ngl::fx::set_float(parameters.tree_alpha_test_value, (f32)((f64)lods.frond_alpha / 255.0));

            ngl::fx::begin_pass(effect, pass);

            ngl::d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_CW);

            ngl::d3d9::set_vertex_buffer(&branch_vertex_buffer, &references::speed_tree_branch_format.get(), 0, 0);
            ngl::d3d9::set_index_buffer(&branch_index_buffers[lod]);

            ngl::d3d9::references::device.get()->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
                                                                      0,
                                                                      0,
                                                                      branch_vertex_count,
                                                                      0,
                                                                      branch_strip_lengths[lod] - 2);

            drawn = true;
        }

        retail::sub_A3CA10((u32*)instance->tree, lod_level);
    }

    if (drawn)
        ngl::fx::end_pass(effect, pass);
}

// sub_7CEA90
void speed_tree_wrapper::render_fronds(const speed_tree_render_params* params) {
    speedtree::lod_values lods;
    retail::sub_A3C7D0((u32*)&lods);

    ngl::scene* scene = ngl::references::current_scene.read();

    bool depth_only = !scene->color_target || scene->depth_bias_enabled;

    find_technique(references::speed_tree_fronds_guard.get(),
                   1,
                   references::speed_tree_fronds_technique.get(),
                   "fronds");

    find_technique(references::speed_tree_fronds_guard.get(),
                   2,
                   references::speed_tree_fronds_depth_technique.get(),
                   "frondsdepth");

    ngl::fx::technique* technique = depth_only ?
        references::speed_tree_fronds_depth_technique.read() :
        references::speed_tree_fronds_technique.read();

    ngl::fx::pass* pass = technique->passes;

    upload_wind_matrices();

    if (!depth_only)
        upload_lighting(params->light);

    math::mat43 basis = axis_basis();

    upload_transforms(basis);

    ngl::fx::set_vector(parameters.dof, math::vector4(1.0f, 1.0f, 1.0f, 0.0f));
    ngl::fx::set_texture(parameters.composite_leaf_map, textures[8]);

    if (!depth_only)
        ngl::fx::set_texture(parameters.self_shadow_map, textures[15]);

    bool drawn = false;
    i32 index  = 0;

    for (speed_tree_instance* instance = params->region.instances; instance; instance = instance->next) {
        if (index >= params->region.instance_count)
            break;

        ++index;

        if (!instance->in_view(basis))
            continue;

        f32 lod_level = (f32)retail::sub_A3C9E0((u32*)instance->tree);

        if (!scene->color_target)
            retail::sub_A3CA10((u32*)instance->tree, 0.0f);

        retail::sub_A3CE30((f32*)instance->tree, (f32*)&lods, -1.0f);

        i32 lod = lods.frond_lod;

        if (lods.frond_alpha > 0.0f && frond_strip_lengths && lod > -1 && frond_strip_lengths[lod]) {
            if (!depth_only) {
                u32 color = instance->color_source->get_color();

                upload_color((const u8*)&color);
            }

            ngl::fx::set_float(parameters.wind_matrix_offset, (f32)instance->wind_matrix_offset);
            ngl::fx::set_vector(parameters.tree_position,
                                vector4d(instance->position, (f32)((f64)instance->rotation / 57.295780181884766)));
            ngl::fx::set_float(parameters.tree_scale, instance->scale);
            ngl::fx::set_float(parameters.tree_alpha_test_value, (f32)((f64)lods.frond_alpha / 255.0));

            pass->states.alpha_test_enabled = TRUE;

            ngl::fx::begin_pass(effect, pass);

            ngl::d3d9::set_render_state(D3DRS_ALPHATESTENABLE, FALSE);
            ngl::d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);

            ngl::d3d9::draw_indexed_primitive(D3DPT_TRIANGLESTRIP,
                                              frond_strip_lengths[lod],
                                              0,
                                              &frond_index_buffers[lod],
                                              D3DFMT_INDEX16,
                                              frond_vertex_count,
                                              0,
                                              &frond_vertex_buffer,
                                              &references::speed_tree_frond_format.get());

            drawn = true;
        }

        retail::sub_A3CA10((u32*)instance->tree, lod_level);
    }

    if (drawn)
        ngl::fx::end_pass(effect, pass);
}

// sub_7CFC10
void speed_tree_wrapper::render_leafs(const speed_tree_render_params* params) {
    speedtree::lod_values lods;
    retail::sub_A3C7D0((u32*)&lods);

    ngl::scene* scene = ngl::references::current_scene.read();

    bool depth_only = !scene->color_target || scene->depth_bias_enabled;

    find_technique(references::speed_tree_leaves_guard.get(),
                   1,
                   references::speed_tree_leaves_technique.get(),
                   "leaves");

    find_technique(references::speed_tree_leaves_guard.get(),
                   2,
                   references::speed_tree_leaves_depth_technique.get(),
                   "leavesdepth");

    find_technique(references::speed_tree_leaves_guard.get(),
                   4,
                   references::speed_tree_leaves_translucent_technique.get(),
                   "leavestranslucent");

    ngl::fx::technique* technique = depth_only ?
        references::speed_tree_leaves_depth_technique.read() :
        references::speed_tree_leaves_technique.read();

    if (params->translucent)
        technique = references::speed_tree_leaves_translucent_technique.read();

    ngl::fx::pass* pass = technique->passes;

    upload_wind_matrices();

    ngl::fx::update_scene_parameters(effect);

    math::mat44 unit_square(math::vector4(0.0f,  0.5f,  0.5f, 0.0f),
                            math::vector4(0.0f, -0.5f,  0.5f, 0.0f),
                            math::vector4(0.0f, -0.5f, -0.5f, 0.0f),
                            math::vector4(0.0f,  0.5f, -0.5f, 0.0f));

    ngl::fx::set_matrix(parameters.leaf_unit_square, ~unit_square);
    ngl::fx::set_matrix(parameters.model_view, math::mat44(scene->world_to_view));
    ngl::fx::set_vector(parameters.dof, math::vector4(1.0f, 1.0f, 1.0f, 0.0f));

    // two sets of ten, in radians
    auto* angles = (math::vector4*)((u8*)parameters.leaf_angles->data + 16);

    const f32* angles_060 = references::speed_tree_wind.get().leaf_angles_060;
    const f32* angles_084 = references::speed_tree_wind.get().leaf_angles_084;

    for (u32 index = 0; index < 20; ++index) {
        angles[index] = math::vector4((f32)((f64)angles_060[index] / 57.295780181884766),
                                      (f32)((f64)angles_084[index] / 57.295780181884766),
                                      0.0f,
                                      0.0f);
    }

    if (!depth_only)
        upload_lighting(params->light);

    math::mat43 basis = axis_basis();

    ngl::fx::set_matrix(parameters.model_view_projection, math::mat44(basis) * scene->world_to_screen);
    ngl::fx::set_matrix(parameters.local_to_world, math::mat44(basis));

    ngl::fx::set_texture(parameters.composite_leaf_map, textures[8]);
    ngl::fx::set_vector(parameters.horizon_projection_u, params->horizon_projection_u);
    ngl::fx::set_vector(parameters.horizon_projection_v, params->horizon_projection_v);
    ngl::fx::set_texture(parameters.horizon_map, params->prelight->horizon_map.pointer);
    ngl::fx::set_float(parameters.leaf_cards_active, 1.0f);
    ngl::fx::set_vector(parameters.leaf_angle_scalars, math::vector4(0.4f, 0.4f, 0.0f, 0.0f));

    math::dir3 &view_direction = references::speed_tree_view_direction.get();

    // compared bit for bit
    if (std::bit_cast<u32>(view_direction[0]) != std::bit_cast<u32>(scene->view_direction[0]) ||
        std::bit_cast<u32>(view_direction[1]) != std::bit_cast<u32>(scene->view_direction[1]) ||
        std::bit_cast<u32>(view_direction[2]) != std::bit_cast<u32>(scene->view_direction[2])) {

        view_direction = scene->view_direction;

        f32 heading = (f32)std::atan2((f64)view_direction[2], (f64)view_direction[0]);
        f32 pitch   = (f32)std::asin((f64)view_direction[1]);

        references::speed_tree_view_heading.write((f32)(heading * 57.295780181884766));
        references::speed_tree_view_pitch  .write((f32)(-pitch * 57.295780181884766));
    }

    f32 pitch = references::speed_tree_view_pitch.read();

    if (pitch > 45.0f)
        pitch = 45.0f;
    else if (-45.0f > pitch)
        pitch = -45.0f;

    ngl::fx::set_vector(parameters.camera_angles,
                        math::vector4((f32)((f64)references::speed_tree_view_heading.read() * 3.1415927410125732 / 180.0),
                                      (f32)((f64)pitch * 3.1415927410125732 / 180.0),
                                      0.0f,
                                      0.0f));

    if (!params->translucent)
        ngl::fx::set_float(parameters.opacity, 0.0f);

    bool drawn = false;
    i32 index  = 0;

    for (speed_tree_instance* instance = params->region.instances; instance; instance = instance->next) {
        if (index >= params->region.instance_count)
            break;

        ++index;

        if (params->fades) {
            f32 fade = params->fades[index - 1];

            // opaque passes take the fully faded-in trees, translucent passes the rest
            if ((fade == 1.0f) == params->translucent)
                continue;

            if (params->translucent)
                ngl::fx::set_float(parameters.opacity, fade);
        }

        if (!instance->in_view(basis))
            continue;

        f32 lod_level = (f32)retail::sub_A3C9E0((u32*)instance->tree);

        if (!scene->color_target)
            retail::sub_A3CA10((u32*)instance->tree, 0.0f);

        retail::sub_A3CE30((f32*)instance->tree, (f32*)&lods, -1.0f);

        if (!depth_only) {
            u32 color = instance->color_source->get_color();

            upload_color((const u8*)&color);
        }

        ngl::fx::set_float(parameters.wind_matrix_offset, (f32)instance->wind_matrix_offset);
        ngl::fx::set_vector(parameters.tree_position,
                            vector4d(instance->position, (f32)((f64)instance->rotation / 57.295780181884766)));
        ngl::fx::set_float(parameters.tree_scale, instance->scale);

        for (u32 layer = 0; layer < 2; ++layer) {
            i32 lod = lods.leaf_lods[layer];

            if (lod < 0)
                continue;

            ngl::fx::set_float(parameters.tree_alpha_test_value, (f32)((f64)lods.leaf_alphas[layer] / 255.0));

            const speed_tree_leaf_lod &leaf_lod = leaf_lods[lod];

            if (leaf_lod.card_count <= 0)
                continue;

            pass->states.alpha_test_enabled = TRUE;

            ngl::fx::begin_pass(effect, pass);

            ngl::d3d9::set_vertex_buffer(&leaf_vertex_buffers[leaf_lod.vertex_buffer],
                                         &references::speed_tree_leaf_format.get(),
                                         0,
                                         0);

            ngl::d3d9::references::device.get()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, (u32)(6 * leaf_lod.card_count) / 3);

            drawn = true;
        }

        retail::sub_A3CA10((u32*)instance->tree, lod_level);
    }

    if (drawn)
        ngl::fx::end_pass(effect, pass);
}
