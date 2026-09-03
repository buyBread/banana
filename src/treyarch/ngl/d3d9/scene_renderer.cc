#include <cstring>

#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/d3d9/scene_renderer.hh"
#include "treyarch/ngl/d3d9/scene_state.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/fx/batch_renderer.hh"
#include "treyarch/ngl/list/render_cursor.hh"
#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/ngl/scene/references.hh"

using namespace treyarch;

static DWORD float_bits(f32 value) {
    DWORD bits;

    std::memcpy(&bits, &value, sizeof(bits));

    return bits;
}

static void invoke_scene_callback(const ngl::scene_callback &callback) {
    if (callback.function)
        callback.function(callback.context);
}

void ngl::d3d9::render_scene(scene* value) {
    ++references::scene_recursion_depth.get();

    ngl::references::current_scene.write(value);

    invoke_scene_callback(value->callbacks[0]);

    for (scene* child = value->first_child; child; child = child->next_sibling)
        render_scene(child);

    ngl::references::current_scene.write(value);

    if (!value->color_target && !value->depth_target) {
        --references::scene_recursion_depth.get();

        return;
    }

    validate_matrices(value);
    apply_scene_state(value);
    invoke_scene_callback(value->callbacks[1]);

    bool depth_bias_changed = false;

    if (value->depth_bias_enabled && (value->depth_bias != 0.0f || value->slope_scale_depth_bias != 0.0f)) {
        depth_bias_changed = true;

        set_render_state(D3DRS_DEPTHBIAS,
                         float_bits(value->depth_bias));
        set_render_state(D3DRS_SLOPESCALEDEPTHBIAS,
                         float_bits(value->slope_scale_depth_bias));
    }

    if (value->specialized_render_list_4)
        fx::render_batch(value->specialized_render_list_4);

    if (value->opaque_render_list_count)
        list::render_nodes(value->opaque_render_list);

    if (value->options & 0x40)
        copy_active_depth();

    invoke_scene_callback(value->callbacks[2]);

    if (value->translucent_render_list_count)
        list::render_nodes(value->translucent_render_list);

    if (depth_bias_changed) {
        set_render_state(D3DRS_DEPTHBIAS, 0);
        set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    }

    if (value->specialized_render_list_count_3 && !value->specialized_render_list_count_2)
        list::render_nodes(value->specialized_render_list_3);

    invoke_scene_callback(value->callbacks[3]);
    invoke_scene_callback(value->callbacks[4]);

    if (value->color_target && value->color_target->gpu_texture.level_count > 1)
        generate_mipmaps(value->color_target);

    --references::scene_recursion_depth.get();

    ngl::references::performance.get().node_count +=
        value->opaque_render_list_count + value->translucent_render_list_count;
}
