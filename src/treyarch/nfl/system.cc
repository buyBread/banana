#include <cstring>
#include <ctime>
#include <lzo/lzo1x.h>

#include "retail.hh"
#include "treyarch/nfl/nfl.hh"
#include "treyarch/nfl/system.hh"
#include "treyarch/nfl/win32/driver.hh"
#include "treyarch/base/tl_system.hh"

using namespace treyarch;

// sub_A15470
u32 treyarch::nfl::init(const init_params* params) {
    references::nfs_drivers.write(nfs_win32_drivers);

    references::current_platform_settings.write(&references::default_platform_settings.get());

    if (params)
        references::nfs_init_params.write(*params);

    for (i32 index = 0; index < references::nfs_drivers_count.read(); ++index) {
        nfd_driver* driver = references::nfs_drivers.read()[index];

        if (references::nfs_init_params.get().buffer_mode != -1)
            driver->init->buffer_mode = references::nfs_init_params.get().buffer_mode;

        if (driver->init->initialize)
            driver->init->initialize(driver);

        nfl::log.msg("driver #{}: {} | media mask: 0x{:X} | buffer mode: {}",
                     index,
                     driver->name,
                     driver->binding->media_mask,
                     driver->init->buffer_mode);
    }

    for (nfs_decompress_batch_group &group : references::nfs_decompress_batch_groups.get()) {
        group.decompress_group_handle = retail::sub_A144D0(); // jq_alloc_batch_group
        group.locked                  = 0;
    }

    // with no work space this only measures it
    const u32 work_space_size = nfs_pre_allocate_work_space(nullptr);

    const init_params &nfs_params = references::nfs_init_params.get();

    nfl::log.msg("files: {} | requests: {} | chunk jobs: {} | buffer mode: {} | thread mode: {} | work space: 0x{:X} bytes",
                 nfs_params.file_count,
                 nfs_params.request_count,
                 nfs_params.chunk_decompress_job_count,
                 nfs_params.buffer_mode,
                 nfs_params.thread_mode,
                 work_space_size);

    return work_space_size;
}

// sub_A17830
void treyarch::nfl::start(void* work_space) {
    if (!work_space)
        return;

    nfs_pre_allocate_work_space((u8*)work_space);

    tx::slot_pool_init(&references::nfs_requests_pool.get(),
                      &references::nfs_requests.read()->entry,
                      references::nfs_init_params.get().request_count,
                      sizeof(nfs_request));

    tx::slot_pool_init(&references::chunk_decompress_job_pool.get(),
                      &references::chunk_decompress_jobs.read()->entry,
                      references::nfs_init_params.get().chunk_decompress_job_count,
                      sizeof(chunk_decompress_job));

    tx::slot_pool_init(&references::nfs_files_pool.get(),
                      &references::nfs_files.read()->entry,
                      references::nfs_init_params.get().file_count,
                      sizeof(nfs_file));

    const i32 lzo_result = lzo_init();

    if (lzo_result != LZO_E_OK)
        nfl::log.err("lzo_init failed -- error {}", lzo_result);

    nfl::log.msg("read buffers: 2 x 0x{:X} | lzo {} | i/o thread priority: {}",
                 references::current_platform_settings.read()->buffer_size,
                 lzo_version_string(),
                 references::thread_priority.read());

    if (references::nfs_init_params.get().thread_mode != 1)
        return;

    references::nfs_mutex.write(CreateMutexA(nullptr, FALSE, nullptr));
    references::nfs_event.write(CreateEventA(nullptr, FALSE, FALSE, nullptr));
    references::cancel_event.write(CreateEventA(nullptr, FALSE, FALSE, nullptr));

    // retail checks the mode a second time
    if (references::nfs_init_params.get().thread_mode != 1)
        return;

    references::nfs_thread.write(CreateThread(nullptr,
                                              0x2000,
                                              nfs_update_thread,
                                              nullptr,
                                              CREATE_SUSPENDED,
                                              nullptr));

    if (!references::nfs_thread.read())
        nfl::log.err("failed to create the i/o thread -- win32 error {}", GetLastError());

    SetThreadPriority(references::nfs_thread.read(), references::thread_priority.read());
    ResumeThread(references::nfs_thread.read());
}

