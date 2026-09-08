#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

SamplerState Texture0 : register(s0);

half4 main(half4 color : COLOR0, float2 texcoord : TEXCOORD0) : COLOR0 {
    half4 texture_color = (half4)tex2D(Texture0, texcoord);

    return texture_color * color;
}

#endif
