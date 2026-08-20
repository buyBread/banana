#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 diffuse_irradiance[9]    : register(c0);
float4 special_effects_matrix[4] : register(c9);

float3 light_position[4]  : register(c13);
float4 light_direction[4] : register(c17);
float3 light_color[4]     : register(c21);
float4 light_falloff[4]   : register(c25);

row_major float4x4 color_matrix  : register(c29);
row_major float3x3 local_to_world : register(c33);

#if NGL_SM_PALETTE_STATIC_HEIGHT_NORMAL
float3 eye               : register(c36);
float3 ambient_direction : register(c37);
float4 fresnel           : register(c38);
float translucent_flag   : register(c39);
float4 rim_lighting      : register(c40);

#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT > 0
float4 shadow_distance             : register(c41);
float4 depth_fog_color             : register(c42);
float4 depth_fog_control           : register(c43);
float4 fog_color                   : register(c44);
float exposure                     : register(c45);
float3 horizon_sky_color           : register(c46);
float3 horizon_sky_direction       : register(c47);
float3 horizon_sun_color           : register(c48);
float3 horizon_sun_direction       : register(c49);
float2 horizon_sun_angles          : register(c50);
float3 horizon_bounce_direction    : register(c51);
float2 horizon_bounce_angles       : register(c52);
float horizon_bounce_ratio         : register(c53);
float3 horizon_night_color         : register(c54);
float micro                        : register(c55);
float inverse_micro                : register(c56);
float2 height_sampler_inverse_size : register(c57);
float3 normal_adjuster             : register(c58);
float4 force_color                 : register(c59);
float4 palette_v                   : register(c60);
float4 palette_u0                  : register(c61);
float4 palette_u1                  : register(c62);
float4 texel_size                  : register(c63);
#else
float4 depth_fog_color             : register(c41);
float4 depth_fog_control           : register(c42);
float4 fog_color                   : register(c43);
float exposure                     : register(c44);
float3 horizon_sky_color           : register(c45);
float3 horizon_sky_direction       : register(c46);
float3 horizon_sun_color           : register(c47);
float3 horizon_sun_direction       : register(c48);
float2 horizon_sun_angles          : register(c49);
float3 horizon_bounce_direction    : register(c50);
float2 horizon_bounce_angles       : register(c51);
float horizon_bounce_ratio         : register(c52);
float3 horizon_night_color         : register(c53);
float micro                        : register(c54);
float inverse_micro                : register(c55);
float2 height_sampler_inverse_size : register(c56);
float3 normal_adjuster             : register(c57);
float4 force_color                 : register(c58);
float4 palette_v                   : register(c59);
float4 palette_u0                  : register(c60);
float4 palette_u1                  : register(c61);
float4 texel_size                  : register(c62);
#endif
#elif NGL_SM_PALETTE_HEIGHT_NORMAL
float4 force_color      : register(c36);
float3 eye              : register(c37);
float3 ambient_direction : register(c38);
float4 fresnel           : register(c39);
float translucent_flag   : register(c40);
float4 rim_lighting       : register(c41);

