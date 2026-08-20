#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

#if NGL_SM_BRIGHT_FULLSCREEN_POSITION_3D

struct vertex_input {
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

struct vertex_output {
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

vertex_output main(vertex_input input) {
    vertex_output output;
    output.position = float4(input.position, 1.0);
    output.texcoord = input.texcoord;
    return output;
}

#elif NGL_SM_BRIGHT_TEXCOORD_OFFSET

float2 texcoord_offset : register(c0);

struct vertex_input {
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

struct vertex_output {
    float4 position        : POSITION0;
    float2 texcoord        : TEXCOORD0;
    float2 offset_texcoord : TEXCOORD1;
};

vertex_output main(vertex_input input) {
    vertex_output output;
    output.position = float4(input.position, 1.0);
    output.texcoord = input.texcoord;
    output.offset_texcoord = input.texcoord +
        texcoord_offset * (1.0 - input.texcoord * 2.0);
    return output;
}

#elif NGL_SM_BRIGHT_FULLSCREEN_POSITION_2D

struct vertex_input {
    float2 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

struct vertex_output {
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

vertex_output main(vertex_input input) {
    vertex_output output;
    output.position = float4(input.position, 0.0, 1.0);
    output.texcoord = input.texcoord;
    return output;
}

#elif NGL_SM_BRIGHT_RAY_ADJUST

float4 ray_adjust : register(c0);

struct vertex_input {
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

struct vertex_output {
    float4 position : POSITION0;
    float4 texcoord : TEXCOORD0;
};

vertex_output main(vertex_input input) {
    vertex_output output;
    output.position = float4(input.position, 1.0);
    output.texcoord =
        (input.texcoord.xyxy - ray_adjust.xyxy) * ray_adjust.zwzw +
        ray_adjust.xyxy;
    return output;
}

#elif NGL_SM_BRIGHT_DOWNSAMPLE_OFFSET

float4 downsample_offset : register(c0);

struct vertex_input {
    float2 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

struct vertex_output {
    float4 position : POSITION0;
    float4 texcoord : TEXCOORD0;
};

vertex_output main(vertex_input input) {
    vertex_output output;
    output.position = float4(input.position, 0.0, 1.0);
    output.texcoord = input.texcoord.xyxy + downsample_offset;
    return output;
}

#elif NGL_SM_BRIGHT_ZOOM

float4 zoom_base : register(c0);

struct vertex_input {
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

struct vertex_output {
    float4 position : POSITION0;
    float4 texcoord : TEXCOORD0;
};

vertex_output main(vertex_input input) {
    vertex_output output;
    output.position = float4(input.position, 1.0);
    output.texcoord.xy = input.texcoord;
    output.texcoord.zw =
        (input.texcoord - zoom_base.xy) * zoom_base.zw +
        zoom_base.xy;
    return output;
}

#endif

#endif
