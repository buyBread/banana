#include "treyarch/apk/file.hh"
#include "treyarch/ngl/font/font.hh"
#include "treyarch/ngl/font/init.hh"
#include "game/four_cc.hh"

using namespace treyarch;

// sub_9DAFB0
void ngl::font_init() {
    references::fonts.get().init();

    apk::register_file_type(four_cc('F', 'O', 'N', 'T'),
                            1,
                            load_font,
                            remove_font);
}
