#include "banana/logging.hh"
#include "treyarch/ngl/font/font.hh"
#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/morph/morph.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/resources/resolver.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/shared/four_cc.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

// sub_9E2170
void* ngl::resources::resolve(fixed_string* name, u32 type) {
    ngl::resource_callback callback = ngl::references::resource_callback.read();

    if (callback)
        return callback(name, type);

    const char* display_name = name->text ? name->text : "(null)";

    switch (type) {
        case four_cc('M', 'E', 'S', 'H'): {
            mesh* value = ngl::references::meshes.get().find(name->hash);

            if (!value)
                banana::log.ngl("unable to find mesh \"{}\" (0x{:08X})", display_name, name->hash.source_hash_code);

            return value;
        }

        case four_cc('F', 'O', 'N', 'T'): {
            font* value = ngl::references::fonts.get().find(name->hash);

            if (!value)
                banana::log.ngl("unable to find font \"{}\" (0x{:08X})", display_name, name->hash.source_hash_code);

            return value;
        }

        case four_cc('M', 'O', 'R', 'H'): {
            morph_set* value = ngl::references::morphs.get().find(name->hash);

            if (!value)
                banana::log.ngl("unable to find morph \"{}\" (0x{:08X})", display_name, name->hash.source_hash_code);

            return value;
        }

        case four_cc('M', 'A', 'T'): {
            material* value = ngl::references::materials.get().find(name->hash);

            if (!value)
                banana::log.ngl("unable to find material \"{}\" (0x{:08X})", display_name, name->hash.source_hash_code);

            return value;
        }

        case four_cc('T', 'E', 'X'): {
            texture* value = ngl::references::textures.get().find(name->hash);

            if (!value)
                banana::log.ngl("unable to locate texture resource \"{}\" (0x{:08X}) -- assigning default texture",
                                display_name,
                                name->hash.source_hash_code);

            return value ? value : ngl::references::default_texture.read();
        }

        case four_cc('F', 'X', '\0'): {
            fx::effect* value = fx::find(name->hash);

            if (!value) {
                memory::report("NGL: Unable to locate effect resource %s - bailing.\n", name->text);

                banana::log.ngl("unable to locate effect resource \"{}\" (0x{:08X}) -- bailing",
                                display_name,
                                name->hash.source_hash_code);
            }

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

    cache = (fx::effect*)resolve(&resource_name, four_cc('F', 'X', '\0'));
}

void ngl::resources::resolve_texture(ngl::texture* &cache, const char* name) {
    if (cache)
        return;

    fixed_string resource_name = make_fixed_string(name);

    cache = (texture*)resolve(&resource_name, four_cc('T', 'E', 'X'));
}
