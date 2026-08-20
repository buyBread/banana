#pragma once

#include "treyarch/ngl/instance_bank/instance_bank.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct material;
    struct mesh;
    struct mesh_node;
    struct mesh_section;

    struct shader {
        virtual ~shader() = default;
        virtual void register_shader() = 0;
        virtual const fixed_string& get_name() const = 0;
        virtual void add_node(mesh_node* node, mesh_section* section, material* value) = 0;
        virtual void unk_010() = 0;
        virtual void bind_material(material* value) = 0;
        virtual void release_material(material* value) = 0;
        virtual void rebase_material(material* value, u32 flags) = 0;
        virtual bool check_material_version(material* value) = 0;
        virtual bool check_vertex_definition_version(mesh_section* section) = 0;
        virtual void bind_section(mesh_section* section, mesh* owner) = 0;

        void* initialization_next;
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
        inline util::memory_reference<shader>      default_shader { 0x00F523E0 };
        inline util::memory_reference<shader_bank> shaders        { 0x011161E0 };
    } // references

    ASSERT_SIZEOF(shader,      0x08);
    ASSERT_SIZEOF(shader_bank, 0x10);
}} // treyarch::ngl
