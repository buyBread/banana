#pragma once

#include <d3d9.h>

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct vertex_definition {
              u32                          vertex_size;
        const D3DVERTEXELEMENT9*           elements;
              IDirect3DVertexDeclaration9* declaration;
    };

    ASSERT_SIZEOF  (vertex_definition,              0x0C);
    ASSERT_OFFSETOF(vertex_definition, vertex_size, 0x00);
    ASSERT_OFFSETOF(vertex_definition, elements,    0x04);
    ASSERT_OFFSETOF(vertex_definition, declaration, 0x08);

}} // treyarch::ngl
