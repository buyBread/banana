#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

#if NGL_SM_PHAT_DEBUG_VARIANT == 5
SamplerState debug_sampler : register(s3);
#else
SamplerState debug_sampler : register(s3);
#endif

#if NGL_SM_PHAT_DEBUG_VARIANT == 1 || NGL_SM_PHAT_DEBUG_VARIANT == 5
float3 eye : register(c0);
#endif

struct pixel_input {
    float3 local_position    : TEXCOORD0;
    float4 texcoord          : TEXCOORD1;
    half4  normal_gobo_x     : TEXCOORD2;
    half4  tangent_gobo_z    : TEXCOORD3;
    half4  binormal_gobo_y   : TEXCOORD4;
    float4 shadow_position_0 : TEXCOORD5;
    float4 shadow_position_1 : TEXCOORD6;
};

half4 main(pixel_input input) : COLOR0 {
#if NGL_SM_PHAT_DEBUG_VARIANT == 0
    return half4(tex2D(debug_sampler, input.texcoord.xy).rgb, 0.0);
#elif NGL_SM_PHAT_DEBUG_VARIANT == 1
    half facing = dot(
        normalize(input.normal_gobo_x.xyz),
        normalize(eye - input.local_position));

    return half4(facing, facing, facing + 1.0, facing + 0.5);
#elif NGL_SM_PHAT_DEBUG_VARIANT == 2
    return half4(tex2D(debug_sampler, input.texcoord.zw).rgb, 0.0);
#elif NGL_SM_PHAT_DEBUG_VARIANT == 3
    float2 projected =
        float2(input.normal_gobo_x.w, input.tangent_gobo_z.w) /
        input.binormal_gobo_y.w + 0.5;

    return half4(tex2D(debug_sampler, projected).rgb, 0.0);
#elif NGL_SM_PHAT_DEBUG_VARIANT == 4
    half2 encoded = tex2D(debug_sampler, input.texcoord.xy).wy * 2.0 - 1.0;
    half3 normal = half3(
        encoded,
        sqrt(max(0.0, 1.0 - dot(encoded, encoded))));

    return half4(normal * 0.5 + 0.5, 0.0);
#elif NGL_SM_PHAT_DEBUG_VARIANT == 5
    half3 eye_direction = normalize(eye - input.local_position);
    half3 reflection_direction = reflect(
        -eye_direction,
        input.normal_gobo_x.xyz);

    return half4(texCUBE(debug_sampler, reflection_direction).rgb, 0.0);
#elif NGL_SM_PHAT_DEBUG_VARIANT == 6
    float2 projected =
        input.shadow_position_0.xy * 0.5 /
        input.shadow_position_0.w + 0.5;

    return half4(tex2D(debug_sampler, projected).rgb, 0.0);
#else
    float2 projected =
        input.shadow_position_1.xy * 0.5 /
        input.shadow_position_1.w + 0.5;

    return half4(tex2D(debug_sampler, projected).rgb, 0.0);
#endif
}

#endif
