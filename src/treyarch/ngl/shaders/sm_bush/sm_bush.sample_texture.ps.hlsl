#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState texture_sampler : register(s3);

struct pixel_input {
    half4 texcoord : TEXCOORD3;
};

half4 main(pixel_input input) : COLOR0 {
#ifdef NGL_SM_BUSH_AUXILIARY_COORDINATES
    half4 sample = tex2D(texture_sampler, input.texcoord.zw);
#else
    half4 sample = tex2D(texture_sampler, input.texcoord.xy);
#endif

    return sample.xyzx * half4(1.0, 1.0, 1.0, 0.0) +
        half4(0.0, 0.0, 0.0, 1.0);
}

#endif
