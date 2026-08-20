#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 ambient_color_lo[8] : register(c0);
float4 ambient_color_hi[8] : register(c8);

row_major float4x4 local_to_gobo                 : register(c16);
row_major float4x4 world_view_projection         : register(c20);
row_major float4x4 light_world_view_projection_0 : register(c24);
row_major float4x4 light_world_view_projection_1 : register(c28);

float4 local_to_ambient     : register(c32);
float4 uv_scale_offset      : register(c33);
float3 fog_eye_to_local     : register(c34);
float3 fog_normal           : register(c35);
float3 fog_position_0       : register(c36);
float3 fog_position_1       : register(c37);
float2 fogs_per_meter       : register(c38);
float4 horizon_map_matrix_u : register(c39);
float4 horizon_map_matrix_v : register(c40);
float4 compressed_to_local  : register(c41);

struct vertex_input {
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal   : NORMAL0;
    float3 tangent  : TANGENT0;
};

struct vertex_output {
    float4 position          : POSITION0;
    float3 local_position    : TEXCOORD0;
    float4 texcoord          : TEXCOORD1;
    float4 normal_gobo_x     : TEXCOORD2;
    float4 tangent_gobo_z    : TEXCOORD3;
    float4 binormal_gobo_y   : TEXCOORD4;
    float4 shadow_position_0 : TEXCOORD5;
    float4 shadow_position_1 : TEXCOORD6;
    float4 ambient_lo        : TEXCOORD7;
    float4 ambient_hi        : TEXCOORD8;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    float4 local_position = float4(
        input.position * compressed_to_local.w + compressed_to_local.xyz,
        1.0);

    output.position = mul(local_position, world_view_projection);
    output.local_position = local_position.xyz;
    output.shadow_position_0 = mul(
        local_position,
        light_world_view_projection_0);
    output.shadow_position_1 = mul(
        local_position,
        light_world_view_projection_1);

    output.texcoord.xy =
        input.texcoord * uv_scale_offset.xy * (1.0 / 1024.0) +
        uv_scale_offset.zw;
    output.texcoord.zw = float2(
        dot(local_position, horizon_map_matrix_u),
        dot(local_position, horizon_map_matrix_v));

    float3 binormal = normalize(cross(input.tangent, input.normal));
    float3 gobo_position = mul(local_position, local_to_gobo).xyz;

    output.normal_gobo_x = float4(input.normal, gobo_position.x);
    output.tangent_gobo_z = float4(input.tangent, gobo_position.z);
    output.binormal_gobo_y = float4(binormal, gobo_position.y);

    float ambient_position = saturate(
        dot(local_position, local_to_ambient)) * 6.999;
    int ambient_index = (int)floor(ambient_position);
    float ambient_fraction = frac(ambient_position);

    output.ambient_lo.rgb = lerp(
        ambient_color_lo[ambient_index].rgb,
        ambient_color_lo[ambient_index + 1].rgb,
        ambient_fraction);
    output.ambient_hi.rgb = lerp(
        ambient_color_hi[ambient_index].rgb,
        ambient_color_hi[ambient_index + 1].rgb,
        ambient_fraction);

    float3 fog_direction = local_position.xyz - fog_eye_to_local;
    float fog_distance = length(fog_direction);
    float inverse_fog_length = 1.0 / dot(fog_normal, fog_direction);
    float2 fog_plane_distances = float2(
        dot(fog_normal, fog_position_0 - fog_eye_to_local),
        dot(fog_normal, fog_position_1 - fog_eye_to_local));
    float2 fog_intersections = saturate(
        fog_plane_distances * inverse_fog_length);

    output.ambient_lo.a = min(
        abs(fog_intersections.x - fog_intersections.y) *
        fog_distance * fogs_per_meter.x,
        fogs_per_meter.y);
    output.ambient_hi.a = 1.0;

    return output;
}

#endif
