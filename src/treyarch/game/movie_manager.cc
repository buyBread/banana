#include "retail.hh"
#include "treyarch/game/movie_manager.hh"

using namespace treyarch;

// loc_6930E0 (a chunk sub_97B160 tail-jumps into)
void movie_manager::release_device_resources() {
    if (bink)
        retail::sub_693000(); // release the upload and bink frame-buffer textures
}

// sub_6ABC30
void movie_manager::restore_device_resources() {
    retail::sub_6ABC30((u32*)this);
}
