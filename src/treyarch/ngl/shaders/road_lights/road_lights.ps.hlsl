#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState diffuse_sampler : register(s0);

struct pixel_input {
    float3 texcoord : TEXCOORD0;
};

float4 main(pixel_input input) : COLOR0 {
    return tex2D(diffuse_sampler, input.texcoord.xy) * input.texcoord.z;
}

#endif
