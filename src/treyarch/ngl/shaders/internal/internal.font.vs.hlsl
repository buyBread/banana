#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float4x4 LocalToScreen : register(c0);

struct vertex_input {
    float3 position : POSITION0;
    float4 color    : COLOR0;
    float2 texcoord : TEXCOORD0;
};

struct vertex_output {
    float4 position : POSITION0;
    float4 color    : COLOR0;
    float2 texcoord : TEXCOORD0;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    float4 screen_position = mul(float4(input.position, 1.0), LocalToScreen);

    output.position = float4(screen_position.xy, input.position.z, screen_position.w);
    output.color    = input.color;
    output.texcoord = input.texcoord;

    return output;
}

#endif
