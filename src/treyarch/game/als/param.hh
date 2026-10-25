#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/math/algebra/vector3d.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace als {
    using param_id = u32;

    // ids below 161 are set by the game, the rest are computed by the als itself
    enum external_float_parameter_types  : u32 { num_external_float_params  = 141 };
    enum external_vector_parameter_types : u32 { num_external_vector_params = 156 };
    enum external_hash_parameter_types   : u32 { num_external_hash_params   = 161 };

    // what a request carries to the state machine
    struct parameter_list {
        vector3d    vector_params[15]; // ids 141..155
        string_hash hash_params[5];    // ids 156..160
        f32         float_params[141]; // ids 0..140
        bool        vector_set[15];
        bool        hash_set[5];
        bool        float_set[141];
        u8          pad_39d[0x03];

        parameter_list();
    };

    // the computed parameters, filled the first time a frame asks for them
    struct internal_parameter_cache {
        vector3d    vectors[6];
        string_hash hashes[14];
        f32         floats[94];
        bool        vector_set[6];
        bool        hash_set[14];
        bool        float_set[94];
        u8          pad_26a[0x02];

        internal_parameter_cache();

        void invalidate_per_layer();
    };

    ASSERT_SIZEOF  (parameter_list,               0x3A0);
    ASSERT_OFFSETOF(parameter_list, hash_params,  0x0B4);
    ASSERT_OFFSETOF(parameter_list, float_params, 0x0C8);
    ASSERT_OFFSETOF(parameter_list, vector_set,   0x2FC);
    ASSERT_OFFSETOF(parameter_list, hash_set,     0x30B);
    ASSERT_OFFSETOF(parameter_list, float_set,    0x310);

    ASSERT_SIZEOF  (internal_parameter_cache,             0x26C);
    ASSERT_OFFSETOF(internal_parameter_cache, hashes,     0x048);
    ASSERT_OFFSETOF(internal_parameter_cache, floats,     0x080);
    ASSERT_OFFSETOF(internal_parameter_cache, vector_set, 0x1F8);
    ASSERT_OFFSETOF(internal_parameter_cache, hash_set,   0x1FE);
    ASSERT_OFFSETOF(internal_parameter_cache, float_set,  0x20C);
}} // treyarch::als
