#pragma once

#include "game/mutex.hh"
#include "vc8/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace amalga {
        struct resource_pack_slot;
    } // amalga

    enum e_scene_flags : u32 {
        scene_flag_active             = 0x00000001,
        scene_flag_stationary         = 0x00000002,
        scene_flag_invisible          = 0x00000004,
        scene_flag_walkable           = 0x00000010,
        scene_flag_never_moves        = 0x00000020,
        scene_flag_beamable           = 0x00000200,
        scene_flag_no_beamable        = 0x00000400,
        scene_flag_lego               = 0x00000800,
        scene_flag_no_lego            = 0x00001000,
        scene_flag_camera_coll        = 0x00002000,
        scene_flag_no_camera_coll     = 0x00004000,
        scene_flag_entity_coll        = 0x00008000,
        scene_flag_no_entity_coll     = 0x00010000,
        scene_flag_prop_physics       = 0x00020000,
        scene_flag_no_prop_physics    = 0x00040000,
        scene_flag_action_facing      = 0x00080000,
        scene_flag_no_action_facing   = 0x00100000,
        scene_flag_is_door            = 0x00200000,
        scene_flag_no_is_door         = 0x00400000,
        scene_flag_door_open          = 0x00800000,
        scene_flag_door_closed        = 0x01000000,
        scene_flag_small_crawl        = 0x02000000,
        scene_flag_non_small_crawl    = 0x04000000,
        scene_flag_non_crawlable      = 0x08000000,
        scene_flag_crawlable          = 0x10000000,
        scene_flag_do_not_inherit     = 0x20000000,
        scene_flag_camera_see_thru    = 0x40000000,
        scene_flag_no_camera_see_thru = 0x80000000
    };

    class wds_entity_manager {

    public:
        engine_recursive_lock              lock;
        u8                                 unk_10[0x0C]; // slots' entity instance lists are allocated from here
        u8                                 reserved_01c[0x14];
        vc8::vector
            <amalga::resource_pack_slot*>* unk_30;       // the slots pushed as resource context
        u8                                 reserved_034[0x04];

        wds_entity_manager();
       ~wds_entity_manager();

        // sub_7FBDE0
        amalga::resource_pack_slot* get_current_resource_context() const {
            return unk_30->size() ? unk_30->back() : nullptr;
        }
    };

    ASSERT_SIZEOF  (wds_entity_manager,         0x38);
    ASSERT_OFFSETOF(wds_entity_manager, unk_10, 0x10);
    ASSERT_OFFSETOF(wds_entity_manager, unk_30, 0x30);
} // treyarch
