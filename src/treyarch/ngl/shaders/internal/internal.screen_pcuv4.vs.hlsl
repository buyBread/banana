#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

struct vertex_input {
    float3 position  : POSITION0;
    float4 color     : COLOR0;
    float2 texcoord0 : TEXCOORD0;
    float2 texcoord1 : TEXCOORD1;
    float2 texcoord2 : TEXCOORD2;
    float2 texcoord3 : TEXCOORD3;
};

struct vertex_output {
    float4 position  : POSITION0;
    float2 texcoord0 : TEXCOORD0;
    float2 texcoord1 : TEXCOORD1;
    float2 texcoord2 : TEXCOORD2;
    float2 texcoord3 : TEXCOORD3;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    output.position  = float4(input.position, 1.0);
    output.texcoord0 = input.texcoord0;
    output.texcoord1 = input.texcoord1;
    output.texcoord2 = input.texcoord2;
    output.texcoord3 = input.texcoord3;

    return output;
}

#endif
