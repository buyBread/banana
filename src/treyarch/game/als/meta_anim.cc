#include "retail.hh"
#include "treyarch/game/als/meta_anim.hh"
#include "treyarch/nal/nal.hh"
#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/memory/heap.hh"

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
        memory::heap::free(name.text);

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
    return memory::heap::allocate((u32)size);
}

void* als::als_nal_meta_anim::operator new[](std::size_t size) noexcept {
    return memory::heap::allocate((u32)size);
}

// inlined into the deleting destructor
void als::als_nal_meta_anim::operator delete(void* allocation) noexcept {
    memory::heap::free(allocation);
}

// inlined into the deleting destructor
void als::als_nal_meta_anim::operator delete[](void* allocation) noexcept {
    memory::heap::free(allocation);
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
                                                                              u32                     unk,
                                                                              void*                   the_anim,
                                                                              animation_logic_system* the_als,
                                                                              state_machine*          the_layer) {

    using create_anim_inst_t = nal_als_instance_bridge* (__thiscall*)(als_meta_anim_by_filename*,
                                                                      nal_als_instance_bridge*,
                                                                      nal::transport_list*,
                                                                      u32,
                                                                      nal::skeleton_map*,
                                                                      u32,
                                                                      void*,
                                                                      animation_logic_system*,
                                                                      state_machine*);

    nal_als_instance_bridge result;
    ((create_anim_inst_t)retail::sub_5BEC90)(this, &result, the_skel, flags, skel_map, unk, the_anim, the_als, the_layer);

    return result;
}

// sub_667F80
bool als::als_meta_anim_by_filename::is_delay_create() {
    return true;
}

// sub_5E2F80
void als::als_meta_anim_by_filename::delay_create(actor* the_actor) {
    ((void (__thiscall*)(als_meta_anim_by_filename*, actor*))retail::sub_5E2F80)(this, the_actor);
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
