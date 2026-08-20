#ifndef NGL_SHADER_RESOURCE_DEPENDENCY

row_major float4x4 screen_matrix : register(c0);
float4 world_translation         : register(c4);

float4 ambient_color_lo[8] : register(c140);
float4 ambient_color_hi[8] : register(c148);
float4 local_to_ambient    : register(c156);
float3 fog_eye_to_local    : register(c157);
float3 fog_normal          : register(c158);
float3 fog_position_0      : register(c159);
float3 fog_position_1      : register(c160);
float2 fogs_per_meter      : register(c161);

struct vertex_input {
    float2 position     : POSITION0;
    float4 packed_data_0 : COLOR0;
    float4 packed_data_1 : COLOR1;
};

struct vertex_output {
    float4 position            : POSITION0;
    float4 world_position      : TEXCOORD0;
    float4 atlas_coordinates   : TEXCOORD1;
    float4 normal_and_code     : TEXCOORD2;
    float4 texture_transform   : TEXCOORD3;
    float4 material_parameters : TEXCOORD4;
    float4 ambient_lo          : COLOR0;
    float4 ambient_hi          : COLOR1;
    float  opaque_alpha        : TEXCOORD9;
};

vertex_output main(vertex_input input) {
    vertex_output output;

    float height_code = floor(255.0 * input.packed_data_1.a);
    float height = height_code * 2.5 + 15.0;
    float2 local_xz = input.position * 0.01;

    float4 local_position = float4(local_xz.x, height, local_xz.y, 1.0);
    output.position       = mul(local_position, screen_matrix);
    output.world_position = local_position + world_translation;

    float primary_code = floor(input.packed_data_1.r * 255.0 + 1.0);
    float primary_group = floor(primary_code / 64.0);
    float primary_remainder = frac(primary_code / 64.0) * 64.0;

    float4 packed_bytes = floor(input.packed_data_0.wzyx * 255.0 + 0.5);
    float direction_code = packed_bytes.x;
    float row_code       = packed_bytes.y;
    float direction_group = floor(direction_code / 64.0);
    float direction_remainder = frac(direction_code / 64.0) * 64.0;
    float row_group     = floor(row_code / 16.0);
    float row_remainder = frac(row_code / 16.0) * 16.0;

    float angle =
        direction_remainder * (2.0 * 3.14159274 / 64.0) -
        (3.14159274 / 32.0);
    float angle_sine;
    float angle_cosine;
    sincos(angle, angle_sine, angle_cosine);

    float vertical_normal = direction_remainder <= 0.0 ? 1.0 : 0.0;
    output.normal_and_code.xyz = lerp(
        float3(-angle_cosine, 0.0, -angle_sine),
        float3(0.0, 1.0, 0.0),
        vertical_normal);
    output.normal_and_code.w = frac(primary_code / 32.0) * 32.0;

    output.atlas_coordinates.x = row_group / 16.0;
    output.atlas_coordinates.y =
        row_remainder / 32.0 + floor(primary_remainder / 32.0) * 0.5;
    output.atlas_coordinates.zw = packed_bytes.zw;

    float direction_high = direction_group >= 2.0 ? 1.0 : 0.0;
    float direction_odd  = frac(direction_group * 0.5) * 2.0;
    float coordinate_y = packed_bytes.z * 0.5 +
        (direction_group > 0.0 ? 0.5 : 0.0);
    float coordinate_z = packed_bytes.w * 0.5 +
        (direction_group >= 3.0 ? 0.5 : 0.0);
    float expanded_y = coordinate_y * (1.0 + direction_odd);
    float coordinate_delta = coordinate_z * 2.0 - expanded_y;

    output.texture_transform.xy = float2(
        expanded_y + direction_high * coordinate_delta,
        coordinate_z - direction_high * coordinate_delta * 0.5);
    output.texture_transform.z = direction_group <= 0.0 ? 1.0 / 8.0 : 1.0 / 16.0;
    output.texture_transform.w = 1.0 / 32.0;

    float material_code_0 = floor(
        (input.packed_data_1.b + 0.01) * 255.0 + 0.5);
    float material_code_1 = floor(
        -input.packed_data_1.g * 255.0 + 0.5);
    float material_major_0 = floor(material_code_0 * 0.25);
    float material_minor_0 = floor(frac(material_code_0 * 0.25) * 4.0);
    float material_major_1 = floor(material_code_1 / 16.0);
    float material_minor_1 = floor(frac(material_code_1 / 16.0) * 16.0);

    output.material_parameters = float4(
        material_major_0 / 63.0,
        (material_minor_0 * 16.0 + material_major_1) / 63.0,
        (material_minor_1 + 0.5) / 15.0,
        0.0);

    float ambient_position = saturate(
        dot(output.world_position, local_to_ambient)) * 6.999;
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

    float3 fog_direction      = output.world_position.xyz - fog_eye_to_local;
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
    output.opaque_alpha = 1.0;

    return output;
}

#endif
