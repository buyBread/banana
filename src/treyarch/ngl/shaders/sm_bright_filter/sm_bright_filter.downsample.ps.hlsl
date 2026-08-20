#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState color_sampler : register(s0);

#if NGL_SM_BRIGHT_LINE_BLUR_16

half4 main(float4 texcoord : TEXCOORD0) : COLOR0 {
    half value = 0.0;

    [unroll]
    for (int index = 0; index < 16; ++index) {
        half interpolation = (half)index / 15.0;
        value += tex2D(
            color_sampler,
            lerp(texcoord.xy, texcoord.zw, interpolation)).r;
    }

    return value * (1.0 / 16.0);
}

#elif NGL_SM_BRIGHT_DOWNSAMPLE_20 || \
      NGL_SM_BRIGHT_DOWNSAMPLE_BLOOM_20 || \
      NGL_SM_BRIGHT_DOWNSAMPLE_ALPHA_20

float4 downsample_offset_0 : register(c0);
float4 downsample_offset_1 : register(c1);
float4 downsample_offset_2 : register(c2);
float4 downsample_offset_3 : register(c3);
float4 downsample_offset_4 : register(c4);
float4 downsample_offset_5 : register(c5);

#if NGL_SM_BRIGHT_DOWNSAMPLE_BLOOM_20
float4 bloom_sample_weights : register(c6);
#endif

half4 sample_group_0(float4 texcoord) {
    return
        tex2D(color_sampler, texcoord.zw + downsample_offset_0.xy) +
        tex2D(color_sampler, texcoord.zw + downsample_offset_0.zw) +
        tex2D(color_sampler, texcoord.zw + downsample_offset_1.xy) +
        tex2D(color_sampler, texcoord.zw + downsample_offset_1.zw);
}

half4 sample_group_1(float4 texcoord) {
    return
        tex2D(color_sampler, texcoord.xy + downsample_offset_2.xy) +
        tex2D(color_sampler, texcoord.xy + downsample_offset_2.zw) +
        tex2D(color_sampler, texcoord.zw + downsample_offset_3.xy) +
        tex2D(color_sampler, texcoord.zw + downsample_offset_3.zw);
}

half4 sample_group_2(float4 texcoord) {
    return
        tex2D(color_sampler, texcoord.xy + downsample_offset_0.xy) +
        tex2D(color_sampler, texcoord.xy + downsample_offset_0.zw) +
        tex2D(color_sampler, texcoord.xy + downsample_offset_1.xy) +
        tex2D(color_sampler, texcoord.xy + downsample_offset_1.zw);
}

half4 sample_group_3(float4 texcoord) {
    return
        tex2D(color_sampler, texcoord.zw + downsample_offset_2.xy) +
        tex2D(color_sampler, texcoord.zw + downsample_offset_2.zw) +
        tex2D(color_sampler, texcoord.zw + downsample_offset_4.xy) +
        tex2D(color_sampler, texcoord.zw + downsample_offset_4.zw);
}

half4 sample_group_4(float4 texcoord) {
    return
        tex2D(color_sampler, texcoord.xy + downsample_offset_3.xy) +
        tex2D(color_sampler, texcoord.xy + downsample_offset_3.zw) +
        tex2D(color_sampler, texcoord.xy + downsample_offset_5.xy) +
        tex2D(color_sampler, texcoord.xy + downsample_offset_5.zw);
}

