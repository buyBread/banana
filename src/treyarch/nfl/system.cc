#include <ctime>

#include "retail.hh"
#include "treyarch/nfl/nfl.hh"
#include "treyarch/nfl/system.hh"
#include "treyarch/base/memory.hh"

using namespace treyarch;

// sub_A15470
u32 treyarch::nfl::init(const init_params* params) {
    references::current_platform_settings.write(&references::default_platform_settings.get());

    if (params)
        references::nfs_init_params.write(*params);

    for (i32 index = 0; index < references::nfs_drivers_count.read(); ++index) {
        nfd_driver* driver = references::nfs_drivers.read()[index];

        if (references::nfs_init_params.get().buffer_mode != -1)
            driver->init->buffer_mode = references::nfs_init_params.get().buffer_mode;

        if (driver->init->initialize)
            driver->init->initialize(driver);
    }

    for (decompress_group &group : references::decompress_groups.get()) {
        group.group  = retail::sub_A144D0(); // jq_alloc_batch_group
        group.in_use = 0;
    }

    // with no work space this only measures it
    return nfs_pre_allocate_work_space(nullptr);
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

    tx::slot_pool_init(&references::chunk_job_pool.get(),
                      &references::chunk_jobs.read()->entry,
                      references::nfs_init_params.get().chunk_job_count,
                      sizeof(chunk_job));

    tx::slot_pool_init(&references::nfs_files_pool.get(),
                      &references::nfs_files.read()->entry,
                      references::nfs_init_params.get().file_count,
                      sizeof(nfs_file));

    retail::sub_A17E70(0x2020, 2, 4, 4, 4, 4, 4, 4, 4, 24); // lzo_init (2.02)

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
                                              (LPTHREAD_START_ROUTINE)retail::sub_A17310,
                                              nullptr,
                                              CREATE_SUSPENDED,
                                              nullptr));

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

        if (buffer_mode != 2 && buffer_mode != 3 && buffer_mode != 4)
            continue;

        nfd_buffer* buffer = driver->buffer;

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

    nfs_request*    requests      = (nfs_request*)   nfs_pre_allocate(sizeof(nfs_request)    * params.request_count,   0x40);
    chunk_job*      chunk_jobs    = (chunk_job*)     nfs_pre_allocate(sizeof(chunk_job)      * params.chunk_job_count, 0x40);
    nfs_request_cb* call_requests = (nfs_request_cb*)nfs_pre_allocate(sizeof(nfs_request_cb) * params.request_count,   0x04);
    nfs_file*       files         = (nfs_file*)      nfs_pre_allocate(sizeof(nfs_file)       * params.file_count,      0x40);
    u8*             handles       =                  nfs_pre_allocate(handle_stride          * params.file_count,      handle_alignment);

    if (!work_space || !requests || !chunk_jobs || !call_requests || !files || !handles || failed_drivers)
        return references::nfs_mem_used.read();

    references::chunk_jobs.write(chunk_jobs);
    references::nfs_file_handles.write(handles);
    references::nfs_requests.write(requests);
    references::nfs_files.write(files);
    references::nfs_file_handle_size.write(handle_stride);
    references::nfs_call_requests.write(call_requests);

    for (i32 index = 0; index < references::nfs_drivers_count.read(); ++index) {
        nfd_driver* driver = references::nfs_drivers.read()[index];

        const i32 buffer_mode = driver->init->buffer_mode;

        if (buffer_mode != 2 && buffer_mode != 3 && buffer_mode != 4)
            continue;

        nfd_buffer* buffer = driver->buffer;

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
            memory::report("nfsPreAllocate: Out of %d bytes, used %d, free %d, total %d", required, used, available, used + available);

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

                break;
            }

            case nfs_request_state_io_error:
                // the request stays allocated until it's canceled or its file closes
                request->state = nfs_request_state_io_error_waiting;

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

        if (request->decompress_group != -1) {
            for (decompress_group &group : references::decompress_groups.get()) {
                if (group.group == request->decompress_group) {
                    group.in_use = 0;

                    break;
                }
            }
        }

        nfs_file* file = nfs_get_file(request->file);

        if (file && file->as.native.flags & nfd_file_flag_deferred_close && !--file->pending_closes)
            nfd_close_file(request->file);

        tx::slot_free(requests_pool, call_request.request);
    }

    references::nfs_call_requests_count.write(0);

    nfs_unlock();

    if (!references::nfs_init_params.get().thread_mode)
        retail::sub_A171A0(); // nfs_update
}
