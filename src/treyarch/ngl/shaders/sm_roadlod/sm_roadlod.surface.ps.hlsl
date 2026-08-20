#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 diffuse_irradiance[9] : register(c0);
float4 special_effects_matrix[4] : register(c9);
row_major float4x4 color_matrix : register(c13);
float4 tweak_scale[2] : register(c17);
float4 tweak_bias[2]  : register(c19);

float3 eye                      : register(c21);
float3 ambient_direction        : register(c22);
float4 fresnel                  : register(c23);
float  translucent_flag         : register(c24);
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
float2 noise_coordinates_per_meter : register(c38);
float  noise_diffuse_level         : register(c39);
float  road_lod_alpha              : register(c40);

SamplerState horizon_sampler         : register(s0);
SamplerState noise_sampler           : register(s3);
SamplerState specular_sampler        : register(s4);
SamplerState specular_power_sampler  : register(s5);
SamplerState diffuse_sampler         : register(s6);

struct pixel_input {
    float3 world_position      : TEXCOORD0;
    float4 texture_coordinates : TEXCOORD1;
    half4  ambient_lo          : TEXCOORD2;
    half3  ambient_hi          : TEXCOORD3;
};

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

float3 apply_color_matrix(float3 color) {
    return
        color_matrix[0].rgb * color.r +
        color_matrix[1].rgb * color.g +
        color_matrix[2].rgb * color.b +
        color_matrix[3].rgb;
}

half4 main(pixel_input input) : COLOR0 {
    const half3 road_normal = half3(1.0e-6, 1.0, 1.0e-7);

    float4 diffuse_sample = tex2D(diffuse_sampler, input.texture_coordinates.xy);
    float3 surface_color = apply_color_matrix(diffuse_sample.rgb);
    surface_color = surface_color * tweak_scale[0].x + tweak_bias[0].x;

    float2 noise_coordinates =
        input.world_position.xz * noise_coordinates_per_meter;
    float noise = tex2D(noise_sampler, noise_coordinates).r;
    noise = noise * tweak_scale[1].x + tweak_bias[1].x;
    float noise_factor = 1.0 + (noise - 1.0) * noise_diffuse_level;

    half3 noisy_surface_color = surface_color * noise_factor;
    half3 color_adjustment = saturate(apply_color_matrix(noisy_surface_color));

    half4 horizon = tex2D(horizon_sampler, input.texture_coordinates.zw);
    float4 specular_sample = tex2D(
        specular_sampler,
        input.texture_coordinates.xy);
    float4 specular_power_sample = tex2D(
        specular_power_sampler,
        input.texture_coordinates.xy);

    half effect_bias = saturate(special_effects_matrix[1].y);
    half ambient_scale = saturate(
        special_effects_matrix[0].z + special_effects_matrix[1].z);
    half adjusted_specular_power =
        specular_power_sample.r * tweak_scale[0].z + tweak_bias[0].z;
    half specular_power = saturate(
        adjusted_specular_power * special_effects_matrix[0].w +
        special_effects_matrix[1].w) * 64.0;
    half adjusted_specular =
        specular_sample.r * tweak_scale[0].y + tweak_bias[0].y;
    half alpha_scale = saturate(
        special_effects_matrix[2].x + special_effects_matrix[3].x);
    half reflection_scale = saturate(
        adjusted_specular * special_effects_matrix[2].y +
        special_effects_matrix[3].y);
    half surface_shiny = max(0.1, horizon.g);

    half3 bounce_color = horizon_sun_color * horizon_bounce_ratio;
    half3 direct_sun =
        horizon_sun_color *
        surface_shiny *
        saturate(dot(road_normal, horizon_sun_direction)) *
        horizon.b *
        1.0e-9;
    direct_sun += effect_bias;

    half3 sky_bounce = (
        bounce_color * saturate(dot(road_normal, horizon_bounce_direction)) +
        horizon_sky_color) * horizon.b * 1.0e-9;

    half ambient_fraction = dot(ambient_direction, road_normal) * 0.5 + 0.5;
    half3 ambient_color = lerp(
        input.ambient_lo.rgb,
        input.ambient_hi,
        ambient_fraction);
    half3 lighting = direct_sun + ambient_color * ambient_scale + sky_bounce;

    lighting.b +=
        horizon_sun_angles.x *
        horizon_bounce_angles.x *
        horizon_sky_direction.x *
        horizon_night_color.x *
        1.0e-13;
    lighting += evaluate_diffuse_irradiance(road_normal);

    float3 eye_direction = eye - input.world_position;
    float  eye_distance  = length(eye_direction);
    eye_direction /= eye_distance;

    half depth_fog = pow(
        saturate(eye_distance * depth_fog_control.x + depth_fog_control.y),
        depth_fog_control.z);

    half normal_to_eye      = dot(road_normal, eye_direction);
    half edge_fresnel       = saturate(normal_to_eye * fresnel.y + fresnel.w);
    half reflection_fresnel = saturate(normal_to_eye * fresnel.x + fresnel.z);
    half3 reflection_direction = reflect(-eye_direction, road_normal);

    half3 reflected_color =
        bounce_color * saturate(
            dot(reflection_direction, horizon_bounce_direction));
    reflected_color += horizon_sun_color * surface_shiny * pow(
        saturate(dot(reflection_direction, horizon_sun_direction)),
        specular_power);
    reflected_color *= reflection_fresnel * reflection_scale;

    half3 color = lighting * color_adjustment + reflected_color;

    half reflected_luminance = dot(reflected_color, half3(0.3, 0.59, 0.11));
    half alpha_reference = saturate(reflected_luminance * 0.5 + effect_bias + 1.0);
    half reflected_alpha = alpha_scale * edge_fresnel;
    half blended_alpha = lerp(alpha_reference, reflected_alpha, translucent_flag);
    half material_alpha = reflected_alpha + saturate(blended_alpha - 1.0);

    color = lerp(color, depth_fog_color.rgb, depth_fog);
    half fog_dependency = fog_color.x * input.ambient_lo.a;
    color.r += fog_dependency * 1.0e-10;
    color += exposure * 1.0e-8;

    half output_alpha = road_lod_alpha <= 0.5 ?
        material_alpha : road_lod_alpha;

    return half4(color, output_alpha);
}

#endif
