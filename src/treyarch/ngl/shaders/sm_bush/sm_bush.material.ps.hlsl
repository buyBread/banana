#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 diffuse_irradiance[9] : register(c0);
float4 special_effects_matrix[4] : register(c9);

float3 light_position[4]  : register(c13);
float4 light_direction[4] : register(c17);
float3 light_color[4]     : register(c21);
float4 light_falloff[4]   : register(c25);

row_major float4x4 diffuse_color_matrix : register(c29);
row_major float3x3 local_to_world       : register(c33);

float3 eye               : register(c36);
float3 ambient_direction : register(c37);
float4 fresnel           : register(c38);

#ifdef NGL_SM_BUSH_SHADOWED
float  translucent_flag         : register(c39);
float4 rim_lighting             : register(c40);
float4 shadow_distance          : register(c41);
float4 depth_fog_color          : register(c42);
float4 depth_fog_control        : register(c43);
float4 fog_color                : register(c44);
float  exposure                 : register(c45);
float3 horizon_sky_color        : register(c46);
float3 horizon_sky_direction    : register(c47);
float3 horizon_sun_color        : register(c48);
float3 horizon_sun_direction    : register(c49);
float2 horizon_sun_angles       : register(c50);
float3 horizon_bounce_direction : register(c51);
float2 horizon_bounce_angles    : register(c52);
float  horizon_bounce_ratio     : register(c53);
float3 horizon_night_color      : register(c54);
#else
float4 rim_lighting             : register(c39);
float4 depth_fog_color          : register(c40);
float4 depth_fog_control        : register(c41);
float4 fog_color                : register(c42);
float  exposure                 : register(c43);
float3 horizon_sky_color        : register(c44);
float3 horizon_sky_direction    : register(c45);
float3 horizon_sun_color        : register(c46);
float3 horizon_sun_direction    : register(c47);
float2 horizon_sun_angles       : register(c48);
float3 horizon_bounce_direction : register(c49);
float2 horizon_bounce_angles    : register(c50);
float  horizon_bounce_ratio     : register(c51);
float3 horizon_night_color      : register(c52);
#endif

SamplerState horizon_sampler          : register(s0);
#ifdef NGL_SM_BUSH_SHADOWED
SamplerState shadow_sampler           : register(s1);
#endif
SamplerState environment_sampler      : register(s3);
SamplerState gobo_sampler             : register(s4);
SamplerState diffuse_specular_sampler : register(s5);

struct pixel_input {
    half3  local_position    : TEXCOORD0;
    float4 shadow_position   : TEXCOORD1;
    half4  texcoord          : TEXCOORD3;
    half4  tangent_gobo_z    : TEXCOORD4;
    half4  binormal_gobo_y   : TEXCOORD5;
    half4  normal_gobo_x     : TEXCOORD6;
    half4  ambient_lo        : TEXCOORD7;
    half4  ambient_hi        : TEXCOORD8;
    half2  horizon_texcoord  : TEXCOORD9;
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

#ifdef NGL_SM_BUSH_SHADOWED
half compare_shadow(float2 texcoord, float depth) {
    return tex2D(shadow_sampler, texcoord).r >= depth ? 1.0 : 0.0;
}

half sample_shadow(float4 position) {
    const float offset = 1.0 / 1024.0;
    float3 projected = position.xyz / position.w;
    float  depth = saturate(projected.z);

    half visibility =
        compare_shadow(projected.xy + float2(-offset, -offset), depth) +
        compare_shadow(projected.xy + float2(0.0,     -offset), depth) +
        compare_shadow(projected.xy + float2(offset,  -offset), depth) +
        compare_shadow(projected.xy + float2(-offset, 0.0), depth) +
        compare_shadow(projected.xy, depth) +
        compare_shadow(projected.xy + float2(offset,  0.0), depth) +
        compare_shadow(projected.xy + float2(-offset, offset), depth) +
        compare_shadow(projected.xy + float2(0.0,     offset), depth) +
        compare_shadow(projected.xy + float2(offset,  offset), depth);

    return visibility * (1.0 / 12.0) + (1.0 / 36.0);
}
#endif

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
        diffuse_color_matrix[0].rgb * color.r +
        diffuse_color_matrix[1].rgb * color.g +
        diffuse_color_matrix[2].rgb * color.b +
        diffuse_color_matrix[3].rgb);
}