// sub_A14FC0
u32 treyarch::nfl::nfs_pre_allocate_work_space(u8* work_space) {
    // the measuring pass leaves its total in nfs_mem_used, which becomes the free space of the carving pass
    references::nfs_mem     .write(work_space);
    references::nfs_mem_free.write(work_space ? references::nfs_mem_used.read() : 0);
    references::nfs_mem_used.write(0);

    u32 handle_size      = 1;
    u32 handle_alignment = 1;
    u32 failed_drivers   = 0;
    u8* driver_buffers[32][2];

    for (i32 index = 0; index < references::nfs_drivers_count.read(); ++index) {
        nfd_driver* driver = references::nfs_drivers.read()[index];

        if (handle_size < driver->file_operations->handle_size)
            handle_size = driver->file_operations->handle_size;

        if (handle_alignment < driver->file_operations->handle_alignment)
            handle_alignment = driver->file_operations->handle_alignment;

        const i32 buffer_mode = driver->init->buffer_mode;

        if (buffer_mode != buffer_mode_unaligned && buffer_mode != buffer_mode_all && buffer_mode != buffer_mode_vm)
            continue;

        nfd_buffer* buffer = driver->buffers_object;

        u32 alignment = buffer->address_alignment;

        if (alignment < buffer->size_alignment)
            alignment = buffer->size_alignment;

        if (buffer->buffers[0].data)
            continue;

        buffer->buffer_size = references::current_platform_settings.read()->buffer_size;

        for (i32 slot = 0; slot < 2; ++slot) {
            driver_buffers[index][slot] = nfs_pre_allocate(buffer->buffer_size, alignment);

            failed_drivers |= (driver_buffers[index][slot] == nullptr) << index;
        }
    }

    const u32 handle_stride = (handle_alignment + handle_size - 1) / handle_alignment * handle_alignment;

    const init_params &params = references::nfs_init_params.get();

    nfs_request*          requests      = (nfs_request*)         nfs_pre_allocate(sizeof(nfs_request)          * params.request_count,              0x40);
    chunk_decompress_job* jobs          = (chunk_decompress_job*)nfs_pre_allocate(sizeof(chunk_decompress_job) * params.chunk_decompress_job_count, 0x40);
    nfs_request_cb*       call_requests = (nfs_request_cb*)      nfs_pre_allocate(sizeof(nfs_request_cb)       * params.request_count,              0x04);
    nfs_file*             files         = (nfs_file*)            nfs_pre_allocate(sizeof(nfs_file)             * params.file_count,                 0x40);
    u8*                   handles       =                        nfs_pre_allocate(handle_stride                * params.file_count,                 handle_alignment);

    if (!work_space || !requests || !jobs || !call_requests || !files || !handles || failed_drivers)
        return references::nfs_mem_used.read();

    references::chunk_decompress_jobs.write(jobs);
    references::nfs_file_handles.write(handles);
    references::nfs_requests.write(requests);
    references::nfs_files.write(files);
    references::nfs_file_handle_size.write(handle_stride);
    references::nfs_call_requests.write(call_requests);

    for (i32 index = 0; index < references::nfs_drivers_count.read(); ++index) {
        nfd_driver* driver = references::nfs_drivers.read()[index];

        const i32 buffer_mode = driver->init->buffer_mode;

        if (buffer_mode != buffer_mode_unaligned && buffer_mode != buffer_mode_all && buffer_mode != buffer_mode_vm)
            continue;

        nfd_buffer* buffer = driver->buffers_object;

        if (buffer->buffers[0].data)
            continue;

        buffer->buffers[0].data      = driver_buffers[index][0];
        buffer->buffers[0].locked_by = -1;
        buffer->buffers[1].data      = driver_buffers[index][1];
        buffer->buffers[1].locked_by = -1;
    }

    return references::nfs_mem_used.read();
}

