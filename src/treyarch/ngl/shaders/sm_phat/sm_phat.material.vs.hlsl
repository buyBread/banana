#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 ambient_color_lo[8] : register(c0);
float4 ambient_color_hi[8] : register(c8);

row_major float4x4 local_to_gobo        : register(c16);
row_major float4x4 light_to_screen_0    : register(c20);
row_major float4x4 light_to_screen_1    : register(c24);
row_major float4x4 compressed_to_uv     : register(c28);
row_major float4x4 compressed_to_screen : register(c32);
row_major float4x4 compressed_to_local  : register(c36);

float4 local_to_ambient     : register(c40);
float3 fog_eye_to_local     : register(c41);
float3 fog_normal           : register(c42);
float3 fog_position_0       : register(c43);
float3 fog_position_1       : register(c44);
float2 fogs_per_meter       : register(c45);
float4 horizon_map_matrix_u : register(c46);
float4 horizon_map_matrix_v : register(c47);

struct vertex_input {
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal   : NORMAL0;
    float3 binormal : BINORMAL0;
    float3 tangent  : TANGENT0;
};

struct vertex_output {
    float4 position          : POSITION0;
    float3 local_position    : TEXCOORD0;
    float4 texcoord          : TEXCOORD1;
    float4 normal_gobo_x     : TEXCOORD2;
    float4 tangent_gobo_z    : TEXCOORD3;
    float4 binormal_gobo_y   : TEXCOORD4;
    float4 shadow_position_0 : TEXCOORD5;
    float4 shadow_position_1 : TEXCOORD6;
    float4 ambient_lo        : TEXCOORD7;
    float4 ambient_hi        : TEXCOORD8;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    float4 local_position = mul(
        float4(input.position.xyz, 1.0),
        compressed_to_local);

    output.position = mul(
        float4(input.position.xyz, 1.0),
        compressed_to_screen);
    output.local_position = local_position.xyz;
    output.shadow_position_0 = mul(local_position, light_to_screen_0);
    output.shadow_position_1 = mul(local_position, light_to_screen_1);

    float4 source_texcoord = float4(input.texcoord, 0.0, 1.0);
    output.texcoord.xy = mul(source_texcoord, compressed_to_uv).xy;
    output.texcoord.z = dot(local_position, horizon_map_matrix_u);
    output.texcoord.w = dot(local_position, horizon_map_matrix_v);

    float ambient_position =
        saturate(dot(local_position, local_to_ambient)) * 6.999;
    int ambient_index = (int)floor(ambient_position);
    float ambient_fraction = frac(ambient_position);

    output.ambient_lo.rgb = lerp(
        ambient_color_lo[ambient_index].rgb,
        ambient_color_lo[ambient_index + 1].rgb,
        ambient_fraction);
    output.ambient_hi.rgb = lerp(
        ambient_color_hi[ambient_index].rgb,
        ambient_color_hi[ambient_index + 1].rgb,
        ambient_fraction);

    float3 fog_direction = local_position.xyz - fog_eye_to_local;
    float fog_distance = length(fog_direction);
    float inverse_fog_length = 1.0 / dot(fog_normal, fog_direction);
    float fog_start = saturate(
        dot(fog_normal, fog_position_0 - fog_eye_to_local) *
        inverse_fog_length);
    float fog_end = saturate(
        dot(fog_normal, fog_position_1 - fog_eye_to_local) *
        inverse_fog_length);

    output.ambient_lo.a = min(
        abs(fog_start - fog_end) * fog_distance * fogs_per_meter.x,
        fogs_per_meter.y);
    output.ambient_hi.a = input.position.w * (1.0 / 255.0);

    float3 gobo_position = mul(local_position, local_to_gobo).xyz;

    output.normal_gobo_x   = float4(input.normal,  gobo_position.x);
    output.tangent_gobo_z  = float4(input.tangent, gobo_position.z);
    output.binormal_gobo_y = float4(input.binormal, gobo_position.y);

    return output;
}

#endif
