#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState source_sampler : register(s3);

half4 main(float2 texture_coordinates : TEXCOORD1) : COLOR0 {
    return tex2D(source_sampler, texture_coordinates);
}

#endif
