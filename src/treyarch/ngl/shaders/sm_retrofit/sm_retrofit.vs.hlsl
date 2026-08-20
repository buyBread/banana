#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 ambient_color_lo[8] : register(c0);
float4 ambient_color_hi[8] : register(c8);

row_major float4x4 light_world_view_projection : register(c16);
row_major float4x4 screen_matrix               : register(c20);

float4 local_to_ambient     : register(c24);
float4 lod_crossfade_control : register(c25);
float3 fog_eye_to_local     : register(c26);
float3 fog_normal           : register(c27);
float3 fog_position_0       : register(c28);
float3 fog_position_1       : register(c29);
float2 fogs_per_meter       : register(c30);
float4 horizon_map_matrix_u : register(c31);
float4 horizon_map_matrix_v : register(c32);
float4 vertex_transform     : register(c33);

struct vertex_input {
    float3 position   : POSITION0;
    float2 texcoord_0 : TEXCOORD0;
    float2 texcoord_1 : TEXCOORD1;
    float4 color      : COLOR0;
    float3 normal     : NORMAL0;
    float3 tangent    : TANGENT0;
    float3 binormal   : BINORMAL0;
};

struct vertex_output {
    float4 position             : POSITION0;
    float4 local_position       : TEXCOORD0;
    float4 texture_coordinates  : TEXCOORD1;
    float4 material_parameters  : TEXCOORD2;
    float4 normal_horizon_u     : TEXCOORD3;
    float4 tangent_horizon_v    : TEXCOORD4;
    float3 binormal             : TEXCOORD5;
    float4 shadow_position      : TEXCOORD6;
    float4 ambient_lo           : COLOR0;
    float4 ambient_hi           : COLOR1;
    float  lod_crossfade        : TEXCOORD7;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    float3 local_position = input.position * vertex_transform.w + vertex_transform.xyz;
    float4 homogeneous_position = float4(local_position, 1.0);

    output.position            = mul(homogeneous_position, screen_matrix);
    output.local_position      = homogeneous_position;
    output.texture_coordinates =
        float4(input.texcoord_0, input.texcoord_1) * (1.0 / 1024.0);
    output.material_parameters = input.color;
    output.normal_horizon_u    = float4(
        input.normal,
        dot(homogeneous_position, horizon_map_matrix_u));
    output.tangent_horizon_v = float4(
        input.tangent,
        dot(homogeneous_position, horizon_map_matrix_v));
    output.binormal        = input.binormal;
    output.shadow_position = mul(homogeneous_position, light_world_view_projection);
    output.lod_crossfade   = lod_crossfade_control.y;

    float ambient_position = saturate(dot(homogeneous_position, local_to_ambient)) * 6.999;
    int   ambient_index    = (int)floor(ambient_position);
    float ambient_fraction = frac(ambient_position);

    output.ambient_lo.rgb = lerp(
        ambient_color_lo[ambient_index].rgb,
        ambient_color_lo[ambient_index + 1].rgb,
        ambient_fraction);
    output.ambient_hi.rgb = lerp(
        ambient_color_hi[ambient_index].rgb,
        ambient_color_hi[ambient_index + 1].rgb,
        ambient_fraction);

    float3 fog_direction      = local_position - fog_eye_to_local;
    float  fog_distance       = length(fog_direction);
    float  inverse_fog_length = 1.0 / dot(fog_normal, fog_direction);
    float  fog_start          = saturate(
        dot(fog_normal, fog_position_0 - fog_eye_to_local) * inverse_fog_length);
    float  fog_end            = saturate(
        dot(fog_normal, fog_position_1 - fog_eye_to_local) * inverse_fog_length);

    output.ambient_lo.a = min(
        abs(fog_start - fog_end) * fog_distance * fogs_per_meter.x,
        fogs_per_meter.y);
    output.ambient_hi.a = 1.0;

    return output;
}

#endif
