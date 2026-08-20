#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 ambient_color_lo[8] : register(c0);
float4 ambient_color_hi[8] : register(c8);

row_major float4x4 local_to_gobo             : register(c16);
row_major float4x4 world_view_projection     : register(c20);
row_major float4x4 shadow_view_projection_0  : register(c24);
row_major float4x4 shadow_view_projection_1  : register(c28);

float4 local_to_ambient     : register(c32);
float3 fog_eye_to_local     : register(c33);
float3 fog_normal           : register(c34);
float3 fog_position_0       : register(c35);
float3 fog_position_1       : register(c36);
float2 fogs_per_meter       : register(c37);
float4 horizon_map_matrix_u : register(c38);
float4 horizon_map_matrix_v : register(c39);
float4 vertex_transform     : register(c40);

struct vertex_input {
    float3 position            : POSITION0;
    float2 texture_coordinates : TEXCOORD0;
    float3 normal              : NORMAL0;
    float3 tangent             : TANGENT0;
};

struct vertex_output {
    float4 position          : POSITION0;
    float3 world_position    : TEXCOORD0;
    float4 texture_coordinates : TEXCOORD1;
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

    float3 world_position =
        input.position * vertex_transform.w + vertex_transform.xyz;
    float4 homogeneous_position = float4(world_position, 1.0);

    output.position          = mul(homogeneous_position, world_view_projection);
    output.world_position    = world_position;
    output.shadow_position_0 = mul(homogeneous_position, shadow_view_projection_0);
    output.shadow_position_1 = mul(homogeneous_position, shadow_view_projection_1);

    output.texture_coordinates.xy = input.texture_coordinates / 1024.0;
    output.texture_coordinates.z =
        dot(homogeneous_position, horizon_map_matrix_u);
    output.texture_coordinates.w =
        dot(homogeneous_position, horizon_map_matrix_v);

    output.normal_gobo_x.xyz  = input.normal;
    output.tangent_gobo_z.xyz = input.tangent;
    output.binormal_gobo_y.xyz = normalize(cross(input.tangent, input.normal));

    float3 gobo_position = mul(homogeneous_position, local_to_gobo).xyz;
    output.normal_gobo_x.w   = gobo_position.x;
    output.tangent_gobo_z.w  = gobo_position.z;
    output.binormal_gobo_y.w = gobo_position.y;

    float ambient_position = saturate(
        dot(homogeneous_position, local_to_ambient)) * 6.999;
    int   ambient_index    = (int)floor(ambient_position);
    float ambient_fraction = frac(ambient_position);

    output.ambient_lo.rgb = lerp(
        ambient_color_lo[ambient_index].rgb,
        ambient_color_lo[ambient_index + 1].rgb,
        ambient_fraction);
    output.ambient_hi.rgb = lerp(
        ambient_color_hi[ambient_index].rgb,
        ambient_color_hi[ambient_index + 1].rgb,
        ambient_fraction);

    float3 fog_direction      = world_position - fog_eye_to_local;
    float  fog_distance       = length(fog_direction);
    float  inverse_fog_length = 1.0 / dot(fog_normal, fog_direction);
    float  fog_start          = saturate(
        dot(fog_normal, fog_position_0 - fog_eye_to_local) * inverse_fog_length);
    float  fog_end            = saturate(
        dot(fog_normal, fog_position_1 - fog_eye_to_local) * inverse_fog_length);

    output.ambient_lo.a = min(
        abs(fog_start - fog_end) * fog_distance * fogs_per_meter.x,
        fogs_per_meter.y);
    output.ambient_hi.a = 1.0;

    return output;
}

#endif
