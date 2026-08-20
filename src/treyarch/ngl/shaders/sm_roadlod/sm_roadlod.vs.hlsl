#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 ambient_color_lo[8] : register(c0);
float4 ambient_color_hi[8] : register(c8);

row_major float4x4 screen_matrix : register(c16);
row_major float4x4 world_matrix  : register(c20);

float4 local_to_ambient : register(c24);
float3 fog_eye_to_local : register(c25);
float3 fog_normal       : register(c26);
float3 fog_position_0   : register(c27);
float3 fog_position_1   : register(c28);
float2 fogs_per_meter   : register(c29);
float4 vertex_transform : register(c30);

struct vertex_input {
    float2 position            : POSITION0;
    float2 texture_coordinates : TEXCOORD0;
};

struct vertex_output {
    float4 position            : POSITION0;
    float3 world_position      : TEXCOORD0;
    float4 texture_coordinates : TEXCOORD1;
    float4 ambient_lo          : TEXCOORD2;
    float4 ambient_hi          : TEXCOORD3;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    float3 local_position = float3(
        input.position.x * vertex_transform.w + vertex_transform.x,
        14.5,
        input.position.y * vertex_transform.w + vertex_transform.z);
    float3 world_position = mul(float4(local_position, 1.0), world_matrix).xyz;

    output.position       = mul(float4(local_position, 1.0), screen_matrix);
    output.world_position = world_position;

    output.texture_coordinates.xy = input.texture_coordinates / 1024.0;
    output.texture_coordinates.z =
        world_position.x * 0.000103 + 0.343264;
    output.texture_coordinates.w =
        world_position.z * -0.000206 + 0.563229;

    float ambient_position = saturate(
        dot(float4(world_position, 1.0), local_to_ambient)) * 6.999;
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

    float3 fog_direction      = world_position - fog_eye_to_local;
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
