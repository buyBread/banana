#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float3x4 world[64] : register(c0);
row_major float4x4 world_view_projection : register(c192);
float4 uv_scale_offset                   : register(c196);

struct vertex_input {
    float3 position      : POSITION0;
    float2 texcoord      : TEXCOORD0;
    int4   blend_indices : BLENDINDICES0;
    float4 blend_weights : BLENDWEIGHT0;
};

struct vertex_output {
    float4 position       : POSITION0;
    float2 texcoord       : TEXCOORD1;
    float2 depth_position : TEXCOORD8;
};

float4 transform_position(float4 position, int index) {
    return float4(mul(world[index], position), 1.0);
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

vertex_output main(vertex_input input) {
    vertex_output output;

    float4 mesh_position = float4(input.position * 0.001, 1.0);
    float4 local_position = skin_position(
        mesh_position,
        input.blend_indices,
        input.blend_weights);

    output.position = mul(
        float4(local_position.xyz, 1.0),
        world_view_projection);
    output.texcoord =
        input.texcoord * uv_scale_offset.xy * (1.0 / 1024.0) +
        uv_scale_offset.zw;
    output.depth_position = output.position.zw;

    return output;
}

#endif
