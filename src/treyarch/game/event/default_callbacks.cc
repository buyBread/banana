#include "retail.hh"
#include "treyarch/game/event/default_callbacks.hh"
#include "treyarch/game/event/event_manager.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace references {
        // static event-type ids, hashed from their names by the CRT initializers
        util::memory_reference<string_hash> collide_ragdoll     { 0x0102CB84 };
        util::memory_reference<string_hash> footstep_l          { 0x0102C5E4 };
        util::memory_reference<string_hash> footstep_r          { 0x0102C6C8 };
        util::memory_reference<string_hash> handplant_l         { 0x0102CABC };
        util::memory_reference<string_hash> handplant_r         { 0x0102C808 };
        util::memory_reference<string_hash> play_sound          { 0x0102CB58 };
        util::memory_reference<string_hash> play_sound_on_trans { 0x0102CB64 };
        util::memory_reference<string_hash> play_effect         { 0x0102C654 };
        util::memory_reference<string_hash> trail_1             { 0x0102C6D4 };
        util::memory_reference<string_hash> trail_2             { 0x0102C684 };
        util::memory_reference<string_hash> trail_3             { 0x0102C658 };
        util::memory_reference<string_hash> trail_4             { 0x0102C754 };
        util::memory_reference<string_hash> tentacle_1          { 0x0102C288 };
        util::memory_reference<string_hash> tentacle_2          { 0x0102C390 };
        util::memory_reference<string_hash> tentacle_3          { 0x0102C2B0 };
        util::memory_reference<string_hash> tentacle_4          { 0x0102C720 };
        util::memory_reference<string_hash> tentacle_5          { 0x0102C5E0 };
        util::memory_reference<string_hash> tentacle_6          { 0x0102CAC0 };
        util::memory_reference<string_hash> tentacle_7          { 0x0102C38C };
        util::memory_reference<string_hash> tentacle_8          { 0x0102C350 };
        util::memory_reference<string_hash> tentacle_9          { 0x0102C284 };
        util::memory_reference<string_hash> tentacle_10         { 0x0102CCB4 };
        util::memory_reference<string_hash> tentacle_11         { 0x0102C648 };
        util::memory_reference<string_hash> tentacle_12         { 0x0102C280 };
        util::memory_reference<string_hash> tentacle_13         { 0x0102C5D8 };
        util::memory_reference<string_hash> tentacle_14         { 0x0102C5D0 };
        util::memory_reference<string_hash> tentacle_15         { 0x0102C4EC };
        util::memory_reference<string_hash> tentacle_16         { 0x0102C444 };
        util::memory_reference<string_hash> set_vulnerable      { 0x0102C6B0 };
        util::memory_reference<string_hash> in_limbo            { 0x0102CAAC };
        util::memory_reference<string_hash> particle_event      { 0x0102C7A4 };
        util::memory_reference<string_hash> hit_pause           { 0x0102CB78 };
}} // treyarch::references

using namespace treyarch;

// sub_667060; the handlers react to animation events, so they stay native
void treyarch::register_default_event_callbacks() {
    event_manager::add_default_callback(references::collide_ragdoll.read(),     (code_event_callback_function)retail::sub_666EB0, nullptr, false);
    event_manager::add_default_callback(references::footstep_l.read(),          (code_event_callback_function)retail::sub_661710, nullptr, false);
    event_manager::add_default_callback(references::footstep_r.read(),          (code_event_callback_function)retail::sub_661770, nullptr, false);
    event_manager::add_default_callback(references::handplant_l.read(),         (code_event_callback_function)retail::sub_6617D0, nullptr, false);
    event_manager::add_default_callback(references::handplant_r.read(),         (code_event_callback_function)retail::sub_6617D0, nullptr, false);
    event_manager::add_default_callback(references::play_sound.read(),          (code_event_callback_function)retail::sub_6618A0, nullptr, false);
    event_manager::add_default_callback(references::play_sound_on_trans.read(), (code_event_callback_function)retail::sub_612FC0, nullptr, false);
    event_manager::add_default_callback(references::play_effect.read(),         (code_event_callback_function)retail::sub_613030, nullptr, false);
    event_manager::add_default_callback(references::trail_1.read(),             (code_event_callback_function)retail::sub_661AA0, nullptr, false);
    event_manager::add_default_callback(references::trail_2.read(),             (code_event_callback_function)retail::sub_661AC0, nullptr, false);
    event_manager::add_default_callback(references::trail_3.read(),             (code_event_callback_function)retail::sub_661AE0, nullptr, false);
    event_manager::add_default_callback(references::trail_4.read(),             (code_event_callback_function)retail::sub_661B00, nullptr, false);
    event_manager::add_default_callback(references::tentacle_1.read(),          (code_event_callback_function)retail::sub_627520, nullptr, false);
    event_manager::add_default_callback(references::tentacle_2.read(),          (code_event_callback_function)retail::sub_627540, nullptr, false);
    event_manager::add_default_callback(references::tentacle_3.read(),          (code_event_callback_function)retail::sub_627560, nullptr, false);
    event_manager::add_default_callback(references::tentacle_4.read(),          (code_event_callback_function)retail::sub_627580, nullptr, false);
    event_manager::add_default_callback(references::tentacle_5.read(),          (code_event_callback_function)retail::sub_6275A0, nullptr, false);
    event_manager::add_default_callback(references::tentacle_6.read(),          (code_event_callback_function)retail::sub_6275C0, nullptr, false);
    event_manager::add_default_callback(references::tentacle_7.read(),          (code_event_callback_function)retail::sub_6275E0, nullptr, false);
    event_manager::add_default_callback(references::tentacle_8.read(),          (code_event_callback_function)retail::sub_627600, nullptr, false);
    event_manager::add_default_callback(references::tentacle_9.read(),          (code_event_callback_function)retail::sub_627620, nullptr, false);
    event_manager::add_default_callback(references::tentacle_10.read(),         (code_event_callback_function)retail::sub_627640, nullptr, false);
    event_manager::add_default_callback(references::tentacle_11.read(),         (code_event_callback_function)retail::sub_627660, nullptr, false);
    event_manager::add_default_callback(references::tentacle_12.read(),         (code_event_callback_function)retail::sub_627680, nullptr, false);
    event_manager::add_default_callback(references::tentacle_13.read(),         (code_event_callback_function)retail::sub_6276A0, nullptr, false);
    event_manager::add_default_callback(references::tentacle_14.read(),         (code_event_callback_function)retail::sub_6276C0, nullptr, false);
    event_manager::add_default_callback(references::tentacle_15.read(),         (code_event_callback_function)retail::sub_6276E0, nullptr, false);
    event_manager::add_default_callback(references::tentacle_16.read(),         (code_event_callback_function)retail::sub_627700, nullptr, false);
    event_manager::add_default_callback(references::set_vulnerable.read(),      (code_event_callback_function)retail::sub_6130A0, nullptr, false);
    event_manager::add_default_callback(references::in_limbo.read(),            (code_event_callback_function)retail::sub_640C90, nullptr, false);
    event_manager::add_default_callback(references::particle_event.read(),      (code_event_callback_function)retail::sub_6131F0, nullptr, false);
    event_manager::add_default_callback(references::hit_pause.read(),           (code_event_callback_function)retail::sub_6132B0, nullptr, false);
}
