#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 material_fresnel     : register(c5);
float4 sky_color            : register(c6);
float4 environment_vertical : register(c7);
float4 environment_horizon  : register(c8);
float4 horizon_color        : register(c9);

float4 diffuse_irradiance[9] : register(c140);
float3 eye                   : register(c149);
float3 ambient_direction     : register(c150);

#ifdef NGL_SM_BUILDINGLOD_OPAQUE
float4 depth_fog_color          : register(c151);
float4 depth_fog_control        : register(c152);
float4 fog_color                : register(c153);
float  exposure                 : register(c154);
float3 horizon_sky_color        : register(c155);
float3 horizon_sky_direction    : register(c156);
float3 horizon_sun_color        : register(c157);
float3 horizon_sun_direction    : register(c158);
float2 horizon_sun_angles       : register(c159);
float3 horizon_bounce_direction : register(c160);
float2 horizon_bounce_angles    : register(c161);
float  horizon_bounce_ratio     : register(c162);
float3 horizon_night_color      : register(c163);
#else
float  translucent_flag         : register(c151);
float4 depth_fog_color          : register(c152);
float4 depth_fog_control        : register(c153);
float4 fog_color                : register(c154);
float  exposure                 : register(c155);
float3 horizon_sky_color        : register(c156);
float3 horizon_sky_direction    : register(c157);
float3 horizon_sun_color        : register(c158);
float3 horizon_sun_direction    : register(c159);
float2 horizon_sun_angles       : register(c160);
float3 horizon_bounce_direction : register(c161);
float2 horizon_bounce_angles    : register(c162);
float  horizon_bounce_ratio     : register(c163);
float3 horizon_night_color      : register(c164);
#endif

SamplerState environment_sampler : register(s3);
SamplerState shininess_sampler   : register(s4);
SamplerState diffuse_sampler     : register(s5);

struct pixel_input {
    half3 world_position      : TEXCOORD0;
    half2 atlas_coordinates   : TEXCOORD1;
    half3 normal              : TEXCOORD2;
    half4 texture_transform   : TEXCOORD3;
    half3 material_parameters : TEXCOORD4;
    half4 ambient_lo          : COLOR0;
    half3 ambient_hi          : COLOR1;
    float opaque_alpha        : TEXCOORD9;
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

half4 main(pixel_input input) : COLOR0 {
    float2 wrapped_texcoord =
        input.atlas_coordinates +
        frac(input.texture_transform.xy) * input.texture_transform.zw;
    float2 unwrapped_texcoord =
        input.atlas_coordinates +
        input.texture_transform.xy * input.texture_transform.zw;
    float2 texcoord_dx = ddx(unwrapped_texcoord);
    float2 texcoord_dy = ddy(unwrapped_texcoord);

    half4 shininess = tex2Dgrad(
        shininess_sampler,
        wrapped_texcoord,
        texcoord_dx,
        texcoord_dy);
    half4 diffuse = tex2Dgrad(
        diffuse_sampler,
        wrapped_texcoord,
        texcoord_dx,
        texcoord_dy);

    float3 eye_direction = eye - input.world_position;
    float  eye_distance  = length(eye_direction);
    eye_direction /= eye_distance;

    half depth_fog = pow(
        saturate(eye_distance * depth_fog_control.x + depth_fog_control.y),
        depth_fog_control.z);

    half3 reflection_direction = reflect(-eye_direction, input.normal);
    half reflection_fresnel = saturate(
        dot(eye_direction, -input.normal) * material_fresnel.x +
        material_fresnel.y);

    half surface_shiny = max(0.1, input.material_parameters.y);
    half specular_power = (shininess.b + 0.0865) * 64.0;
    half3 bounce_color = horizon_sun_color * horizon_bounce_ratio;
    half3 reflected_color =
        bounce_color * saturate(dot(reflection_direction, horizon_bounce_direction));
    reflected_color += horizon_sun_color * surface_shiny * pow(
        saturate(dot(reflection_direction, horizon_sun_direction)),
        specular_power);
    reflected_color *= shininess.b;

    half3 normalized_reflection = normalize(reflection_direction);
    half horizontal_reflection = length(normalized_reflection.xz);
    half3 environment_vector =
        normalized_reflection.y * environment_vertical.xyz +
        horizontal_reflection * environment_horizon.xyz;
    half3 environment_sample = texCUBE(
        environment_sampler,
        reflection_direction).rgb;
    half environment_gradient = saturate(
        reflection_direction.y * sky_color.a + horizon_color.a);
    half3 sky_environment = 1.0 + sky_color.rgb * 1.0e-4;
    half3 environment_color = lerp(
        horizon_color.rgb,
        sky_environment,
        environment_gradient);
    half3 mapped_environment =
        environment_vector * environment_sample * environment_color;
    half environment_mix = shininess.b * (1.0 - shininess.g);
    half3 surface_response = lerp(
        diffuse.rgb,
        mapped_environment,
        environment_mix);
    surface_response *=
        saturate(reflection_fresnel + shininess.g) * 0.83;

    half3 direct_sun =
        horizon_sun_color *
        surface_shiny *
        saturate(dot(input.normal, horizon_sun_direction)) *
        input.material_parameters.z *
        1.0e-9;
    half3 sky_bounce = (
        bounce_color * saturate(dot(input.normal, horizon_bounce_direction)) +
        horizon_sky_color) * input.material_parameters.z * 1.0e-9;
    half ambient_fraction = dot(ambient_direction, input.normal) * 0.5 + 0.5;
    half3 ambient_color = lerp(input.ambient_lo.rgb, input.ambient_hi, ambient_fraction);
    half3 lighting = direct_sun + ambient_color + sky_bounce;

    lighting.b +=
        horizon_sun_angles.x *
        horizon_bounce_angles.x *
        horizon_sky_direction.x *
        horizon_night_color.x *
        1.0e-13;
    lighting += evaluate_diffuse_irradiance(input.normal);

    half3 color = lighting * surface_response + reflected_color;
    color = lerp(color, depth_fog_color.rgb, depth_fog);
    color.r += fog_color.x * input.ambient_lo.a * 1.0e-10;
    color += exposure * 1.0e-8;

#ifdef NGL_SM_BUILDINGLOD_OPAQUE
    half output_alpha = saturate(input.opaque_alpha);
#else
    half reflected_luminance = dot(reflected_color, half3(0.3, 0.59, 0.11));
    half output_alpha = lerp(
        reflected_luminance * 0.5,
        1.0 - reflected_luminance * 0.5,
        translucent_flag);
#endif

    return half4(color, output_alpha);
}

#endif
