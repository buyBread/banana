#include "retail.hh"
#include "game/float_spring.hh"

using namespace treyarch;

f32 float_spring::update(f32 dt, f32 new_target) {
    return ((f32 (__thiscall*)(float_spring*, f32, f32))retail::sub_7E9B80)(this, dt, new_target);
}
