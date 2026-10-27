#include "retail.hh"
#include "game/amalga/resource_directory.hh"
#include "game/amalga/pixel_shader/resource_handler.hh"
#include "game/shader_resource_manager.hh"
#include "treyarch/ngl/d3d9/shader_program_cache.hh"

using namespace treyarch;

// sub_767EA0
i32 amalga::pixel_shader_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer*) {
    u32 name_hash;
    retail::sub_5FD120((u32*)descriptor, &name_hash);

    const DWORD* bytecode = (const DWORD*)descriptor->raw_payload;

    // the program is never released, it stays filed after its pack goes away
    if (behavior != load)
        return 0;

    shader_resource_manager* manager = treyarch::references::shader_resource_manager.read();

    manager->pixel_programs.find(name_hash);

    IDirect3DPixelShader9* program;
    ngl::d3d9::shader_program_cache::create_pixel_program(bytecode, &program);

    manager->pixel_programs[name_hash] = program;

    // retail would return 1 if the slot's address were null
    manager->pixel_programs[name_hash];

    return 0;
}
