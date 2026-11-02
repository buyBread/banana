#pragma once

#include "vc8/vector.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/math/algebra.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class finger_of_god;
    class igo_map_poi;

    class igo_3d_radar_widget : public igo_3d_widget {

    public:
        vc8::vector<igo_map_poi*>   icons;
        vc8::vector<igo_map_poi*>   kind_15_icons;
        vc8::vector<finger_of_god*> fingers_of_god;
        igo_3d_widget*              default_poi;        // "radar_poi_default"
        igo_3d_widget*              hero_poi;
        igo_3d_widget*              city_life_poi;
        igo_3d_widget*              height_line;
        igo_3d_widget*              range_info;
        igo_3d_widget*              ring_poi;
        igo_3d_widget*              city_life_ring_poi;
        u8                          reserved_19c[0x0C];
        i32                         north_arrow_member; // "NORTHARROW"
        i32                         vector_path_member; // "VECTOR_PATH"
        u8                          reserved_1b0[0x10];
        f32                         locked_zoom;
        u8                          reserved_1c4[0x04];
        vector3d                    locked_center;
        bool                        minimap_locked;
        bool                        buildings_invisible;
        bool                        wallmode_disabled;
        u8                          reserved_1d7[0x19];

        explicit igo_3d_radar_widget(mash::generating_vtable_version so_do_nothing) : igo_3d_widget(so_do_nothing) {}

        ~igo_3d_radar_widget() override;

        // writes the source vptr over a radar the retail constructor built
        void stamp_source_vtable() {
            *(mash::vtable_value_t*)this = mash::generate_vtable<igo_3d_radar_widget>();
        }

        void init() override;
        void update(f32 time_inc) override;
        void render() override;
        void spawn_entity() override;

        virtual bool unk_054(i32 kind);

        void lock_minimap(vector3d center, f32 zoom); // ignores a zoom of 0 or below
        void unlock_minimap();

        void set_buildings_invisible(bool invisible);
        void set_wallmode_disabled  (bool disabled);
    };

    ASSERT_SIZEOF  (igo_3d_radar_widget,                      0x1F0);
    ASSERT_OFFSETOF(igo_3d_radar_widget, icons,               0x150);
    ASSERT_OFFSETOF(igo_3d_radar_widget, kind_15_icons,       0x160);
    ASSERT_OFFSETOF(igo_3d_radar_widget, fingers_of_god,      0x170);
    ASSERT_OFFSETOF(igo_3d_radar_widget, default_poi,         0x180);
    ASSERT_OFFSETOF(igo_3d_radar_widget, city_life_ring_poi,  0x198);
    ASSERT_OFFSETOF(igo_3d_radar_widget, north_arrow_member,  0x1A8);
    ASSERT_OFFSETOF(igo_3d_radar_widget, vector_path_member,  0x1AC);
    ASSERT_OFFSETOF(igo_3d_radar_widget, locked_zoom,         0x1C0);
    ASSERT_OFFSETOF(igo_3d_radar_widget, locked_center,       0x1C8);
    ASSERT_OFFSETOF(igo_3d_radar_widget, minimap_locked,      0x1D4);
    ASSERT_OFFSETOF(igo_3d_radar_widget, buildings_invisible, 0x1D5);
    ASSERT_OFFSETOF(igo_3d_radar_widget, wallmode_disabled,   0x1D6);
} // treyarch
