#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 diffuse_irradiance[9] : register(c0);
float4 special_effects_matrix[4] : register(c9);
row_major float4x4 color_matrix : register(c13);
row_major float3x3 local_to_world_rotation : register(c17);

float3 eye                      : register(c20);
float3 ambient_direction        : register(c21);
float4 fresnel                  : register(c22);
float  translucent_flag         : register(c23);
float4 shadow_distance          : register(c24);
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

SamplerState horizon_sampler    : register(s0);
SamplerState shadow_sampler     : register(s1);
SamplerState environment_sampler : register(s3);
SamplerState diffuse_sampler    : register(s4);
SamplerState shininess_sampler  : register(s5);
SamplerState grunge_sampler     : register(s6);
SamplerState normal_sampler     : register(s7);

struct pixel_input {
    half3 local_position      : TEXCOORD0;
    half4 texture_coordinates : TEXCOORD1;
    half  material_parameter  : TEXCOORD2;
    half4 normal_horizon_u    : TEXCOORD3;
    half4 tangent_horizon_v   : TEXCOORD4;
    half3 binormal            : TEXCOORD5;
    float3 shadow_position    : TEXCOORD6;
    half4 ambient_lo          : COLOR0;
    half3 ambient_hi          : COLOR1;
    float  lod_crossfade      : TEXCOORD7;
};

half compare_shadow(float2 texcoord, float depth) {
    return tex2D(shadow_sampler, texcoord).r >= depth ? 1.0 : 0.0;
}

