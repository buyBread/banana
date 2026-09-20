#include "treyarch/ngl/morph/vertex_codec.hh"

using namespace treyarch;

u32 ngl::morph_geometry::get_vertex_element_size(u8 type) {
    switch (type) {
        case 0:
        case 4:
        case 5:
        case 6:
        case 8:
        case 9:
        case 11:
        case 13:
        case 14:
        case 15:
            return 4;

        case 1:
        case 7:
        case 10:
        case 12:
        case 16:
            return 8;

        case 2:
            return 12;

        case 3:
            return 16;

        default:
            return 0;
    }
}

void ngl::morph_geometry::read_component_value(u32 value_type, const u8* &source, vector4 &output) {
    output = { 0.0f, 0.0f, 0.0f, 0.0f };

    switch (value_type) {
        case 0:
        case 1:
        case 2:
        case 3: {
            u32 count = value_type + 1;
            
            const f32* values = (const f32*)source;

            for (u32 index = 0; index < count; ++index)
                (&output.x)[index] = values[index];

            source += count * sizeof(f32);

            break;
        }

        case 4:
        case 5:
        case 6: {
            u32 count = value_type == 4 ? 1 : value_type == 5 ? 2 : 4;
            
            const i32* values = (const i32*)source;

            for (u32 index = 0; index < count; ++index)
                (&output.x)[index] = (f32)values[index];

            source += count * sizeof(i32);

            break;
        }

        case 7:
        case 8: {
            u32 count = value_type == 7 ? 2 : 4;
            
            const i16* values = (const i16*)source;

            for (u32 index = 0; index < count; ++index)
                (&output.x)[index] = (f32)values[index];

            source += count * sizeof(i16);

            break;
        }

        case 9: {
            for (u32 index = 0; index < 4; ++index)
                (&output.x)[index] = (f32)source[index];

            source += 4;

            break;
        }
    }
}

void ngl::morph_geometry::decode_base_vertices(      mesh_section*      section,
                                               const D3DVERTEXELEMENT9 &element,
                                               const u8*                mesh_data,
                                                     vector4*           output) {

    if (element.Type != D3DDECLTYPE_SHORT4N)
        return;

    void* mapped_data;
    u32 vertex_stride = section->vertex_definition_data->vertex_size;
    section->vertex_buffer->Lock( section->vertex_offset,
                                  section->vertex_count * vertex_stride,
                                 &mapped_data,
                                  0);

    const f32* offset = (const f32*)(mesh_data + 0x20);
    const f32* scale  = (const f32*)(mesh_data + 0x30);

    for (u32 index = 0; index < section->vertex_count; ++index) {
        const i16* encoded = (const i16*)((const u8*)mapped_data +
                                         index * vertex_stride + element.Offset);

        output[index].x = offset[0] + (f32)((f64)encoded[0] / 32767.0) * scale[0];
        output[index].y = offset[1] + (f32)((f64)encoded[1] / 32767.0) * scale[1];
        output[index].z = offset[2] + (f32)((f64)encoded[2] / 32767.0) * scale[2];
        output[index].w = 1.0f;
    }

    section->vertex_buffer->Unlock();
}

void ngl::morph_geometry::encode_morph_vertices(      IDirect3DVertexBuffer9*  buffer,
                                                      u32                      buffer_offset,
                                                      u32                      vertex_stride,
                                                      u32                      vertex_count,
                                                const D3DVERTEXELEMENT9       &element,
                                                const u8*                      mesh_data,
                                                const vector4*                 input) {

    if (element.Type != D3DDECLTYPE_SHORT4N)
        return;

    void* mapped_data;
    buffer->Lock( buffer_offset,
                  vertex_count * vertex_stride,
                 &mapped_data,
                  0);

    const f32* offset = (const f32*)(mesh_data + 0x20);
    const f32* scale  = (const f32*)(mesh_data + 0x30);

    f32 reciprocal_x = 1.0f / scale[0];
    f32 reciprocal_y = 1.0f / scale[1];
    f32 reciprocal_z = 1.0f / scale[2];

    for (u32 index = 0; index < vertex_count; ++index) {
        i16* encoded = (i16*)((u8*)mapped_data + index * vertex_stride + element.Offset);

        encoded[0] = (i16)(i32)((f32)((input[index].x - offset[0]) * reciprocal_x) * 32767.0f);
        encoded[1] = (i16)(i32)((f32)((input[index].y - offset[1]) * reciprocal_y) * 32767.0f);
        encoded[2] = (i16)(i32)((f32)((input[index].z - offset[2]) * reciprocal_z) * 32767.0f);
        encoded[3] = (i16)(i32)(input[index].w * 32767.0f);
    }

    buffer->Unlock();
}
