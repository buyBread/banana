#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/debug/primitive_batches.hh"
#include "treyarch/ngl/debug/render.hh"
#include "treyarch/ngl/list/init.hh"
#include "treyarch/ngl/scene/references.hh"
#include "util/gimmie/fn.hh"

using namespace treyarch;

using error_fn = void(__cdecl*)(const char*, ...);
using void_fn  = void(__cdecl*)();
using scene_fn = void(__cdecl*)(ngl::scene*);

ngl::scene* __cdecl ngl::present() {
    if (references::current_scene.read() != references::root_scene.read())
        util::gimmie::fn<error_fn>(0x009CC940)("nglPresent called while one or more scenes were still active (need to call nglListEndScene).");

    ngl::debug::reset_primitive_batches(); // ?
    ngl::debug::render();
    
    util::gimmie::fn<scene_fn>(0x009DA420)(references::root_scene.read());
    util::gimmie::fn<void_fn> (0x009E7010)();

    return ngl::list_init();
}