half sample_shadow(float3 position) {
    const float offset = 1.0 / 1024.0;
    float depth = saturate(position.z);

    half visibility =
        compare_shadow(position.xy + float2(-offset, -offset), depth) +
        compare_shadow(position.xy + float2(0.0,     -offset), depth) +
        compare_shadow(position.xy + float2(offset,  -offset), depth) +
        compare_shadow(position.xy + float2(-offset, 0.0), depth) +
        compare_shadow(position.xy, depth) +
        compare_shadow(position.xy + float2(offset,  0.0), depth) +
        compare_shadow(position.xy + float2(-offset, offset), depth) +
        compare_shadow(position.xy + float2(0.0,     offset), depth) +
        compare_shadow(position.xy + float2(offset,  offset), depth);

    return visibility * (1.0 / 12.0) + (1.0 / 36.0);
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

half4 main(pixel_input input) : COLOR0 {
    half shadow_visibility = sample_shadow(input.shadow_position);

    float3 eye_direction = eye - input.local_position;
    float  eye_distance  = length(eye_direction);
    eye_direction /= eye_distance;

    half shadow_fade = saturate(
        max(eye_distance - shadow_distance.x, 0.0) /
        (shadow_distance.y - shadow_distance.x));
    shadow_visibility = lerp(shadow_visibility, 1.0, shadow_fade);
    shadow_visibility = 1.0 +
        saturate(eye_distance - 4.0) * (shadow_visibility - 1.0);

    half depth_fog = pow(
        saturate(eye_distance * depth_fog_control.x + depth_fog_control.y),
        depth_fog_control.z);

    half4 horizon = tex2D(
        horizon_sampler,
        half2(input.normal_horizon_u.w, input.tangent_horizon_v.w));

    half4 normal_sample = tex2D(normal_sampler, input.texture_coordinates.xy);
    half2 tangent_normal = normal_sample.ag * 2.0 - 1.0;
    half  normal_height  = sqrt(1.0 - dot(tangent_normal, tangent_normal));
    half3 normal = normalize(
        input.tangent_horizon_v.xyz * tangent_normal.x +
        input.binormal * tangent_normal.y +
        input.normal_horizon_u.xyz * normal_height);

    half4 shininess = tex2D(shininess_sampler, input.texture_coordinates.xy);

    half effect_bias = saturate(special_effects_matrix[1].y);
    half ambient_scale = saturate(
        input.material_parameter * special_effects_matrix[0].y +
        special_effects_matrix[1].z);
    half specular_power = saturate(
        shininess.g * special_effects_matrix[0].z +
        special_effects_matrix[1].w) * 64.0;
    half surface_shiny = max(0.1, horizon.g);

    half3 direct_sun =
        horizon_sun_color *
        surface_shiny *
        saturate(dot(normal, horizon_sun_direction)) *
        horizon.b *
        1.0e-9;
    direct_sun = direct_sun * shadow_visibility + effect_bias;

    half3 bounce_color = horizon_sun_color * horizon_bounce_ratio;
    half3 sky_bounce = (
        bounce_color * saturate(dot(normal, horizon_bounce_direction)) +
        horizon_sky_color) * horizon.b * 1.0e-9;

    half ambient_fraction = dot(ambient_direction, normal) * 0.5 + 0.5;
    half3 ambient_color = lerp(input.ambient_lo.rgb, input.ambient_hi, ambient_fraction);
    half3 lighting = direct_sun + ambient_color * ambient_scale + sky_bounce;

    lighting.b +=
        horizon_sun_angles.x *
        horizon_bounce_angles.x *
        horizon_sky_direction.x *
        horizon_night_color.x *
        1.0e-13;

    lighting += evaluate_diffuse_irradiance(normal);

    half4 grunge_detail = tex2D(
        grunge_sampler,
        input.texture_coordinates.zw * 8.0);
    half4 grunge_base = tex2D(grunge_sampler, input.texture_coordinates.zw);
    half3 grunge_mask = saturate(1.0 - grunge_detail.rgb * grunge_base.a * 4.0);

    half4 diffuse = tex2D(diffuse_sampler, input.texture_coordinates.xy);
    half3 color_adjustment = saturate(
        color_matrix[0].rgb * diffuse.r +
        color_matrix[1].rgb * diffuse.g +
        color_matrix[2].rgb * diffuse.b +
        color_matrix[3].rgb);
    half3 surface_color = grunge_mask * color_adjustment;

    half alpha_scale = saturate(
        diffuse.a * special_effects_matrix[2].x +
        special_effects_matrix[3].x);
    half reflection_scale = saturate(
        shininess.r * special_effects_matrix[2].y +
        special_effects_matrix[3].y);
    half environment_scale = saturate(
        shininess.a * special_effects_matrix[2].z +
        special_effects_matrix[3].z);

    half normal_to_eye      = dot(normal, eye_direction);
    half edge_fresnel       = saturate(normal_to_eye * fresnel.y + fresnel.w);
    half reflection_fresnel = saturate(normal_to_eye * fresnel.x + fresnel.z);
    half3 reflection_direction = reflect(-eye_direction, normal);

    half3 reflected_color =
        bounce_color * saturate(dot(reflection_direction, horizon_bounce_direction));
    reflected_color += horizon_sun_color * surface_shiny * pow(
        saturate(dot(reflection_direction, horizon_sun_direction)),
        specular_power);
    reflected_color *= shadow_visibility * reflection_fresnel * reflection_scale;

    half3 color = lighting * surface_color + reflected_color;

    half3 environment_direction = mul(reflection_direction, local_to_world_rotation);
    half3 environment = texCUBE(environment_sampler, environment_direction).rgb;
    color += environment * environment_scale;

    half reflected_luminance = dot(reflected_color, half3(0.3, 0.59, 0.11));
    half alpha_reference = saturate(reflected_luminance * 0.5 + effect_bias + 1.0);
    half reflected_alpha = alpha_scale * edge_fresnel;
    half blended_alpha = lerp(alpha_reference, reflected_alpha, translucent_flag);
    half material_alpha = reflected_alpha + saturate(blended_alpha - 1.0);

    color = lerp(color, depth_fog_color.rgb, depth_fog);
    color.r += fog_color.x * input.ambient_lo.a * 1.0e-10;
    color += exposure * 1.0e-8;

    half output_alpha = saturate(material_alpha - 200.0) + saturate(input.lod_crossfade);

    return half4(color, output_alpha);
}

#endif