// inlined @ sub_A14FC0
u8* treyarch::nfl::nfs_pre_allocate(u32 size, u32 alignment) {
    u8* const mem       = references::nfs_mem.read();
    const u32 used      = references::nfs_mem_used.read();
    const u32 available = references::nfs_mem_free.read();
    const u32 required  = size + alignment - 1;

    if (mem) {
        if (required > available) {
            tl_fatal("nfsPreAllocate: Out of %d bytes, used %d, free %d, total %d", required, used, available, used + available);

            return nullptr;
        }

        references::nfs_mem_free.write(available - required);
    }

    references::nfs_mem_used.write(used + required);

    return mem ?
        (u8*)(((u32)mem + used + alignment - 1) / alignment * alignment) : nullptr;
}

// inlined @ sub_A15530
void treyarch::nfl::nfs_lock() {
    if (references::nfs_init_params.get().thread_mode == 1)
        WaitForSingleObject(references::nfs_mutex.read(), INFINITE);
}

// inlined @ sub_A15530
void treyarch::nfl::nfs_unlock() {
    if (references::nfs_init_params.get().thread_mode == 1)
        ReleaseMutex(references::nfs_mutex.read());
}

// inlined @ sub_A15F90
nfl::nfs_request* treyarch::nfl::nfs_get_request(i32 id) {
    const i32 index = tx::slot_index(&references::nfs_requests_pool.get(), id);

    return index == -1 ? nullptr : &references::nfs_requests.read()[index];
}

// inlined @ sub_A15530
nfl::nfs_file* treyarch::nfl::nfs_get_file(i32 id) {
    const i32 index = tx::slot_index(&references::nfs_files_pool.get(), id);

    return index == -1 ? nullptr : &references::nfs_files.read()[index];
}

// inlined @ sub_A15530
i32 treyarch::nfl::nfs_get_native_file_id(i32 id) {
    nfs_file* file = nfs_get_file(id);

    if (!file)
        return -1;

    return file->file_type == nfs_file_type_subfile ? file->as.subfile.parent : id;
}

// inlined @ sub_A164E0
nfl::nfd_driver* treyarch::nfl::nfs_get_file_driver(i32 id) {
    nfs_file* file = nfs_get_file(nfs_get_native_file_id(id));

    return file ? file->as.native.driver : nullptr;
}

// inlined @ sub_A15530
// inlined @ sub_A158E0
nfl::nfd_driver* treyarch::nfl::nfs_get_media_driver(u32 media) {
    for (i32 index = 0; index < references::nfs_drivers_count.read(); ++index) {
        nfd_driver* driver = references::nfs_drivers.read()[index];

        if (driver && driver->binding && media & driver->binding->media_mask)
            return driver;
    }

    return nullptr;
}

// inlined @ sub_A15530
u8* treyarch::nfl::nfs_get_file_handle(i32 id) {
    const i32 index = tx::slot_index(&references::nfs_files_pool.get(), nfs_get_native_file_id(id));

    return index == -1 ?
        nullptr : references::nfs_file_handles.read() + index * references::nfs_file_handle_size.read();
}

// inlined @ sub_A173E0
i32 treyarch::nfl::nfs_export_request_state(i32 state) {
    switch (state) {
        case nfs_request_state_waiting:
        case nfs_request_state_working:
            return request_state_active;

        case nfs_request_state_workdone:
            return request_state_completed;

        case nfs_request_state_canceling:
        case nfs_request_state_canceled:
            return request_state_canceled;

        case nfs_request_state_timeout:
            return request_state_timeout;

        case nfs_request_state_io_error:
        case nfs_request_state_io_error_waiting:
            return request_state_error;

        default:
            return request_state_invalid;
    }
}

