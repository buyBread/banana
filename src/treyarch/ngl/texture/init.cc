#include "treyarch/amalga/file.hh"
#include "treyarch/ngl/texture/init.hh"
#include "treyarch/ngl/texture/texture.hh"

using namespace treyarch;

void ngl::texture_init() {
    initialize_texture_directory();

    amalga::register_file_type(amalga::four_cc('T', 'E', 'X'),
                               3,
                               load_texture,
                               remove_texture);
}
