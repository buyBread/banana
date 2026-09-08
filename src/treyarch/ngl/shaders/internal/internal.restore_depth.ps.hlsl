#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState Texture0 : register(s0);

struct pixel_output {
    float4 color : COLOR0;
    float  depth : DEPTH0;
};

pixel_output main(float2 texcoord : TEXCOORD0) {
    pixel_output output;

    output.color = 0.0;
    output.depth = tex2D(Texture0, texcoord).r;

    return output;
}

#endif
