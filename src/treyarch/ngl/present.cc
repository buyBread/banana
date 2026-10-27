#include "banana/logging.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/list/list.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/base/tl_system.hh"

using namespace treyarch;

// sub_9DA500
ngl::scene* ngl::present() {
    if (references::build_scene.read() != references::root_build_scene.read())
        tl_fatal("nglPresent called while one or more scenes were still active (need to call nglListEndScene).");

    ngl::debug::reset_primitive_batches();
    ngl::debug::render();
    
    ngl::sort_scene(references::root_build_scene.read());

    ngl::ngli_list_send();

    return ngl::ngli_list_init();
}
