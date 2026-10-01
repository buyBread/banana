#include "retail.hh"
#include "treyarch/game/movie_manager.hh"

using namespace treyarch;

// loc_6930E0 (a chunk sub_97B160 tail-jumps into)
void movie_manager::release_device_resources() {
    if (bink)
        release_textures();
}

// sub_6ABC30
void movie_manager::restore_device_resources() {
    retail::sub_6ABC30((u32*)this);
}

// sub_693000
void movie_manager::release_textures() {
    movie_textures &textures = references::movie_textures.get();

    ngl::release_texture(textures.upload_y);
    textures.upload_y = nullptr;
    ngl::release_texture(textures.upload_a);
    textures.upload_a = nullptr;
    ngl::release_texture(textures.upload_cr);
    textures.upload_cr = nullptr;
    ngl::release_texture(textures.upload_cb);
    textures.upload_cb = nullptr;

    for (u32 index = 0; index < 2; ++index) {
        textures.frame_decoded[index] = 0;

        ngl::release_texture(textures.frame_y[index]);
        ngl::release_texture(textures.frame_a[index]);
        ngl::release_texture(textures.frame_cr[index]);
        ngl::release_texture(textures.frame_cb[index]);

        textures.frame_y[index]  = nullptr;
        textures.frame_a[index]  = nullptr;
        textures.frame_cr[index] = nullptr;
        textures.frame_cb[index] = nullptr;
    }
}
