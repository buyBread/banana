#include "retail.hh"
#include "game/amalga/resource_manager.hh"
#include "game/amalga/resource_memory_map.hh"
#include "game/amalga/resource_pack_slot.hh"
#include "game/amalga/resource_partition.hh"
#include "game/als/meta_anim.hh"
#include "game/als/state_machine.hh"
#include "treyarch/nal/nal.hh"
#include "game/fixed_string.hh"
#include "game/hash/algo.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

string_hash als::als_meta_anim_base::my_type_key        { 0x364F0238 }; // "als::als_meta_anim_base"
string_hash als::als_nal_meta_anim::my_type_key         { 0x8160F9F8 }; // "als::als_nal_meta_anim"
string_hash als::als_meta_anim_by_filename::my_type_key { 0x22F7A5D8 }; // "als::als_meta_anim_by_filename"

// inlined @ sub_5F64D0
// inlined @ sub_418F50
void als::meta_key_anim::construct_mashed_class() {
    anim_id.construct_mashed_class();
}

// inlined @ sub_7D8DC0
void als::meta_key_anim::destruct_mashed_class() {
    anim_id.destruct_mashed_class();
}

// inlined @ sub_5EC420
// inlined @ sub_5EC4F0
void als::meta_key_anim::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    anim_id.unmash(mash_info, this, buffer);
}

// sub_5BEB70
void als::meta_key_anim::lookup_nal_animation() {
    fixed_string name;

    name.hash = string_hash(hash::djb2(anim_id.text));
    name.set_text(anim_id.text);

    nal::anim* anim = nal::get_anim(&name);

    if (name.text)
        mm::free(name.text);

    the_anim_ptr = anim;
}

// inlined @ sub_5B1510
// inlined @ sub_5B1530
// inlined @ sub_5F4FB0
// inlined @ sub_5F64D0
void als::als_meta_anim_base::construct_mashed_class() {
    my_name.construct_mashed_class();
}

// inlined @ sub_5B04C0
// inlined @ sub_5B1550
// inlined @ sub_5E3170
// inlined @ sub_5EAE60
void als::als_meta_anim_base::destruct_mashed_class() {
    my_name.destruct_mashed_class();
}

// inlined @ sub_5E2F30
// inlined @ sub_5E3140
// inlined @ sub_5F5760
// inlined @ sub_5F6530
void als::als_meta_anim_base::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    my_name.unmash(mash_info, this, buffer);
}

// inlined @ sub_5F4EF0
mash::virtual_types_key als::als_meta_anim_base::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// inlined @ sub_5F4EF0
bool als::als_meta_anim_base::is_subclass_of(mash::virtual_types_key parent_class) const {
    return mash::mash_virtual_base::get_virtual_type_key() == parent_class;
}

// sub_5B7840
const nal_anim_id &als::als_meta_anim_base::get_anim_name() const {
    return my_name;
}

// sub_5B7850
bool als::als_meta_anim_base::is_anim_looping() const {
    return get_anim_proxy()->flags & 1;
}

// sub_5B7860
f32 als::als_meta_anim_base::get_anim_duration() const {
    return get_anim_proxy()->duration;
}

// sub_5B7870
const nal::transport_list* als::als_meta_anim_base::get_skeleton() const {
    return get_anim_proxy()->transport_list;
}

// sub_71F870
bool als::als_meta_anim_base::is_delay_create() {
    return false;
}

// sub_689610
void als::als_meta_anim_base::delay_create(actor*) {}

// sub_5B4A70
void als::als_meta_anim_base::load_key_anims() {}

// sub_5BECD0
als::als_nal_meta_anim::als_nal_meta_anim() : als_meta_anim_base(),
                                              my_meta_anim_data(nullptr),
                                              unk_2c(0) {}

void* als::als_nal_meta_anim::operator new(std::size_t size) noexcept {
    return mm::allocate((u32)size);
}

void* als::als_nal_meta_anim::operator new[](std::size_t size) noexcept {
    return mm::allocate((u32)size);
}

// inlined into the deleting destructor
void als::als_nal_meta_anim::operator delete(void* allocation) noexcept {
    mm::free(allocation);
}

// inlined into the deleting destructor
void als::als_nal_meta_anim::operator delete[](void* allocation) noexcept {
    mm::free(allocation);
}

// sub_5B1510
void als::als_nal_meta_anim::construct_mashed_class() {
    als_meta_anim_base::construct_mashed_class();

    unk_2c            = 0;
    my_meta_anim_data = nullptr;
}

