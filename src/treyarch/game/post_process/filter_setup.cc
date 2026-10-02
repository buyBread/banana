#include <bit>
#include <cmath>

#include "treyarch/game/post_process/post_process.hh"
#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/display.hh"
#include "treyarch/ngl/timing/frame_timer.hh"

using namespace treyarch;

// sub_730120
void post_process::setup_luminosity_filter(      treyarch::blitter*     owner,
                                                 texture_array*         targets,
                                                 texture_array*         sources,
                                           const luminosity_parameters* parameters) {

    const luminosity_parameters  defaults { 2.0f, 32.0f };
    const luminosity_parameters &value = parameters ? *parameters : defaults;

    f32 scale = (f32)(1.0 / ((f64)value.unk_04 - (f64)value.unk_00));

    const f32 constants[4] { scale, (f32)((f64)value.unk_00 * (f64)scale), 0.0f, 1.0f };

    ngl::d3d9::references::device.get()->SetPixelShaderConstantF(8, constants, 1);

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
}

// sub_7301E0
void post_process::setup_adapt_filter(      treyarch::blitter* owner,
                                            texture_array*     targets,
                                            texture_array*     sources,
                                      const f32*               parameters) {

    f32 value = parameters ? *parameters : 2.0f;

    if (value > 1.0f)
        value = 1.0f;
    else if (value < 0.0f)
        value = 0.0f;

    const f32 constants[4] { value, 1.0f, 0.0f, 0.0f };

    ngl::d3d9::references::device.get()->SetPixelShaderConstantF(8, constants, 1);

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
    ngl::d3d9::set_sampler_filters(1, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(1, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
}

// sub_730290
void post_process::setup_blend_filter(      treyarch::blitter* owner,
                                            texture_array*     targets,
                                            texture_array*     sources,
                                      const blend_parameters*  parameters) {

    const blend_parameters  defaults { 0.5f, 0.5f };
    const blend_parameters &value = parameters ? *parameters : defaults;

    const f32 constants[4] { value.unk_00, value.unk_04, 0.0f, 0.0f };

    ngl::d3d9::references::device.get()->SetPixelShaderConstantF(8, constants, 1);

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
    ngl::d3d9::set_sampler_filters(1, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(1, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
}

// sub_730330
void post_process::setup_bloom_filter(      treyarch::blitter* owner,
                                            texture_array*     targets,
                                            texture_array*     sources,
                                      const bloom_parameters*  parameters) {

    const bloom_parameters defaults { { 1.0f, 1.0f, 0.0f, 0.255f, 31.0f, 0.99f, 0.5f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f } };

    const f32* values = parameters ? parameters->values : defaults.values;

    const f32 constants[24] { values[1],
                              values[0],
                              0.0f,
                              0.0f,

                              values[3],
                              (f32)(1.0 / ((f64)values[4] * (f64)values[4])),
                              values[5],
                              0.0f,

                              (f32)((f64)values[6] - 2.0 + (f64)values[7]),
                              (f32)(3.0 - (f64)values[6] * 2.0 - (f64)values[7]),
                              values[6],
                              0.0f,

                              std::exp(values[8]),
                              values[9],
                              values[10],
                              0.0f,

                              values[11],
                              values[12],
                              values[13],
                              1.0f,

                              values[14],
                              values[15],
                              values[16],
                              1.0f };

    ngl::d3d9::references::device.get()->SetPixelShaderConstantF(8, constants, 6);

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
    ngl::d3d9::set_sampler_filters(1, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(1, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
    ngl::d3d9::set_sampler_filters(2, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(2, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
}

// sub_7305C0
void post_process::setup_radial_blur_filter(      treyarch::blitter*      owner,
                                                  texture_array*          targets,
                                                  texture_array*          sources,
                                            const radial_blur_parameters* parameters) {

    const radial_blur_parameters  defaults { 4, 0.0f, 0.0f, 0.1f, 0.0f };
    const radial_blur_parameters &value = parameters ? *parameters : defaults;

    f32 diagonal  = (f32)((f64)value.unk_0c * (f64)1.41421356f);
    f64 remaining = (f64)1.41421356f - (f64)diagonal;

    const f32 constants[4] { value.unk_04,
                            -value.unk_08,
                             (f32)((f64)value.unk_10 / remaining),
                             (f32)((f64)-diagonal * (f64)value.unk_10 / remaining) };

    ngl::d3d9::references::device.get()->SetPixelShaderConstantF(8, constants, 1);

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
}

// sub_7306B0
void post_process::setup_depth_of_field_filter(      treyarch::blitter* owner,
                                                     texture_array*     targets,
                                                     texture_array*     sources,
                                               const void*              parameters) {

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
    ngl::d3d9::set_sampler_filters(1, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(1, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
    ngl::d3d9::set_sampler_filters(2, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(2, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
}

// sub_730710
void post_process::setup_spherize_filter(      treyarch::blitter* owner,
                                               texture_array*     targets,
                                               texture_array*     sources,
                                         const f32*               parameters) {

    const f32 constants[4] { parameters ? *parameters : 0.0f, 1.0f, 0.0f, 0.0f };

    ngl::d3d9::references::device.get()->SetPixelShaderConstantF(8, constants, 1);

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
}

// sub_730790
void post_process::setup_vignette_filter(      treyarch::blitter*   owner,
                                               texture_array*       targets,
                                               texture_array*       sources,
                                         const vignette_parameters* parameters) {

    const vignette_parameters defaults { { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, 0 };

    const f32* values = parameters ? parameters->values : defaults.values;

    const f32 constants[12] { values[0], values[1], values[2], values[3],
                              values[4], -values[5], 0.0f,     0.0f,
                              values[6], values[7], values[8], values[9] };

    ngl::d3d9::references::device.get()->SetPixelShaderConstantF(8, constants, 3);

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
    ngl::d3d9::set_sampler_filters(1, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(1, D3DTADDRESS_MIRROR, D3DTADDRESS_MIRROR);
}

// sub_7308D0
void post_process::setup_noise_filter(      treyarch::blitter* owner,
                                            texture_array*     targets,
                                            texture_array*     sources,
                                      const noise_parameters*  parameters) {

    const noise_parameters defaults { { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f } };

    const f32* values = parameters ? parameters->values : defaults.values;

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
    ngl::d3d9::set_sampler_filters(1, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(1, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP);

    // x87; __ftol2_sse truncates and only the low dword is used
    f32 elapsed = (f32)(i32)(ngl::get_vblank_milliseconds() * (f32)ngl::timing::references::list_tick.read());

    f32 drift   = (f32)((f64)elapsed * (f64)3.90625e-6f);
    f32 drift_u = (f32)((f64)-values[2] * (f64)drift);
    f32 drift_v = (f32)((f64)values[3] * (f64)drift);
    f32 scale   = (f32)(1.0 / ((f64)values[5] - (f64)values[4]));

    const f32 constants[12] { (f32)((f64)values[0] * 0.00390625),
                              (f32)((f64)values[0] * 0.00390625),
                              scale,
                              (f32)((f64)-values[4] * (f64)scale),

                              0.5f,
                              0.25f,
                              0.125f,
                              0.0625f,

                              0.03125f,
                              0.015625f,
                              0.0078125f,
                              0.0f };

    owner->tap_scale_u[0]  = 1.0f;
    owner->tap_scale_v[0]  = 1.0f;
    owner->tap_offset_u[0] = 0.0f;
    owner->tap_offset_v[0] = 0.0f;

    // each further tap doubles the previous one's scale and drift
    f32 octave = 1.0f;

    for (u32 tap = 1; tap < 8; ++tap) {
        owner->tap_scale_u[tap]  = (f32)((f64)values[1] * (f64)octave);
        owner->tap_scale_v[tap]  = (f32)((f64)values[1] * (f64)octave);
        owner->tap_offset_u[tap] = (f32)((f64)drift_u * (f64)octave);
        owner->tap_offset_v[tap] = (f32)((f64)drift_v * (f64)octave);

        octave *= 2.0f;
    }

    ngl::d3d9::references::device.get()->SetPixelShaderConstantF(8, constants, 3);
}

// sub_730CC0
void post_process::setup_color_adjust_filter(      treyarch::blitter*       owner,
                                                   texture_array*           targets,
                                                   texture_array*           sources,
                                             const color_adjust_parameters* parameters) {

    const color_adjust_parameters defaults { { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f } };

    const f32* values = parameters ? parameters->values : defaults.values;

    const f32 constants[12] { std::exp(values[0]), values[1], values[2], 0.0f,
                              values[3],           values[4], values[5], 1.0f,
                              values[6],           values[7], values[8], 1.0f };

    ngl::d3d9::references::device.get()->SetPixelShaderConstantF(8, constants, 3);

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
}

// sub_73C840
void post_process::setup_gaussian_blur_filter(      treyarch::blitter* owner,
                                                    texture_array*     targets,
                                                    texture_array*     sources,
                                              const blur_parameters*   parameters) {

    const blur_parameters  defaults { 1, 3, 0 };
    const blur_parameters &value = parameters ? *parameters : defaults;

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);

    f32 texel_u = 0.0f;
    f32 texel_v = 0.0f;

    if (sources && sources->count) {
        const ngl::d3d9::texture_resource &source = sources->textures[0].texture;

        if (value.vertical)
            texel_v = (f32)(1.0 / (f64)(f32)(i32)source.height);
        else
            texel_u = (f32)(1.0 / (f64)(f32)(i32)source.width);
    }

    // a tent of `radius + 1` down to 1 on each side of the center, normalized; the setter caps radius at 7
    f32 weights[8] {};

    weights[0] = (f32)((f64)(f32)value.radius + 1.0);

    f32 sum = weights[0];

    for (i32 tap = 1; tap <= value.radius; ++tap) {
        weights[tap] = (f32)((f64)(f32)(value.radius - tap) + 1.0);
        sum          = (f32)((f64)weights[tap] * 2.0 + (f64)sum);
    }

    f32 inverse_sum = (f32)(1.0 / (f64)sum);

    for (f32 &weight : weights)
        weight = (f32)((f64)weight * (f64)inverse_sum);

    ngl::d3d9::references::device.get()->SetPixelShaderConstantF(0, weights, 2);

    // tap 15 is left alone
    const f64 positions[15] { 0.0, -1.0, 1.0, -2.0, 2.0, -3.0, 3.0, -4.0, 4.0, -5.0, 5.0, -6.0, 6.0, -7.0, 7.0 };

    for (u32 tap = 0; tap < 15; ++tap) {
        owner->tap_scale_u[tap]  = 1.0f;
        owner->tap_scale_v[tap]  = 1.0f;
        owner->tap_offset_u[tap] = (f32)((f64)texel_u * positions[tap]);
        owner->tap_offset_v[tap] = (f32)((f64)texel_v * positions[tap]);
    }
}

// sub_73CEC0
void post_process::setup_poisson_disc_blur_filter(      treyarch::blitter*            owner,
                                                        texture_array*                targets,
                                                        texture_array*                sources,
                                                  const poisson_disc_blur_parameters* parameters) {

    const poisson_disc_blur_parameters  defaults { 1, 3.0f };
    const poisson_disc_blur_parameters &value = parameters ? *parameters : defaults;

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);

    f32 texel_u = 0.0f;
    f32 texel_v = 0.0f;

    if (sources && sources->count) {
        const ngl::d3d9::texture_resource &source = sources->textures[0].texture;

        texel_u = (f32)((f64)value.radius / (f64)(f32)(i32)source.width);
        texel_v = (f32)((f64)value.radius / (f64)(f32)(i32)source.height);
    }

    const f32 disc[8][2] { {  0.0f,       0.0f      },
                           {  0.527837f, -0.085868f },
                           { -0.040088f,  0.536087f },
                           { -0.670445f, -0.179949f },
                           { -0.419418f, -0.616039f },
                           {  0.440453f, -0.639399f },
                           { -0.757088f,  0.349334f },
                           {  0.574619f,  0.685879f } };

    for (u32 tap = 0; tap < 8; ++tap) {
        owner->tap_scale_u[tap]  = 1.0f;
        owner->tap_scale_v[tap]  = 1.0f;
        owner->tap_offset_u[tap] = (f32)((f64)texel_u * (f64)disc[tap][0]);
        owner->tap_offset_v[tap] = (f32)((f64)texel_v * (f64)disc[tap][1]);
    }
}

// sub_73D140, not a function in the IDA database
void post_process::setup_luminosity_range_avg_filter(      treyarch::blitter* owner,
                                                           texture_array*     targets,
                                                           texture_array*     sources,
                                                     const void*              parameters) {

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);

    f32 texel_u = 0.0f;
    f32 texel_v = 0.0f;

    if (sources && sources->count) {
        const ngl::d3d9::texture_resource &source = sources->textures[0].texture;

        texel_u = (f32)(1.0 / (f64)(f32)(i32)source.width);
        texel_v = (f32)(1.0 / (f64)(f32)(i32)source.height);
    }

    // a 4x4 grid of taps centered on the pixel
    f32 origin_u = (f32)((f64)texel_u * -1.5);
    f32 origin_v = (f32)((f64)texel_v * -1.5);

    for (i32 tap = 0; tap < 16; ++tap) {
        owner->tap_scale_u[tap]  = 1.0f;
        owner->tap_scale_v[tap]  = 1.0f;
        owner->tap_offset_u[tap] = (f32)((f64)(f32)(tap & 3)  * (f64)texel_u + (f64)origin_u);
        owner->tap_offset_v[tap] = (f32)((f64)(f32)(tap >> 2) * (f64)texel_v + (f64)origin_v);
    }
}

// sub_73D250
void post_process::setup_depth_of_field_blur_filter(      treyarch::blitter*              owner,
                                                          texture_array*                  targets,
                                                          texture_array*                  sources,
                                                    const depth_of_field_blur_parameters* parameters) {

    const depth_of_field_blur_parameters  defaults { 0.5f, 0.0f, 0.0f, 3, nullptr };
    const depth_of_field_blur_parameters &value = parameters ? *parameters : defaults;

    IDirect3DDevice9* device = ngl::d3d9::references::device.get();

    matrix4x4 transform;

    if (value.unk_10)
        transform = value.unk_10->transpose();
    else
        transform.identity();

    device->SetPixelShaderConstantF(4, (const f32*)&transform, 4);

    f64 range = (f64)value.unk_08 - (f64)value.unk_04;

    // retail uploads two registers from a four-float block, so the second register is the stack
    // copy of `defaults` that follows it; the block's last zero lands on `defaults.unk_00`
    const f32 constants[8] { value.unk_00,
                             (f32)(1.0 / range),
                             (f32)((f64)value.unk_04 / range),
                             0.0f,

                             defaults.unk_04,
                             defaults.unk_08,
                             std::bit_cast<f32>(defaults.unk_0c),
                             0.0f }; // `defaults.unk_10`, null

    device->SetPixelShaderConstantF(8, constants, 2);

    ngl::d3d9::set_sampler_filters(0, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
    ngl::d3d9::set_sampler_filters(1, D3DTEXF_POINT, D3DTEXF_POINT, D3DTEXF_NONE, 1);
    ngl::d3d9::set_sampler_address(1, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
}
