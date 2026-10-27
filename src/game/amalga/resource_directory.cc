#include "retail.hh"
#include "game/amalga/resource_directory.hh"
#include "game/amalga/resource_manager.hh"
#include "game/amalga/resource_pack_slot.hh"
#include "game/amalga/resource_partition.hh"
#include "game/binary_search.hh"
#include "game/four_cc.hh"

using namespace treyarch;

// inlined @ sub_76B810
void amalga::resource_descriptor::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    // the shared stream says whether the optional extension was mashed at all
    u8 class_mashed = 0xFF;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(extension, this, buffer);
    else
        extension = nullptr;
}

// sub_72DF90
// inlined @ sub_73A130, sub_75F8D0, sub_75E2C0
u8* amalga::resource_descriptor::calculate_adjusted_payload() const {
    const u32 payload_mode = references::resource_type_records.get()[(u32)type].payload_mode;

    if (!raw_payload)
        return nullptr;

    if (!payload_mode)
        return raw_payload + *(u32*)(raw_payload + 4);

    return payload_mode == 2 ? raw_payload + 8 : raw_payload;
}

// sub_72DF30
i32 amalga::resource_descriptor::compare_name_hash(const void* key, const void* entry) {
    u32 find_me;
    retail::sub_5FD120((u32*)key, &find_me);

    u32 hash;
    retail::sub_5FD120((u32*)*(resource_descriptor* const*)entry, &hash);

    if (find_me <= hash)
        return -(i32)(find_me < hash);

    return 1;
}

// sub_76E550
void amalga::resource_directory::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    descriptors.unmash(mash_info, this, buffer);

    u8 class_mashed = 0xFF;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(unk_3c4, this, buffer);
    else
        unk_3c4 = nullptr;
}

// sub_73A130
void amalga::resource_directory::constructor_common(resource_pack_slot* slot) {
    pack_slot = slot;

    u8* base = slot->header_mem_addr[buffer_location_main];

    for (u32 index = 0; index < descriptors.size; ++index) {
        resource_descriptor* descriptor = descriptors.data[index];

        descriptor->raw_payload      = base + descriptor->payload_offset;
        descriptor->adjusted_payload = descriptor->calculate_adjusted_payload();
    }
}

// sub_73A1A0
amalga::resource_descriptor* amalga::resource_directory::find_local_resource(const resource_key &key) const {
    const i32 begin_idx = (i32)type_starts[key.type];
    const i32 end_idx   = begin_idx + (i32)type_counts[key.type];

    i32 index = 0;

    if (!binary_search_array_cmp(&key, descriptors.data, begin_idx, end_idx, &index, resource_descriptor::compare_name_hash))
        return nullptr;

    return descriptors.data[index];
}

// sub_74DC80
amalga::resource_descriptor* amalga::resource_directory::find_resource(const resource_key &key, resource_directory** out_dir) {
    resource_descriptor* found = nullptr;

    if (out_dir)
        *out_dir = nullptr;

    if (resource_descriptor* descriptor = find_local_resource(key)) {
        if (out_dir)
            *out_dir = this;

        return descriptor;
    }

    resource_partition* partition = pack_slot->my_partition;

    // common packs also look in the act pack
    if (partition->partition_enum == resource_partition_common) {
        vc8::vector<resource_partition*>* partitions = resource_manager::references::partitions.read();

        if ((*partitions)[resource_partition_act]->pack_slots.size()) {
            resource_manager::references::unk_010300a8.get().acquire();

            resource_pack_slot* act_slot = (*resource_manager::references::partitions.read())[resource_partition_act]->pack_slots[0];

            resource_manager::references::unk_010300a8.get().release();

            if (act_slot->slot_state == slot_state_ready) {
                resource_directory* act_directory = act_slot->pack_directory;

                if (resource_descriptor* descriptor = act_directory->find_local_resource(key)) {
                    if (out_dir)
                        *out_dir = act_directory;

                    return descriptor;
                }
            }
        }
    }

    if (parent) {
        found = ((resource_directory*)parent)->find_resource(key, out_dir);

        if (found)
            return found;
    }

    if (partition->type != partition_type_stack)
        return found;

    for (resource_pack_slot** slot = partition->pack_slots.begin(); slot != partition->pack_slots.end(); ++slot) {
        if ((*slot)->slot_state != slot_state_ready)
            continue;

        resource_directory* directory = (*slot)->pack_directory;

        if (resource_descriptor* descriptor = directory->find_local_resource(key)) {
            if (out_dir)
                *out_dir = directory;

            return descriptor;
        }
    }

    return found;
}

