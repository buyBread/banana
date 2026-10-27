#include "retail.hh"
#include "banana/logging.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/version.hh"
#include "treyarch/ngl/gpu/d3d9/device.hh"
#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/init_list/init.hh"
#include "treyarch/ngl/list/list.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/resources/resources.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/texture/texture.hh"

using namespace treyarch;

// sub_9E45C0
bool ngl::can_release_file(const apk::file* file) {
    return file->last_frame_ref + 1 < (i32)references::frame.read();
}

// sub_9E5CD0
ngl::scene* ngl::init(HWND window) {
    banana::log.ngl("Nyarlathotep's Graphics Laboratory"
                    " "
                    NGL_VERSION);

    references::render_window.write(window);

    _controlfp(_PC_24, _MCW_PC);

    ngl::debug::init();
    
    ngl::mesh_init();
    ngl::texture_init();

    references::frame.write(0);

    ngl::d3d9::init();

    ngl::dispatch_init_list();
    
    retail::sub_7C2DE0(); /* nullsub (retail)
                             some omitted default fx shader? (milestone) */

    ngl::resources::init();

    ngl::scene* root_build_scene = ngl::ngli_list_init();

    references::initialized.write(1);

    banana::log.ngl("initialized");

    return root_build_scene;
}
