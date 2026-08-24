#pragma once

#include "treyarch/ngl/init_list/init_list.hh"
#include "treyarch/ngl/instance_bank/instance_bank.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct material;
    struct mesh_node;
    struct mesh_section;

    struct geometry_shader : init_list {
        virtual ~geometry_shader() = default;

        virtual void register_item() override;
        
        virtual const fixed_string &get_name() const = 0;
        virtual void add_node(mesh_node* node, mesh_section* section, material* value) = 0;

        u32 id;
    };

    struct morph_geometry_shader : geometry_shader {
        virtual ~morph_geometry_shader() = default;

        virtual const fixed_string &get_name() const override;
        virtual void add_node(mesh_node* node, mesh_section* section, material* value) override = 0;
    };

    struct geometry_shader_name {
        static string_hash get(const geometry_shader* value);
    };

    struct geometry_shader_bank : instance_bank<geometry_shader, geometry_shader_name> {};

    void initialize_geometry_shader_bank();

    namespace references {
        inline util::memory_reference<geometry_shader_bank>  geometry_shaders                 { 0x01116200 };
        inline util::memory_reference<u32>                   next_geometry_shader_id          { 0x011188D4 };
        inline util::memory_reference<morph_geometry_shader> morph_geometry_shader_instance   { 0x00F52F50 };
        inline util::memory_reference<fixed_string>          morph_geometry_shader_name       { 0x01118620 };
        inline util::memory_reference<u32>                   morph_geometry_shader_name_guard { 0x01118628 };
    } // references

    ASSERT_SIZEOF  (geometry_shader,     0x0C);
    ASSERT_OFFSETOF(geometry_shader, id, 0x08);

    ASSERT_SIZEOF(geometry_shader_bank, 0x10);

    ASSERT_SIZEOF(morph_geometry_shader, 0x0C);
}} // treyarch::ngl
