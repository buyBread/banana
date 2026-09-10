#include "retail.hh"
#include "banana/logging.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/version.hh"
#include "treyarch/ngl/d3d9/init.hh"
#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/init_list/init.hh"
#include "treyarch/ngl/list/init.hh"
#include "treyarch/ngl/mesh/init.hh"
#include "treyarch/ngl/resources/init.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/texture/init.hh"

using namespace treyarch;

ngl::scene* ngl::init(HWND window) {
    banana::log.dbg("Nyarlathotep's Graphics Laboratory"
                    " "
                    NGL_VERSION);

    references::render_window.write(window);

    _controlfp(_PC_24, _MCW_PC);

    ngl::debug::init();
    
    ngl::mesh_init();
    ngl::texture_init();

    references::frame_epoch.write(0);

    ngl::d3d9::init();

#ifdef NDEBUG
    /* sm_phat is currently still incorrect (somehow??),
       so we shouldn't bundle our shader programs in a normal build right now... */
    retail::sub_9CCAF0();
#else
    ngl::dispatch_init_list();
#endif
    
    retail::sub_7C2DE0(); /* nullsub (retail)
                             some omitted default fx shader? (milestone) */

    ngl::resources::init();

    ngl::scene* root_scene = ngl::list_init();

    references::initialized.write(1);

    banana::log.dbg("NGL initialized");

    return root_scene;
}
