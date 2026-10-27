#include "treyarch/apk/file.hh"
#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/fx/init.hh"
#include "game/four_cc.hh"

using namespace treyarch;

// sub_9E1F10
void ngl::fx::init() {
    initialize_directory();

    apk::register_file_type(four_cc('F', 'X', '\0'),
                            1,
                            load,
                            remove);

    references::initialization_state.write(0);
}
