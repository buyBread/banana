#pragma once

#include "treyarch/ngl/scene/parameters.hh"

namespace treyarch { namespace ngl {
    scene* __cdecl list_begin_scene(e_scene_parameter_source parameter_source);
    void   __cdecl list_end_scene();
    scene* __cdecl list_select_scene(scene* value);

    const char* __cdecl set_scene_name(const char* name);
    void        __cdecl set_scene_callback(e_scene_callback_type   type,
                                           scene_callback_function function,
                                           void*                   context = nullptr);

    void __cdecl set_clear_flags(u32 flags);
    void __cdecl set_clear_color(f32 red, f32 green, f32 blue, f32 alpha);
    void __cdecl set_animation_time(f32 time);

    scene* __cdecl set_scene_option_group_0(bool first,
                                            bool second,
                                            bool third);
    scene* __cdecl set_scene_option_group_1(bool first,
                                            bool second,
                                            bool third);
}} // treyarch::ngl
