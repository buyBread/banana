#pragma once

#include <d3d9.h>

#include "treyarch/amalga/file.hh"
#include "treyarch/shared/container/skip_list.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct texture;
}}

namespace treyarch { namespace ngl { namespace fx {
    enum e_annotation_type : u32 {
        annotation_none,
        annotation_bool,
        annotation_int,
        annotation_float,
        annotation_vector,
        annotation_matrix,
        annotation_string
    };

    /* retail PC values; SM3 .ii nglFXParameterType is turbo omega super outdated */
    enum e_parameter_type : u32 {
        parameter_none                         =   0,
        parameter_bool                         =   1,
        parameter_int                          =   2,
        parameter_float                        =   3,
        parameter_vector                       =   4,
        parameter_matrix                       =   5,
        parameter_sampler                      =  14,
        parameter_scene_matrix                 =  15,
        parameter_bone_array_world             =  16,
        parameter_bone_influences              =  17,
        parameter_view_position                =  21,
        parameter_viewport_pixel_size          =  23,
        parameter_frame                        =  24,
        parameter_time                         =  25,
        parameter_first_specialized_matrix     =  31,
        parameter_last_specialized_matrix      =  54,
        parameter_horizon_projection_u         =  55,
        parameter_horizon_projection_v         =  56,
        parameter_lightmap_color               =  57,
        parameter_window_color                 =  58,
        parameter_fog_color                    =  59,
        parameter_fog_control                  =  60,
        parameter_ambient_info                 =  61,
        parameter_horizon_info                 =  62,
        parameter_horizon_texture              =  63,
        parameter_light_info                   =  64,
        parameter_shadow_distances             =  65,
        parameter_shadow_buffer_size           =  66,
        parameter_shadow_buffer_size_1         =  67,
        parameter_shadow_buffer_size_2         =  68,
        parameter_view_projection_shadow       =  69,
        parameter_view_projection_shadow_1     =  70,
        parameter_view_projection_shadow_2     =  71,
        parameter_shadow_texture               =  72,
        parameter_shadow_texture_1             =  73,
        parameter_shadow_texture_2             =  74,
        parameter_post_info                    =  75,
        parameter_depth_texture                =  76,
        parameter_number_directional_lights    =  82,
        parameter_directional_light_directions =  83,
        parameter_directional_light_colors     =  84,
        parameter_number_point_lights          =  85,
        parameter_point_light_positions_ranges =  86,
        parameter_point_light_colors           =  87,
        parameter_ibl_parameters               =  92,
        parameter_character_highlight          =  93,
        parameter_framebuffer_texture          = 104,
        parameter_environment_map              = 105,
        parameter_decal_projection             = 108,
        parameter_viewport_to_world            = 109,
        parameter_world_to_viewport            = 110,
        parameter_ui_parameters                = 111,
        parameter_tint_color                   = 112,
        parameter_decal_texture_matrix         = 115,
        parameter_decompressed_position_scale  = 122,
        parameter_decompressed_position_bias   = 123,
        parameter_last                         = 124
    };

    struct function_binding {
        i32 parameter_index;
        u32 handle;
        u32 destination_type;
        u32 source_class;
        u32 source_offset;
        u32 register_count;
    };

    struct function {
        const DWORD*      microcode;
        void*             shader;
        i32               binding_count;
        function_binding* bindings;
    };

    struct program_set {
        function vertex_programs[2];
        function pixel_programs[2];
        u32      alpha_test_enabled;
    };

    struct render_states {
        u32 alpha_blend_enabled;
        u32 source_blend;
        u32 destination_blend;
        u32 blend_operation;
        u32 texture_factor;

        u32 alpha_test_enabled;
        u32 alpha_function;
        u32 alpha_reference;

        f32 depth_bias;
        f32 slope_scale_depth_bias;

        u32 dither_enabled;
        u32 cull_mode;
        u32 fill_mode;
        u32 shade_mode;
        u32 color_write_enabled;

        u32 depth_test_enabled;
        u32 depth_function;
        u32 depth_write_enabled;

        u32 stencil_enabled;
        u32 stencil_pass;
        u32 stencil_fail;
        u32 stencil_depth_fail;
        u32 stencil_function;
        u32 stencil_mask;
        u32 stencil_reference;
        u32 stencil_write_mask;

