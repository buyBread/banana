#pragma once

#include <cstring>

#include "banana/util/types.hh"
#include "banana/util/macros/sanity_assert.hh"

namespace treyarch { namespace ngl {
    struct mesh_section;
    struct scene;

    namespace fx {
        struct mesh_node_data;
    } // fx

    union render_node_sort_key {
        u32 integer;
        f32 floating;
    };

    enum e_sort_type : u32 {
        sort_opaque      = 0,
        sort_translucent = 1
    };

    struct sort_info {
        e_sort_type          type;
        render_node_sort_key key;
    };

    struct render_node_vtbl {
        // sub_8B3300
        virtual ~render_node_vtbl() {}

        // sub_5B4A70
        virtual void dummy() {}
    };

    struct render_node : render_node_vtbl {
        render_node*         next;
        render_node_sort_key sort_key;

        virtual void render() = 0;

        virtual void get_desc(char* desc) {
            std::strcpy(desc, "");
        }

        // sub_689610
        virtual void get_sort_info(sort_info &) {}
    };

    struct shader_node : render_node {
        fx::mesh_node_data* mesh_node;
        mesh_section*       section;

        void get_desc(char* desc) override;

        virtual i32 get_list_type();

        void set_ifl_frame();
    };

    struct geometry_shader_node : render_node {};

    void   dispatch_render(render_node* value);
    scene* list_add_node(render_node* value);
    void   list_push_node(render_node* value, e_sort_type type);

    ASSERT_SIZEOF  (render_node_sort_key, 0x04);

    ASSERT_SIZEOF  (sort_info,       0x08);
    ASSERT_OFFSETOF(sort_info, type, 0x00);
    ASSERT_OFFSETOF(sort_info, key,  0x04);

    ASSERT_SIZEOF  (render_node,           0x0C);
    ASSERT_OFFSETOF(render_node, next,     0x04);
    ASSERT_OFFSETOF(render_node, sort_key, 0x08);

    ASSERT_SIZEOF  (shader_node,            0x14);
    ASSERT_OFFSETOF(shader_node, mesh_node, 0x0C);
    ASSERT_OFFSETOF(shader_node, section,   0x10);
}} // treyarch::ngl
