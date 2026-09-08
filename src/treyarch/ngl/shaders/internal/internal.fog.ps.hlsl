#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 FogValues : register(c0);
float4 RecalcZ   : register(c1);
float4 FogColor  : register(c2);

SamplerState Texture0 : register(s0);

half4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float stored_depth = tex2D(Texture0, texcoord).r;
    float depth        = RecalcZ.x / (1.0 + frac(-stored_depth) * RecalcZ.y);
    float fog          = saturate(depth * FogValues.y + FogValues.x);
    half  alpha        = (half)(fog * FogValues.w + FogValues.z);

    return half4(FogColor.rgb, alpha);
}

#endif
