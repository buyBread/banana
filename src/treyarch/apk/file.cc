#include "treyarch/apk/file.hh"
#include "treyarch/base/tl_system.hh"

using namespace treyarch;

// sub_A6AC90
i32 apk::file::get_section_index(string_hash section_name) const {
    for (u32 index = 0; index < n_sections; ++index) {
        if (sections[index].name == section_name)
            return (i32)index;
    }

    return -1;
}

// sub_A6ABF0
apk::file_entry* apk::file::get_file(string_hash name, u32 type) const {
    if (!file_types)
        return nullptr;

    for (const file_type_entry* current = file_types; current->type; current = current->next(n_sections)) {
        if (current->type != type)
            continue;

        file_entry* entries = current->first_entry;

        // the first entry gets a look before the search
        if (entries->name.hash == name)
            return entries;

        i32 low  = 0;
        i32 high = (i32)current->n_entries - 1;

        while (low <= high) {
            const i32 middle = (low + high) / 2;

            auto entry = (file_entry*)((u8*)entries + middle * current->entry_stride());

            if (name.source_hash_code > entry->name.hash.source_hash_code)
                low = middle + 1;
            else if (name.source_hash_code < entry->name.hash.source_hash_code)
                high = middle - 1;
            else
                return entry;
        }

        return nullptr;
    }

    return nullptr;
}

// sub_A6AA40
void* apk::file_entry::get_data(const file* owner,
                                u32         type,
                                u32         section_index,
                                u32         occurrence) const {

    (void)occurrence;

    if (!owner->file_types)
        return nullptr;

    for (const file_type_entry* current = owner->file_types; current->type; current = current->next(owner->n_sections)) {
        if (current->type != type)
            continue;

        const u8 section_slot = current->mapping(section_index).section_slot();

        if (section_slot == absent_section_slot)
            return nullptr;

        return (void*)section(section_slot);
    }

    return nullptr;
}

// sub_A6AAA0
u32 apk::file::get_data_size(file_entry* entry,
                             i32         section_index,
                             u32         occurrence) const {

    (void)occurrence;

    file_type_entry* type = file_types;

    if (!type || section_index == -1 || !type->type)
        return 0;

    u32 section_offset = 0;

    do {
        if (type->n_entries) {
            u8* entry_data = (u8*)type->first_entry;

            for (u32 index = 0; index < type->n_entries; ++index) {
                auto current_entry = (file_entry*)entry_data;
                u8   section_slot  = type->mapping(section_index).section_slot();

                if (section_offset) {
                    if (section_slot != absent_section_slot)
                        return (u32)current_entry->section(section_slot) - section_offset;
                } else if (current_entry == entry) {
                    if (section_slot == absent_section_slot)
                        return 0;

                    section_offset = (u32)current_entry->section(section_slot);
                }

                entry_data += type->entry_stride();
            }
        }

        type = type->next(n_sections);
    } while (type->type);

    if (section_offset) {
        const file_section &section = sections[section_index];

        return (u32)section.data + section.size - section_offset;
    }

    return 0;
}

void* apk::file::resolve_data_reference(      data_reference reference,
                                        const u8*            string_base) const {

    if (reference.section_index() == string_section_index)
        return (void*)(string_base + reference.byte_offset());

    return (u8*)sections[reference.section_index()].data + reference.byte_offset();
}

// sub_A6ACC0
void apk::file::apply_fixups(      data_reference* &fixup_data,
                             const u8*              string_base) {

    while (!fixup_data->is_terminator()) {
        auto target = (data_reference*)resolve_data_reference(*fixup_data++, string_base);

        *(void**)target = resolve_data_reference(*target, string_base);
    }

    ++fixup_data;
}

// sub_A6AD30
void apk::file::apply_references(      data_reference* reference_data,
                                 const u8*             string_base) {

    if (!reference_data)
        return;

    u32   previous_type  = 0;
    u32   previous_hash  = 0;
    void* previous_value = nullptr;
    bool  have_previous  = false;

    while (!reference_data->is_terminator()) {
        auto reference = (resource_reference*)reference_data;
        auto target    = (void**)resolve_data_reference(reference->target, string_base);

        if ((u32)reference->name.text & 1)
            reference->name.text = (char*)(string_base + ((u32)reference->name.text & ~1u));

        if (have_previous &&
            previous_type == reference->type &&
            previous_hash == reference->name.hash.source_hash_code) {

            *target = previous_value;
        } else {
            resource_callback callback = references::resource_locator_callback.read();

            *target = callback ? callback(&reference->name, reference->type) : nullptr;

            previous_type  = reference->type;
            previous_hash  = reference->name.hash.source_hash_code;
            previous_value = *target;
            have_previous  = true;
        }

        reference_data = (data_reference*)(reference + 1);
    }
}

// sub_A6ABC0
u32 apk::file::get_num_files() const {
    u32 count = 0;

    if (file_types) {
        for (const file_type_entry* type = file_types; type->type; type = type->next(n_sections))
            count += type->n_entries;
    }

    return count;
}

// sub_A6AE90
void apk::file::invoke_section_load_callbacks() {
    for (u32 index = 0; index < n_sections; ++index) {
        file_section                &section  = sections[index];
        section_callback_list_entry* callback = find_section_callback(section.name);

        if (callback && callback->load)
            callback->load(this, &section, callback->user_data);
    }
}

