#include "retail.hh"
#include "treyarch/game/wds/entity/interface/player_reflexes.hh"

using namespace treyarch;

void player_reflexes::disable(bool force) {
    retail::sub_7EFF80((u8*)this, force);
}

void player_spider_senses::disable(bool unk, bool force) {
    retail::sub_7EA840((i32)this, unk, force);
}