#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT > 0
float4 shadow_distance             : register(c42);
float4 depth_fog_color             : register(c43);
float4 depth_fog_control           : register(c44);
float4 fog_color                   : register(c45);
float exposure                     : register(c46);
float3 horizon_sky_color           : register(c47);
float3 horizon_sky_direction       : register(c48);
float3 horizon_sun_color           : register(c49);
float3 horizon_sun_direction       : register(c50);
float2 horizon_sun_angles          : register(c51);
float3 horizon_bounce_direction    : register(c52);
float2 horizon_bounce_angles       : register(c53);
float horizon_bounce_ratio         : register(c54);
float3 horizon_night_color         : register(c55);
float micro                        : register(c56);
float inverse_micro                : register(c57);
float2 height_sampler_inverse_size : register(c58);
float3 normal_adjuster             : register(c59);
float4 palette_v                   : register(c60);
float4 palette_u0                  : register(c61);
float4 palette_u1                  : register(c62);
float4 texel_size                  : register(c63);
#else
float4 depth_fog_color             : register(c42);
float4 depth_fog_control           : register(c43);
float4 fog_color                   : register(c44);
float exposure                     : register(c45);
float3 horizon_sky_color           : register(c46);
float3 horizon_sky_direction       : register(c47);
float3 horizon_sun_color           : register(c48);
float3 horizon_sun_direction       : register(c49);
float2 horizon_sun_angles          : register(c50);
float3 horizon_bounce_direction    : register(c51);
float2 horizon_bounce_angles       : register(c52);
float horizon_bounce_ratio         : register(c53);
float3 horizon_night_color         : register(c54);
float micro                        : register(c55);
float inverse_micro                : register(c56);
float2 height_sampler_inverse_size : register(c57);
float3 normal_adjuster             : register(c58);
float4 palette_v                   : register(c59);
float4 palette_u0                  : register(c60);
float4 palette_u1                  : register(c61);
float4 texel_size                  : register(c62);
#endif
#else
float3 eye               : register(c36);
float3 ambient_direction : register(c37);
float4 fresnel           : register(c38);
float translucent_flag   : register(c39);
float4 rim_lighting      : register(c40);

#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT > 0
float4 shadow_distance          : register(c41);
float4 depth_fog_color          : register(c42);
float4 depth_fog_control        : register(c43);
float4 fog_color                : register(c44);
float exposure                  : register(c45);
float3 horizon_sky_color        : register(c46);
float3 horizon_sky_direction    : register(c47);
float3 horizon_sun_color        : register(c48);
float3 horizon_sun_direction    : register(c49);
float2 horizon_sun_angles       : register(c50);
float3 horizon_bounce_direction : register(c51);
float2 horizon_bounce_angles    : register(c52);
float horizon_bounce_ratio      : register(c53);
float3 horizon_night_color      : register(c54);
#if NGL_SM_PALETTE_NORMAL_FORCE_COLOR_LAST
float4 palette_v                : register(c55);
float4 palette_u0               : register(c56);
float4 palette_u1               : register(c57);
float4 texel_size               : register(c58);
float4 force_color              : register(c59);
#else
float4 force_color              : register(c55);
float4 palette_v                : register(c56);
float4 palette_u0               : register(c57);
float4 palette_u1               : register(c58);
float4 texel_size               : register(c59);
#endif
#else
float4 depth_fog_color          : register(c41);
float4 depth_fog_control        : register(c42);
float4 fog_color                : register(c43);
float exposure                  : register(c44);
float3 horizon_sky_color        : register(c45);
float3 horizon_sky_direction    : register(c46);
float3 horizon_sun_color        : register(c47);
float3 horizon_sun_direction    : register(c48);
float2 horizon_sun_angles       : register(c49);
float3 horizon_bounce_direction : register(c50);
float2 horizon_bounce_angles    : register(c51);
float horizon_bounce_ratio      : register(c52);
float3 horizon_night_color      : register(c53);
#if NGL_SM_PALETTE_NORMAL_FORCE_COLOR_LAST
float4 palette_v                : register(c54);
float4 palette_u0               : register(c55);
float4 palette_u1               : register(c56);
float4 texel_size               : register(c57);
float4 force_color              : register(c58);
#else
float4 force_color              : register(c54);
float4 palette_v                : register(c55);
float4 palette_u0               : register(c56);
float4 palette_u1               : register(c57);
float4 texel_size               : register(c58);
#endif
#endif
#endif

SamplerState horizon_sampler : register(s0);
#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT > 0
SamplerState shadow_sampler_0 : register(s1);
#endif
#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT > 1
SamplerState shadow_sampler_1 : register(s2);
#endif
SamplerState environment_sampler : register(s3);
SamplerState gobo_sampler        : register(s4);
#if NGL_SM_PALETTE_HEIGHT_NORMAL
SamplerState height_sampler      : register(s5);
SamplerState micro_sampler       : register(s6);
SamplerState index_sampler       : register(s7);
SamplerState palette_sampler     : register(s8);
#else
SamplerState normal_sampler      : register(s5);
SamplerState index_sampler       : register(s6);
SamplerState palette_sampler     : register(s7);
#endif

