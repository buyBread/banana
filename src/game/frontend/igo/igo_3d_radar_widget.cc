#include "retail.hh"
#include "game/frontend/igo/igo_3d_radar_widget.hh"

using namespace treyarch;

// sub_6FBE50
igo_3d_radar_widget::~igo_3d_radar_widget() {
    delete default_poi;
    delete hero_poi;
    delete city_life_poi;
    delete height_line;
    delete range_info;
    delete ring_poi;
    delete city_life_ring_poi;
}

// slot 1
void igo_3d_radar_widget::init() {
    ((void (__thiscall*)(igo_3d_radar_widget*))retail::sub_693140)(this);
}

// slot 2
void igo_3d_radar_widget::update(f32 time_inc) {
    ((void (__thiscall*)(igo_3d_radar_widget*, f32))retail::sub_6BF8B0)(this, time_inc);
}

// slot 3
void igo_3d_radar_widget::render() {
    ((void (__thiscall*)(igo_3d_radar_widget*))retail::sub_6F7350)(this);
}

// sub_6EDB20
void igo_3d_radar_widget::spawn_entity() {
    igo_3d_widget::spawn_entity();

    unattached_widgets.push_back(default_poi);

    string_hash member;

    member.initialize(mash::ALLOCATED, "NORTHARROW");
    north_arrow_member = get_mesh_index(member);

    member.initialize(mash::ALLOCATED, "VECTOR_PATH");
    vector_path_member = get_mesh_index(member);
}

// slot 54
bool igo_3d_radar_widget::unk_054(i32 kind) {
    return ((bool (__thiscall*)(igo_3d_radar_widget*, i32))retail::sub_691E70)(this, kind);
}

// sub_691EC0
void igo_3d_radar_widget::lock_minimap(vector3d center, f32 zoom) {
    if (zoom <= 0.0f)
        return;

    minimap_locked = true;
    locked_center  = center;
    locked_zoom    = zoom;
}

// sub_691F10
void igo_3d_radar_widget::unlock_minimap() {
    minimap_locked = false;
    locked_zoom    = 1.0f;
}

// sub_691F30
void igo_3d_radar_widget::set_buildings_invisible(bool invisible) {
    buildings_invisible = invisible;
}

// sub_691F40
void igo_3d_radar_widget::set_wallmode_disabled(bool disabled) {
    wallmode_disabled = disabled;
}
