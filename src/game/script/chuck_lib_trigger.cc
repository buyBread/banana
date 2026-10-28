#include <cstring>

#include "retail.hh"
#include "game/arch_base.hh"
#include "game/script/chuck_lib_trigger.hh"
#include "game/trigger.hh"
#include "game/wds/entity/entity.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// sub_83E040
u32 slc_trigger_t::find_instance(const mash::string &name) const {
    string_hash hash;

    hash.initialize(mash::ALLOCATED, name.c_str());

    auto* trigger = (arch_base*)retail::sub_7456D0((u32*)trigger_manager::inst(), &hash.source_hash_code); // trigger_manager::find_instance

    return trigger->my_handle.value;
}

bool slf__find_trigger__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855080)(this, stack);
}

bool slf__find_trigger__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855100)(this, stack);
}

bool slf__trigger_exists__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855160)(this, stack);
}

bool slf__trigger_exists__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8551D0)(this, stack);
}

bool slf__create_trigger__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855250)(this, stack);
}

bool slf__create_trigger__str__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855310)(this, stack);
}

bool slf__create_trigger__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8553F0)(this, stack);
}

bool slf__create_trigger__str__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8554E0)(this, stack);
}

bool slf__create_double_sphere_trigger__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8555D0)(this, stack);
}

bool slf__create_box_trigger_from_frame__vector3d__vector3d__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8556C0)(this, stack);
}

bool slf__create_box_trigger__vector3d__vector3d__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8557A0)(this, stack);
}

bool slf__create_box_trigger__str__vector3d__vector3d__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855AA0)(this, stack);
}

bool slf__destroy_trigger__trigger__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855DC0)(this, stack);
}

bool slf__trigger_is_valid__trigger__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_855E00)(this, stack);
}

// sub_854AC0
bool slf__trigger__get_abs_position__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    // a code system's trigger reads as the origin
    vector3d result = self && !self->is_code_owned() ? self->get_abs_position() : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_880C00
bool slf__trigger__get_triggered_entity_list__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto list  = (chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04);

    if (!self)
        return true;

    vc8::list<arch_base_vhandle>* contained = self->contained_list;

    // emptied even without a list; each entity is a cell holding its handle
    list->clear();

    if (!contained)
        return true;

    for (vc8::list<arch_base_vhandle>::node* node = contained->begin(); node != contained->end(); node = node->next) {
        chuck::vm::chuck_dynamic_array_element_t element;

        list->push_back(element);
        *(u32*)list->back().stuff = node->value.value;
    }

    return true;
}

// sub_854B40
bool slf__trigger__contains__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto ent   = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

    f32 result = self && ent && ent->is_an_entity_base() && self->point_inside(&ent->my_abs_po->get_position()) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_854BD0
bool slf__trigger__set_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on    = stack.local_num(0x04);

    if (self && !self->is_code_owned())
        retail::sub_74BC60((u32*)self, on != 0.0f); // trigger::set_active

    return true;
}

// sub_854C20
bool slf__trigger__set_use_any_char__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on    = stack.local_num(0x04);

    if (self && !self->is_code_owned())
        ((void (__thiscall*)(trigger*, bool))retail::sub_75D2B0)(self, on != 0.0f); // trigger::set_use_any_char

    return true;
}

// sub_854C70
bool slf__trigger__set_multiple_entrance__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on    = stack.local_num(0x04);

    if (self && !self->is_code_owned())
        retail::sub_75D470((u32*)self, on != 0.0f); // trigger::set_multiple_entrance

    return true;
}

// sub_854CC0
bool slf__trigger__set_sees_dead_people__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on    = stack.local_num(0x04);

    if (self && !self->is_code_owned())
        ((void (__thiscall*)(trigger*, bool))retail::sub_74BD70)(self, on != 0.0f); // trigger::set_sees_dead_people

    return true;
}

// sub_854D10
bool slf__trigger__set_uses_extended_events__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on    = stack.local_num(0x04);

    if (self && !self->is_code_owned())
        self->set_uses_extended_events(on != 0.0f);

    return true;
}

// sub_854D60
bool slf__trigger__set_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    // a box keeps its extents
    if (self && !self->is_code_owned() && self->is_sphere_trigger())
        ((void (__thiscall*)(trigger*, f32))retail::sub_7456B0)(self, value); // sphere_trigger::set_radius

    return true;
}

// sub_854DB0
bool slf__trigger__get_radius__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self ? self->get_radius() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_854E10
bool slf__trigger__set_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self  = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = (const vector3d*)&stack.local_uint(0x04);

    if (self && !self->is_code_owned())
        self->set_offset(value);

    return true;
}

// sub_854E50
bool slf__trigger__is_box_trigger__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_box_trigger() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_854ED0
bool slf__trigger__is_sphere_trigger__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_sphere_trigger() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_854F50
bool slf__trigger__is_occupied__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && (self->flags & trigger_flag_occupied) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_880C70
bool slf__trigger__get_box_trigger_frame__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto list  = (chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04);

    // emptied first, so anything but a box leaves it empty
    list->clear();

    if (!self || !self->is_box_trigger())
        return true;

    vector3d frame[3];
    ((void (__thiscall*)(trigger*, vector3d*))retail::sub_72D2F0)(self, frame); // box_trigger::get_frame

    for (const vector3d &axis : frame)
        list->push_back(*(const chuck::vm::chuck_dynamic_array_element_t*)&axis);

    return true;
}

// sub_854FD0
bool slf__trigger__set_box_trigger_frame__vector3d__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x28;

    auto self = (trigger*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto x    = (const vector3d*)&stack.local_uint(0x04);
    auto y    = (const vector3d*)&stack.local_uint(0x10);
    auto z    = (const vector3d*)&stack.local_uint(0x1C);

    if (!self || !self->is_box_trigger())
        return true;

    vector3d frame[3] = { *x, *y, *z };

    ((void (__thiscall*)(trigger*, const vector3d*))retail::sub_745650)(self, frame); // box_trigger::set_frame

    return true;
}
