#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 diffuse_irradiance[9] : register(c0);
float4 special_effects_matrix[4] : register(c9);

float3 light_position[4]  : register(c13);
float4 light_direction[4] : register(c17);
float3 light_color[4]     : register(c21);
float4 light_falloff[4]   : register(c25);

row_major float4x4 diffuse_color_matrix : register(c29);
row_major float4x4 decal_color_matrix   : register(c33);
row_major float3x3 local_to_world       : register(c37);

float3 eye                      : register(c40);
float3 ambient_direction        : register(c41);
float4 fresnel                  : register(c42);
float  translucent_flag         : register(c43);
float4 rim_lighting             : register(c44);
float4 shadow_distance          : register(c45);
float4 depth_fog_color          : register(c46);
float4 depth_fog_control        : register(c47);
float4 fog_color                : register(c48);
float  exposure                 : register(c49);
float3 horizon_sky_color        : register(c50);
float3 horizon_sky_direction    : register(c51);
float3 horizon_sun_color        : register(c52);
float3 horizon_sun_direction    : register(c53);
float2 horizon_sun_angles       : register(c54);
float3 horizon_bounce_direction : register(c55);
float2 horizon_bounce_angles    : register(c56);
float  horizon_bounce_ratio     : register(c57);
float3 horizon_night_color      : register(c58);

float4 diffuse_effect_0 : register(c59);
float4 diffuse_effect_1 : register(c60);
float4 decal_effect_0   : register(c61);
float4 decal_effect_1   : register(c62);
float4 normal_mad_0     : register(c63);
float4 normal_mad_1     : register(c64);
float2 alpha_mad_0      : register(c65);
float2 alpha_mad_1      : register(c66);
float2 alpha_mad_2      : register(c67);

SamplerState horizon_sampler          : register(s0);
SamplerState shadow_sampler_0         : register(s1);
SamplerState shadow_sampler_1         : register(s2);
SamplerState environment_sampler      : register(s3);
SamplerState gobo_sampler             : register(s4);
SamplerState diffuse_specular_sampler : register(s5);
SamplerState diffuse_normal_sampler   : register(s6);
SamplerState decal_alpha_sampler      : register(s7);
SamplerState decal_normal_sampler     : register(s8);

struct pixel_input {
    half3 local_position      : TEXCOORD0;
    half4 texture_coordinates : TEXCOORD1;
    float4 tangent_gobo_z     : TEXCOORD2;
    float4 binormal_gobo_y    : TEXCOORD3;
    float4 normal_gobo_x      : TEXCOORD4;
    half4 ambient_lo          : TEXCOORD5;
    half3 ambient_hi          : TEXCOORD6;
    half2 horizon_coordinates : TEXCOORD7;
    half4 shadow_position_0   : TEXCOORD8;
    half4 shadow_position_1   : TEXCOORD9;
};

struct local_light {
    float3 direction;
    half3  color;
};

local_light evaluate_local_light(int index, float3 position) {
    local_light output;

    float3 to_light = light_position[index] - position;
    float  distance_squared = dot(to_light, to_light);
    output.direction = normalize(to_light);

    half distance_attenuation = saturate(
        distance_squared * light_falloff[index].x +
        light_falloff[index].y);
    half directional_attenuation = saturate(
        dot(output.direction, light_direction[index].xyz) *
        light_falloff[index].z +
        light_falloff[index].w);
    directional_attenuation = max(
        directional_attenuation,
        light_direction[index].w);

    output.color =
        light_color[index] *
        distance_attenuation *
        directional_attenuation;

    return output;
}

half compare_shadow(SamplerState shadow_map, half4 position) {
    half3 projected_position = position.xyz / position.w;
    half  depth = saturate(projected_position.z);

    return tex2D(shadow_map, projected_position.xy).r >= depth ? 1.0 : 0.0;
}