half evaluate_specular(
    half3 reflection_direction,
    local_light light,
    half power)
{
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

    half3 normal = input.normal_gobo_x.xyz;
    float3 eye_direction = eye - input.local_position;
    float  eye_distance  = length(eye_direction);
    eye_direction /= eye_distance;

#ifdef NGL_SM_BUSH_SHADOWED
    half shadow_visibility = sample_shadow(input.shadow_position);
    half shadow_fade = saturate(
        max(eye_distance - shadow_distance.x, 0.0) /
        (shadow_distance.y - shadow_distance.x));
    shadow_visibility = lerp(shadow_visibility, 1.0, shadow_fade);
    shadow_visibility = 1.0 +
        saturate(eye_distance - 4.0) * (shadow_visibility - 1.0);
#else
    half shadow_visibility = 1.0;
#endif

    half depth_fog = pow(
        saturate(eye_distance * depth_fog_control.x + depth_fog_control.y),
        depth_fog_control.z);

    half effect_bias = saturate(special_effects_matrix[1].y);
    half ambient_scale = saturate(
        special_effects_matrix[0].z + special_effects_matrix[1].z);
    half specular_power = saturate(
        special_effects_matrix[0].w + special_effects_matrix[1].w) * 64.0;

    half4 horizon = tex2D(horizon_sampler, input.horizon_texcoord);
    half surface_shiny = max(0.1, horizon.g);
    half3 bounce_color = horizon_sun_color * horizon_bounce_ratio;

    half3 reflection_direction = reflect(-eye_direction, normal);
    half normal_to_eye = dot(normal, eye_direction);

    half3 reflected_color =
        lights[0].color * evaluate_specular(
            reflection_direction, lights[0], specular_power) +
        lights[1].color * evaluate_specular(
            reflection_direction, lights[1], specular_power) +
        lights[2].color * evaluate_specular(
            reflection_direction, lights[2], specular_power) +
        lights[3].color * evaluate_specular(
            reflection_direction, lights[3], specular_power);

    half3 horizon_reflection =
        bounce_color * saturate(
            dot(reflection_direction, horizon_bounce_direction));
    horizon_reflection += horizon_sun_color * surface_shiny * pow(
        saturate(dot(reflection_direction, horizon_sun_direction)),
        specular_power);
    reflected_color += horizon_reflection * shadow_visibility;

    half reflection_fresnel = saturate(
        normal_to_eye * fresnel.x + fresnel.z);
    half reflection_scale = saturate(special_effects_matrix[3].y);
    reflected_color *= reflection_fresnel * reflection_scale;

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
        input.ambient_hi.rgb,
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

    half4 diffuse_specular = tex2D(
        diffuse_specular_sampler,
        input.texcoord.xy);
    half3 surface_color = apply_color_matrix(diffuse_specular.rgb);
    half3 color = lighting * surface_color + reflected_color + rim_color;

    half environment_scale = saturate(special_effects_matrix[3].z);
    half3 environment_direction = mul(reflection_direction, local_to_world);
    half3 environment = texCUBE(
        environment_sampler,
        environment_direction).rgb;
    color += environment * environment_scale;

    color = lerp(color, depth_fog_color.rgb, depth_fog);
    color.r += fog_color.x * input.ambient_lo.a * 1.0e-10;
    color += exposure * 1.0e-8;

#ifdef NGL_SM_BUSH_SHADOWED
    half edge_fresnel = saturate(normal_to_eye * fresnel.y + fresnel.w);
    half alpha_scale = saturate(
        diffuse_specular.a * special_effects_matrix[2].x +
        special_effects_matrix[3].x);
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
#else
    half output_alpha = diffuse_specular.a;
#endif

    return half4(color, output_alpha);
}

#endif
