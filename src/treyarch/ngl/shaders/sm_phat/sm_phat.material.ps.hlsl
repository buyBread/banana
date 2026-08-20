#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 diffuse_irradiance[9];

#if NGL_SM_PHAT_LIGHT_COUNT == 4
float3 light_position[4];
float4 light_falloff[4];
float3 light_color[4];
float4 light_direction[4];
row_major float4x4 diffuse_matrix;
#elif NGL_SM_PHAT_LIGHT_COUNT == 2
row_major float4x4 diffuse_matrix;
float3 light_position[2];
float4 light_falloff[2];
float3 light_color[2];
float4 light_direction[2];
#else
row_major float4x4 diffuse_matrix;
#endif

#if NGL_SM_PHAT_SHADOW_COUNT > 0
float2 shadow_distance;
#endif

#if NGL_SM_PHAT_HORIZON
float3 horizon_sky_color;
float3 horizon_sky_direction;
float3 horizon_sun_color;
float3 horizon_sun_direction;
float2 horizon_sun_angles;
float3 horizon_bounce_direction;
float2 horizon_bounce_angles;
float  horizon_bounce_ratio;
float3 horizon_night_color;
#endif

float4 depth_fog_color;
float4 depth_fog_control;
float4 fog_color;
float  exposure;
float3 ambient_direction;

#if NGL_SM_PHAT_LIGHT_COUNT > 0
float4 rim_lighting;
#endif

#if NGL_SM_PHAT_NORMAL
float4 normal_mad;
#endif

#if NGL_SM_PHAT_ENVIRONMENT > 0
float3 local_to_world_x;
float3 local_to_world_y;
float3 local_to_world_z;
#endif

float3 eye;
float4 effects_mul;
float4 effects_add;
float3 opacity_mad;
float4 fresnel;
float4 force_color;

#if NGL_SM_PHAT_HORIZON
SamplerState horizon_sampler : register(s0);
#endif

#if NGL_SM_PHAT_SHADOW_COUNT > 0
SamplerState shadow_sampler_0 : register(s1);
#endif

#if NGL_SM_PHAT_SHADOW_COUNT > 1
SamplerState shadow_sampler_1 : register(s2);
#endif

#if NGL_SM_PHAT_GOBO
SamplerState gobo_sampler : register(s3);
#endif

#if NGL_SM_PHAT_DIFFUSE
SamplerState diffuse_sampler : register(s4);
#endif

#if NGL_SM_PHAT_OPACITY
SamplerState opacity_sampler : register(s5);
#endif

#if NGL_SM_PHAT_SPECULARITY
SamplerState specularity_sampler : register(s6);
#endif

#if NGL_SM_PHAT_SPECULAR_EXPONENT
SamplerState specular_exponent_sampler : register(s7);
#endif

#if NGL_SM_PHAT_ENVIRONMENT == 2
SamplerState reflectivity_sampler : register(s8);
#endif

#if NGL_SM_PHAT_EMISSIVENESS
SamplerState emissiveness_sampler : register(s9);
#endif

#if NGL_SM_PHAT_NORMAL
SamplerState normal_sampler : register(s10);
#endif

#if NGL_SM_PHAT_ENVIRONMENT > 0
SamplerState environment_sampler : register(s11);
#endif

struct pixel_input {
    float3 local_position    : TEXCOORD0;
    float4 texcoord          : TEXCOORD1;
    half4  normal_gobo_x     : TEXCOORD2;
    half4  tangent_gobo_z    : TEXCOORD3;
    half4  binormal_gobo_y   : TEXCOORD4;
    float4 shadow_position_0 : TEXCOORD5;
    float4 shadow_position_1 : TEXCOORD6;
    half4  ambient_lo        : TEXCOORD7;
    half4  ambient_hi        : TEXCOORD8;
};

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

#if NGL_SM_PHAT_SHADOW_COUNT > 0
half sample_shadow_0(float4 position) {
    float inverse_w = 1.0 / position.w;
    float3 projected = position.xyz * inverse_w;

#if NGL_SM_PHAT_SHADOW_COUNT == 1
    half shadow = 0.0;

    [unroll]
    for (int y = -1; y <= 1; ++y) {
        [unroll]
        for (int x = -1; x <= 1; ++x) {
            half depth = tex2D(
                shadow_sampler_0,
                projected.xy + float2(x, y)).x;
            shadow += depth >= saturate(projected.z) ? 1.0 : 0.0;
        }
    }

    return shadow / 9.0;
#else
    half depth = tex2D(shadow_sampler_0, projected.xy).x;

    return depth >= saturate(projected.z) ? 1.0 : 0.0;
#endif
}
#endif

