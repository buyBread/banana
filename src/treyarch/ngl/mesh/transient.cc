#include <cstring>

#include "treyarch/ngl/list/list.hh"
#include "treyarch/ngl/mesh/mesh.hh"

using namespace treyarch;

// sub_9E2C10
ngl::mesh* ngl::create_transient_mesh(u32 flags, u32 section_count) {
    mesh* value = (mesh*)list_alloc(0x50, 16);

    std::memset(value, 0, 0x50);

    value->flags         = flags | 0x20000;
    value->section_count = section_count;
    value->sections      = (mesh_section_table_entry*)list_alloc(8 * section_count, 16);

    for (u32 index = 0; index < section_count; ++index) {
        value->sections[index].flags   = 0;
        value->sections[index].section = nullptr;
    }

    *(u32*)&value->sphere[3]         = 0x749DC5AE;
    *(u32*)&value->position_scale[0] = 0x749DC5AE;
    *(u32*)&value->position_scale[1] = 0x749DC5AE;
    *(u32*)&value->position_scale[2] = 0x749DC5AE;

    return value;
}

// sub_9E2B60
u32 ngl::attach_transient_mesh_section(mesh*         value,
                                       mesh_section* section,
                                       material*     section_material,
                                       u32           flags) {

    for (u32 index = 0; index < value->section_count; ++index) {
        mesh_section_table_entry &entry = value->sections[index];

        if (entry.section)
            continue;

        entry.section           = section;
        section->material_data  = section_material;
        entry.flags             = flags;

        return index;
    }

    return value->section_count;
}
