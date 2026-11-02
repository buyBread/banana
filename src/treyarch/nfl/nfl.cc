#include <cstring>
#include <ctime>
#include <lzo/lzo1x.h>

#include "retail.hh"
#include "treyarch/nfl/nfl.hh"
#include "treyarch/nfl/system.hh"

using namespace treyarch;

// sub_A15470
u32 treyarch::nfl::init(const init_params* params) {
    system::platform_settings = &system::default_platform_settings;

    if (params)
        system::init_params = *params;

    for (i32 index = 0; index < system::drivers_count; ++index) {
        driver::driver_t* driver = system::drivers[index];

        if (system::init_params.buffer_mode != -1)
            driver->init->buffer_mode = system::init_params.buffer_mode;

        if (driver->init->initialize)
            driver->init->initialize(driver);

        nfl::log.msg("driver #{}: {} | media mask: 0x{:X} | buffer mode: {}",
                     index,
                     driver->name,
                     driver->binding->media_mask,
                     driver->init->buffer_mode);
    }

    for (system::decompress_batch_group_t &group : system::decompress_batch_groups) {
        group.decompress_group_handle = retail::sub_A144D0(); // jq_alloc_batch_group
        group.locked                  = 0;
    }

    // with no work space this only measures it
    const u32 work_space_size = system::pre_allocate_work_space(nullptr);

    nfl::log.msg("files: {} | requests: {} | chunk jobs: {} | buffer mode: {} | thread mode: {} | work space: 0x{:X} bytes",
                 system::init_params.file_count,
                 system::init_params.request_count,
                 system::init_params.chunk_decompress_job_count,
                 system::init_params.buffer_mode,
                 system::init_params.thread_mode,
                 work_space_size);

    return work_space_size;
}

// sub_A17830
void treyarch::nfl::start(void* work_space) {
    if (!work_space)
        return;

    system::pre_allocate_work_space((u8*)work_space);

    tx::slot_pool_init(&system::requests_pool,
                       &system::requests->entry,
                       system::init_params.request_count,
                       sizeof(system::request_t));

    tx::slot_pool_init(&system::chunk_decompress_job_pool,
                       &system::chunk_decompress_jobs->entry,
                       system::init_params.chunk_decompress_job_count,
                       sizeof(system::chunk_decompress_job));

    tx::slot_pool_init(&system::files_pool,
                       &system::files->entry,
                       system::init_params.file_count,
                       sizeof(system::file_t));

    const i32 lzo_result = lzo_init();

    if (lzo_result != LZO_E_OK)
        nfl::log.err("lzo_init failed -- error {}", lzo_result);

    nfl::log.msg("read buffers: 2 x 0x{:X} | lzo {} | i/o thread priority: {}",
                 system::platform_settings->buffer_size,
                 lzo_version_string(),
                 system::thread_priority);

    if (system::init_params.thread_mode != 1)
        return;

    system::mutex        = CreateMutexA(nullptr, FALSE, nullptr);
    system::event        = CreateEventA(nullptr, FALSE, FALSE, nullptr);
    system::cancel_event = CreateEventA(nullptr, FALSE, FALSE, nullptr);

    // retail checks the mode a second time
    if (system::init_params.thread_mode != 1)
        return;

    system::thread = CreateThread(nullptr,
                                  0x2000,
                                  system::update_thread,
                                  nullptr,
                                  CREATE_SUSPENDED,
                                  nullptr);

    if (!system::thread)
        nfl::log.err("failed to create the i/o thread -- win32 error {}", GetLastError());

    SetThreadPriority(system::thread, system::thread_priority);
    ResumeThread(system::thread);
}

// sub_A173E0
void treyarch::nfl::update() {
    system::call_requests_count = 0;

    system::lock();

    for (i32 id = tx::slot_first(&system::requests_pool); id != -1;) {
        system::request_t* request = system::get_request(id);

        id = tx::slot_next(&system::requests_pool, id);

        const clock_t now = clock();

        switch (request->state) {
            case system::request_state_waiting:
                // a stall watchdog nothing reads
                if (request->bytes_read != request->last_bytes_read) {
                    request->last_progress_time = now;
                    request->last_bytes_read    = request->bytes_read;
                }

                clock();

                break;

            case system::request_state_workdone:
            case system::request_state_canceled: {
                system::request_cb_t &call_request = system::call_requests[system::call_requests_count++];

                call_request.state    = system::export_request_state(request->state);
                call_request.request  = request->entry.id;
                call_request.callback = request->callback;
                call_request.release  = 1;
                call_request.user     = request->user;

                const i32 elapsed = now - request->start_time;

                if (request->state == system::request_state_canceled)
                    nfl::log.dbg("canceled {} \"{}\" after {} ms", request->entry.id, system::file_path(request->file), elapsed);
                else if (elapsed > 10000)
                    nfl::log.wrn("slow request {} \"{}\" -- 0x{:X} bytes in {} ms",
                                 request->entry.id,
                                 system::file_path(request->file),
                                 request->output_size,
                                 elapsed);
                else
                    nfl::log.dbg("complete {} \"{}\" -- 0x{:X} bytes in {} ms",
                                 request->entry.id,
                                 system::file_path(request->file),
                                 request->output_size,
                                 elapsed);

                break;
            }

            case system::request_state_io_error:
                // the request stays allocated until it's canceled or its file closes
                request->state = system::request_state_io_error_waiting;

                nfl::log.err("request {} \"{}\" failed -- 0x{:X} of 0x{:X} bytes read{}",
                             request->entry.id,
                             system::file_path(request->file),
                             request->bytes_read,
                             request->disk_size,
                             request->driver->io_state == driver::io_state_error ? ", streaming stopped until it's canceled" : "");

                if (request->callback) {
                    system::request_cb_t &call_request = system::call_requests[system::call_requests_count++];

                    call_request.state    = system::export_request_state(request->state);
                    call_request.request  = request->entry.id;
                    call_request.callback = request->callback;
                    call_request.release  = 0;
                    call_request.user     = request->user;
                }

                break;
        }
    }

    if (system::call_requests_count) {
        system::unlock();

        for (i32 index = 0; index < system::call_requests_count; ++index) {
            system::request_cb_t &call_request = system::call_requests[index];

            if (call_request.callback)
                call_request.callback(call_request.state, call_request.request, call_request.user);
        }

        system::lock();
    }

    for (i32 index = 0; index < system::call_requests_count; ++index) {
        system::request_cb_t &call_request = system::call_requests[index];

        if (!call_request.release)
            continue;

        system::request_t* request = system::get_request(call_request.request);

        if (request->group_handle != -1) {
            for (system::decompress_batch_group_t &group : system::decompress_batch_groups) {
                if (group.decompress_group_handle == request->group_handle) {
                    group.locked = 0;

                    break;
                }
            }
        }

        system::file_t* file = system::get_file(request->file);

        if (file && file->as.native.flags & driver::file_flag_deferred_close && !--file->as.native.pending_requests)
            driver::close_file(request->file);

        tx::slot_free(&system::requests_pool, call_request.request);
    }

    system::call_requests_count = 0;

    system::unlock();

    if (!system::init_params.thread_mode)
        system::update();
}

