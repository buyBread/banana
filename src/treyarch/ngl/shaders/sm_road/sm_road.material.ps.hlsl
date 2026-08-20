#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 diffuse_irradiance[9]     : register(c0);
float4 special_effects_matrix[4] : register(c9);
row_major float4x4 color_matrix  : register(c13);
float4 tweak_scale[2]            : register(c17);
float4 tweak_bias[2]             : register(c19);

float3 eye               : register(c21);
float3 ambient_direction : register(c22);
float4 fresnel           : register(c23);
float  translucent_flag  : register(c24);

#if NGL_SM_ROAD_SHADOW_COUNT > 0
float4 shadow_distance          : register(c25);
float4 depth_fog_color          : register(c26);
float4 depth_fog_control        : register(c27);
float4 fog_color                : register(c28);
float  exposure                 : register(c29);
float3 horizon_sky_color        : register(c30);
float3 horizon_sky_direction    : register(c31);
float3 horizon_sun_color        : register(c32);
float3 horizon_sun_direction    : register(c33);
float2 horizon_sun_angles       : register(c34);
float3 horizon_bounce_direction : register(c35);
float2 horizon_bounce_angles    : register(c36);
float  horizon_bounce_ratio     : register(c37);
float3 horizon_night_color      : register(c38);
float2 micro_coordinates_per_meter  : register(c39);
float2 grunge_coordinates_per_meter : register(c40);
float2 noise_coordinates_per_meter  : register(c41);
float4 micro_sampler_inverse_size    : register(c42);
float4 grunge_sampler_inverse_size   : register(c43);
float4 micro_grunge_height_scale     : register(c44);
float4 noise_micro_diffuse_level     : register(c45);
float3 grunge_diffuse_low            : register(c46);
float2 height_sampler_inverse_size   : register(c47);
#else
float4 depth_fog_color          : register(c25);
float4 depth_fog_control        : register(c26);
float4 fog_color                : register(c27);
float  exposure                 : register(c28);
float3 horizon_sky_color        : register(c29);
float3 horizon_sky_direction    : register(c30);
float3 horizon_sun_color        : register(c31);
float3 horizon_sun_direction    : register(c32);
float2 horizon_sun_angles       : register(c33);
float3 horizon_bounce_direction : register(c34);
float2 horizon_bounce_angles    : register(c35);
float  horizon_bounce_ratio     : register(c36);
float3 horizon_night_color      : register(c37);
float2 micro_coordinates_per_meter  : register(c38);
float2 grunge_coordinates_per_meter : register(c39);
float2 noise_coordinates_per_meter  : register(c40);
float4 micro_sampler_inverse_size    : register(c41);
float4 grunge_sampler_inverse_size   : register(c42);
float4 micro_grunge_height_scale     : register(c43);
float4 noise_micro_diffuse_level     : register(c44);
float3 grunge_diffuse_low            : register(c45);
float2 height_sampler_inverse_size   : register(c46);
#endif

SamplerState horizon_sampler : register(s0);
#if NGL_SM_ROAD_SHADOW_COUNT > 0
SamplerState shadow_sampler_0 : register(s1);
#endif
#if NGL_SM_ROAD_SHADOW_COUNT > 1
SamplerState shadow_sampler_1 : register(s2);
#endif
SamplerState grunge_sampler        : register(s3);
SamplerState noise_sampler         : register(s4);
SamplerState specular_sampler      : register(s5);
SamplerState specular_power_sampler : register(s6);
SamplerState diffuse_sampler       : register(s7);
SamplerState micro_sampler         : register(s8);
SamplerState height_sampler        : register(s9);

struct pixel_input {
    float3 world_position      : TEXCOORD0;
    float4 texture_coordinates : TEXCOORD1;
    half4  normal_gobo_x       : TEXCOORD2;
    half4  tangent_gobo_z      : TEXCOORD3;
    half4  binormal_gobo_y     : TEXCOORD4;
#if NGL_SM_ROAD_SHADOW_COUNT > 0
    float4 shadow_position_0   : TEXCOORD5;
#endif
#if NGL_SM_ROAD_SHADOW_COUNT > 1
    float4 shadow_position_1   : TEXCOORD6;
#endif
    half4  ambient_lo          : TEXCOORD7;
    half4  ambient_hi          : TEXCOORD8;
};