// sub_A173E0
void treyarch::nfl::update() {
    references::nfs_call_requests_count.write(0);

    nfs_lock();

    tx::slot_pool* requests_pool = &references::nfs_requests_pool.get();

    for (i32 id = tx::slot_first(requests_pool); id != -1;) {
        nfs_request* request = nfs_get_request(id);

        id = tx::slot_next(requests_pool, id);

        const clock_t now = clock();

        switch (request->state) {
            case nfs_request_state_waiting:
                // a stall watchdog nothing reads
                if (request->bytes_read != request->last_bytes_read) {
                    request->last_progress_time = now;
                    request->last_bytes_read    = request->bytes_read;
                }

                clock();

                break;

            case nfs_request_state_workdone:
            case nfs_request_state_canceled: {
                nfs_request_cb &call_request =
                    references::nfs_call_requests.read()[references::nfs_call_requests_count.get()++];

                call_request.state    = nfs_export_request_state(request->state);
                call_request.request  = request->entry.id;
                call_request.callback = request->callback;
                call_request.release  = 1;
                call_request.user     = request->user;

                const i32 elapsed = now - request->start_time;

                if (request->state == nfs_request_state_canceled)
                    nfl::log.dbg("canceled {} \"{}\" after {} ms", request->entry.id, nfs_file_path(request->file), elapsed);
                else if (elapsed > 10000)
                    nfl::log.wrn("slow request {} \"{}\" -- 0x{:X} bytes in {} ms",
                                 request->entry.id,
                                 nfs_file_path(request->file),
                                 request->output_size,
                                 elapsed);
                else
                    nfl::log.dbg("complete {} \"{}\" -- 0x{:X} bytes in {} ms",
                                 request->entry.id,
                                 nfs_file_path(request->file),
                                 request->output_size,
                                 elapsed);

                break;
            }

            case nfs_request_state_io_error:
                // the request stays allocated until it's canceled or its file closes
                request->state = nfs_request_state_io_error_waiting;

                nfl::log.err("request {} \"{}\" failed -- 0x{:X} of 0x{:X} bytes read{}",
                             request->entry.id,
                             nfs_file_path(request->file),
                             request->bytes_read,
                             request->disk_size,
                             request->driver->io_state == nfd_io_state_error ? ", streaming stopped until it's canceled" : "");

                if (request->callback) {
                    nfs_request_cb &call_request =
                        references::nfs_call_requests.read()[references::nfs_call_requests_count.get()++];

                    call_request.state    = nfs_export_request_state(request->state);
                    call_request.request  = request->entry.id;
                    call_request.callback = request->callback;
                    call_request.release  = 0;
                    call_request.user     = request->user;
                }

                break;
        }
    }

    if (references::nfs_call_requests_count.read()) {
        nfs_unlock();

        for (i32 index = 0; index < references::nfs_call_requests_count.read(); ++index) {
            nfs_request_cb &call_request = references::nfs_call_requests.read()[index];

            if (call_request.callback)
                call_request.callback(call_request.state, call_request.request, call_request.user);
        }

        nfs_lock();
    }

    for (i32 index = 0; index < references::nfs_call_requests_count.read(); ++index) {
        nfs_request_cb &call_request = references::nfs_call_requests.read()[index];

        if (!call_request.release)
            continue;

        nfs_request* request = nfs_get_request(call_request.request);

        if (request->group_handle != -1) {
            for (nfs_decompress_batch_group &group : references::nfs_decompress_batch_groups.get()) {
                if (group.decompress_group_handle == request->group_handle) {
                    group.locked = 0;

                    break;
                }
            }
        }

        nfs_file* file = nfs_get_file(request->file);

        if (file && file->as.native.flags & nfd_file_flag_deferred_close && !--file->as.native.pending_requests)
            nfd_close_file(request->file);

        tx::slot_free(requests_pool, call_request.request);
    }

    references::nfs_call_requests_count.write(0);

    nfs_unlock();

    if (!references::nfs_init_params.get().thread_mode)
        nfs_update();
}

// sub_A17310
DWORD __stdcall treyarch::nfl::nfs_update_thread(void* parameter) {
    for (;;) {
        WaitForSingleObject(references::nfs_event.read(), INFINITE);

        nfs_lock();

        const bool exit   = references::thread_exit.read() != 0;
        const i32  result = nfs_update();

        nfs_unlock();

        if (exit)
            return 0;

        if (!result)
            continue;

        if (references::alertable_wait.read()) {
            if (result == 1) {
                HANDLE handles[2] = { references::nfd_win32_io_work.get().io_event, references::cancel_event.read() };

                WaitForMultipleObjectsEx(2, handles, FALSE, INFINITE, TRUE);
            }
        } else
            Sleep(references::thread_sleep.read());

        if (references::nfs_init_params.get().thread_mode == 1)
            SetEvent(references::nfs_event.read());
    }
}

