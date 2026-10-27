#include "treyarch/ngl/gpu/d3d9/device.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/apk/file.hh"
#include "treyarch/base/tl_system.hh"
#include "treyarch/mm/mm.hh"
#include "game/four_cc.hh"

using namespace treyarch;

// sub_9E45C0
bool ngl::can_release_texture(const texture* value) {
    return value->last_frame_reference + 1 < (i32)references::frame.read();
}

// sub_9EAE60
void ngl::process_texture(apk::file* owner, void** mapped_sections) {
    i32 image_section = owner->get_section_index
        (string_hash(four_cc('I', 'M', 'G')));

    texture* value = (texture*)mapped_sections[image_section];

    value->owner_file = owner;
    value->last_frame_reference = -1;

    if (value->flags & texture_animated)
        return;

    i32 physical_section = owner->get_section_index
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

// sub_9E27D0
void ngl::destroy_texture(texture* value) {
    if (!value)
        return;

    bool owned = (value->flags & runtime_texture_owned) != 0;

    if (!can_release_texture(value))
        d3d9::wait_for_rendering();

    if (value->depth_target) {
        destroy_texture(value->depth_target);
        
        value->depth_target = nullptr;
    }

    if (value->frames)
        tl_mem_free(value->frames);

    if (value->gpu_texture.resource && !value->gpu_texture.resource->Release())
        value->gpu_texture.resource = nullptr;

    if (value->render_target)
        value->render_target->Release();

    if (owned) {
        if (value->name.text)
            mm::free(value->name.text);

        tl_mem_free(value);
    }
}

void ngl::initialize_texture_directory() {
    references::texture_directory.get().init();
}

// sub_9E45E0
void ngl::apk_texture_load_callback(apk::file*       owner,
                                    apk::file_entry* entry,
                                    void**           mapped_sections,
                                    void*            user_data) {

    (void)entry;
    (void)user_data;

    i32 image_section = owner->get_section_index
        (string_hash(four_cc('I', 'M', 'G')));

    texture* value = (texture*)mapped_sections[image_section];

    process_texture(owner, mapped_sections);

    if (!references::resource_callback_function.read())
        references::texture_directory.get().add(value);
}

// sub_9E4620
void ngl::apk_texture_delete_callback(apk::file*       owner,
                                      apk::file_entry* entry,
                                      void**           mapped_sections,
                                      void*            user_data) {

    (void)entry;
    (void)user_data;

    i32 image_section = owner->get_section_index
        (string_hash(four_cc('I', 'M', 'G')));

    texture* value = (texture*)mapped_sections[image_section];

    if (!references::resource_callback_function.read())
        references::texture_directory.get().del(value);

    if (value->last_frame_reference + 1 >= (i32)references::frame.read())
        d3d9::wait_for_rendering();

    destroy_texture(value);
}

// sub_9E4680
void ngl::texture_init() {
    initialize_texture_directory();

    apk::register_file_type(four_cc('T', 'E', 'X'),
                            3,
                            apk_texture_load_callback,
                            apk_texture_delete_callback);
}
