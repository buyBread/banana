#include "game/speedtree/render_node.hh"
#include "treyarch/ngl/font/render.hh"
#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/ngl/list/render_callback.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/morph/render_node.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/shaders/fake_peds/render_node.hh"
#include "treyarch/ngl/shaders/pcuv/render_node.hh"
#include "treyarch/ngl/shaders/puv/render_node.hh"
#include "treyarch/ngl/shaders/sm_phat/render_node.hh"
#include "treyarch/ngl/shaders/sm_phatnormal/render_node.hh"
#include "treyarch/ngl/shaders/smsky/render_node.hh"
#include "retail.hh"

#ifdef DEBUG
    #include <unordered_set>
    #include <windows.h>

    #include "banana/logging.hh"
#endif

using namespace treyarch;

// retail producers still build nodes on retail vftables
struct native_node_class {
    void* vftable;
    void  (*render)(ngl::render_node* value);
};

template <typename node_type>
void render_native_node(ngl::render_node* value) {
    ((node_type*)value)->node_type::render();
}

const native_node_class* find_native_node_class(void* vftable) {
    static const native_node_class classes[] {
        { &ngl::references::render_callback_node_vftable.get(),                 render_native_node<ngl::render_callback_node>            },
        { &ngl::fx::references::shader_node_vftable.get(),                      render_native_node<ngl::fx::shader_node>                 },
        { &ngl::references::sm_lod_mesh_instance_node_vftable.get(),            render_native_node<ngl::sm_lod_mesh_instance_node>       },
        { &ngl::references::quad_node_vftable.get(),                            render_native_node<ngl::quad_node>                       },
        { &ngl::references::string_node_vftable.get(),                          render_native_node<ngl::string_node>                     },
        { &ngl::shaders::pcuv::references::node_vftable.get(),                  render_native_node<ngl::shaders::pcuv::node>             },
        { &ngl::shaders::puv::references::node_vftable.get(),                   render_native_node<ngl::shaders::puv::node>              },
        { &ngl::shaders::smsky::references::node_vftable.get(),                 render_native_node<ngl::shaders::smsky::node>            },
        { &ngl::shaders::fake_peds::references::render_node_vftable.get(),      render_native_node<ngl::shaders::fake_peds::render_node> },
        { &ngl::references::morph_geometry_shader_node_vftable.get(),           render_native_node<ngl::morph_geometry_shader_node>      },
        { &ngl::shaders::sm_phat::references::node_vftable.get(),               render_native_node<ngl::shaders::sm_phat::node>          },
        { &ngl::shaders::sm_phatnormal::references::node_vftable.get(),         render_native_node<ngl::shaders::sm_phatnormal::node>    },
        { &treyarch::references::speed_tree_billboards_only_node_vftable.get(), render_native_node<speed_tree_billboards_only_node>      },
        { &treyarch::references::speed_tree_billboards_node_vftable.get(),      render_native_node<speed_tree_billboards_node>           },
        { &treyarch::references::speed_tree_branches_node_vftable.get(),        render_native_node<speed_tree_branches_node>             },
        { &treyarch::references::speed_tree_fronds_node_vftable.get(),          render_native_node<speed_tree_fronds_node>               },
        { &treyarch::references::speed_tree_leafs_node_vftable.get(),           render_native_node<speed_tree_leafs_node>                },
    };

    for (const native_node_class &entry : classes) {
        if (entry.vftable == vftable)
            return &entry;
    }

    return nullptr;
}

#ifdef DEBUG
// once per render target, for retail-built nodes no source class renders yet
void log_unowned_node(void* vftable) {
    static std::unordered_set<void*> seen;
    static void* last = nullptr; // kill performance on debug builds slightly less

    void* target = ((void**)vftable)[2];

    if (target != last) {
        last = target;

        if (seen.insert(target).second)
            banana::log.ngl("unowned render node -- address: {:p} | vtable: {:p}", target, vftable);
    }
}
#endif

void ngl::dispatch_render(render_node* value) {
    void* vftable = *(void**)value;

    if (const native_node_class* native = find_native_node_class(vftable)) {
        native->render(value);

        return;
    }

#ifdef DEBUG
    log_unowned_node(vftable);
#endif

    value->render();
}

// sub_8A2550
void ngl::shader_node::get_desc(char* desc) {
    retail::sub_8A2550((u32*)this, desc);
}

// sub_8A25E0
i32 ngl::shader_node::get_list_type() {
    return retail::sub_8A25E0((u32*)this);
}

// sub_884E10
ngl::scene* ngl::list_add_node(render_node* value) {
    sort_info sorting;
    value->get_sort_info(sorting);

    value->sort_key = sorting.key;

    scene* current = references::build_scene.read();

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

// inlined @ sub_7D8160, sub_7D8390
void ngl::list_push_node(render_node* value, e_sort_type type) { // keeps the node's own sort key
    scene* current = references::build_scene.read();

    if (type == sort_translucent) {
        value->next = current->translucent_render_list;

        ++current->translucent_render_list_count;
        current->translucent_render_list = value;
    } else {
        value->next = current->opaque_render_list;

        ++current->opaque_render_list_count;
        current->opaque_render_list = value;
    }
}
