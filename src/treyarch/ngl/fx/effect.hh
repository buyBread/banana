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

    /* the first 24 values are  rescued from the SM3 ngl_fx.h;
       retail extends this table through 124 and specializes value 15 during load from the scene matrix component mask stored in its payload */
    enum e_parameter_type : u32 {
        parameter_none,
        parameter_bool,
        parameter_int,
        parameter_float,
        parameter_vector,
        parameter_matrix,
        parameter_texture,
        parameter_procedural_texture,
        parameter_temporary_texture,
        parameter_sampler,
        parameter_scene_matrix,
        parameter_bone_array_world,
        parameter_bone_influences,
        parameter_random,
        parameter_view_position,
        parameter_view_direction,
        parameter_viewport_pixel_size,
        parameter_frame,
        parameter_time,
        parameter_last_time,
        parameter_elapsed_time,
        parameter_ambient_light,
        parameter_directional_light,
        parameter_point_light
    };

    struct function_binding {
        i32   parameter_index;
        u32   handle;
        u32   count;
        u32   destination_type;
        u32   destination_offset;
        char* name;
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
        parameter*       effect_binding;
        parameter*       next;
        e_parameter_type type;
        void*            data;
        u32              data_size;
        i32              annotation_count;
        void*            annotations;
    };

    struct reader_writer_lock {
        volatile i32 readers;
        volatile i32 writer;
    };

    struct effect;

    struct effect_runtime {
        effect*             owner;
        effect*             zero_point_lights;
        effect*             two_point_lights;
        effect*             four_point_lights;
        reader_writer_lock* technique_locks;
        effect_runtime*     next;
        i32                 priority;
        u8                  queued;
        u8                  reserved_01d[3];
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
        u32             mapped_state_024[4];
        parameter*      parameter_chains[3];
        u32             reserved_040;
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

        inline util::memory_reference<texture*>  parameter_texture_fallback { 0x01117174 };
        inline util::memory_reference<texture**> parameter_texture_fallback_pointer { 0x01117E5C };
    } // references

    ASSERT_SIZEOF  (function_binding,                     0x18);
    ASSERT_OFFSETOF(function_binding, parameter_index,    0x00);
    ASSERT_OFFSETOF(function_binding, handle,             0x04);
    ASSERT_OFFSETOF(function_binding, count,              0x08);
    ASSERT_OFFSETOF(function_binding, destination_type,   0x0C);
    ASSERT_OFFSETOF(function_binding, destination_offset, 0x10);
    ASSERT_OFFSETOF(function_binding, name,               0x14);

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

    ASSERT_SIZEOF  (parameter,                   0x2C);
    ASSERT_OFFSETOF(parameter, name,             0x00);
    ASSERT_OFFSETOF(parameter, semantic,         0x08);
    ASSERT_OFFSETOF(parameter, effect_binding,   0x10);
    ASSERT_OFFSETOF(parameter, next,             0x14);
    ASSERT_OFFSETOF(parameter, type,             0x18);
    ASSERT_OFFSETOF(parameter, data,             0x1C);
    ASSERT_OFFSETOF(parameter, data_size,        0x20);
    ASSERT_OFFSETOF(parameter, annotation_count, 0x24);
    ASSERT_OFFSETOF(parameter, annotations,      0x28);

    ASSERT_SIZEOF(reader_writer_lock, 0x08);

    ASSERT_SIZEOF  (effect_runtime,                    0x20);
    ASSERT_OFFSETOF(effect_runtime, owner,             0x00);
    ASSERT_OFFSETOF(effect_runtime, zero_point_lights, 0x04);
    ASSERT_OFFSETOF(effect_runtime, two_point_lights,  0x08);
    ASSERT_OFFSETOF(effect_runtime, four_point_lights, 0x0C);
    ASSERT_OFFSETOF(effect_runtime, technique_locks,   0x10);
    ASSERT_OFFSETOF(effect_runtime, next,              0x14);
    ASSERT_OFFSETOF(effect_runtime, priority,          0x18);
    ASSERT_OFFSETOF(effect_runtime, queued,            0x1C);

    ASSERT_SIZEOF  (effect,                   0x48);
    ASSERT_OFFSETOF(effect, flags,            0x00);
    ASSERT_OFFSETOF(effect, name,             0x04);
    ASSERT_OFFSETOF(effect, technique_count,  0x0C);
    ASSERT_OFFSETOF(effect, techniques,       0x10);
    ASSERT_OFFSETOF(effect, parameter_count,  0x14);
    ASSERT_OFFSETOF(effect, parameters,       0x18);
    ASSERT_OFFSETOF(effect, parameter_data,   0x20);
    ASSERT_OFFSETOF(effect, parameter_chains, 0x34);
    ASSERT_OFFSETOF(effect, runtime,          0x44);

    ASSERT_SIZEOF(effect_directory, 0x10);
}}} // treyarch::ngl::fx
