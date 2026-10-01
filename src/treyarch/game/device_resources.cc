#include "treyarch/game/game.hh"
#include "treyarch/game/movie_manager.hh"
#include "treyarch/game/post_process/post_process.hh"
#include "treyarch/game/shadow/shadow.hh"

using namespace treyarch;

// sub_97B160
void game::release_device_resources() {
    post_process::release_device_resources();
    shadow::release_device_resources();

    movie_manager* movies = references::movie_manager.read();

    if (movies)
        movies->release_device_resources();
}

// sub_97B180
void game::restore_device_resources() {
    post_process::create_device_resources();
    shadow::create_device_resources();

    movie_manager* movies = references::movie_manager.read();

    if (movies)
        movies->restore_device_resources();
}
