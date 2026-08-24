#include "treyarch/amalga/file.hh"
#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/fx/init.hh"
#include "treyarch/shared/four_cc.hh"

using namespace treyarch;

void ngl::fx::init() {
    initialize_directory();

    amalga::register_file_type(four_cc('F', 'X', '\0'),
                               1,
                               load,
                               remove);

    references::initialization_state.write(0);
}
