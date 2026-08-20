#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState color_sampler : register(s0);

#if NGL_SM_BRIGHT_GAUSSIAN_BLUR_13 || \
    NGL_SM_BRIGHT_WARPED_GAUSSIAN_BLUR_X || \
    NGL_SM_BRIGHT_WARPED_GAUSSIAN_BLUR_Y

float blur_weights[13] : register(c0);
float2 blur_kernel[13] : register(c13);
float brightness_multiplier : register(c26);

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
#if NGL_SM_BRIGHT_WARPED_GAUSSIAN_BLUR_X
    float kernel_scale = (texcoord.x - 0.5) * (texcoord.x - 0.5) * 8.0;
#elif NGL_SM_BRIGHT_WARPED_GAUSSIAN_BLUR_Y
    float kernel_scale = (texcoord.y - 0.5) * (texcoord.y - 0.5) * 4.0;
#else
    const float kernel_scale = 1.0;
#endif

    float4 color = 0.0;

    [unroll]
    for (int index = 0; index < 13; ++index) {
        color += tex2D(
            color_sampler,
            texcoord + blur_kernel[index] * kernel_scale) *
            blur_weights[index];
    }

    return color * brightness_multiplier;
}

#elif NGL_SM_BRIGHT_GAUSSIAN_VERTICAL_NEAR

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    const float offset = 0.00333333341;

    return (
        tex2D(color_sampler, texcoord + float2(0.0, -offset)) +
        tex2D(color_sampler, texcoord + float2(0.0,  offset))) *
        0.3125;
}

#elif NGL_SM_BRIGHT_GAUSSIAN_VERTICAL_OFFSET

float4 gaussian_vertical_offset : register(c0);

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    float2 offset = gaussian_vertical_offset.wy;

    return (
        tex2D(color_sampler, texcoord - offset) +
        tex2D(color_sampler, texcoord + offset)) *
        0.3125;
}

#elif NGL_SM_BRIGHT_GAUSSIAN_VERTICAL_FAR

float4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    const float offset = 0.0133333337;

    return (
        tex2D(color_sampler, texcoord + float2(0.0, -offset)) +
        tex2D(color_sampler, texcoord + float2(0.0,  offset))) *
        0.3125;
}

#elif NGL_SM_BRIGHT_GAUSSIAN_BLUR_3

float4 gaussian_offset : register(c0);

half4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    half4 color = tex2D(color_sampler, texcoord) * 0.375;
    color += tex2D(color_sampler, texcoord + gaussian_offset.xy) * 0.3125;
    color += tex2D(color_sampler, texcoord + gaussian_offset.zw) * 0.3125;

    return color;
}

#elif NGL_SM_BRIGHT_BOX_BLUR_4 || NGL_SM_BRIGHT_BOX_BLUR_RGB_PRESERVE_ALPHA

float4 sample_offset : register(c0);

half4 sample_box(float2 texcoord) {
    return
        tex2D(color_sampler, texcoord + sample_offset.xy) +
        tex2D(color_sampler, texcoord + sample_offset.zw) +
        tex2D(color_sampler, texcoord - sample_offset.xy) +
        tex2D(color_sampler, texcoord - sample_offset.zw);
}

half4 main(float2 texcoord : TEXCOORD0) : COLOR0 {
    half4 filtered = sample_box(texcoord) * 0.25;

#if NGL_SM_BRIGHT_BOX_BLUR_RGB_PRESERVE_ALPHA
    filtered.a = tex2D(color_sampler, texcoord).a;
#endif

    return filtered;
}

#endif

#endif
