#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState scene_color_sampler : register(s0);

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float4 color = tex2D(scene_color_sampler, texcoord);

    return color;
}

#endif