#if NGL_SM_ROAD_SHADOW_COUNT > 0
half compare_shadow(
    SamplerState shadow_sampler,
    float2 texture_coordinates,
    float depth)
{
    return tex2D(shadow_sampler, texture_coordinates).r >= depth ? 1.0 : 0.0;
}

half sample_shadow_0(float4 position) {
    float3 projected = position.xyz / position.w;
    float  depth     = saturate(projected.z);

#if NGL_SM_ROAD_SHADOW_COUNT == 1
    const float offset = 1.0 / 1024.0;

    half visibility =
        compare_shadow(shadow_sampler_0, projected.xy + float2(-offset, -offset), depth) +
        compare_shadow(shadow_sampler_0, projected.xy + float2(0.0,     -offset), depth) +
        compare_shadow(shadow_sampler_0, projected.xy + float2(offset,  -offset), depth) +
        compare_shadow(shadow_sampler_0, projected.xy + float2(-offset, 0.0), depth) +
        compare_shadow(shadow_sampler_0, projected.xy, depth) +
        compare_shadow(shadow_sampler_0, projected.xy + float2(offset,  0.0), depth) +
        compare_shadow(shadow_sampler_0, projected.xy + float2(-offset, offset), depth) +
        compare_shadow(shadow_sampler_0, projected.xy + float2(0.0,     offset), depth) +
        compare_shadow(shadow_sampler_0, projected.xy + float2(offset,  offset), depth);

    return visibility * (1.0 / 12.0) + (1.0 / 36.0);
#else
    return compare_shadow(shadow_sampler_0, projected.xy, depth);
#endif
}
#endif

#if NGL_SM_ROAD_SHADOW_COUNT > 1
half sample_shadow_1(float4 position) {
    float3 projected = position.xyz / position.w;

    return compare_shadow(
        shadow_sampler_1,
        projected.xy,
        saturate(projected.z));
}
#endif

half evaluate_shadow(pixel_input input, float eye_distance) {
#if NGL_SM_ROAD_SHADOW_COUNT == 1
    half visibility = sample_shadow_0(input.shadow_position_0);
#elif NGL_SM_ROAD_SHADOW_COUNT == 2
    half visibility_0 = sample_shadow_0(input.shadow_position_0);
    half visibility_1 = sample_shadow_1(input.shadow_position_1);
    half cascade_blend = saturate(eye_distance - shadow_distance.x);
    half visibility = lerp(visibility_0, visibility_1, cascade_blend);
    visibility = saturate(visibility * 0.84 + 0.16);
#else
    return 1.0;
#endif

#if NGL_SM_ROAD_SHADOW_COUNT > 0
    half distance_fade = saturate(
        max(eye_distance - shadow_distance.x, 0.0) /
        (shadow_distance.y - shadow_distance.x));
    visibility = lerp(visibility, 1.0, distance_fade);
    visibility = 1.0 +
        saturate(eye_distance - 4.0) * (visibility - 1.0);

    return visibility;
#endif
}

half3 evaluate_diffuse_irradiance(half3 normal) {
    half3 normal_squared = normal * normal;
    half3 normal_cross   = normal.yzx * normal;

    return
        diffuse_irradiance[0].rgb * normal_squared.x +
        diffuse_irradiance[1].rgb * normal_squared.y +
        diffuse_irradiance[2].rgb * normal_squared.z +
        diffuse_irradiance[3].rgb * normal_cross.x +
        diffuse_irradiance[4].rgb * normal_cross.y +
        diffuse_irradiance[5].rgb * normal_cross.z +
        diffuse_irradiance[6].rgb * normal.x +
        diffuse_irradiance[7].rgb * normal.y +
        diffuse_irradiance[8].rgb * normal.z;
}

half3 apply_color_matrix(float3 color) {
    return saturate(
        color_matrix[0].rgb * color.r +
        color_matrix[1].rgb * color.g +
        color_matrix[2].rgb * color.b +
        color_matrix[3].rgb);
}

