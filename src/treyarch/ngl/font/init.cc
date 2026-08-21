#include "treyarch/amalga/file.hh"
#include "treyarch/ngl/font/font.hh"
#include "treyarch/ngl/font/init.hh"
#include "treyarch/shared/four_cc.hh"

using namespace treyarch;

void ngl::font_init() {
    references::fonts.get().initialize();

    amalga::register_file_type(four_cc('F', 'O', 'N', 'T'),
                               1,
                               load_font,
                               remove_font);
}
