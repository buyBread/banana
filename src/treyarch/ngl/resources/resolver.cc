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

void* __cdecl ngl::resources::resolve(fixed_string* name, u32 type) {
    ngl::resource_callback callback = ngl::references::resource_callback.read();

    if (callback)
        return callback(name, type);

    switch (type) {
        case four_cc('M', 'E', 'S', 'H'):
            return ngl::references::meshes.get().find(name->hash);

        case four_cc('F', 'O', 'N', 'T'):
            return ngl::references::fonts.get().find(name->hash);

        case four_cc('M', 'O', 'R', 'H'):
            return ngl::references::morphs.get().find(name->hash);

        case four_cc('M', 'A', 'T'):
            return ngl::references::materials.get().find(name->hash);

        case four_cc('T', 'E', 'X'): {
            texture* value = ngl::references::textures.get().find(name->hash);

            return value ? value : ngl::references::default_texture.read();
        }

        case four_cc('F', 'X', '\0'): {
            fx::effect* value = fx::find(name->hash);

            if (!value)
                memory::report("NGL: Unable to locate effect resource %s - bailing.\n", name->text);

            return value;
        }

        default:
            return nullptr;
    }
}
