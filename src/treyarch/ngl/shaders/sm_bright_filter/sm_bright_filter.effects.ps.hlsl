#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

#if NGL_SM_BRIGHT_DAMAGE_LUMINANCE

float damage_interpolation : register(c0);
float luminance_bias       : register(c1);

SamplerState color_sampler  : register(s0);
SamplerState bright_sampler : register(s1);

float4 main(
    float2 texcoord        : TEXCOORD0,
    float2 offset_texcoord : TEXCOORD1) : COLOR0
{
    const float3 luminance_weights = float3(0.3, 0.59, 0.11);

    float4 offset_color = tex2D(color_sampler, offset_texcoord);
    float value = lerp(
        offset_color.r,
        dot(offset_color.rgb, luminance_weights),
        damage_interpolation);

    float3 base_color = tex2D(color_sampler, texcoord).rgb;
    float base_luminance = dot(base_color, luminance_weights);
    value += base_luminance * base_luminance * damage_interpolation;

    float4 bright_color = tex2D(bright_sampler, offset_texcoord);
    value +=
        bright_color.a *
        bright_color.r *
        damage_interpolation *
        1.0e-5;

    return pow(value, luminance_bias);
}

#elif NGL_SM_BRIGHT_EXPOSURE_TARGET_VIGNETTE

float exposure          : register(c0);
float4 target_color     : register(c1);
float4 interpolation    : register(c2);

SamplerState color_sampler : register(s0);

float random_value(float2 value) {
    return frac(sin(dot(value, float2(81.2394867, 17.3480244))) * 59.3948402);
}

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float2 centered = texcoord - 0.5;
    float angle = atan2(centered.y, centered.x);
    float radius_squared = dot(centered, centered);
    float vignette = saturate(1.0 - radius_squared * 0.5);
    float noise = random_value(
        texcoord + float2(angle, radius_squared) + interpolation.y);
    noise = (1.0 - (noise * 0.2 + 0.2)) * vignette * 1.2;

    float4 color = tex2D(color_sampler, texcoord);
    float luminance = dot(
        color.rgb * exposure,
        float3(0.3, 0.59, 0.11));
    float4 targeted = lerp(
        color,
        luminance * target_color,
        interpolation.x);

    return float4(targeted.rgb * noise, targeted.a);
}

#elif NGL_SM_BRIGHT_RADIAL_OVERLAY

float4 interpolation : register(c0);

SamplerState color_sampler   : register(s0);
SamplerState texture_sampler : register(s1);

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float2 centered = texcoord - 0.5;
    float radius = length(centered);
    float blend = clamp(radius - 0.6, 0.0, 0.8) * interpolation.y;
    float angle = atan2(centered.y, centered.x);
    float2 radial_texcoord =
        float2(cos(angle), sin(angle)) * 0.25 + 0.5;

    return lerp(
        tex2D(color_sampler, texcoord),
        tex2D(texture_sampler, radial_texcoord),
        blend);
}

#elif NGL_SM_BRIGHT_GOD_RAYS

float3 god_ray_color         : register(c0);
float3 god_ray_depth_control : register(c1);
float4 sun_position          : register(c2);
float4 sun_ring_start        : register(c3);
float4 sun_ring_end          : register(c4);
float4 sun_color_r           : register(c5);
float4 sun_color_g           : register(c6);
float4 sun_color_b           : register(c7);

SamplerState color_sampler  : register(s0);
SamplerState depth_sampler  : register(s1);
SamplerState dither_sampler : register(s2);

float decode_depth(float4 sample) {
    return dot(
        sample.wxy,
        float3(0.99609381, 0.00389099144, 1.51991853e-5));
}

float4 main(
    float2 texcoord        : TEXCOORD0,
    float2 screen_position : VPOS) : COLOR0
{
    float distance_to_sun = length(
        texcoord * sun_position.zw + sun_position.xy);
    float4 ring = 1.0 - saturate(
        (distance_to_sun - sun_ring_start.xzyw) *
        sun_ring_end.xzyw);
    float3 sun_color = float3(
        dot(sun_color_r, ring),
        dot(sun_color_g, ring),
        dot(sun_color_b, ring));

    float4 scene = tex2D(color_sampler, texcoord);
    scene.rgb *= god_ray_color;

    float depth = decode_depth(tex2D(depth_sampler, texcoord));
    float depth_visibility = clamp(
        (depth - god_ray_depth_control.y) * god_ray_depth_control.x,
        0.0,
        god_ray_depth_control.z);
    bool background = depth >= 1.0;
    scene.rgb *= background ? 0.0 : 1.0;

    float sun_visibility = saturate(scene.a * 1.75 + (background ? 1.0 : 0.0));
    scene.rgb += sun_color * sun_visibility;

    float dither = tex2D(
        dither_sampler,
        screen_position * (1.0 / 64.0)).r *
        (1.0 / 128.0) -
        (1.0 / 256.0);
    scene.rgb += dither;
    scene.a = depth == 1.0 ? depth_visibility * 0.25 : depth_visibility;

    return scene;
}

#elif NGL_SM_BRIGHT_DEPTH_GATE

SamplerState color_sampler : register(s0);
SamplerState depth_sampler : register(s1);

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float4 color = tex2D(color_sampler, texcoord);
    float4 depth_sample = tex2D(depth_sampler, texcoord);
    float depth = depth_sample.w + depth_sample.x + depth_sample.y;

    return float4(color.rgb, depth >= 3.0 ? 1.0 : 0.0);
}

#elif NGL_SM_BRIGHT_BLOOM_DEPTH_GATE

float3 bloom_depth_control : register(c0);

SamplerState color_sampler : register(s0);
SamplerState depth_sampler : register(s1);

float decode_depth(float4 sample) {
    return dot(
        sample.wxy,
        float3(0.99609381, 0.00389099144, 1.51991853e-5));
}

half4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float depth = decode_depth(tex2D(depth_sampler, texcoord));
    half visibility = clamp(
        (depth - bloom_depth_control.y) * bloom_depth_control.x,
        0.0,
        bloom_depth_control.z);

    if (visibility == 1.0)
        visibility *= 0.25;

    return half4(tex2D(color_sampler, texcoord).rgb, visibility);
}

#elif NGL_SM_BRIGHT_DEPTH_MASK

float3 depth_control : register(c0);

SamplerState depth_sampler : register(s0);

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float depth = 1.0 - tex2D(depth_sampler, texcoord).r;
    float visibility = clamp(
        (depth - depth_control.y) * depth_control.x,
        0.0,
        depth_control.z);

    return float4(0.0, 0.0, 0.0, visibility);
}

#endif

#endif
