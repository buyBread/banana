#include <ctime>

#include "treyarch/nfl/nfl.hh"
#include "treyarch/nfl/system.hh"

using namespace treyarch;

// sub_A15F20
bool treyarch::nfl::nfs_validate_destination(u8* destination) {
    *destination = 0;

    return true;
}

// sub_A15EF0
i32 treyarch::nfl::nfs_allocate_decompress_batch_job() {
    for (nfs_decompress_batch_group &group : references::nfs_decompress_batch_groups.get()) {
        if (!group.locked) {
            group.locked = 1;

            return group.decompress_group_handle;
        }
    }

    return -1;
}

// sub_A15E60
void treyarch::nfl::nfs_cancel_request(i32 id) {
    nfs_request* request = nfs_get_request(id);

    if (!request)
        return;

    const i32 state = request->state;

    // anything with I/O or decompression in flight has to be stopped by the driver first
    if (state == nfs_request_state_working || request->active_decompress_jobs) {
        request->state = nfs_request_state_canceling;

        SetEvent(references::cancel_event.read());
    } else if (state == nfs_request_state_io_error_waiting ||
               state == nfs_request_state_io_error ||
              (state != nfs_request_state_canceling && state != nfs_request_state_workdone))

        request->state = nfs_request_state_canceled;
}

// sub_A15F90
i32 treyarch::nfl::add_request(const request_params* params) {
    if (!nfs_validate_destination(params->destination))
        return -1;

    if (!nfs_get_file(params->file_id))
        return -1;

    nfs_lock();

    const i32 id = tx::slot_new(&references::nfs_requests_pool.get());

    if (id == -1) {
        nfs_unlock();

        return -1;
    }

    nfs_request* request = nfs_get_request(id);
    nfs_file*    file    = nfs_get_file(nfs_get_native_file_id(params->file_id));

    request->type     = params->type;
    request->file     = params->file_id;
    request->driver   = file ? file->as.native.driver : nullptr;
    request->priority = params->priority;
    request->callback = params->callback;
    request->user     = params->user;

    const clock_t now = clock();

    request->start_time = now;
    request->deadline   = params->timeout ? now + 1000 * params->timeout / 1000 : 0x7FFFFFFF;

    request->age                    = now;
    request->state                  = nfs_request_state_waiting;
    request->destination            = params->destination;
    request->decompress             = params->decompress;
    request->output_size            = params->output_size;
    request->disk_size              = params->read_size;
    request->last_progress_time     = now + 60000;
    request->offset                 = params->offset;
    request->bytes_read             = 0;
    request->bytes_decompressed     = 0;
    request->bytes_scheduled        = 0;
    request->active_decompress_jobs = 0;
    request->last_bytes_read        = 0;

    *request->destination = 0;

    request->read_buffers_locked[0] = 0;
    request->read_buffers_locked[1] = 0;
    request->read_buffer_slot       = -1;
    request->group_handle           = params->decompress ? nfs_allocate_decompress_batch_job() : -1;

    if (references::nfs_init_params.get().thread_mode == 1) {
        ReleaseMutex(references::nfs_mutex.read());

        // retail checks the mode a second time
        if (references::nfs_init_params.get().thread_mode == 1)
            SetEvent(references::nfs_event.read());
    }

    return id;
}

// sub_A16260
void treyarch::nfl::cancel_request(i32 id) {
    nfs_lock();

    nfs_cancel_request(id);

    nfs_unlock();
}

// sub_A164E0
i32 treyarch::nfl::nfs_schedule_request(nfd_driver* driver) {
    u32 best_priority = 0;
    u32 best_age      = 0xFFFFFFFF;
    i32 best_id       = -1;

    tx::slot_pool* requests_pool = &references::nfs_requests_pool.get();

    for (i32 next_id = tx::slot_first(requests_pool); next_id != -1;) {
        const i32    id      = next_id;
        nfs_request* request = nfs_get_request(id);

        next_id = tx::slot_next(requests_pool, id);

        if (!request->driver && !nfs_open_file_async(id)) {
            nfs_lock();

            nfs_cancel_request(id);

            nfs_unlock();

            continue;
        }

        if (request->state != nfs_request_state_waiting || request->bytes_read == request->disk_size)
            continue;

        if (driver && nfs_get_file_driver(request->file) != driver)
            continue;

        const u32 priority = (u32)request->priority;
        const u32 age      = (u32)request->age;

        if (priority > best_priority) {
            best_priority = priority;
            best_age      = age;
            best_id       = id;
        } else if (priority == best_priority && age < best_age) {
            best_age = age;
            best_id  = id;
        }
    }

    return best_id;
}

// sub_A166F0
nfl::e_nfd_error treyarch::nfl::nfs_execute_request(nfd_driver* driver, i32 id) {
    if (driver->io_state != nfd_io_state_idle)
        return nfd_error_invalid_state;

    nfs_request* request = nfs_get_request(id);

    nfd_lock_read_buffer(driver, id, nfd_find_unlocked_read_buffer(driver));

    nfs_file* file = nfs_get_file(request->file);

    nfd_io_command command;

    command.offset           = request->bytes_read + request->offset;
    command.type             = request->type;
    command.read_buffer_slot = request->read_buffer_slot;
    command.request          = id;
    command.file_handle      = nfs_get_file_handle(request->file);
    command.file_size        = file->size;

    if (request->type) {
        command.buffer = request->destination;
        command.size   = request->output_size;
    } else {
        const nfd_buffer* buffers = driver->buffers_object;

        command.buffer = buffers->buffers[command.read_buffer_slot].data;
        command.size   = request->disk_size - request->bytes_read;

        if (command.size > buffers->buffer_size)
            command.size = buffers->buffer_size;
    }

    if (file->file_type == nfs_file_type_subfile) {
        u32 chunk_size  = file->as.subfile.chunk_size;
        u32 stride_size = file->as.subfile.stride_size;

        if (!chunk_size && !stride_size) {
            stride_size = 0x40000000;
            chunk_size  = 0x40000000;
        }

        const u32 chunk_offset = request->bytes_read % chunk_size;

        if (command.size > chunk_size - chunk_offset)
            command.size = chunk_size - chunk_offset;

        command.offset      = chunk_offset + file->as.subfile.offset + stride_size * (request->bytes_read / chunk_size);
        command.file_handle = nfs_get_file_handle(file->as.subfile.parent);
    }

    request->state = nfs_request_state_working;

    // the one point in a pass where the main thread can get in
    nfs_unlock();

    const e_nfd_error error = nfd_io_execute(driver, &command);

    nfs_lock();

    request = nfs_get_request(id);

    if (request && request->state == nfs_request_state_working && error)
        request->state = nfs_request_state_io_error;

    return error;
}
