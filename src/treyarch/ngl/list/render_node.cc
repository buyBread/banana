#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/ngl/font/render.hh"
#include "treyarch/ngl/list/render_callback.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "treyarch/ngl/shaders/pcuv/render_node.hh"
#include "treyarch/ngl/shaders/puv/render_node.hh"

#ifdef DEBUG
    #include <unordered_set>

    #include "banana/logging.hh"
#endif

using namespace treyarch;

void ngl::render_node::render() {
    if (vtable == &render_callback::references::node_vtable.get()) {
        render_callback::render((render_callback::node*)this);

        return;
    }

    if (vtable == &fx::references::node_vtable.get()) {
        fx::render((fx::render_node*)this);

        return;
    }

    if (vtable == &sm_lod_mesh_instance::references::node_vtable.get())
        return; // nullsub

    if (vtable == &quad_renderer::references::node_vtable.get()) {
        quad_renderer::render((quad_renderer::node*)this);

        return;
    }

    if (vtable == &string_renderer::references::node_vtable.get()) {
        string_renderer::render((string_renderer::node*)this);

        return;
    }

    if (vtable == &shaders::pcuv::references::node_vtable.get()) {
        shaders::pcuv::render((shaders::pcuv::render_node*)this);

        return;
    }

    if (vtable == &shaders::puv::references::node_vtable.get()) {
        shaders::puv::render((shaders::puv::render_node*)this);

        return;
    }

    /*
        fallback
    */

    using render_function = void(__thiscall*)(render_node*);

#ifndef DEBUG
    ((render_function*)vtable)[2](this);
#else
    static std::unordered_set<void*> seen;
    static void* last = nullptr; // kill performance on debug builds slightly less

    render_function fn = ((render_function*)vtable)[2];

    auto* address = (void*)fn;

    if (address != last) {
        last = address;

        if (seen.insert(address).second)
            banana::log.ngl("unowned render node -- address: {:p} | vtable: {:p}", address, vtable);
    }

    fn(this);
#endif
}
