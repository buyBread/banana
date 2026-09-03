#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "treyarch/ngl/fx/material.hh"

using namespace treyarch;

static u32 parameter_name_hash(const ngl::fx::parameter &value) {
    return value.name.hash.source_hash_code;
}

void ngl::fx::bind_material(material* value, effect* effect_data) {
    for (i32 material_index = 0; material_index < value->parameter_count; ++material_index) {
        parameter &material_parameter = value->parameters[material_index];

        material_parameter.effect_binding =
            (parameter*)&references::parameter_binding_sentinel.get();

        u32 name_hash = parameter_name_hash(material_parameter);

        for (i32 effect_index = 0; effect_index < effect_data->parameter_count; ++effect_index) {
            parameter &effect_parameter = effect_data->parameters[effect_index];

            if (name_hash && name_hash == parameter_name_hash(effect_parameter)) {
                material_parameter.effect_binding = &effect_parameter;
                
                break;
            }
        }
    }

    value->technique_index = 0;

    u32 technique_hash = value->technique_name.hash.source_hash_code;

    if (technique_hash) {
        i32 technique_index = 0;

        for (; technique_index < effect_data->technique_count; ++technique_index) {
            if (effect_data->techniques[technique_index].name.hash.source_hash_code == technique_hash) {
                value->technique_index = technique_index;
                
                break;
            }
        }

        if (technique_index == effect_data->technique_count) {
            const char* name = value->technique_name.text;
            char hash_name[11];

            if (!name) {
                std::sprintf(hash_name, "0x%08X", technique_hash);
                name = hash_name;
            }

            if (*name >= '0' && *name <= '9')
                value->technique_index = std::atoi(name);
        }
    }

    value->bound_effect = effect_data;
}

void ngl::fx::copy_material_parameters(material* value) {
    for (i32 index = 0; index < value->parameter_count; ++index) {
        parameter &source = value->parameters[index];
        parameter* destination = source.effect_binding;

        u8* source_data      = (u8*)source.data;
        u8* destination_data = (u8*)destination->data;
        u32 byte_count       = (destination->data_size + 15) & ~15u;

        for (u32 offset = 0; offset < byte_count; offset += 16)
            std::memcpy(destination_data + offset, source_data + offset, 16);
    }
}

void ngl::fx::build_animated_texture_parameter_chain(effect* value) {
    value->animated_texture_parameter_chain =
        (parameter*)&references::parameter_chain_sentinel.get();

    for (i32 index = 0; index < value->parameter_count; ++index) {
        parameter &entry = value->parameters[index];

        if ((u32)entry.type != 11)
            continue;

        u8* texture_data = (u8*)*(void**)entry.data;

        if (!(texture_data[0x10] & 1))
            continue;

        entry.animated_texture_chain_next = value->animated_texture_parameter_chain;
        value->animated_texture_parameter_chain = &entry;
    }
}

void ngl::fx::build_animated_texture_parameter_chain(effect* effect_data, material* value) {
    (void)effect_data;

    value->animated_texture_parameter_chain =
        (parameter*)&references::parameter_chain_sentinel.get();

    for (i32 index = 0; index < value->parameter_count; ++index) {
        parameter &entry = value->parameters[index];

        if ((u32)entry.type != 11)
            continue;

        u8* texture_data = (u8*)*(void**)entry.data;

        if (!(texture_data[0x10] & 1))
            continue;

        entry.animated_texture_chain_next = value->animated_texture_parameter_chain;
        value->animated_texture_parameter_chain = &entry;
    }
}
