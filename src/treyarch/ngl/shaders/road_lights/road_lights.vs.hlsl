#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float4x4 world_to_screen : register(c0);

struct vertex_input {
    float4 light : POSITION0;
};

struct vertex_output {
    float4 position : POSITION0;
    float3 texcoord : TEXCOORD0;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    float sine;
    float cosine;

    output.texcoord.z = max(1.0 - abs(input.light.y + 0.01), 0.0);

    float light_height = max(input.light.y, -0.01);

    output.position = mul(
        float4(input.light.x, light_height, input.light.z, 1.0),
        world_to_screen);

    sincos(input.light.w, sine, cosine);

    output.texcoord.xy = float2(cosine, sine) *
        float2(0.70710678, -0.70710678) + 0.5;

    return output;
}

#endif