float signed_height(SamplerState source_sampler, float2 coordinates) {
    return tex2D(source_sampler, coordinates).r * 2.0 - 1.0;
}

half3 evaluate_surface_normal(
    pixel_input input,
    out float micro_sample,
    out float grunge_sample)
{
    float2 micro_coordinates =
        input.world_position.xz * micro_coordinates_per_meter;
    float2 grunge_coordinates =
        input.world_position.xz * grunge_coordinates_per_meter;
    float2 height_coordinates = input.texture_coordinates.xy;

    micro_sample  = tex2D(micro_sampler, micro_coordinates).r;
    grunge_sample = tex2D(grunge_sampler, grunge_coordinates).r;
    float height_sample = tex2D(height_sampler, height_coordinates).r;

    float3 center_samples = float3(
        micro_sample * 2.0 - 1.0,
        grunge_sample * 2.0 - 1.0,
        height_sample * 2.0 - 1.0);
    float3 x_samples = float3(
        signed_height(
            micro_sampler,
            micro_coordinates + float2(micro_sampler_inverse_size.x, 0.0)),
        signed_height(
            grunge_sampler,
            grunge_coordinates + float2(grunge_sampler_inverse_size.x, 0.0)),
        signed_height(
            height_sampler,
            height_coordinates + float2(height_sampler_inverse_size.x, 0.0)));
    float3 y_samples = float3(
        signed_height(
            micro_sampler,
            micro_coordinates + float2(0.0, micro_sampler_inverse_size.y)),
        signed_height(
            grunge_sampler,
            grunge_coordinates + float2(0.0, grunge_sampler_inverse_size.y)),
        signed_height(
            height_sampler,
            height_coordinates + float2(0.0, height_sampler_inverse_size.y)));

    float height_scale = micro_sampler_inverse_size.x;
    float center_height = dot(
        center_samples,
        micro_grunge_height_scale.xyz) * height_scale;
    float x_height = dot(
        x_samples,
        micro_grunge_height_scale.xyz) * height_scale;
    float y_height = dot(
        y_samples,
        micro_grunge_height_scale.xyz) * height_scale;

    center_height = clamp(center_height, -height_scale, height_scale);
    x_height      = clamp(x_height,      -height_scale, height_scale);
    y_height      = clamp(y_height,      -height_scale, height_scale);

    float3 tangent_normal = float3(
        -(x_height - center_height) * micro_sampler_inverse_size.y,
        -(y_height - center_height) * micro_sampler_inverse_size.x,
        micro_sampler_inverse_size.x * micro_sampler_inverse_size.y);

    return normalize(
        input.tangent_gobo_z.xyz * tangent_normal.x +
        input.binormal_gobo_y.xyz * tangent_normal.y +
        input.normal_gobo_x.xyz * tangent_normal.z);
}

half3 evaluate_surface_color(
    pixel_input input,
    float micro_sample,
    float grunge_sample)
{
    half3 diffuse =
        tex2D(diffuse_sampler, input.texture_coordinates.xy).rgb *
        tweak_scale[0].x + tweak_bias[0].x;
    half grunge_blend =
        grunge_sample * tweak_scale[0].w + tweak_bias[0].w;
    half3 surface = lerp(grunge_diffuse_low, diffuse, grunge_blend);

    float2 noise_coordinates =
        input.world_position.xz * noise_coordinates_per_meter;
    half noise =
        tex2D(noise_sampler, noise_coordinates).r *
        tweak_scale[1].x + tweak_bias[1].x;
    half micro =
        micro_sample * tweak_scale[1].y + tweak_bias[1].y;

    surface *= 1.0 + (noise - 1.0) * noise_micro_diffuse_level.x;
    surface *= 1.0 + (micro - 1.0) * noise_micro_diffuse_level.y;

    return apply_color_matrix(surface);
}

