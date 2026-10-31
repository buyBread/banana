#include "retail.hh"
#include "game/wds/entity/interface/weapon_interface_extender.hh"

using namespace treyarch;

entity* weapon_interface_extender::get_holder() const {
    return (entity*)retail::sub_4E22C0((u32*)this);
}
