#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float4x4 world_view_projection : register(c0);

struct vertex_input {
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

struct vertex_output {
    float4 position        : POSITION0;
    float2 texcoord        : TEXCOORD1;
    float2 projected_depth : TEXCOORD8;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    output.position        = mul(float4(input.position, 1.0), world_view_projection);
    output.projected_depth = output.position.zw;
    output.texcoord        = input.texcoord;

    return output;
}

#endif
