#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 depth_fog_color             : register(c0);
float exposure                    : register(c1);
float4 sun_direction              : register(c2);
float4 sun_color                  : register(c3);
float4 mie_phase_parameters       : register(c4);
float4 beta_rayleigh              : register(c5);
float4 beta_mie                   : register(c6);
float4 beta_dash_rayleigh         : register(c7);
float4 beta_dash_mie              : register(c8);
float4 one_over_beta_rayleigh_mie : register(c9);
float4 night_scale                : register(c10);
float4 night_power                : register(c11);
float4 night_red                  : register(c12);
float4 night_green                : register(c13);
float4 night_blue                 : register(c14);
float4 night_base                 : register(c15);

SamplerState dither_sampler : register(s0);

struct pixel_input {
    float3 direction       : TEXCOORD0;
    float2 screen_position : VPOS;
};

float4 main(pixel_input input) : COLOR0 {
    float inverse_length     = rsqrt(dot(input.direction, input.direction));
    float3 direction         = input.direction * inverse_length;
    float dome_position      = saturate(
        (1.0 - input.direction.y * inverse_length) * night_scale.x);
    float dome_gradient      = pow(dome_position, night_power.x);
    float sun_height         = saturate(pow(direction.y, sun_direction.w));
    float sun_cosine         = dot(direction, sun_direction.xyz);
    float view_distance      = lerp(
        one_over_beta_rayleigh_mie.w,
        beta_dash_mie.w,
        sun_height);
    float3 optical_depth     =
        view_distance * beta_rayleigh.rgb +
        view_distance * beta_mie.rgb;
    float3 extinction        = 1.0 - exp(-optical_depth);
    float rayleigh_phase     = sun_cosine * sun_cosine + 1.0;
    float phase_base         =
        mie_phase_parameters.y + mie_phase_parameters.z * sun_cosine;
    float mie_phase          = rsqrt(phase_base) / phase_base;

    mie_phase *= mie_phase_parameters.x;
    float3 scattering        = extinction * (
        beta_dash_rayleigh.rgb * rayleigh_phase +
        beta_dash_mie.rgb * mie_phase);

    scattering *= one_over_beta_rayleigh_mie.rgb;
    scattering *= mie_phase_parameters.w;
    scattering *= sun_color.rgb;

    float4 gradient    = dome_gradient.xxxx;
    float3 night_color = float3(
        dot(gradient, night_red),
        dot(gradient, night_green),
        dot(gradient, night_blue)) + night_base.rgb;
    float fog_amount   = saturate(dome_gradient * depth_fog_color.w);
    float3 color       = scattering * sun_color.w + night_color;

    color = lerp(color, depth_fog_color.rgb, fog_amount);
    color = 1.0 - exp2(color * exposure);

    float dither = tex2D(dither_sampler, input.screen_position / 64.0).r;
    dither = dither / 128.0 - 1.0 / 256.0;

    return float4(color + dither, 0.0);
}

#endif
