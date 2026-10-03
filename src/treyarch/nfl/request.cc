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
    for (decompress_group &group : references::decompress_groups.get()) {
        if (!group.in_use) {
            group.in_use = 1;

            return group.group;
        }
    }

    return -1;
}

// sub_A15E60
void treyarch::nfl::nfs_cancel_request(i32 id) {
    request* request = get_request(id);

    if (!request)
        return;

    const i32 state = request->state;

    // anything with I/O or decompression in flight has to be stopped by the driver first
    if (state == internal_request_state_working || request->chunk_jobs) {
        request->state = internal_request_state_canceling;

        SetEvent(references::cancel_event.read());
    } else if (state == internal_request_state_io_error_waiting ||
               state == internal_request_state_io_error ||
              (state != internal_request_state_canceling && state != internal_request_state_workdone))

        request->state = internal_request_state_canceled;
}

// sub_A15F90
i32 treyarch::nfl::add_request(const request_params* params) {
    if (!nfs_validate_destination(params->destination))
        return -1;

    if (!get_file(params->file))
        return -1;

    lock();

    const i32 id = references::request_pool.get().allocate();

    if (id == -1) {
        unlock();

        return -1;
    }

    request* request = get_request(id);
    file*    file    = get_file(resolve_file(params->file));

    request->type     = params->type;
    request->file     = params->file;
    request->driver   = file ? file->driver : nullptr;
    request->priority = params->priority;
    request->callback = params->callback;
    request->user     = params->user;

    const clock_t now = clock();

    request->start_time = now;
    request->deadline   = params->timeout ? now + 1000 * params->timeout / 1000 : 0x7FFFFFFF;

    request->age                = now;
    request->state              = internal_request_state_waiting;
    request->destination        = params->destination;
    request->decompress         = params->decompress;
    request->output_size        = params->output_size;
    request->read_size          = params->read_size;
    request->last_progress_time = now + 60000;
    request->offset             = params->offset;
    request->bytes_read         = 0;
    request->bytes_decompressed = 0;
    request->bytes_scheduled    = 0;
    request->chunk_jobs         = 0;
    request->last_bytes_read    = 0;

    *request->destination = 0;

    request->holds_buffer[0]  = 0;
    request->holds_buffer[1]  = 0;
    request->buffer           = -1;
    request->decompress_group = params->decompress ? nfs_allocate_decompress_batch_job() : -1;

    if (references::params.get().thread_mode == 1) {
        ReleaseMutex(references::mutex.read());

        // retail checks the mode a second time
        if (references::params.get().thread_mode == 1)
            SetEvent(references::work_event.read());
    }

    return id;
}
