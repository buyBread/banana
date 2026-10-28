#pragma once

#include "vc8/map.hh"
#include "game/frontend/igo/igo_3d_gta_widget.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_map_poi.hh"
#include "game/frontend/igo/poi_icon_widget.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/slot_pool.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class city_life_tracker;
    class entity_tracker_manager;

    // the stealth meter over a tracked entity
    class stealth_widget_t {

    public:
        void**           vtable;
        u32              state;          // the +0x0C drawable shows in states 1 and 2
        igo_3d_drawable* meter;
        igo_3d_drawable* unk_0c;
        u8               reserved_010[0x0C];
        bool             active;
        u8               reserved_01d;
        bool             activated;

        void draw();
    };

    // an FX_CYL_POI cylinder spawned around the tracked entity
    class entity_fog_obj {

    public:
        arch_base_vhandle fog_vh;
        arch_base_vhandle entity;
        bool              active;
        bool              requires_senses;
    };

    class entity_tracker {

    public:
        arch_base_vhandle       ent;
        entity_tracker_manager* parent;
        igo_map_poi*            mm_icon;
        vector3d                poi_icon_offset;
        poi_icon_widget*        poi_icon;
        bool                    poi_show_distance_info;
        vector3d                stealth_widget_offset;
        stealth_widget_t*       stealth_widget;
        bool                    shows_in_spidey_sense;
        bool                    shows_out_of_spidey_sense;
        entity_fog_obj*         fog_obj;
        igo_3d_gta_widget*      gta_widget;
        bool                    gta_widget_active;
        bool                    unk_41;                     // while clear, set_poi_active drives the gta widget too
        f32                     gta_inner_ring_radius;
        f32                     gta_outer_ring_radius;
        bool                    always_draw_up_down;
        bool                    urgent;

        entity_tracker(entity_tracker_manager* parent, arch_base_vhandle ent);
        virtual ~entity_tracker();

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }
    };

    using entity_tracker_id        = u32;
    using entity_tracker_slot_pool = slot_pool<entity_tracker*, entity_tracker_id>;
    using entity_tracker_map       = vc8::map<arch_base_vhandle, entity_tracker_id>;

    class entity_tracker_manager {

    public:
        entity_tracker_map                tracker_map;
        u8                                reserved_00c[0x04];
        entity_tracker_slot_pool          tracker_slot_pool;
        u32                               unk_60;
        entity_tracker_id                 hero_tracker_id;           // -1 for none
        vc8::map<i32, city_life_tracker*> city_life_trackers;
        i32                               next_city_life_tracker_id;
        ngl::quad*                        quad;
        bool                              poi_urgent;                // while set, only urgent trackers draw their poi icon

        void render();

        entity_tracker*   id_to_ptr(entity_tracker_id id);
        entity_tracker_id get_tracker_by_entity(arch_base_vhandle ent);

        entity_tracker_id create_entity_tracker(arch_base_vhandle ent, bool* ok);
        void              destroy_entity_tracker(entity_tracker_id id);

        void set_hero_tracker(arch_base_vhandle hero);
    };

    ASSERT_OFFSETOF(stealth_widget_t, meter,     0x08);
    ASSERT_OFFSETOF(stealth_widget_t, active,    0x1C);
    ASSERT_OFFSETOF(stealth_widget_t, activated, 0x1E);

    ASSERT_SIZEOF  (entity_fog_obj,                  0x0C);
    ASSERT_OFFSETOF(entity_fog_obj, entity,          0x04);
    ASSERT_OFFSETOF(entity_fog_obj, active,          0x08);
    ASSERT_OFFSETOF(entity_fog_obj, requires_senses, 0x09);

    ASSERT_SIZEOF  (entity_tracker,                            0x50);
    ASSERT_OFFSETOF(entity_tracker, ent,                       0x04);
    ASSERT_OFFSETOF(entity_tracker, mm_icon,                   0x0C);
    ASSERT_OFFSETOF(entity_tracker, poi_icon_offset,           0x10);
    ASSERT_OFFSETOF(entity_tracker, poi_icon,                  0x1C);
    ASSERT_OFFSETOF(entity_tracker, poi_show_distance_info,    0x20);
    ASSERT_OFFSETOF(entity_tracker, stealth_widget_offset,     0x24);
    ASSERT_OFFSETOF(entity_tracker, stealth_widget,            0x30);
    ASSERT_OFFSETOF(entity_tracker, shows_in_spidey_sense,     0x34);
    ASSERT_OFFSETOF(entity_tracker, shows_out_of_spidey_sense, 0x35);
    ASSERT_OFFSETOF(entity_tracker, fog_obj,                   0x38);
    ASSERT_OFFSETOF(entity_tracker, gta_widget,                0x3C);
    ASSERT_OFFSETOF(entity_tracker, gta_widget_active,         0x40);
    ASSERT_OFFSETOF(entity_tracker, unk_41,                    0x41);
    ASSERT_OFFSETOF(entity_tracker, gta_inner_ring_radius,     0x44);
    ASSERT_OFFSETOF(entity_tracker, gta_outer_ring_radius,     0x48);
    ASSERT_OFFSETOF(entity_tracker, always_draw_up_down,       0x4C);
    ASSERT_OFFSETOF(entity_tracker, urgent,                    0x4D);

    ASSERT_SIZEOF  (entity_tracker_manager,                            0x80);
    ASSERT_OFFSETOF(entity_tracker_manager, tracker_slot_pool,         0x10);
    ASSERT_OFFSETOF(entity_tracker_manager, hero_tracker_id,           0x64);
    ASSERT_OFFSETOF(entity_tracker_manager, city_life_trackers,        0x68);
    ASSERT_OFFSETOF(entity_tracker_manager, next_city_life_tracker_id, 0x74);
    ASSERT_OFFSETOF(entity_tracker_manager, quad,                      0x78);
    ASSERT_OFFSETOF(entity_tracker_manager, poi_urgent,                0x7C);
} // treyarch
