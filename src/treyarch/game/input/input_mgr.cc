#include "treyarch/game/input/input_mgr.hh"

using namespace treyarch;

void input_mgr::poll_devices() {
    for (i32 index = 0; index != max_devices; ++index) {
        input_device* device = devices[index];

        if (device)
            device->poll();
    }
}
