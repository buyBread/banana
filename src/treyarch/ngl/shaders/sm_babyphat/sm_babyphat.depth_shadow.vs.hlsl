#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float4x4 compressed_to_uv     : register(c0);
row_major float4x4 compressed_to_screen : register(c4);

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

    output.texcoord = mul(
        float4(input.texcoord, 0.0, 1.0),
        compressed_to_uv).xy;
    output.position = mul(
        float4(input.position, 1.0),
        compressed_to_screen);
    output.depth_position =
        output.position.zwzz * float4(1.0, 1.0, 0.0, 0.0);

    return output;
}

#endif
