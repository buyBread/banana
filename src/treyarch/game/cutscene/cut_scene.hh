#pragma once

#include "treyarch/nal/scene_anim.hh"
#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/avl_tree.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "treyarch/shared/math/algebra.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    namespace amalga {
        struct resource_pack_slot;
    } // amalga

    // an entity taking part in the cut scene; spawned from entity_class when no entity has its name
    class entity_class_entry {

    public:
        // an animation event raised on the entity when the cut scene reaches its frame
        class cutscene_animation_event : public mash::mash_virtual_base {

        public:
            string_hash event;
            f32         frame;         // 30/s
            u32         param_type;    // 1 an integer, 2 a float, 3 a string_hash, anything else none
            u32         param;
            bool        when_skipping; // also raised while the cut scene is being skipped
            u8          pad_015[0x03];

            virtual u32         get_param_type() const;
            virtual f32         get_param_float() const;
            virtual i32         get_param_int() const;
            virtual string_hash get_param_hash() const;
        };

        enum e_flags : u32 {
            keep_spawned = 0x08, // a spawned entity stays after the cut scene
            place_at_end = 0x10, // move the entity to end_position and end_rotation when the cut scene ends
            was_hidden   = 0x20  // the entity's 0x4000 flag before the cut scene, restored after it
        };

        string_hash  name;
        string_hash  entity_class;
        u32          flags;
        u32          unk_0c;
        u32          unk_10;
        string_hash  variant;
        i32          variant_values[8]; // given to the variant interface unless the first is -1
        vector3d     end_position;
        vector3d     end_rotation;      // in degrees
        u32          unk_50;
        mash::string morph_name;        // given to the morph interface

        mash::vector
            <cutscene_animation_event> events;

        arch_base_vhandle entity;
        bool              unk_78;
        u8                pad_079[0x03];

        bool operator<(const entity_class_entry &other) const {
            return name < other.name;
        }

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // an entity hookup_current_segment parents to the hero
    struct cut_scene_segment_attachment {
        mash::string name;
        mash::string unk_0c;
        u32          unk_18;
        u8           unk_1c[0x14];

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // one scene anim of the cut scene; the player plays the segments in order
    class cut_scene_segment {

    public:
        enum e_flags : u32 {
            skippable = 0x01,
            silent    = 0x20  // stops the cut scene sound instead of playing its own
        };

        u32              flags;
        i32              hour;       // sets the game clock to hour:00 unless negative
        u32*             unk_08;     // an optional 4-byte block
        nal::scene_anim* scene_anim; // looked up from name when constructed
        string_hash      name;

        mash::vector_basic
            <string_hash> regions; // brought in while the segment plays
        mash::vector
            <cut_scene_segment_attachment> attachments;

        string_hash sound;

        // inlined @ sub_805510
        // inlined @ sub_805CB0
        // inlined @ sub_805D20
        // inlined @ sub_805DB0
        f32 get_duration() const {
            return scene_anim ? scene_anim->duration : 0.0f;
        }

        // sub_75D8E0
        string_hash get_sound() const {
            return sound;
        }

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // fade out to color, hold, then fade back in
    struct cut_scene_fade {
        f32 fade_out_time;
        f32 hold_time;
        f32 fade_in_time;
        u32 color;
    };

    class cut_scene {

    public:
        enum e_flags : u32 {
            skippable            = 0x0001,
            hero_unaffected      = 0x0002, // play leaves the hero and the controls alone
            no_fades             = 0x0008,
            takes_igo            = 0x0080, // play hides the radar and disables the interface until the end
            play_once            = 0x0200, // ignores play_count
            end_on_black         = 0x1000, // the end fade doesn't fade back in
            keep_render_distance = 0x8000  // the render distance stays 250 instead of going out to 2500
        };

        void* self; // the resource handler points this at the cut scene itself

        mash::avl_tree
            <entity_class_entry> entity_class_entries;
        mash::vector
            <cut_scene_segment> segments;

        u32                         flags;
        f32                         unk_2c;
        mash::string                unk_30;
        mash::string                name; // create_cut_scene sets it
        mash::string                unk_48;
        u32                         play_count;
        i32                         unk_58; // play clamps it to -1..10
        f32                         unk_5c; // -1..20
        f32                         unk_60; // -1..1
        u32                         unk_64;
        f32                         unk_68;
        u32                         unk_6c;
        amalga::resource_pack_slot* resource_context;
        cut_scene_fade              start_fade;
        cut_scene_fade              end_fade;
        entity_class_entry          hero_entry;        // its events are raised on the hero
        u8                          unk_110[0x02];
        bool                        use_focus;         // use focus over the camera's when focus_from_camera is clear
        u8                          unk_113;
        f32                         focus[3];
        bool                        focus_from_camera;
        bool                        wait_for_regions;  // hold the first segment until the regions are in
        bool                        black_suit;        // play puts the hero in the black suit
        bool                        red_suit;          // or the red one; with neither the suit stays
        f32                         near_plane;

        // sub_805000
        mash::string get_name() const {
            return name;
        }

        bool has_sound() const;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (entity_class_entry::cutscene_animation_event,                0x18);
    ASSERT_OFFSETOF(entity_class_entry::cutscene_animation_event, event,         0x04);
    ASSERT_OFFSETOF(entity_class_entry::cutscene_animation_event, when_skipping, 0x14);

    ASSERT_SIZEOF  (entity_class_entry,               0x7C);
    ASSERT_OFFSETOF(entity_class_entry, variant,      0x14);
    ASSERT_OFFSETOF(entity_class_entry, end_position, 0x38);
    ASSERT_OFFSETOF(entity_class_entry, morph_name,   0x54);
    ASSERT_OFFSETOF(entity_class_entry, events,       0x60);
    ASSERT_OFFSETOF(entity_class_entry, entity,       0x74);
    ASSERT_OFFSETOF(entity_class_entry, unk_78,       0x78);

    ASSERT_SIZEOF  (cut_scene_segment_attachment,         0x30);
    ASSERT_OFFSETOF(cut_scene_segment_attachment, unk_18, 0x18);

    ASSERT_SIZEOF  (cut_scene_segment,              0x3C);
    ASSERT_OFFSETOF(cut_scene_segment, scene_anim,  0x0C);
    ASSERT_OFFSETOF(cut_scene_segment, regions,     0x14);
    ASSERT_OFFSETOF(cut_scene_segment, attachments, 0x24);
    ASSERT_OFFSETOF(cut_scene_segment, sound,       0x38);

    ASSERT_SIZEOF  (cut_scene_fade,       0x10);
    ASSERT_OFFSETOF(cut_scene_fade, color, 0x0C);

    ASSERT_SIZEOF  (cut_scene,                       0x128);
    ASSERT_OFFSETOF(cut_scene, entity_class_entries, 0x004);
    ASSERT_OFFSETOF(cut_scene, segments,             0x014);
    ASSERT_OFFSETOF(cut_scene, flags,                0x028);
    ASSERT_OFFSETOF(cut_scene, name,                 0x03C);
    ASSERT_OFFSETOF(cut_scene, play_count,           0x054);
    ASSERT_OFFSETOF(cut_scene, resource_context,     0x070);
    ASSERT_OFFSETOF(cut_scene, start_fade,           0x074);
    ASSERT_OFFSETOF(cut_scene, end_fade,             0x084);
    ASSERT_OFFSETOF(cut_scene, hero_entry,           0x094);
    ASSERT_OFFSETOF(cut_scene, use_focus,            0x112);
    ASSERT_OFFSETOF(cut_scene, focus,                0x114);
    ASSERT_OFFSETOF(cut_scene, focus_from_camera,    0x120);
    ASSERT_OFFSETOF(cut_scene, black_suit,           0x122);
    ASSERT_OFFSETOF(cut_scene, red_suit,             0x123);
    ASSERT_OFFSETOF(cut_scene, near_plane,           0x124);
} // treyarch
