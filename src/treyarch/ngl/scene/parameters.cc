#include <cstring>

#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/scene/parameters.hh"

using namespace treyarch;

u32 ngl::get_scene_parameter_set_size() {
    return 4 * references::scene_parameter_count.read() + 8;
}

ngl::scene_parameters* ngl::allocate_scene_parameters() {
    return (scene_parameters*)list::allocate(get_scene_parameter_set_size(), 16);
}

void ngl::clear_scene_parameters(scene_parameters* parameters) {
    parameters->valid_low  = 0;
    parameters->valid_high = 0;
}

void ngl::copy_scene_parameters(      scene_parameters* destination,
                                const scene_parameters* source) {

    std::memcpy(destination, source, get_scene_parameter_set_size());
}
