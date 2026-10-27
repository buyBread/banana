#pragma once

#include "game/fixed_string.hh"
#include "game/four_cc.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace nal {
    constexpr u32 transport_list_type    = four_cc('A', 'S', 'K', 'L');
    constexpr u32 transport_list_version = 0x20117;

    struct pose_layout;

    // nalTransport, only id 0 (fk) ships
    struct transport {
        u32        id;
        transport* next;
    };

    // ASKL
    struct transport_list {
        fixed_string      name;
        nal::transport*   transports;
        nal::pose_layout* pose_layout;
        u32               unk_10;
        u32               revision;
        u8                pad_18[8];

        // inlined @ sub_9D1860
        // inlined @ sub_9CECB0
        bool has_transport(u32 id) {
            for (nal::transport* value = transports; value; value = value->next) {
                if (value->id == id)
                    return true;
            }

            return false;
        }

        nal::transport* get_transport(u32 id);
    };

    ASSERT_SIZEOF  (transport,       0x08);
    ASSERT_OFFSETOF(transport, id,   0x00);
    ASSERT_OFFSETOF(transport, next, 0x04);

    ASSERT_SIZEOF  (transport_list,              0x20);
    ASSERT_OFFSETOF(transport_list, name,        0x00);
    ASSERT_OFFSETOF(transport_list, transports,  0x08);
    ASSERT_OFFSETOF(transport_list, pose_layout, 0x0C);
    ASSERT_OFFSETOF(transport_list, unk_10,      0x10);
    ASSERT_OFFSETOF(transport_list, revision,    0x14);
}} // treyarch::nal