// sub_5B04C0
void als::als_nal_meta_anim::destruct_mashed_class() {
    als_meta_anim_base::destruct_mashed_class();
}

// sub_5E2F30
void als::als_nal_meta_anim::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    als_meta_anim_base::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_5BED00
mash::virtual_types_key als::als_nal_meta_anim::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5F4EF0
bool als::als_nal_meta_anim::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (als_meta_anim_base::get_virtual_type_key() == parent_class)
        return true;

    if (als_meta_anim_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5BAE20
i32 als::als_nal_meta_anim::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_A944D0
nal::anim* als::als_nal_meta_anim::get_anim_proxy() const {
    return nullptr;
}

// sub_5B14D0
als::nal_als_instance_bridge als::als_nal_meta_anim::create_anim_inst(nal::transport_list*    the_skel,
                                                                      u32                     flags,
                                                                      nal::skeleton_map*      skel_map,
                                                                      u32,
                                                                      void*,
                                                                      animation_logic_system* the_als,
                                                                      state_machine*          the_layer) {

    return my_meta_anim_data->create_anim_inst(the_skel, flags, skel_map, 0, this, the_als, the_layer);
}

// sub_5B14C0
bool als::als_nal_meta_anim::is_delay_create() {
    return my_meta_anim_data->is_delay_create();
}

// sub_5BEC60
void als::als_nal_meta_anim::delay_create(actor* the_actor) {
    my_meta_anim_data->delay_create(the_actor);

    create(my_meta_anim_data, false);
}

// sub_5BEBF0
void als::als_nal_meta_anim::create(als_meta_anim_base* the_meta_anim_data, bool) {
    my_meta_anim_data = the_meta_anim_data;
    my_name           = the_meta_anim_data->get_anim_name();
}

// sub_5B1530
void als::als_meta_anim_by_filename::construct_mashed_class() {
    als_meta_anim_base::construct_mashed_class();

    m_anim_prefix.construct_mashed_class();

    the_anim_ptr = nullptr;
}

// sub_5B1550
void als::als_meta_anim_by_filename::destruct_mashed_class() {
    m_anim_prefix.destruct_mashed_class();

    als_meta_anim_base::destruct_mashed_class();
}

// sub_5E3140
void als::als_meta_anim_by_filename::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    als_meta_anim_base::unmash(mash_info, containing_class_ptr, buffer);

    m_anim_prefix.unmash(mash_info, this, buffer);
}

// sub_5BED60
mash::virtual_types_key als::als_meta_anim_by_filename::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5F4EF0
bool als::als_meta_anim_by_filename::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (als_meta_anim_base::get_virtual_type_key() == parent_class)
        return true;

    if (als_meta_anim_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_71C6B0
i32 als::als_meta_anim_by_filename::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9ED9D0
nal::anim* als::als_meta_anim_by_filename::get_anim_proxy() const {
    return the_anim_ptr;
}

// sub_5BEC90
als::nal_als_instance_bridge als::als_meta_anim_by_filename::create_anim_inst(nal::transport_list*    the_skel,
                                                                              u32                     flags,
                                                                              nal::skeleton_map*      skel_map,
                                                                              u32,
                                                                              void*,
                                                                              animation_logic_system*,
                                                                              state_machine*          the_layer) {

    nal::anim_instance* nal_inst = the_anim_ptr->create_instance(flags, skel_map, the_skel);
    nal_inst->force_looping = the_layer->is_looping();

    nal_als_instance_bridge result;
    result.nal_inst = nal_inst;
    result.als_inst = nullptr;

    return result;
}

// sub_667F80
bool als::als_meta_anim_by_filename::is_delay_create() {
    return true;
}

// sub_5E2F80
void als::als_meta_anim_by_filename::delay_create(actor* the_actor) {
    nal_anim_id the_anim_name = get_anim_id_to_play(the_actor);

    fixed_string name;

    name.hash = string_hash(hash::djb2(the_anim_name.text));
    name.set_text(the_anim_name.text);

    nal::anim* anim = nal::get_anim(&name);

    if (name.text)
        mm::free(name.text);

    // the current context's, then the packs that may hold it
    if (!anim &&
        !(anim = find_anim_in_partition(amalga::resource_partition_mission, the_anim_name, false, nullptr)) &&
        !(anim = find_anim_in_partition(amalga::resource_partition_common,  the_anim_name, false, nullptr)))

        anim = find_anim_in_partition(amalga::resource_partition_district, the_anim_name, true, nullptr);

    the_anim_ptr = anim;
}

// inlined @ sub_5F6220
void als::als_meta_anim_table_shared::construct_mashed_class() {
    my_meta_anim.construct_mashed_class();

    initialize();
}

// inlined @ sub_5E8B00
void als::als_meta_anim_table_shared::destruct_mashed_class() {
    finalize();

    my_meta_anim.destruct_mashed_class();
}

// inlined @ sub_5F5380
void als::als_meta_anim_table_shared::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    my_meta_anim.unmash(mash_info, this, buffer);
}

