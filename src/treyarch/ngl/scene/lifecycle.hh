#pragma once

#include "treyarch/ngl/scene/parameters.hh"

namespace treyarch { namespace ngl {
    scene* list_begin_scene(e_scene_parameter_source parameter_source);
    void   list_end_scene();
    scene* list_select_scene(scene* value);

    const char* set_scene_name(const char* name);

    void set_scene_callback(e_scene_callback_type   type,
                            scene_callback_function function,
                            void*                   context = nullptr);

    void set_clear_flags(u32 flags);
    void set_clear_color(f32 red, f32 green, f32 blue, f32 alpha);
    void set_z_write_enable(bool enable);
    void set_z_test_enable(bool enable);
    void set_animation_time(f32 time);

    scene* set_scene_option_group_0(bool first, bool second, bool third);
    scene* set_scene_option_group_1(bool first, bool second, bool third);
}} // treyarch::ngl
