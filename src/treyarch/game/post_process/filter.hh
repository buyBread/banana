#pragma once

#include <d3d9.h>

#include "treyarch/game/post_process/texture_array.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    struct blitter;

namespace post_process {
    // RTTI Filter
    struct filter {
        void*                  vtable;
        treyarch::blitter*     blitter;
        IDirect3DPixelShader9* program;
        void*                  setup; // called before the blitter draw; null for the down-sample filter

        filter(treyarch::blitter* owner);

        void destroy();
    };

    // RTTI DownSampleFilter
    struct down_sample_filter : filter {
        down_sample_filter(treyarch::blitter* owner);
    };

    struct filter_queue_node {
        post_process::filter* filter;
        texture_array         sources;
        texture_array         targets;
        void*                 parameters;
        filter_queue_node*    next;

        ~filter_queue_node();
    };

    struct filter_queue {
        filter_queue_node* head;
        filter_queue_node* tail;
        u32                count;
        filter_queue_node* free_nodes;

        void reset();
        void clear();
    };

    namespace references {
        inline util::memory_reference<void*> filter_vtable             { 0x00BCA084 };
        inline util::memory_reference<void*> down_sample_filter_vtable { 0x00BCA090 };
    } // references

    ASSERT_SIZEOF  (filter,          0x10);
    ASSERT_OFFSETOF(filter, blitter, 0x04);
    ASSERT_OFFSETOF(filter, program, 0x08);
    ASSERT_OFFSETOF(filter, setup,   0x0C);

    ASSERT_SIZEOF  (filter_queue_node,             0x61C);
    ASSERT_OFFSETOF(filter_queue_node, sources,    0x004);
    ASSERT_OFFSETOF(filter_queue_node, targets,    0x30C);
    ASSERT_OFFSETOF(filter_queue_node, parameters, 0x614);
    ASSERT_OFFSETOF(filter_queue_node, next,       0x618);

    ASSERT_SIZEOF  (filter_queue,             0x10);
    ASSERT_OFFSETOF(filter_queue, free_nodes, 0x0C);
}} // treyarch::post_process