half evaluate_shadow(pixel_input input, float eye_distance) {
    half visibility_0 = compare_shadow(
        shadow_sampler_0,
        input.shadow_position_0);
    half visibility_1 = compare_shadow(
        shadow_sampler_1,
        input.shadow_position_1);

    half cascade_blend = saturate(eye_distance - shadow_distance.x);
    half visibility = lerp(visibility_0, visibility_1, cascade_blend);
    visibility = saturate(visibility * 0.84 + 0.16);

    half distance_fade = saturate(
        max(eye_distance - shadow_distance.x, 0.0) /
        (shadow_distance.y - shadow_distance.x));
    visibility = lerp(visibility, 1.0, distance_fade);
    visibility = 1.0 +
        saturate(eye_distance - 4.0) * (visibility - 1.0);

    return visibility;
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

half3 apply_color_matrix(float3 color, row_major float4x4 color_transform) {
    return saturate(
        color_transform[0].rgb * color.r +
        color_transform[1].rgb * color.g +
        color_transform[2].rgb * color.b +
        color_transform[3].rgb);
}

half evaluate_specular(half3 reflection_direction, local_light light, half power) {
    half reflection = saturate(dot(reflection_direction, light.direction));

    return power > 0.0 ? pow(reflection, power) : 1.0;
}

half4 main(pixel_input input) : COLOR0 {
    local_light lights[4];
    lights[0] = evaluate_local_light(0, input.local_position);
    lights[1] = evaluate_local_light(1, input.local_position);
    lights[2] = evaluate_local_light(2, input.local_position);
    lights[3] = evaluate_local_light(3, input.local_position);

    float3 gobo_position = float3(
        input.normal_gobo_x.w,
        input.binormal_gobo_y.w,
        input.tangent_gobo_z.w);
    float2 gobo_coordinates =
        gobo_position.xz / gobo_position.y + 0.5;
    lights[1].color *= tex2D(gobo_sampler, gobo_coordinates).rgb;

    float3 eye_direction = eye - input.local_position;
    float  eye_distance  = length(eye_direction);
    eye_direction /= eye_distance;

    half shadow_visibility = evaluate_shadow(input, eye_distance);
    half depth_fog = pow(
        saturate(eye_distance * depth_fog_control.x + depth_fog_control.y),
        depth_fog_control.z);

    float4 diffuse_normal_sample = tex2D(
        diffuse_normal_sampler,
        input.texture_coordinates.xy);
    float4 decal_normal_sample = tex2D(
        decal_normal_sampler,
        input.texture_coordinates.zw);
    float4 decal_alpha_sample = tex2D(
        decal_alpha_sampler,
        input.texture_coordinates.zw);

    half2 diffuse_normal =
        diffuse_normal_sample.ag * normal_mad_0.xy + normal_mad_0.zw;
    half2 decal_normal =
        decal_normal_sample.xy * normal_mad_1.xy + normal_mad_1.zw;
    half normal_blend =
        decal_alpha_sample.a * alpha_mad_2.x + alpha_mad_2.y;
    half2 tangent_normal = -lerp(
        diffuse_normal,
        decal_normal,
        normal_blend);
    half normal_height = sqrt(1.0 - dot(tangent_normal, tangent_normal));

    half3 normal = normalize(
        input.tangent_gobo_z.xyz * tangent_normal.x +
        input.binormal_gobo_y.xyz * tangent_normal.y +
        input.normal_gobo_x.xyz * normal_height);
    half3 reflection_direction = reflect(-eye_direction, normal);
    half normal_to_eye = dot(normal, eye_direction);

    half material_blend =
        decal_alpha_sample.a * alpha_mad_1.x + alpha_mad_1.y;
    float4 material_effect_0 = lerp(
        diffuse_effect_0,
        decal_effect_0,
        material_blend);
    float material_effect_1 = lerp(
        diffuse_effect_1.x,
        decal_effect_1.x,
        material_blend);

    half effect_bias = saturate(
        material_effect_1 * special_effects_matrix[0].y +
        special_effects_matrix[1].y);
    half ambient_scale = saturate(
        special_effects_matrix[0].z + special_effects_matrix[1].z);
    half specular_power = saturate(
        material_effect_0.z * special_effects_matrix[0].w +
        special_effects_matrix[1].w) * 64.0;

    half4 horizon = tex2D(horizon_sampler, input.horizon_coordinates);
    half surface_shiny = max(0.1, horizon.g);
    half3 bounce_color = horizon_sun_color * horizon_bounce_ratio;

    half3 reflected_color =
        lights[0].color * evaluate_specular(
            reflection_direction,
            lights[0],
            specular_power) +
        lights[1].color * evaluate_specular(
            reflection_direction,
            lights[1],
            specular_power) +
        lights[2].color * evaluate_specular(
            reflection_direction,
            lights[2],
            specular_power) +
        lights[3].color * evaluate_specular(
            reflection_direction,
            lights[3],
            specular_power);

    half3 horizon_reflection =
        bounce_color * saturate(
            dot(reflection_direction, horizon_bounce_direction));
    horizon_reflection += horizon_sun_color * surface_shiny * pow(
        saturate(dot(reflection_direction, horizon_sun_direction)),
        specular_power);
    reflected_color += horizon_reflection * shadow_visibility;

    half edge_fresnel = saturate(
        normal_to_eye * fresnel.y + fresnel.w);
    half reflection_fresnel = saturate(
        normal_to_eye * fresnel.x + fresnel.z);

    float4 diffuse_specular_sample = tex2D(
        diffuse_specular_sampler,
        input.texture_coordinates.xy);
    half adjusted_diffuse_alpha =
        diffuse_specular_sample.a * alpha_mad_0.x + alpha_mad_0.y;
    half reflection_input = lerp(
        adjusted_diffuse_alpha,
        decal_effect_0.y,
        material_blend);
    material_effect_0.y *= reflection_input;

    half alpha_scale = saturate(
        material_effect_0.x * special_effects_matrix[2].x +
        special_effects_matrix[3].x);
    half reflection_scale = saturate(
        material_effect_0.y * special_effects_matrix[2].y +
        special_effects_matrix[3].y);
    half environment_scale = saturate(
        material_effect_0.w * special_effects_matrix[2].z +
        special_effects_matrix[3].z);

    reflected_color *= reflection_fresnel * reflection_scale;

    half reflected_luminance = dot(reflected_color, half3(0.3, 0.59, 0.11));
    half alpha_reference = saturate(
        reflected_luminance * 0.5 + effect_bias + 1.0);
    half reflected_alpha = alpha_scale * edge_fresnel;
    half blended_alpha = lerp(
        alpha_reference,
        reflected_alpha,
        translucent_flag);
    half output_alpha = reflected_alpha + saturate(blended_alpha - 1.0);

    half3 diffuse_color = apply_color_matrix(
        diffuse_specular_sample.rgb,
        diffuse_color_matrix);
    half3 decal_color = apply_color_matrix(
        decal_alpha_sample.rgb,
        decal_color_matrix);
    half3 surface_color = lerp(
        diffuse_color,
        decal_color,
        material_blend);

    half3 direct_lighting =
        lights[0].color * saturate(dot(normal, lights[0].direction)) +
        lights[1].color * saturate(dot(normal, lights[1].direction)) +
        lights[2].color * saturate(dot(normal, lights[2].direction)) +
        lights[3].color * saturate(dot(normal, lights[3].direction));

    half3 horizon_sun =
        horizon_sun_color *
        surface_shiny *
        saturate(dot(normal, horizon_sun_direction)) *
        horizon.b *
        1.0e-9 *
        shadow_visibility;

    half3 sky_bounce = (
        bounce_color * saturate(dot(normal, horizon_bounce_direction)) +
        horizon_sky_color) * horizon.b * 1.0e-9;

    half ambient_fraction = dot(ambient_direction, normal) * 0.5 + 0.5;
    half3 ambient_color = lerp(
        input.ambient_lo.rgb,
        input.ambient_hi,
        ambient_fraction);

    half3 lighting =
        effect_bias +
        direct_lighting +
        horizon_sun +
        sky_bounce +
        ambient_color * ambient_scale +
        evaluate_diffuse_irradiance(normal);

    lighting.b +=
        horizon_sun_angles.x *
        horizon_bounce_angles.x *
        horizon_sky_direction.x *
        horizon_night_color.x *
        1.0e-13;

    half rim_factor = pow(1.0 - abs(normal_to_eye), rim_lighting.w);
    half3 rim_color =
        (lights[0].color + lights[1].color +
         lights[2].color + lights[3].color) *
        rim_factor *
        ambient_scale *
        rim_lighting.rgb;

    half3 color = lighting * surface_color + reflected_color + rim_color;

    half3 environment_direction = mul(reflection_direction, local_to_world);
    half3 environment = texCUBE(
        environment_sampler,
        environment_direction).rgb;
    color += environment * reflection_fresnel * environment_scale;

    color = lerp(color, depth_fog_color.rgb, depth_fog);
    half fog_dependency = fog_color.x * input.ambient_lo.a;
    color.r += fog_dependency * 1.0e-10;
    color += exposure * 1.0e-8;

    return half4(color, output_alpha);
}

#endif
