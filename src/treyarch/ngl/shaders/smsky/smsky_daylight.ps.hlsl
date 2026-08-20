#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float exposure                    : register(c0);
float4 sun_direction              : register(c1);
float4 sun_color                  : register(c2);
float4 mie_phase_parameters       : register(c3);
float4 beta_rayleigh              : register(c4);
float4 beta_mie                   : register(c5);
float4 beta_dash_rayleigh         : register(c6);
float4 beta_dash_mie              : register(c7);
float4 one_over_beta_rayleigh_mie : register(c8);

struct pixel_input {
    float3 direction : TEXCOORD0;
};

float4 main(pixel_input input) : COLOR0 {
    float3 direction       = normalize(input.direction);
    float sun_height       = saturate(pow(direction.y, sun_direction.w));
    float view_distance    = lerp(
        one_over_beta_rayleigh_mie.w,
        beta_dash_mie.w,
        sun_height);
    float3 optical_depth   =
        view_distance * beta_rayleigh.rgb +
        view_distance * beta_mie.rgb;
    float3 extinction      = 1.0 - exp(-optical_depth);
    float sun_cosine       = dot(direction, sun_direction.xyz);
    float rayleigh_phase   = sun_cosine * sun_cosine + 1.0;
    float phase_base       =
        mie_phase_parameters.y + mie_phase_parameters.z * sun_cosine;
    float mie_phase        = rsqrt(phase_base) / phase_base;

    mie_phase *= mie_phase_parameters.x;
    float3 scattering      = extinction * (
        beta_dash_rayleigh.rgb * rayleigh_phase +
        beta_dash_mie.rgb * mie_phase);

    scattering *= one_over_beta_rayleigh_mie.rgb;
    scattering *= mie_phase_parameters.w;
    scattering *= sun_color.rgb;
    scattering *= sun_color.w;

    return float4(1.0 - exp2(scattering * exposure), 0.0);
}

#endif
