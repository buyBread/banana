#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

struct pixel_input {
    float4 projected_depth : TEXCOORD8;
};

float4 main(pixel_input input) : COLOR0 {
    float depth = input.projected_depth.z / input.projected_depth.w;

    return depth;
}

#endif
