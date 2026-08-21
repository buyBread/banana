#pragma once

#include "treyarch/ngl/init_list/init_list.hh"
#include "treyarch/ngl/shaders/shader.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace registration {
    void initialize();

    namespace references {
        inline ::util::memory_reference<u32> next_shader_id { 0x01118508 };

        inline ::util::memory_reference<shader> fx { 0x00F52A70 };

        inline ::util::memory_reference<shader> sm_bush                   { 0x00F4B928 };
        inline ::util::memory_reference<shader> sm_citylod                { 0x00F4B918 };
        inline ::util::memory_reference<shader> sm_retrofit               { 0x00F4B908 };
        inline ::util::memory_reference<shader> sm_buildinglod            { 0x00F4B8F8 };
        inline ::util::memory_reference<shader> sm_roadlod                { 0x00F4B8E8 };
        inline ::util::memory_reference<shader> sm_decalchar              { 0x00F4B8D8 };
        inline ::util::memory_reference<shader> sm_babyphatnormal         { 0x00F4B8C8 };
        inline ::util::memory_reference<shader> sm_babyphat               { 0x00F4B8B8 };
        inline ::util::memory_reference<shader> sm_decal                  { 0x00F4B8A8 };
        inline ::util::memory_reference<shader> sm_phat_palette_normal    { 0x00F4B898 };
        inline ::util::memory_reference<shader> sm_phatnormal             { 0x00F4B888 };
        inline ::util::memory_reference<shader> sm_road                   { 0x00F4B878 };
        inline ::util::memory_reference<shader> sm_phatpalettecharnormal  { 0x00F4B868 };
        inline ::util::memory_reference<shader> sm_phatpalettechar        { 0x00F4B858 };
        inline ::util::memory_reference<shader> sm_depth_shadow           { 0x00F4B848 };
        inline ::util::memory_reference<shader> smsky                     { 0x00F4B838 };
        inline ::util::memory_reference<shader> sm_bright_filter          { 0x00F4B828 };
        inline ::util::memory_reference<shader> sm_phat_palette           { 0x00F4B818 };
        inline ::util::memory_reference<shader> sm_phatcharnormal         { 0x00F4B808 };
        inline ::util::memory_reference<shader> sm_phatchar               { 0x00F4B7F8 };
        inline ::util::memory_reference<shader> sm_phatspiderman          { 0x00F4B7E8 };
        inline ::util::memory_reference<shader> sm_phat                   { 0x00F4B7D8 };

        inline ::util::memory_reference<shader> puv            { 0x00F4AC08 };
        inline ::util::memory_reference<shader> sm_simple      { 0x00F4ABA0 };
        inline ::util::memory_reference<shader> sm_translucent { 0x00F4AB90 };
        inline ::util::memory_reference<shader> pcuv           { 0x00F4AB80 };

        inline ::util::memory_reference<init_list_function> fake_peds   { 0x00F4ABF0 };
        inline ::util::memory_reference<init_list_function> road_lights { 0x00F4D8D4 };

        inline ::util::memory_reference<void*> fx_parameter_data         { 0x01117E8C };
        inline ::util::memory_reference<u8>    fx_parameter_data_storage { 0x01117ED0 };
    } // references
}}}} // treyarch::ngl::shaders::registration
