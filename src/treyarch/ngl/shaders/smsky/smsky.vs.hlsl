#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float4x4 local_to_screen : register(c0);

struct vertex_input {
    float3 position : POSITION0;
};

struct vertex_output {
    float4 position  : POSITION0;
    float4 direction : TEXCOORD0;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    float3 direction = normalize(input.position);
    float3 position  = direction * float3(8000.0, 4800.0, 8000.0);

    output.position  = mul(float4(position, 1.0), local_to_screen);
    output.direction = float4(direction, 0.0);

    return output;
}

#endif
