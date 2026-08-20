#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState source_sampler : register(s3);

float4 main(half2 texture_coordinates : TEXCOORD3) : COLOR0 {
    return tex2D(source_sampler, texture_coordinates);
}

#endif
