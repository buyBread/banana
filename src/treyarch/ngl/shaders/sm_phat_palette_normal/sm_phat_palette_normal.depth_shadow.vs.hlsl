#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float4x4 world_view_projection : register(c0);
float4 uv_scale_offset                   : register(c4);
float4 compressed_to_local               : register(c5);

struct vertex_input {
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

struct vertex_output {
    float4 position       : POSITION0;
    float2 texcoord       : TEXCOORD1;
    float2 depth_position : TEXCOORD8;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    float4 local_position = float4(
        input.position * compressed_to_local.w + compressed_to_local.xyz,
        1.0);

    output.position = mul(local_position, world_view_projection);
    output.texcoord =
        input.texcoord * uv_scale_offset.xy * (1.0 / 1024.0) +
        uv_scale_offset.zw;
    output.depth_position = output.position.zw;

    return output;
}

#endif
