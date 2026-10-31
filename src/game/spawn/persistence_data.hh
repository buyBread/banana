#pragma once

#include "vc8/map.hh"
#include "vc8/vector.hh"
#include "treyarch/mash/string.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // a script's saved value; the records are only ever made by retail, so these never run their own vtable
    class persistence_data_base {

    public:
        // sub_71F870
        virtual bool is_array() const { return false; }

        // sub_71F870
        virtual bool is_vector3d() const { return false; }

        // sub_71F870
        virtual bool is_str() const { return false; }

        // sub_71F870
        virtual bool is_num() const { return false; }
    };

    class persistence_num : public persistence_data_base {

    public:
        f32 value;
    };

    class persistence_str : public persistence_data_base {

    public:
        mash::string value;
    };

    class persistence_vector3d : public persistence_data_base {

    public:
        vector3d value;
    };

    class persistence_num_array : public persistence_data_base {

    public:
        vc8::vector<f32> values;
    };

    class persistence_str_array : public persistence_data_base {

    public:
        vc8::vector<mash::string> values;
    };

    class persistence_vector3d_array : public persistence_data_base {

    public:
        vc8::vector<vector3d> values;
    };

    // one script instance's saved values, by the name each was saved under
    struct persistence_data {
        f32                                             unk_00; // start_persistence_data_save's number
        vc8::map<mash::string, persistence_data_base*> records;
    };

    ASSERT_SIZEOF  (persistence_num,        0x08);
    ASSERT_OFFSETOF(persistence_num, value, 0x04);

    ASSERT_SIZEOF  (persistence_str,        0x10);
    ASSERT_OFFSETOF(persistence_str, value, 0x04);

    ASSERT_SIZEOF  (persistence_vector3d,        0x10);
    ASSERT_OFFSETOF(persistence_vector3d, value, 0x04);

    ASSERT_SIZEOF  (persistence_num_array,         0x14);
    ASSERT_OFFSETOF(persistence_num_array, values, 0x04);

    ASSERT_SIZEOF  (persistence_str_array,         0x14);
    ASSERT_OFFSETOF(persistence_str_array, values, 0x04);

    ASSERT_SIZEOF  (persistence_vector3d_array,         0x14);
    ASSERT_OFFSETOF(persistence_vector3d_array, values, 0x04);

    ASSERT_SIZEOF  (persistence_data,          0x10);
    ASSERT_OFFSETOF(persistence_data, records, 0x04);
} // treyarch
