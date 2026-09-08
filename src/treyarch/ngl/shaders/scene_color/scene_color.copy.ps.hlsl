#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState scene_color_sampler : register(s0);

half4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    return (half4)tex2D(scene_color_sampler, texcoord);
}

#endif
