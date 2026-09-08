#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState Texture0 : register(s0);
SamplerState Texture1 : register(s1);
SamplerState Texture2 : register(s2);
SamplerState Texture3 : register(s3);

half4 main(float2 texcoord0 : TEXCOORD0,
           float2 texcoord1 : TEXCOORD1,
           float2 texcoord2 : TEXCOORD2,
           float2 texcoord3 : TEXCOORD3) : COLOR0 {

    float4 color = tex2D(Texture1, texcoord1) * 0.25;

    color += tex2D(Texture0, texcoord0) * 0.25;
    color += tex2D(Texture2, texcoord2) * 0.25;

    return (half4)(tex2D(Texture3, texcoord3) * 0.25 + color);
}

#endif