// sub_A171A0
i32 treyarch::nfl::nfs_update() { // 1 while a driver has I/O going, 2 while chunk jobs are pending
    nfd_driver** const drivers       = references::nfs_drivers.read();
    const i32          drivers_count = references::nfs_drivers_count.read();

    i32 can_schedule[32];
    i32 scheduled[32];

    for (i32 index = 0; index < drivers_count; ++index)
        can_schedule[index] = nfs_update_driver(drivers[index]) && nfd_find_unlocked_read_buffer(drivers[index]) != -1;

    for (i32 index = 0; index < drivers_count; ++index) {
        scheduled[index] = can_schedule[index] ?
            nfs_schedule_request(drivers_count == 1 ? nullptr : drivers[index]) : -1;
    }

    for (i32 index = 0; index < drivers_count; ++index) {
        if (scheduled[index] != -1)
            nfs_execute_request(drivers[index], scheduled[index]);
    }

    i32 result = 0;

    for (i32 index = 0; index < drivers_count; ++index) {
        if (drivers[index]->io_state)
            result = 1;

        if (drivers[index]->chunk_decompress_jobs)
            result = 2;
    }

    return result;
}

// sub_A16FA0
i32 treyarch::nfl::nfs_update_driver(nfd_driver* driver) { // nonzero when the driver can take a new command
    nfs_update_chunk_decompression_jobs(driver);

    if (!driver->io_state)
        return 1;

    // completions run in here
    if (driver->io_operations->update && driver->io_operations->update())
        return 0;

    disk_read_t &disk_read = driver->disk_read;

    const i32    id      = disk_read.io_command.request;
    nfs_request* request = nfs_get_request(id);

    if (request) {
        switch (driver->io_state) {
            case nfd_io_state_working:
                if (request->state == nfs_request_state_canceling) {
                    if (driver->io_operations->cancel && driver->io_operations->cancel(disk_read.io_command.file_handle))
                        return 0;

                    driver->io_state = nfd_io_state_canceling;
                }

                return 0;

            case nfd_io_state_workdone:
                if (request->state != nfs_request_state_canceling) {
                    if (!disk_read.bytes_completed)
                        request->state = nfs_request_state_workdone;
                    else if (request->decompress) {
                        request->state = request->bytes_read == request->disk_size && !request->active_decompress_jobs ?
                            nfs_request_state_workdone : nfs_request_state_waiting;

                        request->bytes_read += disk_read.bytes_completed;

                        if (request->bytes_scheduled >= request->output_size)
                            nfd_unlock_read_buffer(driver, id, request->read_buffer_slot);
                        else {
                            nfs_begin_segment_decompress(driver, request);
                            nfs_update_chunk_decompression_jobs(driver);
                        }
                    } else {
                        std::memcpy(request->destination + request->bytes_read, disk_read.io_command.buffer, disk_read.bytes_completed);

                        request->bytes_read += disk_read.bytes_completed;

                        request->state = request->bytes_read != request->disk_size ?
                            nfs_request_state_waiting : nfs_request_state_workdone;

                        nfd_unlock_read_buffer(driver, id, request->read_buffer_slot);
                    }

                    break;
                }

                [[fallthrough]];

            case nfd_io_state_canceled:
                nfd_unlock_read_buffer(driver, id, request->read_buffer_slot);

                if (!request->active_decompress_jobs)
                    request->state = nfs_request_state_canceled;

                break;

            case nfd_io_state_canceling:
                return 0;

            case nfd_io_state_error:
                // stays here, stopping all streaming, until the request is canceled and freed
                if (request->state != nfs_request_state_io_error_waiting) {
                    nfd_unlock_read_buffer(driver, id, disk_read.io_command.read_buffer_slot);

                    request->state = nfs_request_state_io_error;
                }

                return 0;

            default:
                tl_fatal("nfsUpdateDriver: invalid state %s", 0);

                return 0;
        }
    }

    driver->io_state = nfd_io_state_idle;

    disk_read.io_command.size             = 0;
    disk_read.io_command.file_handle      = nullptr;
    disk_read.io_command.offset           = 0;
    disk_read.io_command.request          = -1;
    disk_read.io_command.type             = -1;
    disk_read.io_command.read_buffer_slot = -1;

    return 1;
}
