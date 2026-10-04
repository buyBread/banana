#include <cstring>

#include "retail.hh"
#include "treyarch/amalga/paths.hh"
#include "treyarch/amalga/resource_amalgatoc.hh"
#include "treyarch/amalga/resource_directory.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/amalga/resource_pack_streamer.hh"
#include "treyarch/amalga/resource_partition.hh"
#include "treyarch/app/app.hh"
#include "treyarch/nfl/nfl.hh"
#include "treyarch/shared/binary_search.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/os_file.hh"
#include "treyarch/shared/platform.hh"

using namespace treyarch;

// sub_76F330
void amalga::resource_pack_streamer::frame_advance(f32 dt, limited_timer* time_limit) {
    if (!active)
        return;

    switch (currently_streaming) {
        case 0:
            frame_advance_idle(dt);

            break;

        case 1:
            frame_advance_streaming(dt);

            break;
    }

    if (!pack_slots)
        return;

    dinkumware::vector<resource_pack_slot*> slots(*pack_slots);

    if (my_partition->callback)
        my_partition->callback(my_partition, &slots);

    dinkumware::vector<resource_pack_slot*> unloaded_slots;

    for (u32 index = 0; index < slots.size(); ++index) {
        resource_pack_slot* slot = slots[index];

        const e_slot_state previous_state = slot->slot_state;

        slot->frame_advance(dt, time_limit);

        if (previous_state == slot_state_destructing && slot->slot_state == slot_state_empty)
            unloaded_slots.push_back(slot);
    }

    for (resource_pack_slot* slot : unloaded_slots) {
        if (slot->my_callback)
            slot->my_callback(callback_post_destruct, &slot->my_partition->streamer, slot, nullptr);
    }
}

// sub_76F160
void amalga::resource_pack_streamer::frame_advance_idle(f32) {
    if (!load_queue.size())
        return;

    const resource_pack_queue_entry entry = load_queue.begin()->value;

    load_internal(entry.name, entry.slot_idx, entry.callback);

    if (load_queue.begin() != load_queue.end())
        load_queue.erase(load_queue.begin());
}

// sub_739EB0
void amalga::resource_pack_streamer::frame_advance_streaming(f32 dt) {
    load_timer += dt;

    if (curr_stream_request_id[buffer_location_main] == -1 && curr_stream_request_id[buffer_location_vram] == -1)
        finish_streaming();
}

// sub_739E30
void amalga::resource_pack_streamer::finish_streaming() {
    callback_timer_set &timers = treyarch::references::callback_timers.get();

    timers.lock.acquire();

    if (timers.unk_38 == 1) {
        timers.unk_30 = 0;
        timers.unk_38 = -1;
    }

    timers.timers[1] = 0.0f;

    timers.lock.release();

    resource_pack_slot* slot = curr_slot;

    slot->slot_state = slot_state_constructing;

    if (slot->my_callback)
        slot->my_callback(callback_load_finished, &slot->my_partition->streamer, slot, nullptr);

    currently_streaming = 0;
    curr_pack_name      = string_hash();
    curr_slot           = nullptr;
    curr_slot_idx       = -1;
}

// sub_76F4A0
void amalga::resource_pack_streamer::flush(flush_callback callback, f32 callback_interval) {
    limited_timer time_limit(callback_interval);
    hires_clock_t frame_clock;

    while ((currently_streaming || load_queue.size() || !all_slots_idle()) &&
           !treyarch::references::game.read()->i_quit) {

        if (callback)
            callback();

        nfl::update();
        retail::sub_905050(frame_clock.elapsed_and_reset()); // sound update

        time_limit.reset();

        frame_advance(0.0f, callback_interval > 0.0f ? &time_limit : nullptr);
    }
}

// sub_76F6A0
void amalga::resource_pack_streamer::flush(flush_callback callback) {
    flush(callback, 0.02f);
}

