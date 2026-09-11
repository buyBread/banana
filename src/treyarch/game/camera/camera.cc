#include "treyarch/game/game.hh"
#include "treyarch/game/camera/wds_camera_manager.hh"

using namespace treyarch;

camera_handle game::get_current_view_camera() {
    if (camera_manager && current_view_camera == camera_manager->get_current_camera())
        return camera_manager->get_current_camera();

    return current_view_camera;
}
