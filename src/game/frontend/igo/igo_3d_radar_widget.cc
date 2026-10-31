#include "game/frontend/igo/igo_3d_radar_widget.hh"

using namespace treyarch;

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
