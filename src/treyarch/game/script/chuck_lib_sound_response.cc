#include "retail.hh"
#include "treyarch/game/script/chuck_lib_sound_response.hh"

using namespace treyarch;

bool slf__sound_post_event__entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8656E0)(this, stack);
}

bool slf__sound_post_event__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8657B0)(this, stack);
}

bool slf__sound_post_event__str__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865830)(this, stack);
}

bool slf__sound_set_chase_distance__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8658B0)(this, stack);
}

bool slf__sound_set_countdown_time__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8658D0)(this, stack);
}

bool slf__sound_set_music_intensity_bias__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8658F0)(this, stack);
}

bool slf__sound_set_music_intensity_gain__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865910)(this, stack);
}

bool slf__sound_set_music_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865930)(this, stack);
}

bool slf__sound_set_fixed_mic_zoom__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865970)(this, stack);
}

bool slf__sound_set_dynamic_mic_zoom__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865990)(this, stack);
}

bool slf__sound_get_music_mode__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F5E0)(this, stack);
}

bool slf__alloc_modulator__entity__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8659B0)(this, stack);
}

bool slf__free_modulator__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865A10)(this, stack);
}

bool slf__set_modulator_period__uint__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865A30)(this, stack);
}

bool slf__set_modulator_waveform__uint__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865A50)(this, stack);
}

bool slf__sound_rtpc__entity__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865A90)(this, stack);
}

bool slf__sound_rtpc__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865B70)(this, stack);
}

bool slf__sound_switch__entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865BD0)(this, stack);
}

bool slf__sound_switch__str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865CC0)(this, stack);
}

bool slf__sound_state__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865D60)(this, stack);
}

bool slf__sound_emitter_on__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865DE0)(this, stack);
}

bool slf__sound_emitter_off__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865E00)(this, stack);
}

bool slf__sound_occlusion_enable__bool__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865E20)(this, stack);
}

bool slf__create_sound_response__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865E60)(this, stack);
}

bool slf__create_sound_response__entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865F10)(this, stack);
}

bool slf__create_sound_response__str__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866030)(this, stack);
}

bool slf__destroy_sound_response__sound_response__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8660F0)(this, stack);
}

bool slf__load_bank__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8662D0)(this, stack);
}

bool slf__unload_bank__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866320)(this, stack);
}

bool slf__is_bank_loader_idle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F600)(this, stack);
}

bool slf__is_bank_loaded__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866370)(this, stack);
}

bool slf__sound_response__post__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866150)(this, stack);
}

bool slf__sound_response__wait__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866170)(this, stack);
}

bool slf__sound_response__is_active__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8661A0)(this, stack);
}

bool slf__sound_response__set_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866200)(this, stack);
}
