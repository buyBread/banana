#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

float4 ambient_color_lo[8] : register(c0);
float4 ambient_color_hi[8] : register(c8);

row_major float4x4 screen_matrix : register(c16);
row_major float4x4 world_matrix  : register(c20);

float4 local_to_ambient  : register(c24);
float3 fog_eye_to_local  : register(c25);
float3 fog_normal        : register(c26);
float3 fog_position_0    : register(c27);
float3 fog_position_1    : register(c28);
float2 fogs_per_meter    : register(c29);
float4 vertex_transform  : register(c30);

struct vertex_input {
    float4 position : POSITION0;
    float4 color    : COLOR0;
    float4 normal   : NORMAL0;
};

struct vertex_output {
    float4 position      : POSITION0;
    float4 world_position : TEXCOORD0;
    float4 normal         : TEXCOORD1;
    float4 material_color : TEXCOORD2;
    float4 ambient_lo     : COLOR0;
    float4 ambient_hi     : COLOR1;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    float3 local_position = input.position.xyz * vertex_transform.w + vertex_transform.xyz;
    float3 world_position = mul(float4(local_position, 1.0), world_matrix).xyz;

    output.position = mul(float4(local_position, 1.0), screen_matrix);

    float ambient_position = saturate(dot(float4(world_position, 1.0), local_to_ambient)) * 6.999;
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

    output.world_position = float4(world_position, input.position.w / 255.0);
    output.normal         = input.normal;
    output.material_color = input.color;

    return output;
}

#endif