// sub_A15CE0
i32 treyarch::nfl::open_file(u32 media_mask, const char* path) {
    return system::open_file(media_mask, path, driver::file_flag_read, nullptr);
}

// sub_A16CF0
void treyarch::nfl::close_file(i32 id) {
    system::close_file(id, true);
}

// sub_A15F90
i32 treyarch::nfl::add_request(const request_params* params) {
    if (!system::validate_destination(params->destination))
        return -1;

    if (!system::get_file(params->file_id)) {
        nfl::log.err("request for a dead file {} -- offset 0x{:X}, read 0x{:X}",
                     params->file_id,
                     params->offset,
                     params->read_size);

        return -1;
    }

    system::lock();

    const i32 id = tx::slot_new(&system::requests_pool);

    if (id == -1) {
        system::unlock();

        nfl::log.err("out of request slots ({}) -- \"{}\"",
                     system::requests_pool.count,
                     system::file_path(params->file_id));

        return -1;
    }

    system::request_t* request = system::get_request(id);
    system::file_t*    file    = system::get_file(system::get_native_file_id(params->file_id));

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
    request->state                  = system::request_state_waiting;
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
    request->group_handle           = params->decompress ? system::allocate_decompress_batch_job() : -1;

    if (params->decompress && request->group_handle == -1)
        nfl::log.wrn("request {} \"{}\" -- no free decompress group, it waits on the global batch counters",
                     id,
                     system::file_path(params->file_id));

    if (system::init_params.thread_mode == 1) {
        ReleaseMutex(system::mutex);

        // retail checks the mode a second time
        if (system::init_params.thread_mode == 1)
            SetEvent(system::event);
    }

    nfl::log.dbg("add {} \"{}\" -- offset 0x{:X}, read 0x{:X}, output 0x{:X}, priority {}, decompress {}",
                 id,
                 system::file_path(params->file_id),
                 params->offset,
                 params->read_size,
                 params->output_size,
                 params->priority,
                 params->decompress);

    return id;
}

// sub_A16260
void treyarch::nfl::cancel_request(i32 id) {
    system::lock();

    system::cancel_request(id);

    system::unlock();
}

// sub_A16A60
nfl::e_state treyarch::nfl::get_state() {
    // the worst state across the drivers, invalid with none
    e_state state = state_invalid;

    for (i32 index = 0; index < system::drivers_count; ++index) {
        switch (system::drivers[index]->io_state) {
            case driver::io_state_idle:
            case driver::io_state_workdone:
            case driver::io_state_canceled:
                if (state == state_invalid)
                    state = state_idle;

                break;

            case driver::io_state_working:
            case driver::io_state_canceling:
                if (state == state_invalid || state == state_idle)
                    state = state_busy;

                break;

            case driver::io_state_error:
                state = state_error;

                break;
        }
    }

    return state;
}

// sub_A17B80
void treyarch::nfl::nfl_jq_decompress_module(jq::batch* batch) { // runs on the job queue workers with no lock held
    auto* record = (system::chunk_header*)batch->payload[0];

    const u8* payload = (u8*)record + record->span - record->stored_size;

    if (record->stored_size == record->output_size)
        std::memcpy(batch->payload[1], payload, record->stored_size);
    else {
        lzo_uint out_len = record->output_size;

        const i32 result = lzo1x_decompress(payload, record->stored_size, (u8*)batch->payload[1], &out_len, nullptr);

        if (result != LZO_E_OK)
            nfl::log.err("lzo decompression failed -- error {}, record {}, output {}, stored 0x{:X}, expected 0x{:X}, got 0x{:X}",
                         result,
                         (void*)record,
                         batch->payload[1],
                         record->stored_size,
                         record->output_size,
                         out_len);
        else if (out_len != record->output_size)
            nfl::log.err("lzo output size mismatch -- record {}, output {}, expected 0x{:X}, got 0x{:X}",
                         (void*)record,
                         batch->payload[1],
                         record->output_size,
                         out_len);
    }
}
