#include <cstring>

#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/scene/parameters.hh"

using namespace treyarch;

/*
    two words say which values are present, followed by the values themselves.
    a value can still be present when it's zero, so checking for it and reading it have to stay separate.
*/
bool ngl::has_scene_parameter(const scene_parameters* parameters, u32 id) {
    const u32* words = (const u32*)parameters;

    return (words[id >> 5] & (1u << (id & 31))) != 0;
}

void* ngl::get_scene_parameter(const scene_parameters* parameters, u32 id) {
    return (void*)parameters->values[id];
}

void* ngl::find_scene_parameter(const scene_parameters* parameters, u32 id) {
    return has_scene_parameter(parameters, id) ?
        get_scene_parameter(parameters, id) : nullptr;
}

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
