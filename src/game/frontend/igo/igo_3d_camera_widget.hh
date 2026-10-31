#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/math/algebra.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_camera_widget : public igo_3d_widget {

    public:
        i32       camera_mesh_index; // the "ui_camera_mesh01" member, set when the entity spawns
        u8        reserved_154[0x0C];
        matrix4x4 camera_matrix;
        u8        matrix_dirty;
        bool      edit_mode;

        void set_edit_mode(bool enabled);

        void reset_position(); // (320, 240, -514), no tilt, spin or rotation

        void inc_x_pos();
        void dec_x_pos();
        void inc_y_pos();
        void dec_y_pos();
        void inc_z_pos();
        void dec_z_pos();
        void inc_tilt();
        void dec_tilt();
        void inc_spin();
        void dec_spin();
        void inc_rotation();
        void dec_rotation();

        void move_to_marker(igo_3d_widget*     widget,
                            const string_hash &marker,
                            f32                duration,
                            const matrix4x4*   matrix); // nothing without a widget
    };

    ASSERT_OFFSETOF(igo_3d_camera_widget, camera_mesh_index, 0x150);
    ASSERT_OFFSETOF(igo_3d_camera_widget, camera_matrix,     0x160);
    ASSERT_OFFSETOF(igo_3d_camera_widget, matrix_dirty,      0x1A0);
    ASSERT_OFFSETOF(igo_3d_camera_widget, edit_mode,         0x1A1);
} // treyarch
