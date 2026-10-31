#include <new>

#include "retail.hh"
#include "game/frontend/igo/igo_3d_spidey_distance_widget.hh"

using namespace treyarch;

// sub_68D590
void igo_3d_spidey_distance_widget::set_min_distance(f32 distance) {
    gauge->min_distance = distance;
}

// sub_68D5B0
void igo_3d_spidey_distance_widget::set_max_distance(f32 distance) {
    gauge->max_distance = distance;
}

// sub_68D5D0
void igo_3d_spidey_distance_widget::set_offset(const vector3d &new_offset) {
    gauge->offset.x = new_offset.x;
    gauge->offset.y = new_offset.y;
    gauge->offset.z = new_offset.z;
}

// sub_6DE1C0
void igo_3d_spidey_distance_widget::set_mode(i32 new_mode) {
    gauge->mode = new_mode;

    switch (new_mode) {
        case 1:
        case 2:
            gauge->unk_28 = false;
            set_hot_cold_flip(false);
            break;

        default:
            gauge->unk_28 = true;
            set_hot_cold_flip(true);
            break;
    }
}

// sub_6D0630
void igo_3d_spidey_distance_widget::set_hot_cold_flip(bool flip) {
    distance_gauge_arrows* arrows = gauge->arrows;

    arrows->hot_cold_flip = flip;
    arrows->unk_01        = true;

    for (distance_gauge_part* part : arrows->unk_14)
        part->unk_0c = 1;

    for (distance_gauge_part* part : arrows->unk_24)
        part->unk_0c = 0;
}

// sub_6ECA10
void igo_3d_spidey_distance_widget::set_near_tint(const vector3d &tint) {
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    auto* name = new (storage) mash::string(distance_gauge::near_part_name);
    auto* part = (distance_gauge_part*)retail::sub_6EC790((u32*)gauge->parts, (i32)name); // the part with that name

    name->finalize(mash::ALLOCATED);

    part->tint.x = tint.x;
    part->tint.y = tint.y;
    part->tint.z = tint.z;
}

// sub_6ECAA0
void igo_3d_spidey_distance_widget::set_mid_tint(const vector3d &tint) {
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    auto* name = new (storage) mash::string(distance_gauge::mid_part_name);
    auto* part = (distance_gauge_part*)retail::sub_6EC790((u32*)gauge->parts, (i32)name); // the part with that name

    name->finalize(mash::ALLOCATED);

    part->tint.x = tint.x;
    part->tint.y = tint.y;
    part->tint.z = tint.z;
}

// sub_6ECB30
void igo_3d_spidey_distance_widget::set_far_tint(const vector3d &tint) {
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    auto* name = new (storage) mash::string(distance_gauge::far_part_name);
    auto* part = (distance_gauge_part*)retail::sub_6EC790((u32*)gauge->parts, (i32)name); // the part with that name

    name->finalize(mash::ALLOCATED);

    part->tint.x = tint.x;
    part->tint.y = tint.y;
    part->tint.z = tint.z;
}

// sub_6ECBC0
void igo_3d_spidey_distance_widget::reset_tints() {
    alignas(mash::string) u8 near_storage[sizeof(mash::string)];
    alignas(mash::string) u8 mid_storage[sizeof(mash::string)];
    alignas(mash::string) u8 far_storage[sizeof(mash::string)];

    auto* near_name = new (near_storage) mash::string(distance_gauge::near_part_name);
    auto* part      = (distance_gauge_part*)retail::sub_6EC790((u32*)gauge->parts, (i32)near_name);

    near_name->finalize(mash::ALLOCATED);

    part->tint = distance_gauge::default_tint;

    auto* mid_name = new (mid_storage) mash::string(distance_gauge::mid_part_name);
    part           = (distance_gauge_part*)retail::sub_6EC790((u32*)gauge->parts, (i32)mid_name);

    mid_name->finalize(mash::ALLOCATED);

    part->tint = distance_gauge::default_tint;

    auto* far_name = new (far_storage) mash::string(distance_gauge::far_part_name);
    part           = (distance_gauge_part*)retail::sub_6EC790((u32*)gauge->parts, (i32)far_name);

    far_name->finalize(mash::ALLOCATED);

    part->tint = distance_gauge::default_tint;
}