struct pixel_input {
#if NGL_SM_PALETTE_STATIC_HEIGHT_NORMAL
    half3 local_position : TEXCOORD0;
#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT > 0
    half4 shadow_position_0 : TEXCOORD1;
#endif
#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT > 1
    half4 shadow_position_1 : TEXCOORD2;
#endif
    half4 texcoord        : TEXCOORD3;
    half4 normal_gobo_x   : TEXCOORD4;
    half4 tangent_gobo_z  : TEXCOORD5;
    half4 binormal_gobo_y : TEXCOORD6;
    half4 ambient_lo      : TEXCOORD7;
    half4 ambient_hi      : TEXCOORD8;
#else
    half3 local_position  : TEXCOORD0;
    half4 texcoord        : TEXCOORD1;
    half4 normal_gobo_x   : TEXCOORD2;
    half4 tangent_gobo_z  : TEXCOORD3;
    half4 binormal_gobo_y : TEXCOORD4;
#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT > 0
    half4 shadow_position_0 : TEXCOORD5;
#endif
#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT > 1
    half4 shadow_position_1 : TEXCOORD6;
#endif
    half4 ambient_lo : TEXCOORD7;
    half4 ambient_hi : TEXCOORD8;
#endif
};

struct local_light {
    float3 direction;
    half3 color;
};

struct palette_sample {
    half4 diffuse;
    half4 material;
};

