#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState color_sampler : register(s0);

#if NGL_SM_BRIGHT_HIGH_PASS_LUMINANCE

float high_pass_filter_value : register(c0);

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float3 color = tex2D(color_sampler, texcoord).rgb;
    float luminance = dot(color, float3(0.3, 0.59, 0.11));

    return float4(max(luminance - high_pass_filter_value, 0.0).xxx, 1.0);
}

#elif NGL_SM_BRIGHT_PREMULTIPLY_ALPHA

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float4 color = tex2D(color_sampler, texcoord);

    return float4(color.rgb * color.a, 1.0);
}

#elif NGL_SM_BRIGHT_SAMPLE_TEXTURE

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    return tex2D(color_sampler, texcoord);
}

#elif NGL_SM_BRIGHT_TONE_MAP_LUMINANCE || NGL_SM_BRIGHT_TONE_MAP_DESATURATED

#if NGL_SM_BRIGHT_TONE_MAP_DESATURATED
float4 desaturate_color : register(c0);
#endif

float evaluate_tone_map(float brightness) {
    if (brightness < 1.2)
        return brightness * 0.183333337 + 0.1;

    if (brightness < 1.5)
        return brightness * brightness * (2.0 / 9.0);

    float shoulder = 2.0 - brightness * (2.0 / 3.0);
    return 1.0 - shoulder * shoulder * 0.5;
}

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float3 color = tex2D(color_sampler, texcoord).rgb;
    float value = evaluate_tone_map(color.r + color.g + color.b);

#if NGL_SM_BRIGHT_TONE_MAP_DESATURATED
    return float4(value * desaturate_color.rgb, 1.0);
#else
    return float4(value.xxx, 1.0);
#endif
}

#elif NGL_SM_BRIGHT_SAMPLE_RGB_HALF_ALPHA

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    return float4(tex2D(color_sampler, texcoord).rgb, 0.5);
}

#elif NGL_SM_BRIGHT_SCALE_COLOR

float4 color_scale : register(c0);

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    return tex2D(color_sampler, texcoord) * color_scale;
}

#elif NGL_SM_BRIGHT_SPIDEY_SENSE_BLOOM

float spidey_sense_bloom : register(c0);
float spidey_sense_power : register(c1);

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float3 color = tex2D(color_sampler, texcoord).rgb;
    float luminance = saturate(dot(color, float3(0.299, 0.587, 0.114)));
    float bloom = pow(luminance, spidey_sense_power) * spidey_sense_bloom;

    return float4(color * bloom + color, bloom);
}

#elif NGL_SM_BRIGHT_SAMPLE_TEXTURE_PARTIAL_PRECISION

half4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    return tex2D(color_sampler, texcoord);
}

#elif NGL_SM_BRIGHT_GAMMA_CORRECTION

float gamma_correction : register(c0);

half4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    half source = tex2D(color_sampler, texcoord).r;
    half corrected = pow(source, gamma_correction);

    return half4(corrected.xxx, 0.0);
}

#endif

#endif
