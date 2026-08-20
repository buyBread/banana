#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float3 eye : register(c0);

struct pixel_input {
    float3 local_position : TEXCOORD0;
    half3  direction      : TEXCOORD4;
};

half4 main(pixel_input input) : COLOR0 {
    half facing = dot(
        normalize(input.direction),
        normalize(eye - input.local_position));
    half2 tail = facing * half2(1.0, 0.0) + half2(0.5, 0.0);

    return half4(facing, facing, tail);
}

#endif
