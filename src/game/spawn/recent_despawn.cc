#include "game/spawn/recent_despawn.hh"

using namespace treyarch;

// sub_905DF0
recent_despawn::recent_despawn() {
    record = nullptr;
    item   = nullptr;
    age    = 0.0f;
    unk_0c = false;
    unk_0d = false;

    instance_name.initialize(mash::ALLOCATED);

    persistence = nullptr;
}
