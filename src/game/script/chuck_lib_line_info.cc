#include "retail.hh"
#include "game/line_info.hh"
#include "game/math/references.hh"
#include "game/script/chuck_lib_line_info.hh"

using namespace treyarch;

bool slf__create_line_info__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85AB30)(this, stack);
}

bool slf__destroy_line_info__line_info__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85ABF0)(this, stack);
}

bool slf__line_info__check_collision__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85AC40)(this, stack);
}

bool slf__line_info__check_sphere_collision__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85AEE0)(this, stack);
}

// sub_85B0A0
bool slf__line_info__clear_collision__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (line_info*)stack.local_uint(0x00);

    if (self)
        self->clear();

    return true;
}

// sub_85B0C0
bool slf__line_info__set_start_pos__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self  = (line_info*)stack.local_uint(0x00);
    auto value = (const vector3d*)&stack.local_uint(0x04);

    if (self)
        self->start = *value;

    return true;
}

// sub_85B0F0
bool slf__line_info__get_start_pos__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (line_info*)stack.local_uint(0x00);

    vector3d result = self ? self->start : references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85B140
bool slf__line_info__set_end_pos__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self  = (line_info*)stack.local_uint(0x00);
    auto value = (const vector3d*)&stack.local_uint(0x04);

    if (self)
        self->end = *value;

    return true;
}

// sub_85B170
bool slf__line_info__get_end_pos__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (line_info*)stack.local_uint(0x00);

    vector3d result = self ? self->end : references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85B1C0
bool slf__line_info__get_hit_pos__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (line_info*)stack.local_uint(0x00);

    vector3d result = self ? self->hit_pos : references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85B210
bool slf__line_info__get_hit_normal__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (line_info*)stack.local_uint(0x00);

    vector3d result = self ? self->hit_norm : references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85B260
bool slf__line_info__get_hit_entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (line_info*)stack.local_uint(0x00);

    // a hit entity that has since gone answers 0
    u32 result = self && self->hit_entity.resolve() ? self->hit_entity.value : 0;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85B2D0
bool slf__line_info__did_collide__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (line_info*)stack.local_uint(0x00);

    f32 result = self && self->collision ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__line_info__debug_render__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}
