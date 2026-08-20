#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState source_sampler : register(s3);

float4 main(float2 texture_coordinates : TEXCOORD1) : COLOR0 {
    float4 sample = tex2D(source_sampler, texture_coordinates);

#if NGL_SM_PHATCHARNORMAL_SAMPLE_TEXTURE
    return sample;
#elif NGL_SM_PHATCHARNORMAL_SAMPLE_GREEN
    return sample.g;
#endif
}

#endif
