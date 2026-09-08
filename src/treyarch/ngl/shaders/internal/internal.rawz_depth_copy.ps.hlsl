#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState Texture0 : register(s0);

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float4 packed_depth = tex2D(Texture0, texcoord);
    float depth = dot(packed_depth.arg, float3(0.99609381, 0.00389099144, 0.0000151991853));

    return float4(depth, 0.0, 0.0, 1.0);
}

#endif
