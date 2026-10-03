#include "treyarch/amalga/resource_directory.hh"
#include "treyarch/amalga/resource_pack_slot.hh"

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

        u8* raw_payload = base + descriptor->payload_offset;

        descriptor->raw_payload = raw_payload;

        const u32 payload_mode = references::resource_type_records.get()[descriptor->type].payload_mode;

        if (!raw_payload)
            descriptor->adjusted_payload = nullptr;
        else if (!payload_mode)
            descriptor->adjusted_payload = raw_payload + *(u32*)(raw_payload + 4);
        else
            descriptor->adjusted_payload = payload_mode == 2 ? raw_payload + 8 : raw_payload;
    }
}
