#include <ctime>

#include "retail.hh"
#include "treyarch/nfl/nfl.hh"
#include "treyarch/nfl/system.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

// sub_A15470
u32 treyarch::nfl::init(const init_params* params) {
    references::current_platform_settings.write(&references::default_platform_settings.get());

    if (params)
        references::params.write(*params);

    for (i32 index = 0; index < references::driver_count.read(); ++index) {
        driver* driver = references::drivers.read()[index];

        if (references::params.get().buffer_mode != -1)
            driver->initialization->buffer_mode = references::params.get().buffer_mode;

        if (driver->initialization->initialize)
            driver->initialization->initialize(driver);
    }

    for (decompress_group &group : references::decompress_groups.get()) {
        group.group  = retail::sub_A144D0(); // jq_alloc_batch_group
        group.in_use = 0;
    }

    // with no work space this only measures it
    return nfs_pre_allocate(nullptr);
}

// sub_A17830
void treyarch::nfl::start(void* work_space) {
    if (!work_space)
        return;

    nfs_pre_allocate((u8*)work_space);

    initialize_pool(&references::request_pool.get(),
                    &references::requests.read()->node,
                    references::params.get().request_count,
                    sizeof(request));

    initialize_pool(&references::chunk_job_pool.get(),
                    &references::chunk_jobs.read()->node,
                    references::params.get().chunk_job_count,
                    sizeof(chunk_job));

    initialize_pool(&references::file_pool.get(),
                    &references::files.read()->node,
                    references::params.get().file_count,
                    sizeof(file));

    retail::sub_A17E70(0x2020, 2, 4, 4, 4, 4, 4, 4, 4, 24); // lzo_init (2.02)

    if (references::params.get().thread_mode != 1)
        return;

    references::mutex.write(CreateMutexA(nullptr, FALSE, nullptr));
    references::work_event.write(CreateEventA(nullptr, FALSE, FALSE, nullptr));
    references::cancel_event.write(CreateEventA(nullptr, FALSE, FALSE, nullptr));

    // retail checks the mode a second time
    if (references::params.get().thread_mode != 1)
        return;

    references::thread.write(CreateThread(nullptr,
                                          0x2000,
                                          (LPTHREAD_START_ROUTINE)retail::sub_A17310,
                                          nullptr,
                                          CREATE_SUSPENDED,
                                          nullptr));

    SetThreadPriority(references::thread.read(), references::thread_priority.read());
    ResumeThread(references::thread.read());
}

// sub_A14FC0
u32 treyarch::nfl::nfs_pre_allocate(u8* work_space) {
    // the measuring pass leaves its total in work_space_used, which becomes the free space of the carving pass
    references::work_space     .write(work_space);
    references::work_space_free.write(work_space ? references::work_space_used.read() : 0);
    references::work_space_used.write(0);

    u32 handle_size      = 1;
    u32 handle_alignment = 1;
    u32 failed_drivers   = 0;
    u8* driver_buffers[32][2];

    for (i32 index = 0; index < references::driver_count.read(); ++index) {
        driver* driver = references::drivers.read()[index];

        if (handle_size < driver->file_operations->handle_size)
            handle_size = driver->file_operations->handle_size;

        if (handle_alignment < driver->file_operations->handle_alignment)
            handle_alignment = driver->file_operations->handle_alignment;

        const i32 buffer_mode = driver->initialization->buffer_mode;

        if (buffer_mode != 2 && buffer_mode != 3 && buffer_mode != 4)
            continue;

        driver_buffers_object* buffers_object = driver->buffers_object;

        u32 alignment = buffers_object->address_alignment;

        if (alignment < buffers_object->size_alignment)
            alignment = buffers_object->size_alignment;

        if (buffers_object->buffers[0].data)
            continue;

        buffers_object->buffer_size = references::current_platform_settings.read()->buffer_size;

        for (i32 buffer = 0; buffer < 2; ++buffer) {
            driver_buffers[index][buffer] = nfs_pre_allocate_block(buffers_object->buffer_size, alignment);

            failed_drivers |= (driver_buffers[index][buffer] == nullptr) << index;
        }
    }

    const u32 handle_stride = (handle_alignment + handle_size - 1) / handle_alignment * handle_alignment;

    request*    requests    = (request*)   nfs_pre_allocate_block(sizeof(request)    * references::params.get().request_count,   0x40);
    chunk_job*  chunk_jobs  = (chunk_job*) nfs_pre_allocate_block(sizeof(chunk_job)  * references::params.get().chunk_job_count, 0x40);
    completion* completions = (completion*)nfs_pre_allocate_block(sizeof(completion) * references::params.get().request_count,   0x04);
    file*       files       = (file*)      nfs_pre_allocate_block(sizeof(file)       * references::params.get().file_count,      0x40);
    u8*         handles     =              nfs_pre_allocate_block(handle_stride      * references::params.get().file_count,      handle_alignment);

    if (!work_space || !requests || !chunk_jobs || !completions || !files || !handles || failed_drivers)
        return references::work_space_used.read();

    references::chunk_jobs.write(chunk_jobs);
    references::file_handles.write(handles);
    references::requests.write(requests);
    references::files.write(files);
    references::file_handle_stride.write(handle_stride);
    references::completions.write(completions);

    for (i32 index = 0; index < references::driver_count.read(); ++index) {
        driver* driver = references::drivers.read()[index];

        const i32 buffer_mode = driver->initialization->buffer_mode;

        if (buffer_mode != 2 && buffer_mode != 3 && buffer_mode != 4)
            continue;

        driver_buffers_object* buffers_object = driver->buffers_object;

        if (buffers_object->buffers[0].data)
            continue;

        buffers_object->buffers[0].data      = driver_buffers[index][0];
        buffers_object->buffers[0].locked_by = -1;
        buffers_object->buffers[1].data      = driver_buffers[index][1];
        buffers_object->buffers[1].locked_by = -1;
    }

    return references::work_space_used.read();
}

