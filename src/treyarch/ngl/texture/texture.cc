#include "treyarch/shared/four_cc.hh"
#include "treyarch/shared/memory/memory.hh"
#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/texture/texture.hh"

using namespace treyarch;

bool ngl::can_release_texture(const texture* value) {
    return value->last_frame_reference + 1 < (i32)references::frame_epoch.read();
}

void ngl::process_texture(amalga::file* owner, void** mapped_sections) {
    i32 image_section = owner->find_section_index
        (string_hash(four_cc('I', 'M', 'G')));

    texture* value = (texture*)mapped_sections[image_section];

    value->owner_file = owner;
    value->last_frame_reference = -1;

    if (value->flags & texture_animated)
        return;

    i32 physical_section = owner->find_section_index
        (string_hash(four_cc('P', 'H', 'Y', 'S')));

    const void* source = mapped_sections[physical_section];

    value->gpu_texture.resource = nullptr;
    value->gpu_texture.usage = 0;

    if (value->flags & texture_cube) {
        value->gpu_texture.height = value->gpu_texture.width;
        value->gpu_texture.depth = 6;
        value->gpu_texture.resource_type = D3DRTYPE_CUBETEXTURE;

        d3d9::create_texture_resource(&value->gpu_texture);
        d3d9::upload_cube_texture(&value->gpu_texture, source);
    } else if (value->flags & texture_volume) {
        value->gpu_texture.resource_type = D3DRTYPE_VOLUMETEXTURE;

        d3d9::create_texture_resource(&value->gpu_texture);
    } else {
        value->gpu_texture.depth = 1;
        value->gpu_texture.resource_type = D3DRTYPE_TEXTURE;

        d3d9::create_texture_resource(&value->gpu_texture);
        d3d9::upload_texture(&value->gpu_texture, source);
    }
}

void ngl::release_texture(texture* value) {
    if (!value)
        return;

    if (!can_release_texture(value))
        d3d9::wait_for_rendering();

    if (value->depth_target) {
        release_texture(value->depth_target);
        
        value->depth_target = nullptr;
    }

    if (value->frames)
        memory::free(value->frames);

    if (value->gpu_texture.resource && !value->gpu_texture.resource->Release())
        value->gpu_texture.resource = nullptr;

    if (value->render_target)
        value->render_target->Release();
}

void ngl::initialize_texture_directory() {
    references::textures.get().initialize();
}

void __cdecl ngl::load_texture(amalga::file*       owner,
                               amalga::file_entry* entry,
                               void**              mapped_sections,
                               void*               user_data) {

    (void)entry;
    (void)user_data;

    i32 image_section = owner->find_section_index
        (string_hash(four_cc('I', 'M', 'G')));

    texture* value = (texture*)mapped_sections[image_section];

    process_texture(owner, mapped_sections);

    if (!references::resource_callback.read())
        references::textures.get().insert(value);
}

void __cdecl ngl::remove_texture(amalga::file*       owner,
                                 amalga::file_entry* entry,
                                 void**              mapped_sections,
                                 void*               user_data) {

    (void)entry;
    (void)user_data;

    i32 image_section = owner->find_section_index
        (string_hash(four_cc('I', 'M', 'G')));

    texture* value = (texture*)mapped_sections[image_section];

    if (!references::resource_callback.read())
        references::textures.get().erase(value);

    if (value->last_frame_reference + 1 >= (i32)references::frame_epoch.read())
        d3d9::wait_for_rendering();

    release_texture(value);
}
