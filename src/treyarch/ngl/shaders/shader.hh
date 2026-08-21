#pragma once

#include "treyarch/ngl/instance_bank/instance_bank.hh"
#include "treyarch/ngl/init_list/init_list.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct material;
    struct mesh;
    struct mesh_node;
    struct mesh_section;

    enum class e_shader_id : u32 {
        fx                       = 0,
        sm_bush                  = 1,
        sm_citylod               = 2,
        sm_retrofit              = 3,
        sm_buildinglod           = 4,
        sm_roadlod               = 5,
        sm_decalchar             = 6,
        sm_babyphatnormal        = 7,
        sm_babyphat              = 8,
        sm_decal                 = 9,
        sm_phat_palette_normal   = 10,
        sm_phatnormal            = 11,
        sm_road                  = 12,
        sm_phatpalettecharnormal = 13,
        sm_phatpalettechar       = 14,
        sm_depth_shadow          = 15,
        smsky                    = 16,
        sm_bright_filter         = 17,
        sm_phat_palette          = 18,
        sm_phatcharnormal        = 19,
        sm_phatchar              = 20,
        sm_phatspiderman         = 21,
        sm_phat                  = 22,
        puv                      = 23,
        sm_simple                = 24,
        sm_translucent           = 25,
        pcuv                     = 26,
    };

    struct shader : init_list {
        virtual ~shader() = default;
        virtual void register_item() = 0;
        virtual const fixed_string& get_name() const = 0;
        virtual void add_node(mesh_node* node, mesh_section* section, material* value) = 0;
        virtual void unk_010() = 0;
        virtual void bind_material(material* value) = 0;
        virtual void release_material(material* value) = 0;
        virtual void rebase_material(material* value, u32 flags) = 0;
        virtual bool check_material_version(material* value) = 0;
        virtual bool check_vertex_definition_version(mesh_section* section) = 0;
        virtual void bind_section(mesh_section* section, mesh* owner) = 0;

        bool        disabled;
        u8          reserved_009[3];
        e_shader_id id;
    };

    struct shader_name {
        static string_hash get(const shader* value) {
            return value->get_name().hash;
        }
    };

    struct shader_bank : instance_bank<shader, shader_name> {};

    shader &get_default_shader();
    shader* find_shader(string_hash name);
    
    void initialize_shader_bank();

    namespace references {
        inline ::util::memory_reference<shader>      default_shader { 0x00F523E0 };
        inline ::util::memory_reference<shader_bank> shaders        { 0x011161E0 };
    } // references

    ASSERT_SIZEOF  (shader,           0x10);
    ASSERT_OFFSETOF(shader, disabled, 0x08);
    ASSERT_OFFSETOF(shader, id,       0x0C);

    ASSERT_SIZEOF(shader_bank, 0x10);
}} // treyarch::ngl
