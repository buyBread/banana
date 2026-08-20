#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 diffuse_irradiance[9] : register(c0);
float4 special_effects_matrix[4] : register(c9);
row_major float4x4 color_matrix : register(c13);

float3 eye                      : register(c17);
float3 ambient_direction        : register(c18);
float4 fresnel                  : register(c19);
float  translucent_flag         : register(c20);
float4 depth_fog_color          : register(c21);
float4 depth_fog_control        : register(c22);
float4 fog_color                : register(c23);
float  exposure                 : register(c24);
float3 horizon_sky_color        : register(c25);
float3 horizon_sky_direction    : register(c26);
float3 horizon_sun_color        : register(c27);
float3 horizon_sun_direction    : register(c28);
float2 horizon_sun_angles       : register(c29);
float3 horizon_bounce_direction : register(c30);
float2 horizon_bounce_angles    : register(c31);
float  horizon_bounce_ratio     : register(c32);
float3 horizon_night_color      : register(c33);
float4 shiny                    : register(c34);

struct pixel_input {
    float3 world_position : TEXCOORD0;
    half4  normal         : TEXCOORD1;
    half4  material_color : TEXCOORD2;
    half4  ambient_lo     : COLOR0;
    half3  ambient_hi     : COLOR1;
};

half4 main(pixel_input input) : COLOR0 {
    float3 eye_direction = eye - input.world_position;
    float  eye_distance  = length(eye_direction);
    eye_direction /= eye_distance;

    half depth_fog = pow(
        saturate(eye_distance * depth_fog_control.x + depth_fog_control.y),
        depth_fog_control.z);

    half3 reflection_direction = reflect(-eye_direction, input.normal.xyz);
    half  normal_to_eye        = dot(input.normal.xyz, eye_direction);
    half  edge_fresnel         = saturate(normal_to_eye * fresnel.y + fresnel.w);
    half  reflection_fresnel   = saturate(normal_to_eye * fresnel.x + fresnel.z);

    half3 bounce_color = horizon_sun_color * horizon_bounce_ratio;
    half3 reflected_color =
        bounce_color * saturate(dot(reflection_direction, horizon_bounce_direction));

    half effect_bias = saturate(special_effects_matrix[1].y);
    half ambient_scale = saturate(
        special_effects_matrix[0].z + special_effects_matrix[1].z);
    half specular_power = saturate(
        special_effects_matrix[0].w * shiny.y + special_effects_matrix[1].w) * 64.0;
    half surface_shiny = max(0.1, input.material_color.a);

    reflected_color += horizon_sun_color * surface_shiny * pow(
        saturate(dot(reflection_direction, horizon_sun_direction)),
        specular_power);

    half alpha_scale = saturate(
        special_effects_matrix[2].x + special_effects_matrix[3].x);
    half reflection_scale = saturate(
        shiny.x * special_effects_matrix[2].y + special_effects_matrix[3].y);

    reflected_color *= reflection_fresnel * reflection_scale;

    half reflected_luminance = dot(reflected_color, half3(0.3, 0.59, 0.11));
    half alpha_reference = saturate(reflected_luminance * 0.5 + effect_bias + 1.0);
    half reflected_alpha = alpha_scale * edge_fresnel;
    half blended_alpha = lerp(alpha_reference, reflected_alpha, translucent_flag);

    half output_alpha = reflected_alpha + saturate(blended_alpha - 1.0);

    half3 direct_sun =
        horizon_sun_color *
        surface_shiny *
        saturate(dot(input.normal.xyz, horizon_sun_direction)) *
        input.normal.w;
    direct_sun = direct_sun * 1.0e-9 + effect_bias;

    half3 sky_bounce = (
        bounce_color * saturate(dot(input.normal.xyz, horizon_bounce_direction)) +
        horizon_sky_color) * input.normal.w * 1.0e-9;

    half ambient_fraction = dot(ambient_direction, input.normal.xyz) * 0.5 + 0.5;
    half3 ambient_color = lerp(input.ambient_lo.rgb, input.ambient_hi, ambient_fraction);
    half3 lighting = direct_sun + ambient_color * ambient_scale + sky_bounce;

    lighting.b +=
        horizon_sun_angles.x *
        horizon_bounce_angles.x *
        horizon_sky_direction.x *
        horizon_night_color.x *
        1.0e-13;

    half3 normal_squared = input.normal.xyz * input.normal.xyz;
    half3 normal_cross = input.normal.yzx * input.normal.xyz;
    half3 diffuse_lighting =
        diffuse_irradiance[0].rgb * normal_squared.x +
        diffuse_irradiance[1].rgb * normal_squared.y +
        diffuse_irradiance[2].rgb * normal_squared.z +
        diffuse_irradiance[3].rgb * normal_cross.x +
        diffuse_irradiance[4].rgb * normal_cross.y +
        diffuse_irradiance[5].rgb * normal_cross.z +
        diffuse_irradiance[6].rgb * input.normal.x +
        diffuse_irradiance[7].rgb * input.normal.y +
        diffuse_irradiance[8].rgb * input.normal.z;

    lighting += diffuse_lighting;

    half3 material_color_squared = input.material_color.rgb * input.material_color.rgb;
    half3 color_adjustment = saturate(
        color_matrix[0].rgb * material_color_squared.r +
        color_matrix[1].rgb * material_color_squared.g +
        color_matrix[2].rgb * material_color_squared.b +
        color_matrix[3].rgb);

    half3 color = lighting * color_adjustment + reflected_color;
    color = lerp(color, depth_fog_color.rgb, depth_fog);

    color.r += fog_color.x * input.ambient_lo.a * 1.0e-10;
    color += exposure * 1.0e-8;

    return half4(color, output_alpha);
}

#endif