local_light evaluate_local_light(int index, float3 position) {
    local_light output;

    float3 to_light = light_position[index] - position;
    float distance_squared = dot(to_light, to_light);
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

#if NGL_SM_PALETTE_NORMAL_BILINEAR_ONLY
palette_sample sample_palette_entry(float2 texcoord) {
    palette_sample output;
    float index = tex2D(index_sampler, texcoord).r;

    output.diffuse = tex2D(
        palette_sampler,
        index * palette_v.xy + palette_u0.xy);
    output.material = tex2D(
        palette_sampler,
        index * palette_v.xy + palette_u1.xy);

    return output;
}

palette_sample sample_palette(float2 texcoord) {
    float2 fraction = frac(texcoord / texel_size.xy);

    palette_sample sample_00 = sample_palette_entry(texcoord);
    palette_sample sample_10 = sample_palette_entry(
        texcoord + float2(texel_size.x, 0.0));
    palette_sample sample_01 = sample_palette_entry(
        texcoord + float2(0.0, texel_size.y));
    palette_sample sample_11 = sample_palette_entry(
        texcoord + texel_size.xy);

    palette_sample output;
    output.diffuse = lerp(
        lerp(sample_00.diffuse, sample_10.diffuse, fraction.x),
        lerp(sample_01.diffuse, sample_11.diffuse, fraction.x),
        fraction.y);
    output.material = lerp(
        lerp(sample_00.material, sample_10.material, fraction.x),
        lerp(sample_01.material, sample_11.material, fraction.x),
        fraction.y);

    return output;
}
#else
palette_sample sample_palette_entry(float2 texcoord, float level) {
    palette_sample output;
    float index = tex2Dlod(
        index_sampler,
        float4(texcoord, 0.0, level)).r;

    output.diffuse = tex2D(
        palette_sampler,
        index * palette_v.xy + palette_u0.xy);
    output.material = tex2D(
        palette_sampler,
        index * palette_v.xy + palette_u1.xy);

    return output;
}

palette_sample sample_palette_level(
    float2 texcoord,
    float2 texel,
    float level)
{
    float2 fraction = frac(texcoord / texel);

    palette_sample sample_00 = sample_palette_entry(
        texcoord,
        level);
    palette_sample sample_10 = sample_palette_entry(
        texcoord + float2(texel.x, 0.0),
        level);
    palette_sample sample_01 = sample_palette_entry(
        texcoord + float2(0.0, texel.y),
        level);
    palette_sample sample_11 = sample_palette_entry(
        texcoord + texel,
        level);

    palette_sample output;
    output.diffuse = lerp(
        lerp(sample_11.diffuse, sample_10.diffuse, fraction.y),
        lerp(sample_01.diffuse, sample_00.diffuse, fraction.y),
        fraction.x);
    output.material = lerp(
        lerp(sample_11.material, sample_10.material, fraction.y),
        lerp(sample_01.material, sample_00.material, fraction.y),
        fraction.x);

    return output;
}

palette_sample sample_palette(float2 texcoord) {
    float2 gradient_x = ddx(texcoord) / texel_size.xy;
    float2 gradient_y = ddy(texcoord) / texel_size.xy;
    float footprint = max(length(gradient_x), length(gradient_y));
    float logarithmic_footprint = log2(max(footprint, 0.5));
    float blend = frac(logarithmic_footprint);
    float level = max(floor(log2(footprint)) + 1.0, 0.0);
    float scale = exp2(level);

    palette_sample fine = sample_palette_level(
        texcoord,
        texel_size.xy * scale,
        level);
    palette_sample coarse = sample_palette_level(
        texcoord,
        texel_size.xy * scale * 2.0,
        level + 1.0);

    palette_sample output;
    output.diffuse = lerp(fine.diffuse, coarse.diffuse, blend);
    output.material = lerp(fine.material, coarse.material, blend);

    return output;
}
#endif

#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT > 0
half compare_shadow(
    SamplerState shadow_sampler,
    float2 texcoord,
    float depth)
{
    return tex2D(shadow_sampler, texcoord).r >= depth ? 1.0 : 0.0;
}

half sample_shadow_0(float4 position) {
    float3 projected = position.xyz / position.w;
    float depth = saturate(projected.z);

#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT == 1
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

#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT > 1
half sample_shadow_1(float4 position) {
    float3 projected = position.xyz / position.w;

    return compare_shadow(
        shadow_sampler_1,
        projected.xy,
        saturate(projected.z));
}
#endif

half evaluate_shadow(pixel_input input, float eye_distance) {
#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT == 1
    half visibility = sample_shadow_0(input.shadow_position_0);
#elif NGL_SM_PALETTE_NORMAL_SHADOW_COUNT == 2
    half visibility_0 = sample_shadow_0(input.shadow_position_0);
    half visibility_1 = sample_shadow_1(input.shadow_position_1);
    half cascade_blend = saturate(eye_distance - shadow_distance.x);
    half visibility = lerp(visibility_0, visibility_1, cascade_blend);
    visibility = saturate(visibility * 0.84 + 0.16);
#else
    return 1.0;
#endif

#if NGL_SM_PALETTE_NORMAL_SHADOW_COUNT > 0
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
    half3 normal_cross = normal.yzx * normal;

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

half evaluate_specular(
    half3 reflection_direction,
    local_light light,
    half power)
{
    half reflection = saturate(dot(reflection_direction, light.direction));

    return power > 0.0 ? pow(reflection, power) : 1.0;
}

#if NGL_SM_PALETTE_HEIGHT_NORMAL
half sample_composite_height(float2 texcoord, float2 micro_texcoord) {
    half height = tex2D(height_sampler, texcoord).r;
    half detail = tex2D(micro_sampler, micro_texcoord).r;

    return height + detail * inverse_micro;
}

half3 evaluate_surface_normal(pixel_input input) {
    float2 texcoord = input.texcoord.xy;
    float2 micro_texcoord = texcoord * micro;
    float2 offset_x = float2(height_sampler_inverse_size.x, 0.0);
    float2 offset_y = float2(0.0, height_sampler_inverse_size.y);

    half center = sample_composite_height(texcoord, micro_texcoord);
    half right = sample_composite_height(
        texcoord + offset_x,
        micro_texcoord + offset_x);
    half upper = sample_composite_height(
        texcoord + offset_y,
        micro_texcoord + offset_y);

    half3 tangent_normal = normalize(
        half3(center - right, center - upper, 1.0) * normal_adjuster);

    half3 normal = normalize(input.normal_gobo_x.xyz);
    half3 tangent = normalize(input.tangent_gobo_z.xyz);
#if NGL_SM_PALETTE_STATIC_HEIGHT_NORMAL
    half3 binormal = normalize(cross(tangent, normal));
#else
    half3 binormal = normalize(input.binormal_gobo_y.xyz);
#endif

    return
        tangent * tangent_normal.x +
        binormal * tangent_normal.y +
        normal * tangent_normal.z;
}
#endif

half4 main(pixel_input input) : COLOR0 {
    palette_sample palette = sample_palette(input.texcoord.xy);

    half3 material_controls = saturate(
        half3(
            palette.diffuse.a,
            palette.material.g,
            palette.material.b) *
        special_effects_matrix[2].xyz +
        special_effects_matrix[3].xyz);
    half effect_bias = saturate(
        palette.material.a * special_effects_matrix[0].y +
        special_effects_matrix[1].y);
    half ambient_scale = saturate(
        special_effects_matrix[0].z +
        special_effects_matrix[1].z);
    half specular_power = saturate(
        palette.material.r * special_effects_matrix[0].w +
        special_effects_matrix[1].w) * 64.0;

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

#if NGL_SM_PALETTE_HEIGHT_NORMAL
    half3 normal = evaluate_surface_normal(input);
#else
    half4 normal_sample = tex2D(normal_sampler, input.texcoord.xy);
    half2 tangent_normal = normal_sample.ag * 2.0 - 1.0;
    half normal_height = sqrt(1.0 - dot(tangent_normal, tangent_normal));
    half3 normal = normalize(
        input.tangent_gobo_z.xyz * tangent_normal.x +
        input.binormal_gobo_y.xyz * tangent_normal.y +
        input.normal_gobo_x.xyz * normal_height);
#endif

    float3 eye_direction = eye - input.local_position;
    float eye_distance = length(eye_direction);
    eye_direction /= eye_distance;

    half shadow_visibility = evaluate_shadow(input, eye_distance);
    half depth_fog = pow(
        saturate(eye_distance * depth_fog_control.x + depth_fog_control.y),
        depth_fog_control.z);

    half4 horizon = tex2D(horizon_sampler, input.texcoord.zw);
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

    half edge_fresnel = saturate(
        normal_to_eye * fresnel.y + fresnel.w);
    half reflection_fresnel = saturate(
        normal_to_eye * fresnel.x + fresnel.z);
    reflected_color *= reflection_fresnel * material_controls.y;

    half reflected_luminance = dot(
        reflected_color,
        half3(0.3, 0.59, 0.11));
    half alpha_reference = saturate(
        reflected_luminance * 0.5 + effect_bias + 1.0);
    half reflected_alpha = material_controls.x * edge_fresnel;
    half blended_alpha = lerp(
        alpha_reference,
        reflected_alpha,
        translucent_flag);
    half output_alpha = reflected_alpha + saturate(blended_alpha - 1.0);

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

    half3 surface_color = apply_color_matrix(palette.diffuse.rgb);
    half3 color = lighting * surface_color + reflected_color + rim_color;

    half3 environment_direction = mul(reflection_direction, local_to_world);
    half3 environment = texCUBE(
        environment_sampler,
        environment_direction).rgb;
    color += environment * reflection_fresnel * material_controls.z;

    color = lerp(color, depth_fog_color.rgb, depth_fog);
    half fog_strength = (half)fog_color.x;
    half exposure_strength = (half)exposure;
    color.r += fog_strength * input.ambient_lo.a * (half)1.0e-10;
    color += exposure_strength * (half)1.0e-8;
    color += force_color.rgb;

    return half4(color, output_alpha);
}

#endif
