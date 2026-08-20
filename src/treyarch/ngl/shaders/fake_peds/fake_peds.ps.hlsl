#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState diffuse_sampler : register(s0);

struct pixel_input {
    float4 color    : COLOR0;
    float2 texcoord : TEXCOORD0;
};

float4 main(pixel_input input) : COLOR0 {
    float4 color = tex2D(diffuse_sampler, input.texcoord) * input.color;

    color.rgb *= color.rgb;

    return color;
}

#endif
