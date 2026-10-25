#include "retail.hh"
#include "treyarch/game/event/default_callbacks.hh"
#include "treyarch/game/event/event_manager.hh"

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
