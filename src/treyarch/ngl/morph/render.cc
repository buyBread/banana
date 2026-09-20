#include <cstring>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/ngl/morph/render_node.hh"
#include "treyarch/ngl/morph/vertex_codec.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

namespace treyarch { namespace ngl { namespace morph_geometry {
    void on_stream_segment(d3d9::geometry_stream::segment* value, void* user_data);

    const D3DVERTEXELEMENT9* find_vertex_element(const vertex_definition*  definition,
                                                 const morph_component    &component);

    void apply_dense_frame(const morph_component &component,
                                 f32              weight,
                                 u32              vertex_count,
                                 vector4*         vertices);

    void apply_sparse_frame(const morph_component &component,
                                  f32              weight,
                                  u32              vertex_count,
                                  vector4*         vertices);

    void write_morph_stream(render_node* value,
                            binding*     morph_binding,
                            u32          vertex_stride);
}}} // treyarch::ngl::morph_geometry

void ngl::morph_geometry::on_stream_segment(d3d9::geometry_stream::segment* value, void* user_data) {
    (void)value;
    (void)user_data;
}

const D3DVERTEXELEMENT9* ngl::morph_geometry::find_vertex_element(const vertex_definition*  definition,
                                                                  const morph_component    &component) {

    const D3DVERTEXELEMENT9* element = definition->elements;

    while (element->Type != D3DDECLTYPE_UNUSED) {
        if (element->Usage == component.semantic &&
            element->UsageIndex == component.semantic_index) {

            return element;
        }

        ++element;
    }

    return nullptr;
}

void ngl::morph_geometry::apply_dense_frame(const morph_component &component,
                                                  f32              weight,
                                                  u32              vertex_count,
                                                  vector4*         vertices) {

    const u8* source = (const u8*)component.data;

    for (u32 index = 0; index < vertex_count; ++index) {
        vector4 delta;
        read_component_value(component.value_type, source, delta);

        vertices[index].x += delta.x * weight;
        vertices[index].y += delta.y * weight;
        vertices[index].z += delta.z * weight;
        vertices[index].w += delta.w * weight;
    }
}

void ngl::morph_geometry::apply_sparse_frame(const morph_component &component,
                                                   f32              weight,
                                                   u32              vertex_count,
                                                   vector4*         vertices) {

    const u8* source = (const u8*)component.data;
    u32 vertex_index = 0;

    while (vertex_index < vertex_count) {
        u16 run_count = *(const u16*)source;
        u16 skip_count = *(const u16*)(source + 2);
        source += 4;

        if (vertex_index + run_count > vertex_count)
            break;

        for (u32 index = 0; index < run_count; ++index) {
            vector4 delta;
            read_component_value(component.value_type, source, delta);

            vector4 &vertex = vertices[vertex_index + index];

            vertex.x += (delta.x * weight) * 0.001f;
            vertex.y += (delta.y * weight) * 0.001f;
            vertex.z += (delta.z * weight) * 0.001f;
            vertex.w += (delta.w * weight) * 0.001f;
        }

        vertex_index += run_count + skip_count;
    }
}

