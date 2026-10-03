#include <new>

#include "retail.hh"
#include "treyarch/amalga/paths.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/app/app.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/shared/four_cc.hh"
#include "treyarch/shared/mash/unmash.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/shared/os_file.hh"
#include "treyarch/shared/platform.hh"

using namespace treyarch;

// sub_770160
void amalga::resource_manager::create_inst() {
    void* partitions_allocation = memory::heap::allocate(sizeof(dinkumware::vector<resource_partition*>));

    auto partitions = partitions_allocation ?
        new (partitions_allocation) dinkumware::vector<resource_partition*>() : nullptr;

    references::partitions.write(partitions);
    partitions->reserve(8);

    auto mutex = (engine_recursive_lock*)memory::heap::allocate(sizeof(engine_recursive_lock));

    if (mutex) {
        mutex->owner = 0;
        mutex->state = 0;
        mutex->depth = 0;
    }

    references::resource_context_stack_mutex.write(mutex);

    // and once more after it's published
    mutex->owner = 0;
    mutex->state = 0;
    mutex->depth = 0;

    references::memory_maps          .write(nullptr);
    references::in_use_memory_map    .write(-1);
    references::amalgapak_base_offset.write(0);
    references::amalgapak_id         .write(-1);

    if (!treyarch::references::pack_mode.read()) {
        resource_amalgatoc* amalgatoc = load_amalgapak();

        references::amalgatoc  .write(amalgatoc);
        references::memory_maps.write(&amalgatoc->memory_maps);

        std::array<u32, buffer_location_count> &buffer_size = references::resource_buffer_size.get();

        // some slack on top of whatever the toc asks for
        buffer_size[buffer_location_main] = amalgatoc->resource_buffer_size[buffer_location_main];

        if (buffer_size[buffer_location_main])
            buffer_size[buffer_location_main] += 0x20000;

        buffer_size[buffer_location_vram] = amalgatoc->resource_buffer_size[buffer_location_vram];

        if (buffer_size[buffer_location_vram])
            buffer_size[buffer_location_vram] += 0x20000;

        references::resource_buffer_used.get()[buffer_location_main] = 0;
        references::resource_buffer_used.get()[buffer_location_vram] = 0;

        if (!memory::heap::references::heap_default.read())
            retail::sub_5FB3D0(); // create the default heap

        // pc only ever gets the main buffer
        references::resource_buffer.get()[buffer_location_main] = (u8*)retail::sub_5FAF00
            ((i32)memory::heap::references::heap_default.read(), buffer_size[buffer_location_main], 0x1000);

        retail::sub_76FB40(0); // configure_packs_by_memory_map
    }

    u32 texture_handle;

    references::default_texture
        .write((ngl::texture*)*retail::sub_72BD10(&texture_handle, (i32)ngl::references::default_texture.read()));

    references::white_texture
        .write((ngl::texture*)*retail::sub_72BD10(&texture_handle, (i32)ngl::references::white_texture.read()));

    references::resource_context.write(nullptr);

    apkf::register_section_type(string_hash(four_cc('V', 'R', 'M', 'L')), resolve_vrml_section, nullptr, nullptr);

    references::initialized.write(true);
}

// sub_76E400
amalga::resource_amalgatoc* amalga::resource_manager::load_amalgapak() {
    os_file      file;
    mash::string filename = get_amalgatoc_filename(treyarch::references::platform.read());

    file.open(filename, os_file::file_read);

    u32 toc_size = file.get_size();

    if (!memory::heap::references::heap_default.read())
        retail::sub_5FB3D0(); // create the default heap

    // the toc is unmashed in place, so this allocation is the toc for the rest of the process
    u8* toc = (u8*)retail::sub_5FBD20((u32*)memory::heap::references::heap_default.read(), toc_size, 0);

    file.read(toc, toc_size);

    resource_amalgatoc* amalgatoc = mash::unmash_in_place<resource_amalgatoc>(toc, (i32)toc_size);

    // nothing looks at the result
    string_hash filename_hash;
    filename_hash.initialize(mash::ALLOCATED, filename.c_str());
    amalgatoc->versions.verify(filename_hash);

    file.close();

    return amalgatoc;
}

// sub_72DE10
void* amalga::resource_manager::resolve_vrml_section(apkf::file*,
                                                     apkf::file_section*,
                                                     void*) {

    return references::resource_context.read();
}
