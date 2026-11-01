#pragma once

#include "vc8/vector.hh"
#include "game/arch_base_vhandle.hh"
#include "game/frontend/igo/igo_generic_decay_timer.hh"
#include "game/frontend/igo/igo_generic_input_listener.hh"
#include "game/math/algebra/vector3d.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace references {
        inline util::memory_reference<string_hash> zoom_map_poi_selected   { 0x0102C3E4 };
        inline util::memory_reference<string_hash> zoom_map_poi_unselected { 0x0102CC60 };
    } // references

    enum e_zoom_map_poi_kind : i32 {
        zoom_map_poi_safe_zone_marker = 32,
        zoom_map_poi_free             = 56
    };

    // one of the zoom map's poi records
    struct zoom_map_poi {
        // SM3's ZEROVEC; every file kept its own copy
        inline static const vector3d zerovec = vector3d(0.0f, 0.0f, 0.0f);

        arch_base_vhandle ent;
        vector3d          position;
        u8                reserved_10[0x08];
        i32               kind;
        u32               unk_1c;
        bool              is_shown;
        u8                pad_21[0x03];
        i32               list_index;     // its place in the zoom map's poi_list
        i32               index;          // its own place in the zoom map's pois
        i32               unk_2c;
        bool              tracks_entity;
        u8                pad_31[0x03];
        mash::string      meta_title;
        mash::string      meta_text;
        i32               marker_state;

        void set_default();
        void set(vector3d          pos,     i32 poi_kind);
        void set(arch_base_vhandle tracked, i32 poi_kind); // at the zero vector

        void set_meta_title(mash::string* title); // finalizes the title
        void set_meta_text (mash::string* text);  // finalizes the text
    };

    class igo_3d_zoom_map : public igo_generic_input_listener {

    public:
        u8                      reserved_00c;
        bool                    is_open;
        u8                      reserved_00e[0xC6];
        zoom_map_poi*           current_poi;
        u8                      reserved_0d8[0x28];
        zoom_map_poi            pois[0x1C2];
        vc8::vector
            <zoom_map_poi*>     poi_list;                  // the records in use
        i32                     next_free_search;          // claim_poi starts its search here
        u8                      reserved_8db4[0x1DC];
        bool                    is_zooming;
        u8                      reserved_8f91;
        bool                    unk_8f92;                  // snap_to_point sets it
        u8                      reserved_8f93[0x02];
        bool                    disabled;
        bool                    movement_disabled;
        u8                      reserved_8f97;
        bool                    brought_up_through_script;
        u8                      pad_8f99[0x03];
        igo_generic_decay_timer snap_timer;
        igo_generic_decay_timer unk_8fb8;                  // the select starts it
        bool                    poi_selected;
        u8                      reserved_8fd5[0x10F];
        vector3d                unk_90e4;                  // the update stores the camera position here once (flag +0x90F4)
        u8                      reserved_90f0[0x08];
        vector3d                unk_90f8;
        f32                     unk_9104;
        f32                     unk_9108;
        u8                      reserved_910c[0x10];
        vector3d                unk_911c;
        u8                      reserved_9128[0x20];
        i32                     selected_poi;              // -1 for none
        u8                      reserved_914c[0x24];

        virtual i32  unk_024() = 0;
        virtual void unk_025(f32 unk) = 0;
        virtual i32  unk_026(f32 unk) = 0;
        virtual void draw_map_overlay() = 0;
        virtual void unk_028() = 0;
        virtual u8   unk_029() = 0;
        virtual i32  unk_030() = 0;
        virtual void set_help_text(mash::string text) = 0;
        virtual void unk_032() = 0;
        virtual i32  unk_033() = 0;
        virtual void unk_034() = 0;
        virtual void unk_035() = 0;
        virtual void unk_036() = 0;
        virtual void unk_037(i32 unk) = 0;

        void set_help_text(const char* text) {
            set_help_text(mash::string(text));
        }

        bool is_open_or_zooming() const {
            return is_open || is_zooming;
        }

        i32  get_selected_poi();
        void unselect_poi(); // raises ZOOM_MAP_POI_UNSELECTED when one was selected

        void feature_current_poi(bool feature); // nothing while closed, zooming or selecting

        bool is_poi_in_use     (u32 index);
        void set_poi_shown     (u32 index, bool shown);
        void set_marker_state  (u32 index, i32 state);
        void set_poi_meta_title(u32 index, mash::string* title); // finalizes the title
        void set_poi_meta_text (u32 index, mash::string* text);  // finalizes the text

        i32 claim_poi(); // -1 when every record is in use

        i32 add_poi             (vector3d          pos, i32 kind, i32 unk); // -1 when full
        i32 add_safe_zone_marker(vector3d          pos);
        i32 add_safe_zone_marker(arch_base_vhandle tracked);

        void remove_poi(u32 index); // the last listed record takes its place in poi_list

        void snap_to_point(f32 x, f32 z);
    };

    ASSERT_SIZEOF  (zoom_map_poi,                0x50);
    ASSERT_OFFSETOF(zoom_map_poi, position,      0x04);
    ASSERT_OFFSETOF(zoom_map_poi, kind,          0x18);
    ASSERT_OFFSETOF(zoom_map_poi, is_shown,      0x20);
    ASSERT_OFFSETOF(zoom_map_poi, list_index,    0x24);
    ASSERT_OFFSETOF(zoom_map_poi, tracks_entity, 0x30);
    ASSERT_OFFSETOF(zoom_map_poi, meta_title,    0x34);
    ASSERT_OFFSETOF(zoom_map_poi, meta_text,     0x40);
    ASSERT_OFFSETOF(zoom_map_poi, marker_state,  0x4C);

    ASSERT_SIZEOF  (igo_3d_zoom_map,                            0x9170);
    ASSERT_OFFSETOF(igo_3d_zoom_map, is_open,                   0x000D);
    ASSERT_OFFSETOF(igo_3d_zoom_map, current_poi,               0x00D4);
    ASSERT_OFFSETOF(igo_3d_zoom_map, pois,                      0x0100);
    ASSERT_OFFSETOF(igo_3d_zoom_map, poi_list,                  0x8DA0);
    ASSERT_OFFSETOF(igo_3d_zoom_map, next_free_search,          0x8DB0);
    ASSERT_OFFSETOF(igo_3d_zoom_map, is_zooming,                0x8F90);
    ASSERT_OFFSETOF(igo_3d_zoom_map, unk_8f92,                  0x8F92);
    ASSERT_OFFSETOF(igo_3d_zoom_map, disabled,                  0x8F95);
    ASSERT_OFFSETOF(igo_3d_zoom_map, movement_disabled,         0x8F96);
    ASSERT_OFFSETOF(igo_3d_zoom_map, brought_up_through_script, 0x8F98);
    ASSERT_OFFSETOF(igo_3d_zoom_map, snap_timer,                0x8F9C);
    ASSERT_OFFSETOF(igo_3d_zoom_map, unk_8fb8,                  0x8FB8);
    ASSERT_OFFSETOF(igo_3d_zoom_map, poi_selected,              0x8FD4);
    ASSERT_OFFSETOF(igo_3d_zoom_map, unk_90e4,                  0x90E4);
    ASSERT_OFFSETOF(igo_3d_zoom_map, unk_90f8,                  0x90F8);
    ASSERT_OFFSETOF(igo_3d_zoom_map, unk_9104,                  0x9104);
    ASSERT_OFFSETOF(igo_3d_zoom_map, unk_911c,                  0x911C);
    ASSERT_OFFSETOF(igo_3d_zoom_map, selected_poi,              0x9148);
} // treyarch
