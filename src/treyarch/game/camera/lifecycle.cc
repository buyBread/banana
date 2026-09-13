#include "retail.hh"
#include "treyarch/game/camera/wds_camera_manager.hh"
#include "treyarch/game/game.hh"
#include "treyarch/shared/memory/heap.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace camera_manager_lifecycle_references {
    util::memory_reference<wds_camera_manager*> camera_manager_alias { 0x010FC54C };
}} // treyarch::camera_manager_lifecycle_references

using namespace treyarch;

void game::release_camera_manager() {
    wds_camera_manager* manager = this->camera_manager;

    if (manager) {
        retail::sub_97A400(manager);
        
        memory::heap::free(manager);
    }

    this->camera_manager = nullptr;
    camera_manager_lifecycle_references::camera_manager_alias.write(nullptr);
}

void game::clear_camera_handles() {
    this->base_camera         = nullptr;
    this->current_view_camera = nullptr;
    this->current_game_camera = nullptr;
}

void game::create_camera_manager() {
    void* allocation = memory::heap::allocate(sizeof(wds_camera_manager));

    wds_camera_manager* manager = nullptr;

    if (allocation) {
        try {
            manager = (wds_camera_manager*)retail::sub_97A860(allocation);
        } catch (...) {
            memory::heap::free(allocation);

            throw;
        }
    }

    this->camera_manager = manager;
    camera_manager_lifecycle_references::camera_manager_alias.write(manager);
}
