#pragma once

#include <d3d9.h>

#include "treyarch/ngl/instance_bank/instance_bank.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct vertex_definition {
              u32                          vertex_size;
        const D3DVERTEXELEMENT9*           elements;
              IDirect3DVertexDeclaration9* declaration;
    };

    struct vertex_definition_bank_entry {
        virtual ~vertex_definition_bank_entry() = default;
        virtual void register_vertex_definition() = 0;
        virtual const fixed_string &get_name() const = 0;
        virtual void unk_00c() = 0;

        void* initialization_next;
    };

    struct vertex_definition_bank_entry_name {
        static string_hash get(const vertex_definition_bank_entry* value) {
            return value->get_name().hash;
        }
    };

    struct vertex_definition_bank : instance_bank<vertex_definition_bank_entry,
                                                  vertex_definition_bank_entry_name> {};

    void initialize_vertex_definition_bank();

    namespace references {
        inline util::memory_reference<vertex_definition_bank> vertex_definitions { 0x01116200 };
    } // references

    ASSERT_SIZEOF  (vertex_definition,              0x0C);
    ASSERT_OFFSETOF(vertex_definition, vertex_size, 0x00);
    ASSERT_OFFSETOF(vertex_definition, elements,    0x04);
    ASSERT_OFFSETOF(vertex_definition, declaration, 0x08);

    ASSERT_SIZEOF(vertex_definition_bank_entry, 0x08);
    ASSERT_SIZEOF(vertex_definition_bank,       0x10);
}} // treyarch::ngl
