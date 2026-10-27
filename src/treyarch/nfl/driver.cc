#include "treyarch/nfl/driver.hh"
#include "treyarch/nfl/nfl.hh"

using namespace treyarch;

// sub_A16A60
nfl::e_state treyarch::nfl::get_state() {
    // the worst state across the drivers, invalid with none
    e_state state = state_invalid;

    for (i32 index = 0; index < references::nfs_drivers_count.read(); ++index) {
        switch (references::nfs_drivers.read()[index]->io_state) {
            case nfd_io_state_idle:
            case nfd_io_state_workdone:
            case nfd_io_state_canceled:
                if (state == state_invalid)
                    state = state_idle;

                break;

            case nfd_io_state_working:
            case nfd_io_state_canceling:
                if (state == state_invalid || state == state_idle)
                    state = state_busy;

                break;

            case nfd_io_state_error:
                state = state_error;

                break;
        }
    }

    return state;
}