half4 main(pixel_input input) : COLOR0 {
    float micro_sample;
    float grunge_sample;
    half3 normal = evaluate_surface_normal(
        input,
        micro_sample,
        grunge_sample);

    float3 eye_direction = eye - input.world_position;
    float  eye_distance  = length(eye_direction);
    eye_direction /= eye_distance;

    half shadow_visibility = evaluate_shadow(input, eye_distance);
    half depth_fog = pow(
        saturate(eye_distance * depth_fog_control.x + depth_fog_control.y),
        depth_fog_control.z);

    half4 horizon = tex2D(
        horizon_sampler,
        input.texture_coordinates.zw);
    half surface_shiny = max(0.1, horizon.g);

    half specular_power_sample = tex2D(
        specular_power_sampler,
        input.texture_coordinates.xy).r;
    half adjusted_specular_power =
        specular_power_sample * tweak_scale[0].z + tweak_bias[0].z;
    half specular_power = saturate(
        adjusted_specular_power *
        special_effects_matrix[0].w * 0.75 +
        special_effects_matrix[1].w) * 64.0;

    half adjusted_micro =
        micro_sample * tweak_scale[1].y + tweak_bias[1].y;
    half specular_sample = tex2D(
        specular_sampler,
        input.texture_coordinates.xy).r;
    half adjusted_specular =
        specular_sample * tweak_scale[0].y + tweak_bias[0].y;

    half effect_bias = saturate(special_effects_matrix[1].y);
    half ambient_scale = saturate(
        special_effects_matrix[0].z * 0.65 +
        special_effects_matrix[1].z);
    half alpha_scale = saturate(
        special_effects_matrix[2].x +
        special_effects_matrix[3].x);
    half reflection_scale = saturate(
        (adjusted_specular + adjusted_micro) *
        special_effects_matrix[2].y * 6.0 +
        special_effects_matrix[3].y);

    half3 bounce_color = horizon_sun_color * horizon_bounce_ratio;
    half3 reflection_direction = reflect(-eye_direction, normal);
    half normal_to_eye = dot(normal, eye_direction);

    half3 reflected_color =
        bounce_color * saturate(
            dot(reflection_direction, horizon_bounce_direction));
    reflected_color += horizon_sun_color * surface_shiny * pow(
        saturate(dot(reflection_direction, horizon_sun_direction)),
        specular_power);
    reflected_color *= shadow_visibility;

    half edge_fresnel = saturate(
        normal_to_eye * fresnel.y + fresnel.w);
    half reflection_fresnel = saturate(
        normal_to_eye * fresnel.x + fresnel.z);
    reflected_color *= reflection_fresnel * reflection_scale;

    half reflected_luminance = dot(
        reflected_color,
        half3(0.3, 0.59, 0.11));
    half alpha_reference = saturate(
        reflected_luminance * 0.5 + effect_bias + 1.0);
    half reflected_alpha = alpha_scale * edge_fresnel;
    half blended_alpha = lerp(
        alpha_reference,
        reflected_alpha,
        translucent_flag);
    half output_alpha = reflected_alpha + saturate(blended_alpha - 1.0);

    half3 sky_bounce = (
        bounce_color * saturate(dot(normal, horizon_bounce_direction)) +
        horizon_sky_color) * horizon.b * 1.0e-9;
    half3 direct_sun =
        horizon_sun_color *
        surface_shiny *
        saturate(dot(normal, horizon_sun_direction)) *
        horizon.b *
        1.0e-9 *
        shadow_visibility;

    half ambient_fraction = dot(ambient_direction, normal) * 0.5 + 0.5;
    half3 ambient_color = lerp(
        input.ambient_lo.rgb,
        input.ambient_hi.rgb,
        ambient_fraction);

    half3 lighting =
        effect_bias +
        direct_sun +
        sky_bounce +
        ambient_color * ambient_scale +
        evaluate_diffuse_irradiance(normal);

    lighting.b +=
        horizon_sun_angles.x *
        horizon_bounce_angles.x *
        horizon_sky_direction.x *
        horizon_night_color.x *
        1.0e-13;

    half3 surface_color = evaluate_surface_color(
        input,
        micro_sample,
        grunge_sample);
    half3 color = lighting * surface_color + reflected_color;

    color = lerp(color, depth_fog_color.rgb, depth_fog);
    color.r += fog_color.x * input.ambient_lo.a * 1.0e-10;
    color += exposure * 1.0e-8;

    return half4(color, output_alpha);
}

#endif
