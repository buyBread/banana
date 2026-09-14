#include "retail.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/world/references.hh"
#include "treyarch/game/world/world_dynamics_system.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// todo

void game::release_the_world() {
    world_dynamics_system* world = the_world;

    if (world) {
        retail::sub_97A400(world);

        memory::heap::free(world);
    }

    the_world = nullptr;
    references::g_world_ptr.write(nullptr);
}

void game::clear_camera_handles() {
    base_camera         = nullptr;
    current_view_camera = nullptr;
    current_game_camera = nullptr;
}

void game::create_the_world() {
    void* allocation = memory::heap::allocate(sizeof(world_dynamics_system));

    world_dynamics_system* world = nullptr;

    if (allocation) {
        try {
            world = (world_dynamics_system*)retail::sub_97A860(allocation);
        } catch (...) {
            memory::heap::free(allocation);

            throw;
        }
    }

    the_world = world;
    references::g_world_ptr.write(world);
}
