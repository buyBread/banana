#include <cstring>

#include "banana/logging.hh"
#include "treyarch/ngl/font/font.hh"
#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/morph/morph.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/resources/resources.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/apk/loader.hh"
#include "treyarch/base/tl_system.hh"
#include "game/four_cc.hh"

using namespace treyarch;

// sub_9E2170
void* ngl::resources::get_resource(fixed_string* name, u32 type) {
    ngl::resource_callback callback = ngl::references::resource_callback_function.read();

    if (callback)
        return callback(name, type);

    const char* display_name = name->text ? name->text : "(null)";

    switch (type) {
        case four_cc('M', 'E', 'S', 'H'): {
            mesh* value = ngl::references::mesh_directory.get().find(name->hash);

            if (!value)
                banana::log.ngl("unable to find mesh \"{}\" (0x{:08X})", display_name, name->hash.source_hash_code);

            return value;
        }

        case four_cc('F', 'O', 'N', 'T'): {
            font* value = ngl::references::font_directory.get().find(name->hash);

            if (!value)
                banana::log.ngl("unable to find font \"{}\" (0x{:08X})", display_name, name->hash.source_hash_code);

            return value;
        }

        case four_cc('M', 'O', 'R', 'H'): {
            morph_set* value = ngl::references::morph_directory.get().find(name->hash);

            if (!value)
                banana::log.ngl("unable to find morph \"{}\" (0x{:08X})", display_name, name->hash.source_hash_code);

            return value;
        }

        case four_cc('M', 'A', 'T'): {
            material* value = ngl::references::material_directory.get().find(name->hash);

            if (!value)
                banana::log.ngl("unable to find material \"{}\" (0x{:08X})", display_name, name->hash.source_hash_code);

            return value;
        }

        case four_cc('T', 'E', 'X'): {
            texture* value = ngl::references::texture_directory.get().find(name->hash);

            if (!value)
                banana::log.ngl("unable to locate texture resource \"{}\" (0x{:08X}) -- assigning default texture",
                                display_name,
                                name->hash.source_hash_code);

            return value ? value : ngl::references::default_tex.read();
        }

        case four_cc('F', 'X', '\0'): {
            fx::effect* value = fx::references::effects.get().find(name->hash);

            if (!value)
                tl_fatal("NGL: Unable to locate effect resource %s - bailing.\n", name->text);

            return value;
        }

        default:
            return nullptr;
    }
}

void ngl::resources::resolve_effect(ngl::fx::effect* &cache, const char* name) {
    if (cache)
        return;

    fixed_string resource_name = make_fixed_string(name);

    cache = (fx::effect*)get_resource(&resource_name, four_cc('F', 'X', '\0'));
}

void ngl::resources::resolve_texture(ngl::texture* &cache, const char* name) {
    if (cache)
        return;

    fixed_string resource_name = make_fixed_string(name);

    cache = (texture*)get_resource(&resource_name, four_cc('T', 'E', 'X'));
}

ngl::texture* create_solid_texture(u32 color, const fixed_string &name) {
    ngl::texture* value = ngl::create_texture(0, D3DFMT_A8R8G8B8, 1, 1, 0, 1);

    auto resource = (IDirect3DTexture9*)value->gpu_texture.resource;
    
    D3DLOCKED_RECT locked;

    resource->LockRect(0, &locked, nullptr, 0);
    *(u32*)locked.pBits = color;
    resource->UnlockRect(0);

    ngl::register_runtime_texture(value, name);

    return value;
}

// sub_9E7AC0
void initialize_builtin_textures() {
    ngl::references::white_tex        .write(create_solid_texture(0xFFFFFFFF, ngl::references::white_texture_name.get()));
    ngl::references::black_texture    .write(create_solid_texture(0xFF000000, ngl::references::black_texture_name.get()));
    ngl::references::invisible_texture.write(create_solid_texture(0x00000000, ngl::references::invisible_texture_name.get()));
}

// sub_9E2240
void ngl::resources::init() {
    u32 package_size = references::default_package_size.read();
    void* package_copy = tl_mem_alloc(package_size, 0x1000, 0);

    references::default_package_copy.write(package_copy);

    std::memcpy(package_copy,
                &references::default_package_data.get(),
                package_size);

    apk::load_file_in_place(package_copy);

    fixed_string default_name = make_fixed_string("ngl_default");
    ngl::references::default_tex.write((texture*)get_resource(&default_name, four_cc('T', 'E', 'X')));

    fixed_string system_font_name = make_fixed_string("ngl_sysfont");
    ngl::references::sys_font.write((font*)get_resource(&system_font_name, four_cc('F', 'O', 'N', 'T')));

    banana::log.ngl("default resources -- ngl_default: {} | ngl_sysfont: {}",
                    ngl::references::default_tex.read() ? "found" : "missing",
                    ngl::references::sys_font.read()    ? "found" : "missing");

    initialize_builtin_textures();
    apk::set_resource_callback(get_resource);
}
