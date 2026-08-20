#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState opacity_sampler : register(s0);

struct pixel_input {
    float2 texcoord        : TEXCOORD1;
    float2 projected_depth : TEXCOORD8;
};

float4 main(pixel_input input) : COLOR0 {
    float depth   = input.projected_depth.x / input.projected_depth.y;
    float opacity = tex2D(opacity_sampler, input.texcoord).a;

    return float4(depth, depth, depth, opacity);
}

#endif
