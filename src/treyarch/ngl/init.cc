#include "banana/logging.hh"
#include "util/gimmie/fn.hh"

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
    util::gimmie::fn<void(__cdecl*)()>(0x009CCAF0)();
#else
    ngl::dispatch_init_list();
#endif
    
    util::gimmie::fn<void(__cdecl*)()>(0x007C2DE0)(); /* nullsub;
                                                         some unimplemented default fx shader? */

    ngl::resources::init();

    ngl::scene* root_scene = ngl::list_init();

    references::initialized.write(1);

    return root_scene;
}
