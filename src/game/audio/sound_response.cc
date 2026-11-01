#include "retail.hh"
#include "game/audio/sound_response.hh"

using namespace treyarch;

void sound_response::release(bool clear_event) {
    retail::sub_8FFA10(this, clear_event);
}
