#include <cstring>
#include <ctime>
#include <lzo/lzo1x.h>

#include "retail.hh"
#include "treyarch/nfl/nfl.hh"
#include "treyarch/nfl/system.hh"
#include "treyarch/nfl/win32/driver.hh"

using namespace treyarch;

// sub_A15470
u32 treyarch::nfl::init(const init_params* params) {
    system::references::drivers.write(system::win32_drivers);

    references::current_platform_settings.write(&references::default_platform_settings.get());

    if (params)
        system::references::init_params.write(*params);

    for (i32 index = 0; index < system::references::drivers_count.read(); ++index) {
        driver::driver_t* driver = system::references::drivers.read()[index];

        if (system::references::init_params.get().buffer_mode != -1)
            driver->init->buffer_mode = system::references::init_params.get().buffer_mode;

        if (driver->init->initialize)
            driver->init->initialize(driver);

        nfl::log.msg("driver #{}: {} | media mask: 0x{:X} | buffer mode: {}",
                     index,
                     driver->name,
                     driver->binding->media_mask,
                     driver->init->buffer_mode);
    }

    for (system::decompress_batch_group_t &group : system::references::decompress_batch_groups.get()) {
        group.decompress_group_handle = retail::sub_A144D0(); // jq_alloc_batch_group
        group.locked                  = 0;
    }

    // with no work space this only measures it
    const u32 work_space_size = system::pre_allocate_work_space(nullptr);

    const init_params &system_params = system::references::init_params.get();

    nfl::log.msg("files: {} | requests: {} | chunk jobs: {} | buffer mode: {} | thread mode: {} | work space: 0x{:X} bytes",
                 system_params.file_count,
                 system_params.request_count,
                 system_params.chunk_decompress_job_count,
                 system_params.buffer_mode,
                 system_params.thread_mode,
                 work_space_size);

    return work_space_size;
}

// sub_A17830
void treyarch::nfl::start(void* work_space) {
    if (!work_space)
        return;

    system::pre_allocate_work_space((u8*)work_space);

    tx::slot_pool_init(&system::references::requests_pool.get(),
                      &system::references::requests.read()->entry,
                      system::references::init_params.get().request_count,
                      sizeof(system::request_t));

    tx::slot_pool_init(&references::chunk_decompress_job_pool.get(),
                      &references::chunk_decompress_jobs.read()->entry,
                      system::references::init_params.get().chunk_decompress_job_count,
                      sizeof(system::chunk_decompress_job));

    tx::slot_pool_init(&system::references::files_pool.get(),
                      &system::references::files.read()->entry,
                      system::references::init_params.get().file_count,
                      sizeof(system::file_t));

    const i32 lzo_result = lzo_init();

    if (lzo_result != LZO_E_OK)
        nfl::log.err("lzo_init failed -- error {}", lzo_result);

    nfl::log.msg("read buffers: 2 x 0x{:X} | lzo {} | i/o thread priority: {}",
                 references::current_platform_settings.read()->buffer_size,
                 lzo_version_string(),
                 references::thread_priority.read());

    if (system::references::init_params.get().thread_mode != 1)
        return;

    system::references::mutex.write(CreateMutexA(nullptr, FALSE, nullptr));
    system::references::event.write(CreateEventA(nullptr, FALSE, FALSE, nullptr));
    references::cancel_event.write(CreateEventA(nullptr, FALSE, FALSE, nullptr));

    // retail checks the mode a second time
    if (system::references::init_params.get().thread_mode != 1)
        return;

    system::references::thread.write(CreateThread(nullptr,
                                                  0x2000,
                                                  system::update_thread,
                                                  nullptr,
                                                  CREATE_SUSPENDED,
                                                  nullptr));

    if (!system::references::thread.read())
        nfl::log.err("failed to create the i/o thread -- win32 error {}", GetLastError());

    SetThreadPriority(system::references::thread.read(), references::thread_priority.read());
    ResumeThread(system::references::thread.read());
}

// sub_A173E0
void treyarch::nfl::update() {
    system::references::call_requests_count.write(0);

    system::lock();

    tx::slot_pool* requests_pool = &system::references::requests_pool.get();

    for (i32 id = tx::slot_first(requests_pool); id != -1;) {
        system::request_t* request = system::get_request(id);

        id = tx::slot_next(requests_pool, id);

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
                system::request_cb_t &call_request =
                    system::references::call_requests.read()[system::references::call_requests_count.get()++];

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
                    system::request_cb_t &call_request =
                        system::references::call_requests.read()[system::references::call_requests_count.get()++];

                    call_request.state    = system::export_request_state(request->state);
                    call_request.request  = request->entry.id;
                    call_request.callback = request->callback;
                    call_request.release  = 0;
                    call_request.user     = request->user;
                }

                break;
        }
    }

    if (system::references::call_requests_count.read()) {
        system::unlock();

        for (i32 index = 0; index < system::references::call_requests_count.read(); ++index) {
            system::request_cb_t &call_request = system::references::call_requests.read()[index];

            if (call_request.callback)
                call_request.callback(call_request.state, call_request.request, call_request.user);
        }

        system::lock();
    }

    for (i32 index = 0; index < system::references::call_requests_count.read(); ++index) {
        system::request_cb_t &call_request = system::references::call_requests.read()[index];

        if (!call_request.release)
            continue;

        system::request_t* request = system::get_request(call_request.request);

        if (request->group_handle != -1) {
            for (system::decompress_batch_group_t &group : system::references::decompress_batch_groups.get()) {
                if (group.decompress_group_handle == request->group_handle) {
                    group.locked = 0;

                    break;
                }
            }
        }

        system::file_t* file = system::get_file(request->file);

        if (file && file->as.native.flags & driver::file_flag_deferred_close && !--file->as.native.pending_requests)
            driver::close_file(request->file);

        tx::slot_free(requests_pool, call_request.request);
    }

    system::references::call_requests_count.write(0);

    system::unlock();

    if (!system::references::init_params.get().thread_mode)
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

    const i32 id = tx::slot_new(&system::references::requests_pool.get());

    if (id == -1) {
        system::unlock();

        nfl::log.err("out of request slots ({}) -- \"{}\"",
                     system::references::requests_pool.get().count,
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

    if (system::references::init_params.get().thread_mode == 1) {
        ReleaseMutex(system::references::mutex.read());

        // retail checks the mode a second time
        if (system::references::init_params.get().thread_mode == 1)
            SetEvent(system::references::event.read());
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

    for (i32 index = 0; index < system::references::drivers_count.read(); ++index) {
        switch (system::references::drivers.read()[index]->io_state) {
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
