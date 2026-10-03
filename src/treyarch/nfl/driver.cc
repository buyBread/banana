#include "treyarch/nfl/driver.hh"
#include "treyarch/nfl/nfl.hh"

using namespace treyarch;

// sub_A16A60
i32 treyarch::nfl::media_status() {
    // -1 with no drivers, otherwise the worst of 0 idle, 1 busy and 2 error
    i32 status = -1;

    for (i32 index = 0; index < references::driver_count.read(); ++index) {
        switch (references::drivers.read()[index]->io_state) {
            case 0:
            case 2:
            case 4:
                if (status == -1)
                    status = 0;

                break;

            case 1:
            case 3:
                if (status == -1 || status == 0)
                    status = 1;

                break;

            case 5:
                status = 2;

                break;
        }
    }

    return status;
}
