#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "treyarch/shared/math/algebra/vector3d.hh"
#include "treyarch/shared/mutex.hh"
#include "treyarch/shared/variance_variable.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // what a parameter holds; the typed lookups ask for one
    enum e_param_type : u32 {
        param_type_float          = 0,
        param_type_int            = 1,
        param_type_hash           = 2,
        param_type_vector3d       = 3,
        param_type_float_variance = 4,
        param_type_string         = 5,
        param_type_class          = 6
    };

    // the base of every typed parameter; mashed ones get their retail vtable from the type registry
    class param_base : public mash::mash_virtual_base {

    public:
        e_param_type type;
        u8           unk_08[0x04];
    };

    // the lookups into it stay native for now; each takes the lock
    class alignas(8) param_block {

    public:
        mash::vector
            <param_base>*     params;
        u32                   reserved_004;
        engine_recursive_lock lock;
        u32                   unk_018;
        bool                  owns_params; // the list was allocated on the heap rather than unmashed
        u8                    pad_01d[0x03];

        ~param_block();

        bool         does_parameter_exist   (string_hash parameter) const;
        e_param_type get_parameter_data_type(string_hash parameter) const;

        // a missing parameter reads as zero
        f32                      get_pb_float         (string_hash parameter) const;
        i32                      get_pb_int           (string_hash parameter) const;
        string_hash              get_pb_hash          (string_hash parameter) const;
        vector3d                 get_pb_vector3d      (string_hash parameter) const;
        variance_variable<f32>   get_pb_float_variance(string_hash parameter) const;
        const char*              get_pb_fixedstring   (string_hash parameter) const;
        mash::mash_virtual_base* get_pb_class         (string_hash parameter) const;

        f32      get_optional_pb_float   (string_hash parameter, f32 default_value, bool* exists) const;
        i32      get_optional_pb_int     (string_hash parameter, i32 default_value, bool* exists) const;
        vector3d get_optional_pb_vector3d(string_hash parameter, const vector3d &default_value, bool* exists) const;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (param_base,       0x0C);
    ASSERT_OFFSETOF(param_base, type, 0x04);

    ASSERT_SIZEOF  (param_block,              0x20);
    ASSERT_OFFSETOF(param_block, params,      0x00);
    ASSERT_OFFSETOF(param_block, lock,        0x08);
    ASSERT_OFFSETOF(param_block, unk_018,     0x18);
    ASSERT_OFFSETOF(param_block, owns_params, 0x1C);
} // treyarch
