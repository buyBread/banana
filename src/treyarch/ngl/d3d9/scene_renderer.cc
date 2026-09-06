#include <bit>

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

void ngl::d3d9::render_scene(scene* value) {
    ++references::scene_recursion_depth.get();

    ngl::references::current_scene.write(value);

    value->callbacks[0].invoke();

    // children render first
    for (scene* child = value->first_child; child; child = child->next_sibling)
        render_scene(child);

    ngl::references::current_scene.write(value);

    if (!value->color_target && !value->depth_target) {
        --references::scene_recursion_depth.get();

        return;
    }

    validate_matrices(value);
    apply_scene_state(value);
    value->callbacks[1].invoke();

    bool depth_bias_changed = false;

    if (value->depth_bias_enabled && (value->depth_bias != 0.0f || value->slope_scale_depth_bias != 0.0f)) {
        depth_bias_changed = true;

        set_render_state(D3DRS_DEPTHBIAS,
                         std::bit_cast<DWORD>(value->depth_bias));
        set_render_state(D3DRS_SLOPESCALEDEPTHBIAS,
                         std::bit_cast<DWORD>(value->slope_scale_depth_bias));
    }

    if (value->specialized_render_list_4)
        fx::render_batch(value->specialized_render_list_4);

    if (value->opaque_render_list_count)
        list::render_nodes(value->opaque_render_list);

    // copy depth before drawing translucent nodes
    if (value->options & 0x40)
        copy_active_depth();

    value->callbacks[2].invoke();

    if (value->translucent_render_list_count)
        list::render_nodes(value->translucent_render_list);

    if (depth_bias_changed) {
        set_render_state(D3DRS_DEPTHBIAS, 0);
        set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    }

    // list 3 is only drawn when list 2 is empty
    if (value->specialized_render_list_count_3 && !value->specialized_render_list_count_2)
        list::render_nodes(value->specialized_render_list_3);

    value->callbacks[3].invoke();
    value->callbacks[4].invoke();

    if (value->color_target && value->color_target->gpu_texture.level_count > 1)
        generate_mipmaps(value->color_target);

    --references::scene_recursion_depth.get();

    ngl::references::performance.get().node_count +=
        value->opaque_render_list_count + value->translucent_render_list_count;
}
