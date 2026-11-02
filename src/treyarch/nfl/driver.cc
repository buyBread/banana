#include "treyarch/nfl/driver.hh"
#include "treyarch/nfl/nfl.hh"
#include "treyarch/nfl/system.hh"
#include "treyarch/base/tl_system.hh"

using namespace treyarch;

// inlined @ sub_A166F0
// inlined @ sub_A171A0
i32 treyarch::nfl::driver::find_unlocked_read_buffer(const driver_t* driver) {
    for (i32 slot = 0; slot < 2; ++slot) {
        if (driver->buffers_object->buffers[slot].locked_by == -1)
            return slot;
    }

    return -1;
}

// sub_A162A0
void treyarch::nfl::driver::lock_read_buffer(driver_t* driver, i32 id, i32 slot) {
    driver->buffers_object->buffers[slot].locked_by = id;

    system::request_t* request = system::get_request(id);

    request->read_buffer_slot          = slot;
    request->read_buffers_locked[slot] = 1;
}

// sub_A16310
void treyarch::nfl::driver::unlock_read_buffer(driver_t* driver, i32 id, i32 slot) {
    driver->buffers_object->buffers[slot].locked_by = -1;

    system::request_t* request = system::get_request(id);

    if (request->read_buffer_slot == slot)
        request->read_buffer_slot = -1;

    request->read_buffers_locked[slot] = 0;
}

// sub_A179F0
nfl::driver::e_error treyarch::nfl::driver::io_execute(driver_t* driver, const io_command_t* command) {
    if (command->type != 0 && command->type != 1)
        return error_invalid_arguments;

    if (!command->type) {
        if (!command->size) {
            driver->disk_read.bytes_completed = 0;
            driver->disk_read.io_command      = *command;
            driver->io_state                  = io_state_workdone;

            return error_noerror;
        }

        const i32 buffer_mode = driver->init->buffer_mode;

        switch (buffer_mode) {
            case buffer_mode_nobuffer:
            case buffer_mode_unaligned:
            case buffer_mode_vm: {
                const buffer_t* buffers = driver->buffers_object;
                const u32       address = (u32)command->buffer;

                const bool misaligned =
                    address         != ((buffers->address_alignment + address         - 1) & ~(buffers->address_alignment - 1)) ||
                    command->size   != ((buffers->size_alignment    + command->size   - 1) & ~(buffers->size_alignment    - 1)) ||
                    command->offset != ((buffers->offset_alignment  + command->offset - 1) & ~(buffers->offset_alignment  - 1));

                if (buffer_mode == buffer_mode_nobuffer && misaligned)
                    return error_unaligned_access;

                break;
            }

            case buffer_mode_all:
                break;

            default:
                tl_fatal("Invalid nflInitParams.bufferMode specified\n");

                return error_unaligned_access;
        }
    }

    driver->disk_read.aligned_offset = command->offset;
    driver->disk_read.aligned_size   = command->size;

    const e_error error = driver->io_operations->execute(driver,
                                                         command->file_handle,
                                                         command->type,
                                                         command->offset,
                                                         command->buffer,
                                                         command->size);

    if (!error) {
        driver->disk_read.io_command = *command;
        driver->io_state             = io_state_working;
    }

    return error;
}

// sub_A179A0
void treyarch::nfl::driver::io_complete(driver_t* driver, u32 bytes, i32 error) {
    switch (driver->io_state) {
        case io_state_working:
            if (error) {
                nfl::log.err("request {} \"{}\" read failed -- offset 0x{:X}, size 0x{:X}, error code {}",
                             driver->disk_read.io_command.request,
                             system::request_path(driver->disk_read.io_command.request),
                             driver->disk_read.io_command.offset,
                             driver->disk_read.io_command.size,
                             error);

                driver->io_state = io_state_error;
            } else {
                driver->disk_read.bytes_completed = bytes;
                driver->io_state                  = io_state_workdone;
            }

            break;

        case io_state_canceling:
            driver->io_state = io_state_canceled;

            break;

        default:
            tl_fatal("nfdIoCompleted: called in inappopriate state %s", 0);
    }
}

// sub_A15D00
void treyarch::nfl::driver::close_file(i32 id) {
    system::file_t* file = system::get_file(id);

    if (!file || !file->as.native.driver)
        return;

    driver_t* driver = file->as.native.driver;
    u8*       handle = system::get_file_handle(id);

    if (handle)
        driver->file_operations->close(handle);

    tx::slot_free(&system::references::files_pool.get(), id);
}