half4 main(float4 texcoord : TEXCOORD0) : COLOR0 {
#if NGL_SM_BRIGHT_DOWNSAMPLE_20
    return
        sample_group_0(texcoord) * 0.0625 +
        sample_group_1(texcoord) * 0.09375 +
        sample_group_2(texcoord) * 0.0625 +
        sample_group_3(texcoord) * 0.015625 +
        sample_group_4(texcoord) * 0.015625;
#elif NGL_SM_BRIGHT_DOWNSAMPLE_BLOOM_20
    half4 group_0 = sample_group_0(texcoord) * 0.25;
    half4 group_1 = sample_group_1(texcoord) * 0.25;
    half4 group_2 = sample_group_2(texcoord) * 0.25;
    half4 group_3 = sample_group_3(texcoord) * 0.25;
    half4 group_4 = sample_group_4(texcoord) * 0.25;

    group_0 *= group_0.a;
    group_1 *= group_1.a;
    group_2 *= group_2.a;
    group_3 *= group_3.a;
    group_4 *= group_4.a;

    return
        group_0 * bloom_sample_weights.y +
        group_1 * bloom_sample_weights.x +
        group_2 * bloom_sample_weights.y +
        group_3 * bloom_sample_weights.z +
        group_4 * bloom_sample_weights.z;
#else
    half alpha_0 = sample_group_0(texcoord).a * 0.25;
    half alpha_1 = sample_group_1(texcoord).a * 0.25;
    half alpha_2 = sample_group_2(texcoord).a * 0.25;
    half alpha_3 = sample_group_3(texcoord).a * 0.25;
    half alpha_4 = sample_group_4(texcoord).a * 0.25;

    return
        alpha_0 * half4(0.00725,   0.00725,   0.00725,   0.25) +
        alpha_1 * half4(0.010875,  0.010875,  0.010875,  0.375) +
        alpha_2 * half4(0.00725,   0.00725,   0.00725,   0.25) +
        alpha_3 * half4(0.0018125, 0.0018125, 0.0018125, 0.0625) +
        alpha_4 * half4(0.0018125, 0.0018125, 0.0018125, 0.0625);
#endif
}

#elif NGL_SM_BRIGHT_DOWNSAMPLE_RGB_16

float4 downsample_offset_0 : register(c0);
float4 downsample_offset_1 : register(c1);
float4 downsample_offset_2 : register(c2);
float4 downsample_offset_3 : register(c3);
float4 downsample_offset_4 : register(c4);
float4 downsample_offset_5 : register(c5);
float4 downsample_offset_6 : register(c6);
float4 downsample_offset_7 : register(c7);

half3 sample_pair(float2 texcoord, float4 offset) {
    return
        tex2D(color_sampler, texcoord + offset.xy).rgb +
        tex2D(color_sampler, texcoord + offset.zw).rgb;
}

half4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    half3 color =
        sample_pair(texcoord, downsample_offset_0) +
        sample_pair(texcoord, downsample_offset_1) +
        sample_pair(texcoord, downsample_offset_2) +
        sample_pair(texcoord, downsample_offset_3) +
        sample_pair(texcoord, downsample_offset_4) +
        sample_pair(texcoord, downsample_offset_5) +
        sample_pair(texcoord, downsample_offset_6) +
        sample_pair(texcoord, downsample_offset_7);

    return half4(color * (1.0 / 16.0), 0.0);
}

#elif NGL_SM_BRIGHT_LINE_BLUR_5

half4 main(float4 texcoord : TEXCOORD0) : COLOR0 {
    half3 color = 0.0;

    [unroll]
    for (int index = 0; index < 5; ++index) {
        color += tex2D(
            color_sampler,
            lerp(texcoord.xy, texcoord.zw, (half)index * 0.25)).rgb;
    }

    return half4(color * 0.2, 0.0);
}

#elif NGL_SM_BRIGHT_DOWNSAMPLE_BLOOM_4 || NGL_SM_BRIGHT_DOWNSAMPLE_ALPHA_4

float4 downsample_offset : register(c0);

#if NGL_SM_BRIGHT_DOWNSAMPLE_BLOOM_4
float4 bloom_scale : register(c1);
#endif

half4 sample_box(float2 texcoord) {
    return (
        tex2D(color_sampler, texcoord + downsample_offset.xy) +
        tex2D(color_sampler, texcoord + downsample_offset.zw) +
        tex2D(color_sampler, texcoord - downsample_offset.xy) +
        tex2D(color_sampler, texcoord - downsample_offset.zw)) * 0.25;
}

half4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    half4 color = sample_box(texcoord);

#if NGL_SM_BRIGHT_DOWNSAMPLE_BLOOM_4
    return color * color.a * bloom_scale;
#else
    return half4((color.a * 0.029).xxx, color.a);
#endif
}

#endif

#endif