// sub_76F200
void amalga::resource_pack_streamer::load(const char* pack_name, i32 slot_idx) {
    string_hash name_hash;
    name_hash.initialize(mash::ALLOCATED, pack_name);

    // what's left of a stripped check: every busy slot's name is read and dropped
    for (u32 index = 0; index < pack_slots->size(); ++index) {
        resource_pack_slot* slot = (*pack_slots)[index];

        if (slot && slot->slot_state) {
            u32 slot_name_hash;
            retail::sub_5FD120((u32*)&slot->pack_name, &slot_name_hash);
        }
    }

    // the name isn't terminated when it fills all 32 bytes
    resource_pack_queue_entry entry {};
    std::memcpy(entry.name, pack_name, std::strlen(pack_name));

    entry.slot_idx = slot_idx;
    entry.callback = load_callback;

    load_queue.push_back(entry);

    frame_advance_idle(0.0f);
}

// sub_76EEE0
void amalga::resource_pack_streamer::load_internal(const char*                 pack_name,
                                                   i32                         slot_idx,
                                                   resource_pack_slot_callback callback) {

    string_hash name_hash;
    name_hash.initialize(mash::ALLOCATED, pack_name);

    curr_pack_name = name_hash;
    load_timer     = 0.0f;
    curr_slot_idx  = slot_idx;
    curr_slot      = (*pack_slots)[slot_idx];

    mash::vector<resource_amalgatoc_pack_entry> &pack_entries = resource_manager::references::amalgatoc.read()->pack_entries;

    i32 entry_index = 0;

    curr_pack_entry = binary_search_array_cmp(&name_hash,
                                              pack_entries.data,
                                              0,
                                              (i32)pack_entries.size,
                                              &entry_index,
                                              resource_amalgatoc_pack_entry::compare_name_hash) ?
        pack_entries.data[entry_index] : nullptr;

    aram_buffer = nullptr;

    const e_platform platform = treyarch::references::platform.read();

    mash::string name(pack_name);
    mash::string path = os_file::get_full_path
        (get_packs_directory(platform) + name + references::pack_extensions.get()[platform]);

    i32 file = resource_manager::references::amalgapak_id.read();

    if (!(curr_pack_entry->flags & resource_amalgatoc_pack_entry_flag_shared_file))
        file = nfl::open_file(1, path.c_str());

    nfl::request_params params {};
    params.file     = -1;
    params.type     = -1;
    params.priority = 2;

    curr_file_id = file;

    // the parts sit back to back in the file
    u32 offset = curr_pack_entry->source_offset;

    for (u32 buffer = 0; buffer < buffer_location_count; ++buffer) {
        if (!curr_pack_entry->data_size[buffer])
            continue;

        params.file        = file;
        params.callback    = stream_request_callback;
        params.type        = 0;
        params.offset      = offset;
        params.destination = curr_slot->header_mem_addr[buffer];
        params.decompress  = curr_pack_entry->flags & resource_amalgatoc_pack_entry_flag_compressed;
        params.output_size = curr_pack_entry->data_size[buffer];
        params.read_size   = params.decompress ?
            curr_pack_entry->stored_size[buffer] : curr_pack_entry->data_size[buffer];

        offset += params.read_size;

        params.timeout = 0;
        params.user    = this;

        curr_stream_request_id[buffer] = nfl::add_request(&params);
    }

    currently_streaming = 1;

    curr_slot->notify_load_started(curr_pack_entry, callback);
}

// sub_76EE80
void amalga::resource_pack_streamer::stream_request_callback(i32 state, i32 request, void* user) {
    resource_pack_streamer* streamer = (resource_pack_streamer*)user;

    bool finished = false;

    if (state == nfl::request_state_completed &&
        streamer->currently_streaming == 1 &&
        request == streamer->curr_stream_request_id[buffer_location_main]) {

        streamer->curr_stream_request_id[buffer_location_main] = -1;
        finished = true;
    }

    const bool main_done = streamer->curr_stream_request_id[buffer_location_main] == -1;

    if (state == nfl::request_state_completed &&
        streamer->currently_streaming == 1 &&
        request == streamer->curr_stream_request_id[buffer_location_vram]) {

        streamer->curr_stream_request_id[buffer_location_vram] = -1;
        finished = true;
    }

    if (main_done && streamer->curr_stream_request_id[buffer_location_vram] == -1 && finished)
        streamer->finish_data_read();
}

