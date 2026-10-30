#pragma once

#include "game/math/algebra/vector3d.hh"
#include "treyarch/mash/string_hash.hh"
#include "vc8/map.hh"
#include "vc8/set.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // script values filed by entity and key; the mission manager keeps a saved and a working one for checkpoints
    class script_registry {

    public:
        struct entry {
            string_hash                     entity;
            vc8::map<string_hash, f32>      rational_keys;
            vc8::map<string_hash, vector3d> vector3d_keys;

            // sub_8020C0
            entry() {}

            // sub_8021F0
            entry(const entry &other) : entity(other.entity),
                                        rational_keys(other.rational_keys),
                                        vector3d_keys(other.vector3d_keys) {}

            // sub_802020
            ~entry() {}
        };

        // takes both entries by value, so every comparison copies them
        struct entry_less {
            // sub_802360
            bool operator()(entry a, entry b) const {
                return a.entity < b.entity;
            }
        };

        vc8::set<entry, entry_less> entries;

        // sub_804D60
        script_registry() {
            clear();
        }

        // sub_804B80
        void clear() {
            entries.clear();
        }

        void insert_rational_key(string_hash the_entity, string_hash key, f32      default_value, f32*      value, bool set_value);
        void insert_vector3d_key(string_hash the_entity, string_hash key, vector3d default_value, vector3d* value, bool set_value);
        bool has_vector3d_key   (string_hash the_entity, string_hash key) const;
    };

    ASSERT_SIZEOF  (script_registry::entry,                0x1C);
    ASSERT_OFFSETOF(script_registry::entry, rational_keys, 0x04);
    ASSERT_OFFSETOF(script_registry::entry, vector3d_keys, 0x10);

    ASSERT_SIZEOF(script_registry, 0x0C);
} // treyarch
