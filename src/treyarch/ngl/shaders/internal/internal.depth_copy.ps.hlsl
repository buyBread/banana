#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState Texture0 : register(s0);

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float depth = tex2D(Texture0, texcoord).r;

    return mad(depth, float4(1.0, 0.0, 0.0, 0.0), float4(0.0, 0.0, 0.0, 1.0));
}

#endif