// inlined @ sub_A14FC0
u8* treyarch::nfl::nfs_pre_allocate_block(u32 size, u32 alignment) {
    u8* const work_space = references::work_space.read();
    const u32 used       = references::work_space_used.read();
    const u32 available  = references::work_space_free.read();
    const u32 required   = size + alignment - 1;

    if (work_space) {
        if (required > available) {
            memory::report("nfsPreAllocate: Out of %d bytes, used %d, free %d, total %d", required, used, available, used + available);

            return nullptr;
        }

        references::work_space_free.write(available - required);
    }

    references::work_space_used.write(used + required);

    return work_space ?
        (u8*)(((u32)work_space + used + alignment - 1) / alignment * alignment) : nullptr;
}

// inlined @ sub_A15530
void treyarch::nfl::lock() {
    if (references::params.get().thread_mode == 1)
        WaitForSingleObject(references::mutex.read(), INFINITE);
}

// inlined @ sub_A15530
void treyarch::nfl::unlock() {
    if (references::params.get().thread_mode == 1)
        ReleaseMutex(references::mutex.read());
}

// inlined @ sub_A15F90
nfl::request* treyarch::nfl::get_request(i32 id) {
    const i32 index = references::request_pool.get().find(id);

    return index == -1 ? nullptr : &references::requests.read()[index];
}

// inlined @ sub_A15530
nfl::file* treyarch::nfl::get_file(i32 id) {
    const i32 index = references::file_pool.get().find(id);

    return index == -1 ? nullptr : &references::files.read()[index];
}

// inlined @ sub_A15530
i32 treyarch::nfl::resolve_file(i32 id) {
    file* file = get_file(id);

    if (!file)
        return -1;

    // a subfile keeps its parent's id where the driver would be
    return file->type == 1 ? (i32)file->driver : id;
}

// inlined @ sub_A15530
u8* treyarch::nfl::get_file_handle(i32 id) {
    const i32 index = references::file_pool.get().find(id);

    return index == -1 ?
        nullptr : references::file_handles.read() + index * references::file_handle_stride.read();
}

// inlined @ sub_A173E0
i32 treyarch::nfl::to_public_request_state(i32 state) {
    switch (state) {
        case internal_request_state_waiting:
        case internal_request_state_working:
            return request_state_active;

        case internal_request_state_workdone:
            return request_state_completed;

        case internal_request_state_canceling:
        case internal_request_state_canceled:
            return request_state_canceled;

        case internal_request_state_timeout:
            return request_state_timeout;

        case internal_request_state_io_error:
        case internal_request_state_io_error_waiting:
            return request_state_error;

        default:
            return request_state_invalid;
    }
}

// sub_A173E0
void treyarch::nfl::update() {
    references::completion_count.write(0);

    lock();

    pool &request_pool = references::request_pool.get();

    for (i32 id = request_pool.first_id(); id != -1;) {
        request* request = get_request(id);

        id = request_pool.next_id(id);

        const clock_t now = clock();

        switch (request->state) {
            case internal_request_state_waiting:
                // a stall watchdog nothing reads
                if (request->bytes_read != request->last_bytes_read) {
                    request->last_progress_time = now;
                    request->last_bytes_read    = request->bytes_read;
                }

                clock();

                break;

            case internal_request_state_workdone:
            case internal_request_state_canceled: {
                completion &completion = references::completions.read()[references::completion_count.get()++];

                completion.state    = to_public_request_state(request->state);
                completion.request  = request->node.id;
                completion.callback = request->callback;
                completion.release  = 1;
                completion.user     = request->user;

                break;
            }

            case internal_request_state_io_error:
                // the request stays allocated until it's canceled or its file closes
                request->state = internal_request_state_io_error_waiting;

                if (request->callback) {
                    completion &completion = references::completions.read()[references::completion_count.get()++];

                    completion.state    = to_public_request_state(request->state);
                    completion.request  = request->node.id;
                    completion.callback = request->callback;
                    completion.release  = 0;
                    completion.user     = request->user;
                }

                break;
        }
    }

    if (references::completion_count.read()) {
        unlock();

        for (i32 index = 0; index < references::completion_count.read(); ++index) {
            completion &completion = references::completions.read()[index];

            if (completion.callback)
                completion.callback(completion.state, completion.request, completion.user);
        }

        lock();
    }

    for (i32 index = 0; index < references::completion_count.read(); ++index) {
        completion &completion = references::completions.read()[index];

        if (!completion.release)
            continue;

        request* request = get_request(completion.request);

        if (request->decompress_group != -1) {
            for (decompress_group &group : references::decompress_groups.get()) {
                if (group.group == request->decompress_group) {
                    group.in_use = 0;

                    break;
                }
            }
        }

        file* file = get_file(request->file);

        if (file && file->flags & file_flag_deferred_close && !--file->pending_closes)
            nfd_close_file(request->file);

        request_pool.release(completion.request);
    }

    references::completion_count.write(0);

    unlock();

    if (!references::params.get().thread_mode)
        retail::sub_A171A0(); // one driver pass
}
