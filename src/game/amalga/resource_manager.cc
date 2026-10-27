#include <new>

#include "retail.hh"
#include "game/amalga/paths.hh"
#include "game/amalga/resource_directory.hh"
#include "game/amalga/resource_manager.hh"
#include "game/amalga/resource_partition.hh"
#include "game/app.hh"
#include "game/game.hh"
#include "game/frontend/frontend_manager.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "game/four_cc.hh"
#include "treyarch/mash/unmash.hh"
#include "treyarch/mm/heap.hh"
#include "game/os_file.hh"
#include "game/platform.hh"

using namespace treyarch;

// sub_770160
void amalga::resource_manager::create_inst() {
    void* partitions_allocation = memory::heap::allocate(sizeof(vc8::vector<resource_partition*>));

    auto partitions = partitions_allocation ?
        new (partitions_allocation) vc8::vector<resource_partition*>() : nullptr;

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

    merged_apk::register_section_type(string_hash(four_cc('V', 'R', 'M', 'L')), resolve_vrml_section, nullptr, nullptr);

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
void* amalga::resource_manager::resolve_vrml_section(merged_apk::file*,
                                                     merged_apk::file_section*,
                                                     void*) {

    return references::resource_context.read();
}

// sub_7702F0
void amalga::resource_manager::frame_advance(f32 dt) {
    frame_advance(dt, 0.002f);
}

// sub_76FF50
void amalga::resource_manager::frame_advance(f32 dt, f32 max_ms) {
    references::advancing.write(true);

    // max_ms is taken as is only while 0x01111391 is set and the igo's check says no; otherwise it just raises the 30ms floor
    f32 limit = 0.03f;

    ui_frontend* igo = treyarch::references::frontend.get().igo;

    if (treyarch::references::unk_01111391.read() && igo && !retail::sub_6884D0((u32*)igo))
        limit = max_ms;
    else
        limit = max_ms > 0.03f ? max_ms : 0.03f;

    limited_timer time_limit(limit);
    time_limit.reset();

    retail::sub_87EF10((u32*)references::unk_010f7760.read(), dt);

    vc8::vector<resource_partition*>* partitions = references::partitions.read();

    for (u32 index = 0; index < partitions->size(); ++index)
        (*partitions)[index]->streamer.frame_advance(dt, &time_limit);

    references::advancing.write(false);
}

// sub_739C50
amalga::resource_partition* amalga::resource_manager::get_partition_pointer(resource_pack_slot* pack_slot) {
    vc8::vector<resource_partition*>* partitions = references::partitions.read();

    for (u32 index = 0; index < partitions->size(); ++index) {
        resource_partition* partition = (*partitions)[index];

        const u32 slot_count = partition->pack_slots.size();

        for (u32 slot = 0; slot < slot_count; ++slot) {
            if (partition->pack_slots[slot] == pack_slot)
                return partition;
        }
    }

    return nullptr;
}

// sub_456240
// inlined @ sub_7629D0, sub_75E130
amalga::resource_pack_slot* amalga::resource_manager::get_resource_context() {
    engine_lock_scope lock(references::resource_context_stack_mutex.read());

    vc8::vector<resource_pack_slot*> &stack = references::resource_context_stack.get();

    return stack.size() ? stack.back() : nullptr;
}

// sub_7629D0
u8* amalga::resource_manager::get_resource(const resource_key         &key,
                                                 u32*                  size,
                                                 resource_pack_slot**  slot) {

    engine_lock_scope lock(&references::unk_010300a8.get());

    return get_resource_context()->pack_directory->get_resource(key, size, slot);
}

// sub_75E130
void* amalga::resource_manager::get_merged_resource_or_default(string_hash          name,
                                                               u32                  type,
                                                               resource_pack_slot** slot) {

    engine_lock_scope lock(&references::unk_010300a8.get());

    return get_resource_context()->get_merged_resource_or_default(name, type, slot);
}

// sub_456290
amalga::push_resource_context_stack_object::push_resource_context_stack_object(resource_pack_slot* slot) {
    resource_manager::references::unk_010300a8.get().acquire();
    resource_manager::references::resource_context_stack_mutex.read()->acquire();

    if (slot) {
        m_context = (resource_pack_slot*)retail::sub_767760((i32)slot); // push_resource_context
        m_pushed  = true;
    } else {
        m_context = resource_manager::get_resource_context();
        m_pushed  = false;
    }
}

// sub_44E6C0
amalga::push_resource_context_stack_object::~push_resource_context_stack_object() {
    if (m_pushed)
        retail::sub_757240(); // pop_resource_context

    resource_manager::references::resource_context_stack_mutex.read()->release();
    resource_manager::references::unk_010300a8.get().release();
}

// sub_42AC10
amalga::resource_context_stack_object::resource_context_stack_object(resource_pack_slot* context) {
    resource_manager::references::unk_010300a8.get().acquire();
    resource_manager::references::resource_context_stack_mutex.read()->acquire();

    retail::sub_767760((i32)context); // push_resource_context
}

// sub_42AC40
amalga::resource_context_stack_object::~resource_context_stack_object() {
    retail::sub_757240(); // pop_resource_context

    resource_manager::references::resource_context_stack_mutex.read()->release();
    resource_manager::references::unk_010300a8.get().release();
}
