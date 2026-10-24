#include "retail.hh"
#include "treyarch/game/event/anim_event.hh"

using namespace treyarch;

anim_event::anim_event(string_hash event_type_id, string_hash bone, u32 num_params, f32 unk_1c) {
    using constructor = anim_event* (__thiscall*)(anim_event*, string_hash, string_hash, u32, f32);

    ((constructor)retail::sub_5AD0D0)(this, event_type_id, bone, num_params, unk_1c);
}

anim_event::~anim_event() {
    retail::sub_5AD190((u32*)this);
}
