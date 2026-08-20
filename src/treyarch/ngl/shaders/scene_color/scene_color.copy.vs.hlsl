#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

struct vertex_input {
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

struct vertex_output {
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    output.position = float4(input.position, 1.0);
    output.texcoord = input.texcoord;

    return output;
}

#endif
