#include "retail.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/movie_manager.hh"

using namespace treyarch;

// sub_97B160
void game::release_device_resources() {
    retail::sub_7660F0(); // post-process targets
    retail::sub_771B10(); // shadow targets

    movie_manager* movies = references::movie_manager.read();

    if (movies)
        movies->release_device_resources();
}

// sub_97B180
void game::restore_device_resources() {
    retail::sub_765070(); // post-process targets
    retail::sub_7719A0(); // shadow targets

    movie_manager* movies = references::movie_manager.read();

    if (movies)
        movies->restore_device_resources();
}
