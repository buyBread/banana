#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float3x4 world[64] : register(c0);

float4 ambient_color_lo[8] : register(c192);
float4 ambient_color_hi[8] : register(c200);

row_major float4x4 world_view_projection         : register(c208);
row_major float4x4 shadow_view_projection_0      : register(c212);
row_major float4x4 shadow_view_projection_1      : register(c216);
row_major float4x4 local_to_gobo                 : register(c220);

float4 local_to_ambient     : register(c224);
float4 uv_scale_offset      : register(c225);
float3 fog_eye_to_local     : register(c226);
float3 fog_normal           : register(c227);
float3 fog_position_0       : register(c228);
float3 fog_position_1       : register(c229);
float2 fogs_per_meter       : register(c230);
float4 horizon_map_matrix_u : register(c231);
float4 horizon_map_matrix_v : register(c232);
#if NGL_SM_CHARACTER_FORCE_TINT
float  force_tint           : register(c233);
#endif

struct vertex_input {
    float3 position      : POSITION0;
    float2 texcoord      : TEXCOORD0;
    float3 normal        : NORMAL0;
    float3 binormal      : BINORMAL0;
    float3 tangent       : TANGENT0;
    int4   blend_indices : BLENDINDICES0;
    float4 blend_weights : BLENDWEIGHT0;
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

float4 transform_position(float4 position, int index) {
    return float4(mul(world[index], position), 1.0);
}

float3 transform_direction(float3 direction, int index) {
    return mul((float3x3)world[index], direction);
}

float4 skin_position(
    float4 position,
    int4 blend_indices,
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
    int4 blend_indices,
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

    float4 mesh_position = float4(input.position * 0.001, 1.0);
    float4 local_position = skin_position(
        mesh_position,
        input.blend_indices,
        input.blend_weights);
    float4 homogeneous_position = float4(local_position.xyz, 1.0);

    output.position          = mul(homogeneous_position, world_view_projection);
    output.local_position    = local_position.xyz;
    output.shadow_position_0 = mul(
        homogeneous_position,
        shadow_view_projection_0);
    output.shadow_position_1 = mul(
        homogeneous_position,
        shadow_view_projection_1);

    output.texcoord.xy =
        input.texcoord * uv_scale_offset.xy * 0.001 +
        uv_scale_offset.zw;
    output.texcoord.zw = float2(
        dot(homogeneous_position, horizon_map_matrix_u),
        dot(homogeneous_position, horizon_map_matrix_v));

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

    float3 gobo_position = mul(homogeneous_position, local_to_gobo).xyz;
    output.normal_gobo_x.w   = gobo_position.x;
    output.tangent_gobo_z.w  = gobo_position.z;
    output.binormal_gobo_y.w = gobo_position.y;

    float ambient_position = saturate(
        dot(homogeneous_position, local_to_ambient)) * 6.999;
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

#if NGL_SM_CHARACTER_FORCE_TINT
    if (force_tint > 0.5) {
        output.ambient_lo.rgb = 0.5;
        output.ambient_hi.rgb = 0.5;
    }
#endif

    float3 fog_direction      = local_position.xyz - fog_eye_to_local;
    float  fog_distance       = length(fog_direction);
    float  inverse_fog_length = 1.0 / dot(fog_normal, fog_direction);
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