        u32 point_sprite_enabled;
        f32 point_size;
        f32 point_size_minimum;
        f32 point_size_maximum;

        u32 retail_states[5];
    };

    struct pass {
        fixed_string  name;
        render_states states;
        program_set   programs;
        u32           reserved_0d8;
        program_set*  active_programs;
        u32           post_load_state[3];
    };

    struct technique {
        u32          flags;
        fixed_string name;
        i32          pass_count;
        pass*        passes;
        i32          annotation_count;
        void*        annotations;
    };

    struct parameter {
        fixed_string     name;
        fixed_string     semantic;
        parameter*       animated_texture_chain_next;
        union {
            parameter* effect_binding;
            parameter* next;
        };
        e_parameter_type type;
        void*            data;
        u32              data_size;
        i32              annotation_count;
        void*            annotations;
    };

    struct effect;
    struct render_node;

    struct technique_batch {
        render_node* head;
        i32          count;
    };

    struct effect_runtime {
        effect*          owner;
        effect*          zero_point_lights;
        effect*          two_point_lights;
        effect*          four_point_lights;
        technique_batch* technique_batches;
        effect*          next;
        i32              priority;
        u8               queued;
        u8               reserved_01d[3];
    };

    struct effect {
        u32             flags;
        fixed_string    name;
        i32             technique_count;
        technique*      techniques;
        i32             parameter_count;
        parameter*      parameters;
        u32             mapped_state_01c;
        u8*             parameter_data;
        void*           current_material;
        void*           current_scene;
        u32             current_state_02c;
        u32             current_frame_epoch;
        parameter*      parameter_chains[3];
        parameter*      animated_texture_parameter_chain;
        effect_runtime* runtime;
    };

    struct effect_name {
        static string_hash get(const effect* value) {
            return value->name.hash;
        }
    };

    struct effect_directory : container::skip_list<effect, effect_name> {};

    effect* find(string_hash name);
    void initialize_directory();

    void __cdecl load(amalga::file*       owner,
                      amalga::file_entry* entry,
                      void**              mapped_sections,
                      void*               user_data);
    void __cdecl remove(amalga::file*       owner,
                        amalga::file_entry* entry,
                        void**              mapped_sections,
                        void*               user_data);

    namespace references {
        inline util::memory_reference<effect_directory> effects { 0x011171E4 };

        inline util::memory_reference<texture*>  parameter_texture_fallback         { 0x01117174 };
        inline util::memory_reference<texture**> parameter_texture_fallback_pointer { 0x01117E5C };
        inline util::memory_reference<u32>       parameter_binding_sentinel         { 0x01117E70 };
        inline util::memory_reference<u32>       parameter_chain_sentinel           { 0x01117E9C };
    } // references

    ASSERT_SIZEOF  (function_binding,                   0x18);
    ASSERT_OFFSETOF(function_binding, parameter_index,  0x00);
    ASSERT_OFFSETOF(function_binding, handle,           0x04);
    ASSERT_OFFSETOF(function_binding, destination_type, 0x08);
    ASSERT_OFFSETOF(function_binding, source_class,     0x0C);
    ASSERT_OFFSETOF(function_binding, source_offset,    0x10);
    ASSERT_OFFSETOF(function_binding, register_count,   0x14);

    ASSERT_SIZEOF  (function,                0x10);
    ASSERT_OFFSETOF(function, microcode,     0x00);
    ASSERT_OFFSETOF(function, shader,        0x04);
    ASSERT_OFFSETOF(function, binding_count, 0x08);
    ASSERT_OFFSETOF(function, bindings,      0x0C);

    ASSERT_SIZEOF  (program_set,                     0x44);
    ASSERT_OFFSETOF(program_set, vertex_programs,    0x00);
    ASSERT_OFFSETOF(program_set, pixel_programs,     0x20);
    ASSERT_OFFSETOF(program_set, alpha_test_enabled, 0x40);