void apk::file::invoke_file_type_load_callbacks() {
    invoke_file_type_load_callbacks(0, get_num_files());
}

// sub_A6AEE0
void apk::file::invoke_file_type_load_callbacks(u32 start, u32 count) {
    if (!file_types)
        return;

    u32 current = 0;
    u32 end     = start + count;

    for (file_type_entry* type = file_types; type->type; type = type->next(n_sections)) {
        u32 type_end = current + type->n_entries;

        if (start >= type_end) {
            current = type_end;
            continue;
        }

        u32 first = start > current ? start - current : 0;
        u32 last  = end < type_end ? end - current : type->n_entries;

        file_callback_list_entry* callback = find_file_callback(type->type, type->version);

        if (callback && callback->load) {
            u32 stride = type->entry_stride();
            u8* entry_data = (u8*)type->first_entry + first * stride;

            for (u32 entry_index = first; entry_index < last; ++entry_index) {
                auto entry = (file_entry*)entry_data;
                void* mapped_sections[maximum_section_count] {};

                for (u32 section_index = 0; section_index < n_sections; ++section_index) {
                    u8 section_slot = type->mapping(section_index).section_slot();

                    if (section_slot != absent_section_slot)
                        mapped_sections[section_index] = entry->section(section_slot);
                }

                callback->load(this, entry, mapped_sections, callback->user_data);

                entry_data += stride;
            }
        }

        current = type_end;

        if (current >= end)
            break;
    }
}

// sub_A6B1E0
void apk::file::invoke_load_callbacks() {
    invoke_section_load_callbacks();
    invoke_file_type_load_callbacks();
}

// sub_A6B050
void apk::file::invoke_delete_callbacks() {
    if (!n_sections || !file_types)
        return;

    for (file_type_entry* type = file_types; type->type; type = type->next(n_sections)) {
        file_callback_list_entry* callback = find_file_callback(type->type, type->version);

        if (!callback || !callback->remove)
            continue;

        u8* entry_data = (u8*)type->first_entry;

        for (u32 entry_index = 0; entry_index < type->n_entries; ++entry_index) {
            auto entry = (file_entry*)entry_data;
            void* mapped_sections[maximum_section_count] {};

            for (u32 section_index = 0; section_index < n_sections; ++section_index) {
                u8 section_slot = type->mapping(section_index).section_slot();

                if (section_slot != absent_section_slot)
                    mapped_sections[section_index] = entry->section(section_slot);
            }

            callback->remove(this, entry, mapped_sections, callback->user_data);

            entry_data += type->entry_stride();
        }
    }

    for (u32 index = 0; index < n_sections; ++index) {
        file_section                &section  = sections[index];
        section_callback_list_entry* callback = find_section_callback(section.name);

        if (callback && callback->remove)
            callback->remove(this, &section, callback->user_data);
    }
}

// sub_A6B1B0
void apk::delete_file(file* value) {
    if (!value)
        return;

    value->invoke_delete_callbacks();

    if (value->flags & file_from_file_buf)
        tl_mem_free((u8*)value - sizeof(file_header));
}

apk::file_callback_list_entry* apk::find_file_callback(u32 type, u32 version) {
    for (file_callback_list_entry* entry = references::file_callback_list.read(); entry; entry = entry->next) {
        if (entry->type == type && entry->version == version)
            return entry;
    }

    return nullptr;
}

apk::section_callback_list_entry* apk::find_section_callback(string_hash name) {
    for (section_callback_list_entry* entry = references::section_callback_list.read(); entry; entry = entry->next) {
        if (entry->name == name)
            return entry;
    }

    return nullptr;
}

// sub_A6A9A0
apk::file_callback_list_entry* apk::register_file_type(u32           type,
                                                       u32           version,
                                                       file_callback load,
                                                       file_callback remove,
                                                       void*         user_data) {

    auto entry = (file_callback_list_entry*)tl_mem_alloc
        (sizeof(file_callback_list_entry), 8, 0);

    entry->type      = type;
    entry->version   = version;
    entry->load      = load;
    entry->remove    = remove;
    entry->user_data = user_data;
    entry->next      = references::file_callback_list.read();

    references::file_callback_list.write(entry);

    return entry;
}

// sub_A6A9E0
apk::section_callback_list_entry* apk::register_section_type(string_hash      name,
                                                             section_resolver resolve,
                                                             section_callback load,
                                                             section_callback remove,
                                                             void*            user_data) {

    auto entry = (section_callback_list_entry*)tl_mem_alloc
        (sizeof(section_callback_list_entry), 8, 0);

    entry->name      = name;
    entry->resolve   = resolve;
    entry->load      = load;
    entry->remove    = remove;
    entry->user_data = user_data;
    entry->next      = references::section_callback_list.read();

    references::section_callback_list.write(entry);

    return entry;
}

// sub_A6AA20
void apk::set_resource_callback(resource_callback callback) {
    references::resource_locator_callback.write(callback);
}

// sub_A6AA30
void* apk::resolve_resource(fixed_string* name, u32 type) {
    resource_callback callback = references::resource_locator_callback.read();

    if (!callback)
        return nullptr;

    return callback(name, type);
}