void ngl::morph_geometry::write_morph_stream(render_node* value,
                                             binding*     morph_binding,
                                             u32          vertex_stride) {

    mesh_section* section = value->section;

    morph_set*    set           = morph_binding->set;
    morph_frame   &base_frame   = set->frames[0];
    morph_section &base_section = base_frame.sections[value->section_index];

    auto* buffer        = d3d9::geometry_stream::references::active_buffer.read();
    u32   buffer_offset = value->stream_segment->allocation_start.byte_offset;

    const auto* material_node = (const fx::render_node*)value->material_node;
    const u8*   mesh_data     = material_node->node_data->mesh_data;

    for (u32 component_index = 0; component_index < base_section.component_count; ++component_index) {
        const morph_component &base_component = base_section.components[component_index];

        const D3DVERTEXELEMENT9* element = find_vertex_element(section->vertex_definition_data, base_component);

        if (!element)
            continue;

        auto* vertices = (vector4*)memory::allocate
            (sizeof(vector4) * section->vertex_count, 16, 0);

        if (base_frame.flags & 1)
            decode_base_vertices(section, *element, mesh_data, vertices);
        else
            std::memset(vertices, 0, sizeof(vector4) * section->vertex_count);

        for (u32 frame_index = 0; frame_index < morph_binding->frame_count; ++frame_index) {
            u32 selected_index = morph_binding->frame_indices[frame_index];
            morph_frame &frame = set->frames[selected_index];

            const morph_component &component = frame.sections[value->section_index].components[component_index];

            f32 weight = morph_binding->weights[frame_index];

            if (!(frame.flags & 1))
                apply_dense_frame(component, weight, section->vertex_count, vertices);
            else if (selected_index)
                apply_sparse_frame(component, weight, section->vertex_count, vertices);
        }

        d3d9::binding_cache &bindings = d3d9::references::bindings.get();
        bindings.stream_source      = nullptr;
        bindings.vertex_declaration = nullptr;

        d3d9::references::device.get()->SetStreamSource(0, nullptr, 0, 0);

        D3DVERTEXELEMENT9 output_element = *element;
        output_element.Stream = 1;
        output_element.Offset = 0;

        encode_morph_vertices(buffer,
                              buffer_offset,
                              vertex_stride,
                              section->vertex_count,
                              output_element,
                              mesh_data,
                              vertices);

        memory::free(vertices);
    }
}

void ngl::morph_geometry::render(render_node* value) {
    mesh_section* section = value->section;

    auto*               material_node = (fx::render_node*)value->material_node;
    fx::mesh_node_data* node_data     = material_node->node_data;

    u32 parameter_id = fx::references::parameter_id_morph.read();
    auto* morph_binding = (binding*)node_data->parameters->values[parameter_id];

    const morph_section &base_section = morph_binding->set->frames[0].sections[value->section_index];

    const D3DVERTEXELEMENT9 sentinel { 0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0 };

    D3DVERTEXELEMENT9 elements[33];
    D3DVERTEXELEMENT9 ordered[33];
    u32 element_count = 0;
    u32 morph_stride = 0;

    for (const D3DVERTEXELEMENT9* source = section->vertex_definition_data->elements; source->Type != D3DDECLTYPE_UNUSED; ++source) {
        D3DVERTEXELEMENT9 element = *source;

        for (u32 component_index = 0; component_index < base_section.component_count; ++component_index) {
            const morph_component &component = base_section.components[component_index];

            if (element.Usage == component.semantic && element.UsageIndex == component.semantic_index) {
                element.Stream = 1;
                element.Offset = (WORD)morph_stride;

                morph_stride += get_vertex_element_size(element.Type);

                break;
            }
        }

        elements[element_count++] = element;
    }

    u32 ordered_count = 0;

    for (u32 index = 0; index < element_count; ++index) {
        if (elements[index].Stream == 0)
            ordered[ordered_count++] = elements[index];
    }

    for (u32 index = 0; index < element_count; ++index) {
        if (elements[index].Stream == 1)
            ordered[ordered_count++] = elements[index];
    }

    ordered[ordered_count] = sentinel;

    vertex_definition morphed_definition { section->vertex_definition_data->vertex_size,
                                           ordered,
                                           nullptr };

    IDirect3DDevice9* device = d3d9::references::device.get();

    device->CreateVertexDeclaration(ordered, &morphed_definition.declaration);

    value->stream_segment = d3d9::geometry_stream::allocate_segment(morph_stride * section->vertex_count,
                                                                    morph_stride,
                                                                    on_stream_segment,
                                                                    value);

    write_morph_stream(value, morph_binding, morph_stride);

    d3d9::binding_cache &bindings = d3d9::references::bindings.get();
    bindings.vertex_declaration = nullptr;

    device->SetVertexDeclaration(morphed_definition.declaration);
    device->SetStreamSource(1,
                            d3d9::geometry_stream::references::active_buffer.read(),
                            value->stream_segment->allocation_start.byte_offset,
                            morph_stride);

    vertex_definition* original_definition = material_node->section->vertex_definition_data;
    material_node->section->vertex_definition_data = &morphed_definition;

    material_node->base.render();

    bindings.unk_044[0] = 0;
    bindings.vertex_declaration = nullptr;

    device->SetStreamSource(1, nullptr, 0, 0);

    material_node->section->vertex_definition_data = original_definition;
    morphed_definition.declaration->Release();
}
