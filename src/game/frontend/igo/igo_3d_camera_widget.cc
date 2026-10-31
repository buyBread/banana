#include "retail.hh"
#include "game/frontend/igo/igo_3d_camera_widget.hh"

using namespace treyarch;

// sub_68B0A0
void igo_3d_camera_widget::set_edit_mode(bool enabled) {
    edit_mode = enabled;
}

// sub_68B040
void igo_3d_camera_widget::reset_position() {
    set_position(320.0f, 240.0f, -514.0f);
    set_tilt_angle(0.0f);
    set_spin_angle(0.0f);
    set_rotation_angle(0.0f);
}

// sub_68B0B0
void igo_3d_camera_widget::inc_x_pos() {
    set_center_pos((f32)((f64)parameters->position.get_x() + 10.0), parameters->position.get_y());
}

// sub_68B0E0
void igo_3d_camera_widget::dec_x_pos() {
    set_center_pos((f32)((f64)parameters->position.get_x() - 10.0), parameters->position.get_y());
}

// sub_68B110
void igo_3d_camera_widget::inc_y_pos() {
    set_center_pos(parameters->position.get_x(), (f32)((f64)parameters->position.get_y() + 10.0));
}

// sub_68B140
void igo_3d_camera_widget::dec_y_pos() {
    set_center_pos(parameters->position.get_x(), (f32)((f64)parameters->position.get_y() - 10.0));
}

// sub_68B170
void igo_3d_camera_widget::inc_z_pos() {
    set_position(parameters->position.get_x(),
                 parameters->position.get_y(),
                 (f32)((f64)parameters->position.get_z() + 10.0));
}

// sub_68B1B0
void igo_3d_camera_widget::dec_z_pos() {
    set_position(parameters->position.get_x(),
                 parameters->position.get_y(),
                 (f32)((f64)parameters->position.get_z() - 10.0));
}

// sub_68B1F0
void igo_3d_camera_widget::inc_tilt() {
    set_tilt_angle((f32)((f64)parameters->tilt_angle + (f64)0.01f));
}

// sub_68B220
void igo_3d_camera_widget::dec_tilt() {
    set_tilt_angle((f32)((f64)parameters->tilt_angle - (f64)0.01f));
}

// sub_68B250
void igo_3d_camera_widget::inc_spin() {
    set_spin_angle((f32)((f64)parameters->spin_angle + (f64)0.01f));
}

// sub_68B280
void igo_3d_camera_widget::dec_spin() {
    set_spin_angle((f32)((f64)parameters->spin_angle - (f64)0.01f));
}

// sub_68B2B0
void igo_3d_camera_widget::inc_rotation() {
    set_rotation_angle((f32)((f64)parameters->rotation_angle + (f64)0.01f));
}

// sub_68B2E0
void igo_3d_camera_widget::dec_rotation() {
    set_rotation_angle((f32)((f64)parameters->rotation_angle - (f64)0.01f));
}

// sub_6B2700
void igo_3d_camera_widget::move_to_marker(igo_3d_widget*     widget,
                                          const string_hash &marker,
                                          f32                duration,
                                          const matrix4x4*   matrix) {

    if (!widget)
        return;

    i32 index = widget->get_mesh_index(marker);

    retail::sub_69A830(this, widget, index, duration, matrix); // starts the move towards the member
}
