#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float4x4 texture_matrix          : register(c0);
row_major float4x4 world_view_projection   : register(c4);
float4             compressed_to_position : register(c8);

struct vertex_input {
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

struct vertex_output {
    float4 position       : POSITION0;
    float2 texcoord       : TEXCOORD1;
    float4 depth_position : TEXCOORD8;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    float4 local_position = float4(
        input.position * compressed_to_position.w +
        compressed_to_position.xyz,
        1.0);

    output.position = mul(local_position, world_view_projection);
    output.texcoord = mul(
        float4(input.texcoord, 0.0, 1.0),
        texture_matrix).xy;
    output.depth_position =
        output.position.zwzz * float4(1.0, 1.0, 0.0, 0.0);

    return output;
}

#endif
