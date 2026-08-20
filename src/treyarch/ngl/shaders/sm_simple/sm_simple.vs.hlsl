#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float4x4 local_to_screen : register(c0);
float4 tint_color : register(c4);
float2 uv_scroll : register(c5);

struct vertex_input {
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float4 color    : COLOR0;
};

struct vertex_output {
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float4 color    : COLOR0;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    output.position = mul(float4(input.position, 1.0), local_to_screen);
    output.color    = input.color * tint_color;
    output.texcoord = input.texcoord + uv_scroll;

    return output;
}

#endif
