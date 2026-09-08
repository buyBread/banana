#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState Texture0 : register(s0);
SamplerState Texture1 : register(s1);
SamplerState Texture2 : register(s2);

half4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float4 conversion0 = float4( 1.59579468, -0.813476562,  0.0,        1.16412354);
    float4 conversion1 = float4(-0.87065506,  0.529705048, -1.08166885, 0.0);
    float4 conversion2 = float4( 0.0,        -0.391448975,  2.01782227, 1.0);
    float3 color = tex2D(Texture1, texcoord).a * conversion0.xyz;

    color = mad(tex2D(Texture0, texcoord).a, conversion0.www, color);
    color = mad(tex2D(Texture2, texcoord).a, conversion2.xyz, color);
    color += conversion1.xyz;

    return half4(color, conversion2.w);
}

#endif
