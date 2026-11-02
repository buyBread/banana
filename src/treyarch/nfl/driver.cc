#include "treyarch/nfl/driver.hh"
#include "treyarch/nfl/nfl.hh"
#include "treyarch/base/tl_system.hh"

using namespace treyarch;

// sub_A179A0
void treyarch::nfl::nfd_io_complete(nfd_driver* driver, u32 bytes, i32 error) {
    switch (driver->io_state) {
        case nfd_io_state_working:
            if (error)
                driver->io_state = nfd_io_state_error;
            else {
                driver->disk_read.bytes_completed = bytes;
                driver->io_state                  = nfd_io_state_workdone;
            }

            break;

        case nfd_io_state_canceling:
            driver->io_state = nfd_io_state_canceled;

            break;

        default:
            tl_fatal("nfdIoCompleted: called in inappopriate state %s", 0);
    }
}

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
