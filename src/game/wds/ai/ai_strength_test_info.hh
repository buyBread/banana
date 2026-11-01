#pragma once

#include "game/arch_base_vhandle.hh"
#include "game/boolx.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // SM3 strength_test_info; retail adds a byte after the category name
    class strength_test_info {

    public:
        boolx             m_strength_test_event_raised;
        u8                pad_01[0x03];
        arch_base_vhandle m_object;
        string_hash       m_spidey_category_name;
        bool              unk_0c;
        u8                reserved_0d[0x13];

        bool should_do_strength_test() const {
            return m_strength_test_event_raised;
        }

        // sub_5FD110
        string_hash get_category_name() const {
            return m_spidey_category_name;
        }
    };

    ASSERT_SIZEOF  (strength_test_info,                         0x20);
    ASSERT_OFFSETOF(strength_test_info, m_object,               0x04);
    ASSERT_OFFSETOF(strength_test_info, m_spidey_category_name, 0x08);
    ASSERT_OFFSETOF(strength_test_info, unk_0c,                 0x0C);
} // treyarch