// sub_5D5030
void als::als_meta_anim_table_shared::initialize() {
    my_nal_meta_anims = new als_nal_meta_anim[my_meta_anim.size];

    for (u32 index = 0; index < my_meta_anim.size; ++index)
        my_nal_meta_anims[index].create(my_meta_anim[index], my_meta_anim[index]->is_delay_create());
}

// inlined @ sub_5E8B00
void als::als_meta_anim_table_shared::finalize() {
    delete[] my_nal_meta_anims;

    my_nal_meta_anims = nullptr;
}

// sub_5BB460
als::als_nal_meta_anim* als::als_meta_anim_table_shared::get_nal_meta_anim(const nal_anim_id &the_anim_name, actor* the_actor) const {
    for (u32 index = 0; index < my_meta_anim.size; ++index) {
        als_nal_meta_anim* meta_anim = &my_nal_meta_anims[index];

        if (the_anim_name == meta_anim->my_name) {
            if (meta_anim->is_delay_create())
                meta_anim->delay_create(the_actor);

            return meta_anim;
        }
    }

    return nullptr;
}

// sub_5BB0B0
nal::anim* als::find_anim_in_pack(amalga::resource_pack_slot* slot, bool* found, const nal_anim_id &the_anim_name) {
    if (!slot || !slot->is_data_ready()) {
        if (found)
            *found = false;

        return nullptr;
    }

    amalga::push_resource_context_stack_object context(slot);

    fixed_string name;

    name.hash = string_hash(hash::djb2(the_anim_name.text));
    name.set_text(the_anim_name.text);

    nal::anim* anim = nal::get_anim(&name);

    if (name.text)
        mm::free(name.text);

    return anim;
}

// sub_5E0B50
nal::anim* als::find_anim_in_partition(amalga::e_resource_partition partition_enum,
                                       const nal_anim_id           &the_anim_name,
                                       bool                         every_slot,
                                       bool*                        found) {

    auto partition = amalga::resource_manager::get_partition_pointer(partition_enum);

    if (!partition || !partition->pack_slots.size())
        return nullptr;

    if (!every_slot)
        return find_anim_in_pack(amalga::resource_manager::get_best_context(partition_enum), found, the_anim_name);

    for (amalga::resource_pack_slot* slot : partition->pack_slots) {
        if (nal::anim* anim = find_anim_in_pack(slot, found, the_anim_name))
            return anim;
    }

    return nullptr;
}

// sub_5E0BE0
als::nal_als_anim_bridge als::get_anim_bridge(const als_meta_anim_table_shared* table,
                                              const nal_anim_id                &the_anim_name,
                                              actor*                            the_actor,
                                              bool*                             found) {

    if (found)
        *found = true;

    if (table) {
        if (als_nal_meta_anim* meta_anim = table->get_nal_meta_anim(the_anim_name, the_actor))
            return { nullptr, meta_anim };
    }

    fixed_string name;

    name.hash = string_hash(hash::djb2(the_anim_name.text));
    name.set_text(the_anim_name.text);

    nal::anim* anim = nal::get_anim(&name);

    if (name.text)
        mm::free(name.text);

    // a plain animation from the current context, then the packs that may hold it
    if (anim ||
        (anim = find_anim_in_partition(amalga::resource_partition_mission,  the_anim_name, false, found)) ||
        (anim = find_anim_in_partition(amalga::resource_partition_guest,    the_anim_name, false, found)) ||
        (anim = find_anim_in_partition(amalga::resource_partition_common,   the_anim_name, false, found)) ||
        (anim = find_anim_in_partition(amalga::resource_partition_district, the_anim_name, true,  found)))

        return { anim, nullptr };

    return { nullptr, nullptr };
}
