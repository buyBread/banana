#include "treyarch/game/als/animation_event.hh"

using namespace treyarch;

// sub_A944D0
u32 als::animation_event::get_param_type() const {
    return 0;
}

// sub_720060
f32 als::animation_event::get_float_param() const {
    return -1.0f;
}

// sub_71FB70
i32 als::animation_event::get_int_param() const {
    return -1;
}

// sub_7198C0
string_hash als::animation_event::get_hash_param() const {
    string_hash none;
    none.initialize(mash::ALLOCATED);

    return none;
}

// sub_5AF170
u32 als::animation_event_with_param::get_param_type() const {
    return param_type;
}

// sub_A6FDB0
f32 als::animation_event_with_param::get_float_param() const {
    return *(f32*)&param;
}

// sub_AD5300
i32 als::animation_event_with_param::get_int_param() const {
    return param;
}

// sub_5AF180
string_hash als::animation_event_with_param::get_hash_param() const {
    string_hash value;
    value.initialize(mash::ALLOCATED, nullptr, param);

    return value;
}
