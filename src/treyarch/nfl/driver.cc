#include "treyarch/nfl/driver.hh"
#include "treyarch/nfl/nfl.hh"
#include "treyarch/nfl/system.hh"
#include "treyarch/base/tl_system.hh"

using namespace treyarch;

// inlined @ sub_A166F0
// inlined @ sub_A171A0
i32 treyarch::nfl::nfd_find_unlocked_read_buffer(const nfd_driver* driver) {
    for (i32 slot = 0; slot < 2; ++slot) {
        if (driver->buffers_object->buffers[slot].locked_by == -1)
            return slot;
    }

    return -1;
}

// sub_A162A0
void treyarch::nfl::nfd_lock_read_buffer(nfd_driver* driver, i32 id, i32 slot) {
    driver->buffers_object->buffers[slot].locked_by = id;

    nfs_request* request = nfs_get_request(id);

    request->read_buffer_slot          = slot;
    request->read_buffers_locked[slot] = 1;
}

// sub_A16310
void treyarch::nfl::nfd_unlock_read_buffer(nfd_driver* driver, i32 id, i32 slot) {
    driver->buffers_object->buffers[slot].locked_by = -1;

    nfs_request* request = nfs_get_request(id);

    if (request->read_buffer_slot == slot)
        request->read_buffer_slot = -1;

    request->read_buffers_locked[slot] = 0;
}

// sub_A179F0
nfl::e_nfd_error treyarch::nfl::nfd_io_execute(nfd_driver* driver, const nfd_io_command* command) {
    if (command->type != 0 && command->type != 1)
        return nfd_error_invalid_arguments;

    if (!command->type) {
        if (!command->size) {
            driver->disk_read.bytes_completed = 0;
            driver->disk_read.io_command      = *command;
            driver->io_state                  = nfd_io_state_workdone;

            return nfd_error_noerror;
        }

        const i32 buffer_mode = driver->init->buffer_mode;

        switch (buffer_mode) {
            case buffer_mode_nobuffer:
            case buffer_mode_unaligned:
            case buffer_mode_vm: {
                const nfd_buffer* buffers = driver->buffers_object;
                const u32         address = (u32)command->buffer;

                const bool misaligned =
                    address         != ((buffers->address_alignment + address         - 1) & ~(buffers->address_alignment - 1)) ||
                    command->size   != ((buffers->size_alignment    + command->size   - 1) & ~(buffers->size_alignment    - 1)) ||
                    command->offset != ((buffers->offset_alignment  + command->offset - 1) & ~(buffers->offset_alignment  - 1));

                if (buffer_mode == buffer_mode_nobuffer && misaligned)
                    return nfd_error_unaligned_access;

                break;
            }

            case buffer_mode_all:
                break;

            default:
                tl_fatal("Invalid nflInitParams.bufferMode specified\n");

                return nfd_error_unaligned_access;
        }
    }

    driver->disk_read.aligned_offset = command->offset;
    driver->disk_read.aligned_size   = command->size;

    const e_nfd_error error = driver->io_operations->execute(driver,
                                                             command->file_handle,
                                                             command->type,
                                                             command->offset,
                                                             command->buffer,
                                                             command->size);

    if (!error) {
        driver->disk_read.io_command = *command;
        driver->io_state             = nfd_io_state_working;
    }

    return error;
}

// sub_A179A0
void treyarch::nfl::nfd_io_complete(nfd_driver* driver, u32 bytes, i32 error) {
    switch (driver->io_state) {
        case nfd_io_state_working:
            if (error) {
                nfl::log.err("request {} \"{}\" read failed -- offset 0x{:X}, size 0x{:X}, error code {}",
                             driver->disk_read.io_command.request,
                             nfs_request_path(driver->disk_read.io_command.request),
                             driver->disk_read.io_command.offset,
                             driver->disk_read.io_command.size,
                             error);

                driver->io_state = nfd_io_state_error;
            } else {
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
