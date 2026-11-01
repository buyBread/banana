#include "retail.hh"
#include "game/arch_base.hh"
#include "game/event/event_manager.hh"
#include "game/wds/entity/entity_base.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace references {
    util::memory_reference<string_hash> collide_entity { 0x0102C788 };
}} // treyarch::references

using namespace treyarch;

// sub_428950
arch_base::~arch_base() {
    finalize(mash::ALLOCATED);
}

void arch_base::finalize(mash::allocation_scope scope) {
    ((void (__thiscall*)(arch_base*, mash::allocation_scope))retail::sub_626770)(this, scope);
}

// slot 2
void arch_base::destruct_mashed_class() {
    ((void (__thiscall*)(arch_base*))retail::sub_639150)(this);
}

// slot 3
void arch_base::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(arch_base*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_6391B0)(this,
                                                                                                             mash_info,
                                                                                                             containing_class_ptr,
                                                                                                             buffer);
}

// slot 4
mash::virtual_types_key arch_base::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const arch_base*))retail::sub_4289A0)(this);
}

// slot 6
bool arch_base::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const arch_base*, mash::virtual_types_key))retail::sub_718FB0)(this, parent_class);
}

// slot 8
i32 arch_base::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const arch_base*))retail::sub_4C6B00)(this);
}

// slot 9
bool arch_base::is_an_arch_base() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_667F80)(this);
}

// slot 10
bool arch_base::is_an_entity_base() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 11
bool arch_base::is_an_entity() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 12
bool arch_base::is_an_actor() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 13
bool arch_base::is_a_conglomerate() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 14
bool arch_base::is_a_conglomerate_clone() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 15
bool arch_base::is_a_dynamic_conglomerate_clone() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 16
bool arch_base::is_a_grenade() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 17
bool arch_base::is_a_beam() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 18
bool arch_base::is_a_particle_generator() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 19
bool arch_base::is_a_polytube() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 20
bool arch_base::is_a_sky() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 21
bool arch_base::is_a_sign() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 22
bool arch_base::is_a_web_wall() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 23
bool arch_base::is_a_marker() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 24
bool arch_base::is_an_ai_cover_marker() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 25
bool arch_base::is_an_anchor_marker() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 26
bool arch_base::is_a_line_marker_base() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 27
bool arch_base::is_a_line_anchor() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 28
bool arch_base::is_a_camera() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 29
bool arch_base::is_a_game_camera() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 30
bool arch_base::is_a_marky_camera() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 31
bool arch_base::is_a_spiderman_camera() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 32
bool arch_base::is_an_animated_camera() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 33
bool arch_base::is_a_light_source() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 34
bool arch_base::is_a_door() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 35
bool arch_base::unk_035() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 36
bool arch_base::is_a_trigger() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 37
bool arch_base::is_a_box_trigger() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 38
bool arch_base::is_a_sphere_trigger() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 39
bool arch_base::is_a_double_sphere_trigger() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 40
bool arch_base::unk_040() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 41
bool arch_base::is_a_vehicle() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 42
bool arch_base::is_a_water_box() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 43
bool arch_base::unk_043() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 44
bool arch_base::unk_044() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 45
bool arch_base::is_a_fight_group() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 46
bool arch_base::is_a_fakir() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 47
bool arch_base::is_a_script_instance_shadow() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 48
bool arch_base::is_a_script_controller() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 49
bool arch_base::is_a_mission_progression_data() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 50
bool arch_base::is_a_mission_stage_data() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 51
bool arch_base::is_a_mission_objective_data() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// slot 52
bool arch_base::is_a_finger_of_god() const {
    return ((bool (__thiscall*)(const arch_base*))retail::sub_71F870)(this);
}

// sub_601750
void arch_base::raise_event(string_hash event_type_id) {
    event_manager::raise_event(event_type_id, my_handle);
}

// sub_601770
void arch_base::raise_event(event* raised_event) {
    event_manager::raise_event(raised_event, my_handle);
}

// sub_601790
u32 arch_base::add_callback(string_hash event_type_id, code_event_callback_function function, void* parameters, bool one_shot) {
    if (event_type_id == references::collide_entity.read() && is_an_entity_base())
        ((entity_base*)this)->ext_flags |= 0x10000000;

    return event_manager::add_callback(event_type_id, my_handle, function, parameters, one_shot);
}

// sub_601850
void arch_base::remove_callback(u32 callback_id, string_hash event_type_id) {
    event_manager::remove_callback(callback_id, event_type_id, my_handle);
}

// sub_6017E0
u32 arch_base::add_callback(      string_hash                 event_type_id,
                                  chuck::vm::script_instance* instance,
                                  chuck::vm::script_function* function,
                            const void*                       parameters,
                                  bool                        one_shot) {

    if (event_type_id == references::collide_entity.read() && is_an_entity_base())
        ((entity_base*)this)->ext_flags |= 0x10000000;

    return event_manager::add_callback(event_type_id, my_handle, instance, function, parameters, one_shot);
}

// sub_601880
void arch_base::clear_script_callbacks(chuck::vm::script_executable* script_exe) {
    event_manager::clear_script_callbacks(my_handle, script_exe);
}

// sub_6018A0
void arch_base::clear_script_callback(string_hash function_name) {
    event_manager::clear_script_callback(my_handle, function_name);
}

// sub_6018C0
void arch_base::clear_script_callback(u32 id) {
    event_manager::clear_script_callback(my_handle, id);
}
