#pragma once

#include "treyarch/ngl/scene/parameters.hh"

namespace treyarch { namespace ngl {
    scene*      __cdecl list_begin_scene(e_scene_parameter_source parameter_source);
    const char* __cdecl set_scene_name(const char* name);
    scene*      __cdecl set_scene_option_group_0(bool first,
                                                 bool second,
                                                 bool third);
    scene*      __cdecl set_scene_option_group_1(bool first,
                                                 bool second,
                                                 bool third);
}} // treyarch::ngl
