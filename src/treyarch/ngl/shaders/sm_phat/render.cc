#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/shaders/shader.hh"
#include "treyarch/ngl/shaders/sm_phat/render_node.hh"

using namespace treyarch;

void ngl::shaders::sm_phat::render(render_node* value) {
    d3d9::set_render_state(D3DRS_FILLMODE,
                           ngl::references::wireframe.read() ?
                           D3DFILL_WIREFRAME : D3DFILL_SOLID);

    if (ngl::references::in_shadow_scene.read())
        render_shadow(value);
    else
        render_normal(value);
}
