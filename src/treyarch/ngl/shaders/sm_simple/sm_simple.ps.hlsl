#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState diffuse_sampler : register(s0);
float4 alpha : register(c0);

struct pixel_input {
    float2 texcoord : TEXCOORD0;
    float4 color    : COLOR0;
};

float4 main(pixel_input input) : COLOR0 {
    float4 color = tex2D(diffuse_sampler, input.texcoord) * input.color;

    color.a *= alpha.x;

    return color;
}

#endif