    ASSERT_SIZEOF  (render_states,                       0x8C);
    ASSERT_OFFSETOF(render_states, alpha_blend_enabled,  0x00);
    ASSERT_OFFSETOF(render_states, alpha_test_enabled,   0x14);
    ASSERT_OFFSETOF(render_states, depth_bias,           0x20);
    ASSERT_OFFSETOF(render_states, stencil_enabled,      0x48);
    ASSERT_OFFSETOF(render_states, point_sprite_enabled, 0x68);
    ASSERT_OFFSETOF(render_states, retail_states,        0x78);

    ASSERT_SIZEOF  (pass,                  0xEC);
    ASSERT_OFFSETOF(pass, name,            0x00);
    ASSERT_OFFSETOF(pass, states,          0x08);
    ASSERT_OFFSETOF(pass, programs,        0x94);
    ASSERT_OFFSETOF(pass, active_programs, 0xDC);

    ASSERT_SIZEOF  (technique,                   0x1C);
    ASSERT_OFFSETOF(technique, flags,            0x00);
    ASSERT_OFFSETOF(technique, name,             0x04);
    ASSERT_OFFSETOF(technique, pass_count,       0x0C);
    ASSERT_OFFSETOF(technique, passes,           0x10);
    ASSERT_OFFSETOF(technique, annotation_count, 0x14);
    ASSERT_OFFSETOF(technique, annotations,      0x18);

    ASSERT_SIZEOF  (parameter,                              0x2C);
    ASSERT_OFFSETOF(parameter, name,                        0x00);
    ASSERT_OFFSETOF(parameter, semantic,                    0x08);
    ASSERT_OFFSETOF(parameter, animated_texture_chain_next, 0x10);
    ASSERT_OFFSETOF(parameter, effect_binding,              0x14);
    ASSERT_OFFSETOF(parameter, next,                        0x14);
    ASSERT_OFFSETOF(parameter, type,                        0x18);
    ASSERT_OFFSETOF(parameter, data,                        0x1C);
    ASSERT_OFFSETOF(parameter, data_size,                   0x20);
    ASSERT_OFFSETOF(parameter, annotation_count,            0x24);
    ASSERT_OFFSETOF(parameter, annotations,                 0x28);

    ASSERT_SIZEOF  (technique_batch,        0x08);
    ASSERT_OFFSETOF(technique_batch, head,  0x00);
    ASSERT_OFFSETOF(technique_batch, count, 0x04);

    ASSERT_SIZEOF  (effect_runtime,                    0x20);
    ASSERT_OFFSETOF(effect_runtime, owner,             0x00);
    ASSERT_OFFSETOF(effect_runtime, zero_point_lights, 0x04);
    ASSERT_OFFSETOF(effect_runtime, two_point_lights,  0x08);
    ASSERT_OFFSETOF(effect_runtime, four_point_lights, 0x0C);
    ASSERT_OFFSETOF(effect_runtime, technique_batches, 0x10);
    ASSERT_OFFSETOF(effect_runtime, next,              0x14);
    ASSERT_OFFSETOF(effect_runtime, priority,          0x18);
    ASSERT_OFFSETOF(effect_runtime, queued,            0x1C);

    ASSERT_SIZEOF  (effect,                                   0x48);
    ASSERT_OFFSETOF(effect, flags,                            0x00);
    ASSERT_OFFSETOF(effect, name,                             0x04);
    ASSERT_OFFSETOF(effect, technique_count,                  0x0C);
    ASSERT_OFFSETOF(effect, techniques,                       0x10);
    ASSERT_OFFSETOF(effect, parameter_count,                  0x14);
    ASSERT_OFFSETOF(effect, parameters,                       0x18);
    ASSERT_OFFSETOF(effect, parameter_data,                   0x20);
    ASSERT_OFFSETOF(effect, current_material,                 0x24);
    ASSERT_OFFSETOF(effect, current_scene,                    0x28);
    ASSERT_OFFSETOF(effect, current_frame_epoch,              0x30);
    ASSERT_OFFSETOF(effect, parameter_chains,                 0x34);
    ASSERT_OFFSETOF(effect, animated_texture_parameter_chain, 0x40);
    ASSERT_OFFSETOF(effect, runtime,                          0x44);

    ASSERT_SIZEOF(effect_directory, 0x10);
}}} // treyarch::ngl::fx
