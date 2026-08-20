#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float4x4 local_to_screen : register(c0);
float4 tint_color : register(c4);

struct vertex_input {
    float4 position : POSITION0;
    float4 color    : COLOR0;
    float2 texcoord : TEXCOORD0;
};

struct vertex_output {
    float4 position        : POSITION0;
    float4 color           : COLOR0;
    float2 texcoord        : TEXCOORD0;
    float4 screen_position : TEXCOORD1;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    float4 local_position = float4(input.position.xyz * 0.01, 1.0);

    output.position = mul(local_position, local_to_screen);
    output.color    = input.color * tint_color;
    output.texcoord = input.texcoord / 1024.0;

    output.screen_position = float4(
        (output.position.x + output.position.w) * 0.5,
        (output.position.w - output.position.y) * 0.5,
        output.position.z,
        output.position.w);

    return output;
}

#endif