// sub_757570
bool amalga::resource_directory::get_resource_descriptor(const resource_key          &key,
                                                               u32*                   size,
                                                               resource_pack_slot**   slot,
                                                               resource_descriptor**  descriptor) {

    string_hash empty;
    empty.initialize(mash::ALLOCATED);

    if (key.hash == empty || !pack_slot->is_data_ready())
        return false;

    resource_directory*  found_dir = nullptr;
    resource_descriptor* found     = find_resource(key, &found_dir);

    if (!found)
        return false;

    if (size)
        *size = found->payload_size;

    if (slot)
        *slot = found_dir->pack_slot;

    if (descriptor)
        *descriptor = found;

    return true;
}

// sub_75E2C0
u8* amalga::resource_directory::get_resource(const resource_key         &key,
                                                   u32*                  size,
                                                   resource_pack_slot**  slot) {

    resource_descriptor* descriptor;

    if (!get_resource_descriptor(key, size, slot, &descriptor))
        return nullptr;

    return descriptor->calculate_adjusted_payload();
}

// sub_72DEF0
// inlined @ sub_74DAF0
void* amalga::resource_directory::find_local_merged_resource(string_hash name, u32 type) const {
    merged_apk::file_entry* entry = merged_apk_file->find_entry(name, type);

    return entry ? entry->get_section_data(merged_apk_file, type, 0, 1) : nullptr;
}

// sub_74DAF0
void* amalga::resource_directory::find_merged_resource(string_hash          name,
                                                       u32                  type,
                                                       resource_directory** out_dir) {

    void* found = nullptr;

    if (out_dir)
        *out_dir = nullptr;

    if (void* resource = find_local_merged_resource(name, type)) {
        if (out_dir)
            *out_dir = this;

        return resource;
    }

    resource_partition* partition = pack_slot->my_partition;

    // common packs also look in the act pack
    if (partition->partition_enum == resource_partition_common) {
        vc8::vector<resource_partition*>* partitions = resource_manager::references::partitions.read();

        if ((*partitions)[resource_partition_act]->pack_slots.size()) {
            resource_manager::references::unk_010300a8.get().acquire();

            resource_pack_slot* act_slot = (*resource_manager::references::partitions.read())[resource_partition_act]->pack_slots[0];

            resource_manager::references::unk_010300a8.get().release();

            if (act_slot->slot_state == slot_state_ready) {
                resource_directory* act_directory = act_slot->pack_directory;

                if (void* resource = act_directory->find_local_merged_resource(name, type)) {
                    if (out_dir)
                        *out_dir = act_directory;

                    return resource;
                }
            }
        }
    }

    if (parent) {
        found = ((resource_directory*)parent)->find_merged_resource(name, type, out_dir);

        if (found)
            return found;
    }

    if (partition->type != partition_type_stack)
        return found;

    for (resource_pack_slot** slot = partition->pack_slots.begin(); slot != partition->pack_slots.end(); ++slot) {
        if ((*slot)->slot_state != slot_state_ready)
            continue;

        resource_directory* directory = (*slot)->pack_directory;

        if (void* resource = directory->find_local_merged_resource(name, type)) {
            if (out_dir)
                *out_dir = directory;

            return resource;
        }
    }

    return found;
}

// sub_757490
void* amalga::resource_directory::get_merged_resource(string_hash          name,
                                                      u32                  type,
                                                      resource_pack_slot** slot) {

    if (!name.valid() || !pack_slot->is_data_ready())
        return nullptr;

    resource_directory* found_dir = nullptr;
    void*               resource  = find_merged_resource(name, type, &found_dir);

    if (!resource)
        return nullptr;

    if (slot)
        *slot = found_dir->pack_slot;

    return resource;
}

// sub_7574F0
void* amalga::resource_directory::get_merged_resource_or_default(string_hash          name,
                                                                 u32                  type,
                                                                 resource_pack_slot** slot) {

    if (name.valid() && pack_slot->is_data_ready()) {
        resource_directory* found_dir = nullptr;

        if (void* resource = find_merged_resource(name, type, &found_dir)) {
            if (slot)
                *slot = found_dir->pack_slot;

            return resource;
        }
    }

    if (type == four_cc('T', 'E', 'X'))
        return resource_manager::references::white_texture.read();

    return nullptr;
}
