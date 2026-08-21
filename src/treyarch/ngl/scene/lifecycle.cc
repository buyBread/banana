#include <cstring>

#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/scene/defaults.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/scene/references.hh"

using namespace treyarch;

static ngl::scene* initialize_scene(ngl::scene*                   value,
                                    ngl::e_scene_parameter_source parameter_source) {

    ngl::scene* parent = ngl::references::current_scene.read();
    ngl::scene* source = parent;

    ngl::references::current_scene.write(value);

    value->color_target     = nullptr;
    value->depth_target     = nullptr;
    value->auxiliary_target = nullptr;
    value->local_state_430  = 0;

    if (parameter_source == ngl::scene_parameter_defaults)
        ngl::clear_and_set_default_scene_state(value);
    else {
        if (parameter_source == ngl::scene_parameter_root)
            source = ngl::references::root_scene.read();

        if (parameter_source == ngl::scene_parameter_parent ||
            parameter_source == ngl::scene_parameter_root) {

            std::memcpy(value, source, sizeof(ngl::scene));
        }
    }

    value->parameters = ngl::allocate_scene_parameters();
    
    ngl::clear_scene_parameters(value->parameters);

    if (parameter_source == ngl::scene_parameter_parent)
        ngl::copy_scene_parameters(value->parameters, parent->parameters);
    else if (parameter_source == ngl::scene_parameter_root)
        ngl::copy_scene_parameters(value->parameters, ngl::references::root_scene.read()->parameters);

    value->local_list_state = 0;
    value->options          = 0;
    value->parent           = parent;
    value->next_sibling     = nullptr;
    value->first_child      = nullptr;
    value->last_child       = nullptr;

    std::memset(value->callbacks, 0, sizeof(value->callbacks));

    value->opaque_render_list              = nullptr;
    value->translucent_render_list         = nullptr;
    value->opaque_render_list_count        = 0;
    value->translucent_render_list_count   = 0;
    value->specialized_render_list_0       = nullptr;
    value->specialized_render_list_count_0 = 0;
    value->specialized_render_list_1       = nullptr;
    value->specialized_render_list_count_1 = 0;
    value->specialized_render_list_2       = nullptr;
    value->specialized_render_list_count_2 = 0;
    value->specialized_render_list_3       = nullptr;
    value->specialized_render_list_count_3 = 0;
    value->specialized_render_list_4       = nullptr;
    value->specialized_render_list_count_4 = 0;
    value->name                            = "NGL Scene";

    return value;
}

ngl::scene* __cdecl ngl::list_begin_scene(e_scene_parameter_source parameter_source) {
    auto* value = (scene*)list::allocate(sizeof(scene), 16);

    if (value) {
        value->parameters = allocate_scene_parameters();
        clear_scene_parameters(value->parameters);
    }

    if (!value)
        return nullptr;

    scene* parent = references::current_scene.read();

    if (parent) {
        if (parent->last_child)
            parent->last_child->next_sibling = value;
        else
            parent->first_child = value;

        parent->last_child = value;
    } else
        references::root_scene.write(value);

    initialize_scene(value, parameter_source);

    return value;
}

const char* __cdecl ngl::set_scene_name(const char* name) {
    references::current_scene.get()->name = name;
    
    return name;
}

ngl::scene* __cdecl ngl::set_scene_option_group_0(bool first,
                                                  bool second,
                                                  bool third) {

    scene* value = references::current_scene.read();

    value->options &= 0xFFFFFFF0;

    if (first)
        value->options |= 0x01;
    if (second)
        value->options |= 0x02;
    if (third)
        value->options |= 0x04;

    return value;
}

ngl::scene* __cdecl ngl::set_scene_option_group_1(bool first,
                                                  bool second,
                                                  bool third) {

    scene* value = references::current_scene.read();

    value->options &= 0xFFFFFF0F;

    if (first)
        value->options |= 0x10;
    if (second)
        value->options |= 0x20;
    if (third)
        value->options |= 0x40;

    return value;
}
