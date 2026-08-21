#include <cstring>

#include "treyarch/amalga/loader.hh"
#include "treyarch/ngl/font/font.hh"
#include "treyarch/ngl/resources/init.hh"
#include "treyarch/ngl/resources/resolver.hh"
#include "treyarch/ngl/texture/runtime.hh"
#include "treyarch/shared/four_cc.hh"
#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

static fixed_string make_lookup_name(const char* text) {
    fixed_string name;

    name.text = (char*)text;
    name.hash = string_hash(hash::djb2(text));

    return name;
}

static ngl::texture* create_solid_texture(u32 color, const char* name) {
    ngl::texture* value = ngl::create_runtime_texture(0, D3DFMT_A8R8G8B8, 1, 1, 1);

    auto resource = (IDirect3DTexture9*)value->gpu_texture.resource;
    
    D3DLOCKED_RECT locked;

    resource->LockRect(0, &locked, nullptr, 0);
    *(u32*)locked.pBits = color;
    resource->UnlockRect(0);

    ngl::register_runtime_texture(value, name);

    return value;
}

static void initialize_builtin_textures() {
    ngl::references::white_texture    .write(create_solid_texture(0xFFFFFFFF, "nglwhite"));
    ngl::references::black_texture    .write(create_solid_texture(0xFF000000, "nglblack"));
    ngl::references::invisible_texture.write(create_solid_texture(0x00000000, "nglinvisible"));
}

void ngl::resources::init() {
    u32 package_size = references::default_package_size.read();
    void* package_copy = memory::allocate(package_size, 0x1000, 0);

    references::default_package_copy.write(package_copy);

    std::memcpy(package_copy,
                &references::default_package_data.get(),
                package_size);

    amalga::load_in_place(package_copy);

    fixed_string default_name = make_lookup_name("ngl_default");
    ngl::references::default_texture.write((texture*)resolve(&default_name, four_cc('T', 'E', 'X')));

    fixed_string system_font_name = make_lookup_name("ngl_sysfont");
    ngl::references::system_font.write((font*)resolve(&system_font_name, four_cc('F', 'O', 'N', 'T')));

    initialize_builtin_textures();
    amalga::set_resource_resolver(resolve);
}
