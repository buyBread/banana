#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

struct pixel_input {
    half4 ambient : TEXCOORD8;
};

half4 main(pixel_input input) : COLOR0 {
    return input.ambient.w;
}

#endif
