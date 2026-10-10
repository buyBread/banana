#include "treyarch/amalga/merged_apk/file.hh"
#include "treyarch/shared/four_cc.hh"
#include "treyarch/ngl/texture/init.hh"
#include "treyarch/ngl/texture/texture.hh"

using namespace treyarch;

// sub_9E4680
void ngl::texture_init() {
    initialize_texture_directory();

    amalga::merged_apk::register_file_type(four_cc('T', 'E', 'X'),
                                           3,
                                           load_texture,
                                           remove_texture);
}