// sub_76ECF0
void amalga::resource_pack_streamer::finish_data_read() {
    resource_pack_header* header = (resource_pack_header*)curr_slot->header_mem_addr[buffer_location_main];

    // retail compares the first two version words against the executable's here and drops the result

    mash::mash_info_struct mash_info(mash::UNMASH_MODE,
                                     (u8*)header + header->directory_offset,
                                     (i32)header->directory_size,
                                     true);

    resource_directory* directory = nullptr;

    mash_info.unmash_class(directory, nullptr, mash::NORMAL_BUFFER);
    mash::mash_info_struct::construct_class(directory);

    u32 handle;
    directory->handle = *retail::sub_72BD10(&handle, (i32)directory);

    directory->constructor_common(curr_slot);

    string_hash no_group;
    no_group.initialize(mash::ALLOCATED);

    if (curr_pack_entry->group_hash != no_group) {
        const resource_key group { curr_pack_entry->group_hash, e_resource_type::packfile };

        resource_pack_slot* parent_slot = find_loaded_pack(group, resource_manager::get_partition_pointer(curr_slot));

        retail::sub_5FD120((u32*)parent_slot->pack_directory, &directory->parent);
    }

    curr_slot->pack_directory = directory;

    nfl::close_file(curr_file_id);
}

// sub_739EF0
amalga::resource_pack_slot* amalga::resource_pack_streamer::find_loaded_pack(const resource_key        &pack_name,
                                                                                   resource_partition*  partition_to_search) {

    while (true) {
        u32 name_hash;
        retail::sub_5FD120((u32*)&pack_name, &name_hash);

        resource_pack_slot* slot = partition_to_search->get_pack_slot(string_hash(name_hash));

        if (slot)
            return slot;

        // a miss moves on to the next partition up the chain; game ends it
        resource_partition** partitions = resource_manager::references::partitions.read()->begin();

        if (partition_to_search == partitions[resource_partition_hero] ||
            partition_to_search == partitions[resource_partition_language])

            partition_to_search = partitions[resource_partition_game];
        else if (partition_to_search == partitions[resource_partition_mission] ||
                 partition_to_search == partitions[resource_partition_guest])

            partition_to_search = partitions[resource_partition_act];
        else if (partition_to_search == partitions[resource_partition_sky] ||
                 partition_to_search == partitions[resource_partition_shenani])

            partition_to_search = partitions[resource_partition_common];
        else if (partition_to_search == partitions[resource_partition_common])
            partition_to_search = partitions[resource_partition_game];
        else if (partition_to_search == partitions[resource_partition_act]      ||
                 partition_to_search == partitions[resource_partition_district] ||
                 partition_to_search == partitions[resource_partition_voice])

            partition_to_search = partitions[resource_partition_common];
        else
            return nullptr;
    }
}

// sub_74D9F0
void amalga::resource_pack_streamer::unload_all() {
    load_queue.clear();

    for (u32 index = 0; index < pack_slots->size(); ++index) {
        resource_pack_slot* slot = (*pack_slots)[index];

        if (slot->slot_state != slot_state_ready)
            continue;

        if (slot->my_callback)
            slot->my_callback(callback_pre_destruct, &slot->my_partition->streamer, slot, nullptr);

        slot->slot_state = slot_state_destructing;
    }
}

// sub_74DAB0
bool amalga::resource_pack_streamer::is_idle() const {
    return !currently_streaming && !load_queue.size() && all_slots_idle();
}

// sub_74DA80
bool amalga::resource_pack_streamer::all_slots_idle() const {
    if (!pack_slots)
        return true;

    for (resource_pack_slot* slot : *pack_slots) {
        if (slot->slot_state != slot_state_empty && slot->slot_state != slot_state_ready)
            return false;
    }

    return true;
}
