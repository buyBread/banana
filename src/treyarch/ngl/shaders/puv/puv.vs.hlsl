#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float4x4 local_to_screen : register(c0);

struct vertex_input {
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

struct vertex_output {
    float4 position        : POSITION0;
    float4 projected_depth : TEXCOORD8;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    output.position = mul(float4(input.position, 1.0), local_to_screen);
    output.projected_depth = float4(
        0.0,
        0.0,
        output.position.z,
        output.position.w);

    return output;
}

#endif
