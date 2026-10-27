#include "game/game.hh"
#include "game/movie_manager.hh"
#include "game/post_process/post_process.hh"
#include "game/shadow/shadow.hh"

using namespace treyarch;

// sub_97B160
void game::release_device_resources() {
    post_process::release_device_resources();
    shadow::release_device_resources();

    movie_manager* movies = movie_manager::inst();

    if (movies)
        movies->release_device_resources();
}

// sub_97B180
void game::restore_device_resources() {
    post_process::create_device_resources();
    shadow::create_device_resources();

    movie_manager* movies = movie_manager::inst();

    if (movies)
        movies->restore_device_resources();
}
