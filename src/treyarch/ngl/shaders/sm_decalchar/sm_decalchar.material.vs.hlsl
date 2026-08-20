#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float3x4 world[64] : register(c0);

float4 ambient_color_lo[8] : register(c192);
float4 ambient_color_hi[8] : register(c200);

row_major float4x4 local_to_gobo            : register(c208);
row_major float4x4 screen_matrix            : register(c212);
row_major float4x4 local_matrix             : register(c216);
row_major float4x4 texture_coordinate_0_matrix : register(c220);
row_major float4x4 texture_coordinate_1_matrix : register(c224);
row_major float4x4 light_world_view_projection_0 : register(c228);
row_major float4x4 light_world_view_projection_1 : register(c232);

float4 local_to_ambient     : register(c236);
float3 fog_eye_to_local     : register(c237);
float3 fog_normal           : register(c238);
float3 fog_position_0       : register(c239);
float3 fog_position_1       : register(c240);
float2 fogs_per_meter       : register(c241);
float4 horizon_map_matrix_u : register(c242);
float4 horizon_map_matrix_v : register(c243);

struct vertex_input {
    float3 position      : POSITION0;
    float2 texcoord_0    : TEXCOORD0;
    float2 texcoord_1    : TEXCOORD1;
    float3 normal        : NORMAL0;
    float3 binormal      : BINORMAL0;
    float3 tangent       : TANGENT0;
    int4   blend_indices : BLENDINDICES0;
    float4 blend_weights : BLENDWEIGHT0;
    float4 color         : COLOR0;
};

struct vertex_output {
    float4 position               : POSITION0;
    float4 local_position         : TEXCOORD0;
    float4 texture_coordinates    : TEXCOORD1;
    float4 tangent_gobo_z         : TEXCOORD2;
    float4 binormal_gobo_y        : TEXCOORD3;
    float4 normal_gobo_x          : TEXCOORD4;
    float4 ambient_lo             : TEXCOORD5;
    float4 ambient_hi             : TEXCOORD6;
    float2 horizon_coordinates    : TEXCOORD7;
    float4 shadow_position_0      : TEXCOORD8;
    float4 shadow_position_1      : TEXCOORD9;
};

float4 transform_position(float4 position, int index) {
    return float4(mul(world[index], position), 1.0);
}

float3 transform_direction(float3 direction, int index) {
    return mul((float3x3)world[index], direction);
}

float4 skin_position(
    float4 position,
    int4   blend_indices,
    float4 blend_weights)
{
    return
        transform_position(position, blend_indices.x) * blend_weights.x +
        transform_position(position, blend_indices.y) * blend_weights.y +
        transform_position(position, blend_indices.z) * blend_weights.z +
        transform_position(position, blend_indices.w) * blend_weights.w;
}

float3 skin_direction(
    float3 direction,
    int4   blend_indices,
    float4 blend_weights)
{
    return
        transform_direction(direction, blend_indices.x) * blend_weights.x +
        transform_direction(direction, blend_indices.y) * blend_weights.y +
        transform_direction(direction, blend_indices.z) * blend_weights.z +
        transform_direction(direction, blend_indices.w) * blend_weights.w;
}

vertex_output main(vertex_input input) {
    vertex_output output;

    float4 mesh_position = mul(float4(input.position, 1.0), local_matrix);
    float4 local_position = skin_position(
        mesh_position,
        input.blend_indices,
        input.blend_weights);
    float4 homogeneous_position = float4(local_position.xyz, 1.0);

    output.position          = mul(homogeneous_position, screen_matrix);
    output.local_position    = local_position;
    output.shadow_position_0 = mul(
        homogeneous_position,
        light_world_view_projection_0);
    output.shadow_position_1 = mul(
        homogeneous_position,
        light_world_view_projection_1);

    output.texture_coordinates.xy = mul(
        float4(input.texcoord_0, 0.0, 1.0),
        texture_coordinate_0_matrix).xy;
    output.texture_coordinates.zw = mul(
        float4(input.texcoord_1, 0.0, 1.0),
        texture_coordinate_1_matrix).xy;

    output.normal_gobo_x.xyz = normalize(skin_direction(
        input.normal,
        input.blend_indices,
        input.blend_weights));
    output.tangent_gobo_z.xyz = normalize(skin_direction(
        input.tangent,
        input.blend_indices,
        input.blend_weights));
    output.binormal_gobo_y.xyz = normalize(skin_direction(
        input.binormal,
        input.blend_indices,
        input.blend_weights));

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

    float3 fog_direction      = local_position.xyz - fog_eye_to_local;
    float  fog_distance       = length(fog_direction);
    float  inverse_fog_length = 1.0 / dot(fog_normal, fog_direction);
    float  fog_start          = saturate(
        dot(fog_normal, fog_position_0 - fog_eye_to_local) * inverse_fog_length);
    float  fog_end            = saturate(
        dot(fog_normal, fog_position_1 - fog_eye_to_local) * inverse_fog_length);

    output.ambient_lo.a = min(
        abs(fog_start - fog_end) * fog_distance * fogs_per_meter.x,
        fogs_per_meter.y);

    output.horizon_coordinates = float2(
        dot(homogeneous_position, horizon_map_matrix_u),
        dot(homogeneous_position, horizon_map_matrix_v));

    float3 gobo_position = mul(homogeneous_position, local_to_gobo).xyz;
    output.normal_gobo_x.w   = gobo_position.x;
    output.binormal_gobo_y.w = gobo_position.y;
    output.tangent_gobo_z.w  = gobo_position.z;
    output.ambient_hi.w      = input.color.x;

    return output;
}

#endif
