#pragma once

#include <d3d9.h>

#include "treyarch/game/post_process/blitter.hh"
#include "treyarch/game/post_process/filter.hh"
#include "treyarch/game/post_process/texture_array.hh"
#include "treyarch/ngl/d3d9/texture.hh"
#include "treyarch/ngl/d3d9/vertex_definition.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/shared/math/types/matrix4x4.hh"
#include "treyarch/shared/math/types/vector4.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace post_process {
    // milliseconds on the NGL frame clock; every setter and sub_73C760 compute it inline
    u32 get_blend_time();

    // an effect parameter that blends from `previous` to `target` between the two times
    template <typename T>
    struct blended_parameter {
        T   current;
        T   previous;
        T   target;
        u32 start_time;
        u32 end_time;

        void set(const T &value, u32 duration) {
            previous = current;
            target   = value;

            start(duration);
        }

        void start(u32 duration) {
            if (duration) {
                start_time = get_blend_time();
                end_time   = duration + start_time;
            } else {
                current    = target;
                start_time = 0;
                end_time   = 0;
            }
        }
    };

    struct luminosity_parameters {
        f32 unk_00;
        f32 unk_04;
    };

    // `radius` 1, 3 and 7 select the 3x3, 7x7 and 15x15 gaussian filters
    struct blur_parameters {
        i32 iterations;
        i32 radius;
        i32 vertical; // the gaussian setup offsets its taps along v when set, along u when clear
    };

    struct poisson_disc_blur_parameters {
        i32 iterations;
        f32 radius; // in texels, scales the disc taps; the pass runs while above 0.001
    };

    // built per pass by the render pass (sub_74E440), not a blended block
    struct blend_parameters {
        f32 unk_00;
        f32 unk_04;
    };

    struct bloom_parameters {
        f32 values[17];
    };

    struct radial_blur_parameters {
        i32 iterations;
        f32 unk_04;
        f32 unk_08;
        f32 unk_0c;
        f32 unk_10; // the pass runs while above 0.001
    };

    // the cutscene player writes the first three values and sets `unk_0c` to 1
    struct depth_of_field_blur_parameters {
        f32              unk_00;
        f32              unk_04;
        f32              unk_08;
        i32              unk_0c;
        const matrix4x4* unk_10; // the setup uploads its transpose when set, identity otherwise
    };

    struct vignette_parameters {
        f32 values[10]; // the pass runs while values[9] is above 0.001
        u8  unk_28;
        u8  pad_029[3];
    };

    struct noise_parameters {
        f32 values[6]; // the pass runs while values[0] is above 0.001
    };

    struct color_adjust_parameters {
        f32 values[9];
    };

    // render_pause_menu_blur's taps, authored in texels from -6 to 6
    struct blur_taps {
        vector4 taps[13];
    };

    struct system { // idk the original class name
        u32                   last_update_time;
        u32                   update_delta;
        treyarch::blitter*    blitter;
        mipmap_texture_array* hdr_targets;
        mipmap_texture_array* ldr_targets;
        surface_texture*      average_luminosity;
        surface_texture*      adapted_luminosity;
        mipmap_texture_array* secondary_ldr_targets_0;
        mipmap_texture_array* secondary_ldr_targets_1;
        surface_texture*      adapted_luminosity_next; // swapped with `adapted_luminosity` after the adapt pass
        u8                    reserved_028[0x08];

        down_sample_filter* down_sample;
        filter*             luminosity_filter;
        filter*             gaussian_blur_3x3_filter;
        filter*             gaussian_blur_7x7_filter;
        filter*             gaussian_blur_15x15_filter;
        filter*             poisson_disc_blur_filter;
        filter*             luminosity_range_avg_filter;
        filter*             luminosity_range_avg_accum_filter;
        filter*             adapt_filter;
        filter*             blend_filter;
        filter*             bloom_filter;
        filter*             radial_blur_filter;
        filter*             depth_of_field_blur_filter;
        filter*             depth_of_field_filter;
        filter*             spherize_filter;
        filter*             vignette_filter;
        filter*             noise_filter;
        filter*             color_adjust_filter;

        filter_queue*                queue;
        ngl::d3d9::texture_resource  noise_texture;
        ngl::d3d9::texture_resource* damage_texture; // "fx_spidey_damage", looked up by the vignette pass

        blended_parameter<luminosity_parameters>          luminosity;
        blended_parameter<blur_parameters>                blur;
        blended_parameter<poisson_disc_blur_parameters>   poisson_disc_blur;
        blended_parameter<f32>                            adaptation_rate;
        blended_parameter<bloom_parameters>               bloom;
        blended_parameter<radial_blur_parameters>         radial_blur;
        blended_parameter<depth_of_field_blur_parameters> depth_of_field_blur;
        blended_parameter<f32>                            spherize;
        blended_parameter<vignette_parameters>            vignette;
        blended_parameter<noise_parameters>               noise;
        blended_parameter<color_adjust_parameters>        color_adjust;

        system();

        u32  create_render_targets(u8* work_buffer);
        bool bind_filter_programs();
        void release();

        void set_luminosity         (const luminosity_parameters*          value, u32 duration);
        void set_blur               (const blur_parameters*                value, u32 duration);
        void set_poisson_disc_blur  (const poisson_disc_blur_parameters*   value, u32 duration);
        void set_bloom              (const bloom_parameters*               value, u32 duration);
        void set_radial_blur        (const radial_blur_parameters*         value, u32 duration);
        void set_depth_of_field_blur(const depth_of_field_blur_parameters* value, u32 duration);
        void set_vignette           (const vignette_parameters*            value, u32 duration);
        void set_noise              (const noise_parameters*               value, u32 duration);
        void set_color_adjust       (const color_adjust_parameters*        value, u32 duration);
    };

    struct device_resource_state {
        ngl::texture*           eighth_target;
        f32                     inverse_width;
        u8                      reserved_008[0x04];
        IDirect3DVertexBuffer9* half_texel_vertex_buffer_1;
        ngl::texture*           sixty_fourth_target;
        u8                      reserved_014[0x44];
        IDirect3DVertexBuffer9* quad_vertex_buffer_0;
        f32                     inverse_height;
        u8                      reserved_060[0x04];
        ngl::vertex_definition  quad_vertex_definition;
        ngl::vertex_definition  half_texel_vertex_definition;
        ngl::texture*           render_targets[3]; // full size, then two at half size
        u8                      reserved_088[0x30];
        IDirect3DVertexBuffer9* half_texel_vertex_buffer_0;
        IDirect3DVertexBuffer9* quad_vertex_buffer_1;
        u8                      reserved_0c0[0x3F];
        u8                      initialized;
        u8                      reserved_100[0x2C];
        f32                     unk_12c; // added to bloom value 8; the options menu steps it by about 0.13
        u8                      reserved_130[0x28];
        ngl::texture*           quarter_target;
        u8                      reserved_15c[0x04];
        post_process::system*   system;
    };

    void create_device_resources();
    void release_device_resources();
    void apply_default_parameters();

    void render_pause_menu_blur(f32 strength);
    void render_zoom_map_effect();

    // `filter::setup` callbacks; each falls back to its own defaults when `parameters` is null
    void setup_luminosity_filter(      treyarch::blitter*     owner,
                                       texture_array*         targets,
                                       texture_array*         sources,
                                 const luminosity_parameters* parameters);

    void setup_adapt_filter(      treyarch::blitter* owner,
                                  texture_array*     targets,
                                  texture_array*     sources,
                            const f32*               parameters);

    void setup_blend_filter(      treyarch::blitter* owner,
                                  texture_array*     targets,
                                  texture_array*     sources,
                            const blend_parameters*  parameters);

    void setup_bloom_filter(      treyarch::blitter* owner,
                                  texture_array*     targets,
                                  texture_array*     sources,
                            const bloom_parameters*  parameters);

    void setup_radial_blur_filter(      treyarch::blitter*      owner,
                                        texture_array*          targets,
                                        texture_array*          sources,
                                  const radial_blur_parameters* parameters);

    void setup_depth_of_field_filter(      treyarch::blitter* owner,
                                           texture_array*     targets,
                                           texture_array*     sources,
                                     const void*              parameters);

    void setup_spherize_filter(      treyarch::blitter* owner,
                                     texture_array*     targets,
                                     texture_array*     sources,
                               const f32*               parameters);

    void setup_vignette_filter(      treyarch::blitter*   owner,
                                     texture_array*       targets,
                                     texture_array*       sources,
                               const vignette_parameters* parameters);

    void setup_noise_filter(      treyarch::blitter* owner,
                                  texture_array*     targets,
                                  texture_array*     sources,
                            const noise_parameters*  parameters);

    void setup_color_adjust_filter(      treyarch::blitter*       owner,
                                         texture_array*           targets,
                                         texture_array*           sources,
                                   const color_adjust_parameters* parameters);

    void setup_gaussian_blur_filter(      treyarch::blitter* owner,
                                          texture_array*     targets,
                                          texture_array*     sources,
                                    const blur_parameters*   parameters);

    void setup_poisson_disc_blur_filter(      treyarch::blitter*            owner,
                                              texture_array*                targets,
                                              texture_array*                sources,
                                        const poisson_disc_blur_parameters* parameters);

    void setup_luminosity_range_avg_filter(      treyarch::blitter* owner,
                                                 texture_array*     targets,
                                                 texture_array*     sources,
                                           const void*              parameters);

    void setup_depth_of_field_blur_filter(      treyarch::blitter*              owner,
                                                texture_array*                  targets,
                                                texture_array*                  sources,
                                          const depth_of_field_blur_parameters* parameters);

    namespace references {
        inline util::memory_reference<u8>                    active                     { 0x00F4CD41 };
        inline util::memory_reference<u8>                    bloom_enabled              { 0x00E76F4E }; // initial value 1; also chuck enable_bloom
        inline util::memory_reference<device_resource_state> device_resources           { 0x0102FD50 };
        inline util::memory_reference<D3DVERTEXELEMENT9>     quad_vertex_elements       { 0x00E7AE80 };
        inline util::memory_reference<D3DVERTEXELEMENT9>     half_texel_vertex_elements { 0x00E7AE98 };

        inline util::memory_reference<blur_taps>             pause_menu_blur_horizontal_taps { 0x00E793F0 }; // x components
        inline util::memory_reference<blur_taps>             pause_menu_blur_vertical_taps   { 0x00E794C0 }; // y components
        inline util::memory_reference<u8>                    pause_menu_blur_taps_scaled     { 0x01036E50 }; // never written

        // the taps times the blur strength, written by render_pause_menu_blur for its callback
        inline util::memory_reference<blur_taps> pause_menu_blur_scaled_horizontal_taps { 0x01030940 }; // x
        inline util::memory_reference<blur_taps> pause_menu_blur_scaled_vertical_taps   { 0x010319F0 }; // y

        // offset vectors written by create_device_resources and never read in PC
        inline util::memory_reference<vector4> unk_010311d0 { 0x010311D0 };
        inline util::memory_reference<vector4> unk_01030b80 { 0x01030B80 };
        inline util::memory_reference<vector4> unk_01030100 { 0x01030100 };
        inline util::memory_reference<vector4> unk_01030ec0 { 0x01030EC0 };
        inline util::memory_reference<vector4> unk_01030a20 { 0x01030A20 };
        inline util::memory_reference<vector4> unk_01031e40 { 0x01031E40 };
        inline util::memory_reference<vector4> unk_01030930 { 0x01030930 };
        inline util::memory_reference<vector4> unk_010300e0 { 0x010300E0 };
        inline util::memory_reference<vector4> unk_01030eb0 { 0x01030EB0 };
        inline util::memory_reference<vector4> unk_01030a60 { 0x01030A60 };
        inline util::memory_reference<vector4> unk_01031f70 { 0x01031F70 };
        inline util::memory_reference<vector4> unk_01031ac0 { 0x01031AC0 };
        inline util::memory_reference<vector4> unk_01031e30 { 0x01031E30 };
        inline util::memory_reference<vector4> unk_01030b00 { 0x01030B00 };
        inline util::memory_reference<vector4> unk_01031f80 { 0x01031F80 };
        inline util::memory_reference<vector4> unk_01031de0 { 0x01031DE0 };
        inline util::memory_reference<vector4> unk_01030080 { 0x01030080 };
        inline util::memory_reference<vector4> unk_01032010 { 0x01032010 };
        inline util::memory_reference<vector4> unk_01031dc0 { 0x01031DC0 };
        inline util::memory_reference<vector4> unk_01031d70 { 0x01031D70 };
        inline util::memory_reference<vector4> unk_01031d50 { 0x01031D50 };
        inline util::memory_reference<vector4> unk_01030b20 { 0x01030B20 };
        inline util::memory_reference<vector4> unk_01032000 { 0x01032000 };
        inline util::memory_reference<vector4> unk_01031d40 { 0x01031D40 };
    } // references

    ASSERT_SIZEOF  (luminosity_parameters,          0x08);
    ASSERT_SIZEOF  (blur_parameters,                0x0C);
    ASSERT_SIZEOF  (poisson_disc_blur_parameters,   0x08);
    ASSERT_SIZEOF  (blend_parameters,               0x08);
    ASSERT_SIZEOF  (bloom_parameters,               0x44);
    ASSERT_SIZEOF  (radial_blur_parameters,         0x14);
    ASSERT_SIZEOF  (depth_of_field_blur_parameters, 0x14);
    ASSERT_SIZEOF  (vignette_parameters,            0x2C);
    ASSERT_SIZEOF  (noise_parameters,               0x18);
    ASSERT_SIZEOF  (color_adjust_parameters,        0x24);

    ASSERT_SIZEOF  (system,                                   0x3E0);
    ASSERT_OFFSETOF(system, update_delta,                     0x004);
    ASSERT_OFFSETOF(system, blitter,                          0x008);
    ASSERT_OFFSETOF(system, hdr_targets,                      0x00C);
    ASSERT_OFFSETOF(system, ldr_targets,                      0x010);
    ASSERT_OFFSETOF(system, average_luminosity,               0x014);
    ASSERT_OFFSETOF(system, adapted_luminosity,               0x018);
    ASSERT_OFFSETOF(system, secondary_ldr_targets_0,          0x01C);
    ASSERT_OFFSETOF(system, secondary_ldr_targets_1,          0x020);
    ASSERT_OFFSETOF(system, adapted_luminosity_next,          0x024);
    ASSERT_OFFSETOF(system, down_sample,                      0x030);
    ASSERT_OFFSETOF(system, luminosity_filter,                0x034);
    ASSERT_OFFSETOF(system, poisson_disc_blur_filter,         0x044);
    ASSERT_OFFSETOF(system, adapt_filter,                     0x050);
    ASSERT_OFFSETOF(system, color_adjust_filter,              0x074);
    ASSERT_OFFSETOF(system, queue,                            0x078);
    ASSERT_OFFSETOF(system, noise_texture,                    0x07C);
    ASSERT_OFFSETOF(system, damage_texture,                   0x09C);
    ASSERT_OFFSETOF(system, luminosity,                       0x0A0);
    ASSERT_OFFSETOF(system, blur,                             0x0C0);
    ASSERT_OFFSETOF(system, poisson_disc_blur,                0x0EC);
    ASSERT_OFFSETOF(system, adaptation_rate,                  0x10C);
    ASSERT_OFFSETOF(system, bloom,                            0x120);
    ASSERT_OFFSETOF(system, radial_blur,                      0x1F4);
    ASSERT_OFFSETOF(system, depth_of_field_blur,              0x238);
    ASSERT_OFFSETOF(system, spherize,                         0x27C);
    ASSERT_OFFSETOF(system, vignette,                         0x290);
    ASSERT_OFFSETOF(system, noise,                            0x31C);
    ASSERT_OFFSETOF(system, color_adjust,                     0x36C);

    ASSERT_SIZEOF  (device_resource_state,                               0x164);
    ASSERT_OFFSETOF(device_resource_state, eighth_target,                0x000);
    ASSERT_OFFSETOF(device_resource_state, inverse_width,                0x004);
    ASSERT_OFFSETOF(device_resource_state, half_texel_vertex_buffer_1,   0x00C);
    ASSERT_OFFSETOF(device_resource_state, sixty_fourth_target,          0x010);
    ASSERT_OFFSETOF(device_resource_state, quad_vertex_buffer_0,         0x058);
    ASSERT_OFFSETOF(device_resource_state, inverse_height,               0x05C);
    ASSERT_OFFSETOF(device_resource_state, quad_vertex_definition,       0x064);
    ASSERT_OFFSETOF(device_resource_state, half_texel_vertex_definition, 0x070);
    ASSERT_OFFSETOF(device_resource_state, render_targets,               0x07C);
    ASSERT_OFFSETOF(device_resource_state, half_texel_vertex_buffer_0,   0x0B8);
    ASSERT_OFFSETOF(device_resource_state, quad_vertex_buffer_1,         0x0BC);
    ASSERT_OFFSETOF(device_resource_state, initialized,                  0x0FF);
    ASSERT_OFFSETOF(device_resource_state, unk_12c,                      0x12C);
    ASSERT_OFFSETOF(device_resource_state, quarter_target,               0x158);
    ASSERT_OFFSETOF(device_resource_state, system,                       0x160);
}} // treyarch::post_process
