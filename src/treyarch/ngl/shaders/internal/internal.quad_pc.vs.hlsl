#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float4x4 LocalToScreen : register(c0);

struct vertex_input {
    float3 position : POSITION0;
    float4 color    : COLOR0;
};

struct vertex_output {
    float4 position : POSITION0;
    float4 color    : COLOR0;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    output.position = mul(float4(input.position, 1.0), LocalToScreen);
    output.color    = input.color;

    return output;
}

#endif
