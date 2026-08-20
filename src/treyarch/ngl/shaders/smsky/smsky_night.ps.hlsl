#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 depth_fog_color : register(c0);
float exposure         : register(c1);
float4 night_scale     : register(c2);
float4 night_power     : register(c3);
float4 night_red       : register(c4);
float4 night_green     : register(c5);
float4 night_blue      : register(c6);
float4 night_base      : register(c7);

SamplerState dither_sampler : register(s0);

struct pixel_input {
    float3 direction       : TEXCOORD0;
    float2 screen_position : VPOS;
};

float4 main(pixel_input input) : COLOR0 {
    float inverse_length = rsqrt(dot(input.direction, input.direction));
    float dome_position  = saturate(
        (1.0 - input.direction.y * inverse_length) * night_scale.x);
    float dome_gradient  = pow(dome_position, night_power.x);
    float4 gradient      = dome_gradient.xxxx;
    float3 night_color   = float3(
        dot(gradient, night_red),
        dot(gradient, night_green),
        dot(gradient, night_blue)) + night_base.rgb;
    float fog_amount     = saturate(dome_gradient * depth_fog_color.w);
    float3 color         = lerp(night_color, depth_fog_color.rgb, fog_amount);

    color = 1.0 - exp2(color * exposure);

    float dither = tex2D(dither_sampler, input.screen_position / 64.0).r;
    dither = dither / 128.0 - 1.0 / 256.0;

    return float4(color + dither, 0.0);
}

#endif
