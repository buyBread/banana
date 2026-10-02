#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/ngl/font/render.hh"
#include "treyarch/ngl/list/render_callback.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/morph/render_node.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/shaders/pcuv/render_node.hh"
#include "treyarch/ngl/shaders/fake_peds/render_node.hh"
#include "treyarch/ngl/shaders/puv/render_node.hh"
#include "treyarch/ngl/shaders/sm_phat/render_node.hh"
#include "treyarch/ngl/shaders/sm_phatnormal/render_node.hh"
#include "treyarch/ngl/shaders/smsky/render_node.hh"

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

    if (vtable == &shaders::smsky::references::node_vtable.get()) {
        shaders::smsky::render((shaders::smsky::render_node*)this);

        return;
    }

    if (vtable == &shaders::fake_peds::references::node_vtable.get()) {
        shaders::fake_peds::render((shaders::fake_peds::render_node*)this);

        return;
    }

    if (vtable == &morph_geometry::references::node_vtable.get()) {
        morph_geometry::render((morph_geometry::render_node*)this);

        return;
    }

    if (vtable == &shaders::sm_phat::references::node_vtable.get()) {
        shaders::sm_phat::render((shaders::sm_phat::render_node*)this);

        return;
    }

    if (vtable == &shaders::sm_phatnormal::references::node_vtable.get()) {
        shaders::sm_phatnormal::render((shaders::sm_phatnormal::render_node*)this);

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

void ngl::render_node::get_sort_info(sort_info* result) {
    using get_sort_info_function = void (__thiscall*)(render_node* self, sort_info* result);

    ((get_sort_info_function*)vtable)[4](this, result);
}

// sub_884E10
ngl::scene* ngl::list_add_node(render_node* value) {
    sort_info sorting;
    value->get_sort_info(&sorting);

    value->sort_key = sorting.key;

    scene* current = references::current_scene.read();

    if (sorting.type == sort_translucent) {
        value->next = current->translucent_render_list;

        ++current->translucent_render_list_count;
        current->translucent_render_list = value;
    } else {
        value->next = current->opaque_render_list;

        ++current->opaque_render_list_count;
        current->opaque_render_list = value;
    }

    return current;
}
