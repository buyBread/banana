#include "banana/logging.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/debug/primitive_batches.hh"
#include "treyarch/ngl/debug/render.hh"
#include "treyarch/ngl/list/init.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/render_sort.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

// sub_9DA500
ngl::scene* ngl::present() {
    if (references::current_scene.read() != references::root_scene.read()) {
        memory::report("nglPresent called while one or more scenes were still active (need to call nglListEndScene).");

        banana::log.ngl("ngl::present called while one or more scenes were still active; need to call ngl::list_end_scene!");
    }

    ngl::debug::reset_primitive_batches();
    ngl::debug::render();
    
    ngl::render_sort(references::root_scene.read());

    ngl::list_send();

    return ngl::list_init();
}