#if NGL_SM_PHAT_SHADOW_COUNT > 1
half sample_shadow_1(float4 position) {
    float3 projected = position.xyz / position.w;
    half depth = tex2D(shadow_sampler_1, projected.xy).x;

    return depth >= saturate(projected.z) ? 1.0 : 0.0;
}
#endif

half4 main(pixel_input input) : COLOR0 {
    float4 diffuse_sample = 1.0;

#if NGL_SM_PHAT_DIFFUSE
    diffuse_sample = tex2D(diffuse_sampler, input.texcoord.xy);
#endif

    half3 diffuse_color =
        diffuse_matrix[0].rgb * diffuse_sample.r +
        diffuse_matrix[1].rgb * diffuse_sample.g +
        diffuse_matrix[2].rgb * diffuse_sample.b +
        diffuse_matrix[3].rgb;

    half3 normal = normalize(input.normal_gobo_x.xyz);

#if NGL_SM_PHAT_NORMAL
    half2 encoded_normal =
        tex2D(normal_sampler, input.texcoord.xy).wy * normal_mad.xy +
        normal_mad.zw;
    half normal_z = sqrt(max(0.0, 1.0 - dot(encoded_normal, encoded_normal)));
    normal = normalize(
        encoded_normal.x * input.tangent_gobo_z.xyz +
        encoded_normal.y * input.binormal_gobo_y.xyz +
        normal_z * input.normal_gobo_x.xyz);
#endif

    half4 material = 1.0;

#if NGL_SM_PHAT_SPECULARITY
    material.x = tex2D(specularity_sampler, input.texcoord.xy).x;
#endif
#if NGL_SM_PHAT_SPECULAR_EXPONENT
    material.y = tex2D(specular_exponent_sampler, input.texcoord.xy).x;
#endif
#if NGL_SM_PHAT_ENVIRONMENT == 2
    material.z = tex2D(reflectivity_sampler, input.texcoord.xy).x;
#endif
#if NGL_SM_PHAT_EMISSIVENESS
    material.w = tex2D(emissiveness_sampler, input.texcoord.xy).x;
#endif

    material = saturate(material * effects_mul + effects_add);

    float3 eye_vector = eye - input.local_position;
    float eye_distance = length(eye_vector);
    half3 eye_direction = eye_vector / eye_distance;
    half normal_to_eye = dot(normal, eye_direction);
    half reflection_fresnel = saturate(
        normal_to_eye * fresnel.x + fresnel.z);
    half3 reflection_direction = reflect(-eye_direction, normal);

    material.xz *= reflection_fresnel;
    material.y *= 64.0;

    half shadow = 1.0;

#if NGL_SM_PHAT_SHADOW_COUNT == 1
    half first_shadow = sample_shadow_0(input.shadow_position_0);
    half shadow_blend = saturate(
        (eye_distance - shadow_distance.x) /
        max(shadow_distance.y - shadow_distance.x, 1.0e-10));
    shadow = lerp(first_shadow, 1.0, shadow_blend);
#elif NGL_SM_PHAT_SHADOW_COUNT == 2
    half first_shadow = sample_shadow_0(input.shadow_position_0);
    half second_shadow = sample_shadow_1(input.shadow_position_1);
    half shadow_blend = saturate(
        (eye_distance - shadow_distance.x) /
        max(shadow_distance.y - shadow_distance.x, 1.0e-10));
    shadow = lerp(first_shadow, second_shadow, shadow_blend);
#endif

    half3 direct_diffuse = material.w;
    half3 direct_specular = 0.0;
    half3 direct_rim = 0.0;

#if NGL_SM_PHAT_LIGHT_COUNT > 0
    [loop]
    for (int light = 0; light < NGL_SM_PHAT_LIGHT_COUNT; ++light) {
        float3 light_vector = light_position[light] - input.local_position;
        float light_distance_squared = dot(light_vector, light_vector);
        half3 to_light = normalize(light_vector);
        half attenuation = saturate(
            light_distance_squared * light_falloff[light].x +
            light_falloff[light].y);
        half cone = saturate(
            dot(to_light, light_direction[light].xyz) *
            light_falloff[light].z +
            light_falloff[light].w);
        cone = max(cone, light_direction[light].w);
        attenuation *= cone;

#if NGL_SM_PHAT_GOBO
        if (light == 0) {
            float2 gobo_coordinates =
                float2(input.normal_gobo_x.w, input.tangent_gobo_z.w) /
                input.binormal_gobo_y.w + 0.5;
            attenuation *= tex2D(gobo_sampler, gobo_coordinates).x;
        }
#endif

        attenuation *= shadow;

        half3 energy = light_color[light] * attenuation;
        half diffuse_amount = saturate(dot(normal, to_light));
        half specular_amount = material.y > 0.0 ?
            pow(saturate(dot(reflection_direction, to_light)), material.y) :
            1.0;

        direct_diffuse += energy * diffuse_amount;
        direct_specular += energy * specular_amount;
        direct_rim += light_color[light] * attenuation;
    }
#endif

    half ambient_fraction =
        saturate(dot(ambient_direction, normal) * 0.5 + 0.5);
    half3 ambient_color = lerp(
        input.ambient_lo.rgb,
        input.ambient_hi.rgb,
        ambient_fraction);
    half3 lighting = evaluate_diffuse_irradiance(normal);

#if NGL_SM_PHAT_HORIZON
    half4 horizon = tex2D(horizon_sampler, input.texcoord.zw);
    half bounce = saturate(dot(normal, horizon_bounce_direction));
    half sun = saturate(dot(normal, horizon_sun_direction));
    half3 horizon_lighting =
        horizon_sky_color +
        horizon_sun_color * horizon_bounce_ratio * bounce;
    horizon_lighting *= horizon.z * 1.0e-9;
    horizon_lighting.b +=
        horizon_sky_direction.x *
        horizon_sun_angles.x *
        horizon_bounce_angles.x *
        horizon_night_color.x *
        1.0e-13;
    ambient_color += horizon_lighting;

    half3 horizon_direct_diffuse =
        horizon_sun_color * sun * horizon.z * 1.0e-9;
    direct_diffuse -= material.w;
    direct_diffuse += (material.w + horizon_direct_diffuse) * shadow;

#if NGL_SM_PHAT_LIGHT_COUNT == 0
    half horizon_sun_specular = material.y > 0.0 ?
        pow(
            saturate(dot(reflection_direction, horizon_sun_direction)),
            material.y) :
        1.0;
    half horizon_bounce_specular = saturate(
        dot(reflection_direction, horizon_bounce_direction));
    direct_specular +=
        horizon_sun_color *
        (horizon_sun_specular * max(horizon.y, 0.1) + horizon_bounce_specular * horizon_bounce_ratio) *
        shadow;
#endif
#endif

    lighting += ambient_color * input.ambient_hi.w;
    lighting += direct_diffuse * input.ambient_hi.w;

    half3 color =
        lighting * diffuse_color +
        material.x * direct_specular;

#if NGL_SM_PHAT_ENVIRONMENT > 0
    half3 world_reflection =
        reflection_direction.x * local_to_world_x +
        reflection_direction.y * local_to_world_y +
        reflection_direction.z * local_to_world_z;
    color += texCUBE(environment_sampler, world_reflection).rgb * material.z;
#endif

#if NGL_SM_PHAT_LIGHT_COUNT > 0
    half rim = pow(1.0 - abs(normal_to_eye), rim_lighting.w);
    color += direct_rim * rim * rim_lighting.rgb * input.ambient_hi.w;
#endif

    half alpha = saturate(opacity_mad.x + opacity_mad.y);

#if NGL_SM_PHAT_OPACITY
    alpha = saturate(
        tex2D(opacity_sampler, input.texcoord.xy).x * opacity_mad.x +
        opacity_mad.y);
#endif

    half depth_fog = pow(
        saturate(
            eye_distance * depth_fog_control.x +
            depth_fog_control.y),
        depth_fog_control.z);
    color = lerp(color, depth_fog_color.rgb, depth_fog);
    color.r += fog_color.x * input.ambient_lo.a * 1.0e-10;
    color += exposure * 1.0e-8;
    color += force_color.rgb;

    return half4(color, alpha);
}

#endif
