#include <cstring>

#include "retail.hh"
#include "treyarch/jq/jq.hh"
#include "treyarch/nfl/nfl.hh"
#include "treyarch/nfl/system.hh"
#include "treyarch/nfl/win32/driver.hh"
#include "treyarch/base/tl_system.hh"

using namespace treyarch;

nfl::driver::driver_t** treyarch::nfl::system::drivers = win32_drivers;

// sub_A14FC0
u32 treyarch::nfl::system::pre_allocate_work_space(u8* work_space) {
    // the measuring pass leaves its total in mem_used, which becomes the free space of the carving pass
    mem      = work_space;
    mem_free = work_space ? mem_used : 0;
    mem_used = 0;

    u32 handle_size      = 1;
    u32 handle_alignment = 1;
    u32 failed_drivers   = 0;
    u8* driver_buffers[32][2];

    for (i32 index = 0; index < drivers_count; ++index) {
        driver::driver_t* driver = drivers[index];

        if (handle_size < driver->file_operations->handle_size)
            handle_size = driver->file_operations->handle_size;

        if (handle_alignment < driver->file_operations->handle_alignment)
            handle_alignment = driver->file_operations->handle_alignment;

        const i32 buffer_mode = driver->init->buffer_mode;

        if (buffer_mode != buffer_mode_unaligned && buffer_mode != buffer_mode_all && buffer_mode != buffer_mode_vm)
            continue;

        driver::buffer_t* buffer = driver->buffers_object;

        u32 alignment = buffer->address_alignment;

        if (alignment < buffer->size_alignment)
            alignment = buffer->size_alignment;

        if (buffer->buffers[0].data)
            continue;

        buffer->buffer_size = platform_settings->buffer_size;

        for (i32 slot = 0; slot < 2; ++slot) {
            driver_buffers[index][slot] = pre_allocate(buffer->buffer_size, alignment);

            failed_drivers |= (driver_buffers[index][slot] == nullptr) << index;
        }
    }

    const u32 handle_stride = (handle_alignment + handle_size - 1) / handle_alignment * handle_alignment;

    const nfl::init_params &params = init_params;

    request_t*            requests      = (request_t*)           pre_allocate(sizeof(request_t)            * params.request_count,              0x40);
    chunk_decompress_job* jobs          = (chunk_decompress_job*)pre_allocate(sizeof(chunk_decompress_job) * params.chunk_decompress_job_count, 0x40);
    request_cb_t*         call_requests = (request_cb_t*)        pre_allocate(sizeof(request_cb_t)         * params.request_count,              0x04);
    file_t*               files         = (file_t*)              pre_allocate(sizeof(file_t)               * params.file_count,                 0x40);
    u8*                   handles       =                        pre_allocate(handle_stride                * params.file_count,                 handle_alignment);

    if (!work_space || !requests || !jobs || !call_requests || !files || !handles || failed_drivers)
        return mem_used;

    chunk_decompress_jobs = jobs;
    file_handles          = handles;
    system::requests      = requests;
    system::files         = files;
    file_handle_size      = handle_stride;
    system::call_requests = call_requests;

    for (i32 index = 0; index < drivers_count; ++index) {
        driver::driver_t* driver = drivers[index];

        const i32 buffer_mode = driver->init->buffer_mode;

        if (buffer_mode != buffer_mode_unaligned && buffer_mode != buffer_mode_all && buffer_mode != buffer_mode_vm)
            continue;

        driver::buffer_t* buffer = driver->buffers_object;

        if (buffer->buffers[0].data)
            continue;

        buffer->buffers[0].data      = driver_buffers[index][0];
        buffer->buffers[0].locked_by = -1;
        buffer->buffers[1].data      = driver_buffers[index][1];
        buffer->buffers[1].locked_by = -1;
    }

    return mem_used;
}

// inlined @ sub_A14FC0
u8* treyarch::nfl::system::pre_allocate(u32 size, u32 alignment) {
    const u32 used      = mem_used;
    const u32 available = mem_free;
    const u32 required  = size + alignment - 1;

    if (mem) {
        if (required > available) {
            tl_fatal("nfsPreAllocate: Out of %d bytes, used %d, free %d, total %d", required, used, available, used + available);

            return nullptr;
        }

        mem_free = available - required;
    }

    mem_used = used + required;

    return mem ?
        (u8*)(((u32)mem + used + alignment - 1) / alignment * alignment) : nullptr;
}

// inlined @ sub_A15530
void treyarch::nfl::system::lock() {
    if (init_params.thread_mode == 1)
        WaitForSingleObject(mutex, INFINITE);
}

// inlined @ sub_A15530
void treyarch::nfl::system::unlock() {
    if (init_params.thread_mode == 1)
        ReleaseMutex(mutex);
}

// inlined @ sub_A15F90
nfl::system::request_t* treyarch::nfl::system::get_request(i32 id) {
    const i32 index = tx::slot_index(&requests_pool, id);

    return index == -1 ? nullptr : &requests[index];
}

// inlined @ sub_A15530
nfl::system::file_t* treyarch::nfl::system::get_file(i32 id) {
    const i32 index = tx::slot_index(&files_pool, id);

    return index == -1 ? nullptr : &files[index];
}

// inlined @ sub_A15530
i32 treyarch::nfl::system::get_native_file_id(i32 id) {
    file_t* file = get_file(id);

    if (!file)
        return -1;

    return file->file_type == file_type_subfile ? file->as.subfile.parent : id;
}

// inlined @ sub_A164E0
nfl::driver::driver_t* treyarch::nfl::system::get_file_driver(i32 id) {
    file_t* file = get_file(get_native_file_id(id));

    return file ? file->as.native.driver : nullptr;
}

// inlined @ sub_A15530
// inlined @ sub_A158E0
nfl::driver::driver_t* treyarch::nfl::system::get_media_driver(u32 media) {
    for (i32 index = 0; index < drivers_count; ++index) {
        driver::driver_t* driver = drivers[index];

        if (driver && driver->binding && media & driver->binding->media_mask)
            return driver;
    }

    return nullptr;
}

// inlined @ sub_A15530
u8* treyarch::nfl::system::get_file_handle(i32 id) {
    const i32 index = tx::slot_index(&files_pool, get_native_file_id(id));

    return index == -1 ?
        nullptr : file_handles + index * file_handle_size;
}

// inlined @ sub_A16380
// inlined @ sub_A16D60
nfl::system::chunk_decompress_job* treyarch::nfl::system::get_chunk_decompress_job(i32 id) {
    const i32 index = tx::slot_index(&chunk_decompress_job_pool, id);

    return index == -1 ? nullptr : &chunk_decompress_jobs[index];
}

// inlined @ sub_A173E0
i32 treyarch::nfl::system::export_request_state(i32 state) {
    switch (state) {
        case request_state_waiting:
        case request_state_working:
            return nfl::request_state_active;

        case request_state_workdone:
            return nfl::request_state_completed;

        case request_state_canceling:
        case request_state_canceled:
            return nfl::request_state_canceled;

        case request_state_timeout:
            return nfl::request_state_timeout;

        case request_state_io_error:
        case request_state_io_error_waiting:
            return nfl::request_state_error;

        default:
            return nfl::request_state_invalid;
    }
}

// sub_A15530
i32 treyarch::nfl::system::open_file(u32 media_mask, const char* path, u32 flags, u32* size) {
    char bound_path[0x100];

    // every media bit gets its own try, on the first driver that binds it
    for (i32 bit = 0; bit < 32; ++bit) {
        const u32 media = media_mask & (1 << bit);

        if (!media)
            continue;

        driver::driver_t* driver = get_media_driver(media);

        if (!driver || driver->binding->bind(media, path, bound_path, sizeof(bound_path)))
            continue;

        lock();

        const i32 id = tx::slot_new(&files_pool);

        if (id == -1) {
            unlock();

            nfl::log.err("out of file handles ({}) -- \"{}\"", files_pool.count, path);

            return -1;
        }

        file_t* file = get_file(id);

        unlock();

        file->file_type                  = file_type_native;
        file->as.native.child_count      = 0;
        file->as.native.pending_requests = 0;
        file->as.native.flags            = flags;
        file->as.native.driver           = driver;
        file->as.native.media            = media;

        std::strcpy(file->path, bound_path);

        if (flags & driver::file_flag_lazy_open) {
            file->size             = 0;
            file->as.native.driver = nullptr;
            file->as.native.media  = media_mask;

            return id;
        }

        u8* handle = get_file_handle(id);

        driver::file_info_t info;

        if (!driver->file_operations->open(handle, bound_path, flags, size ? *size : 0) &&
            !driver->file_operations->status(handle, &info)) {

            if (size)
                *size = info.size;

            file->size = info.size;

            nfl::log.dbg("open {} \"{}\" -- 0x{:X} bytes", id, bound_path, info.size);

            return id;
        }

        tx::slot_free(&files_pool, id);
    }

    return -1;
}

// sub_A158E0
bool treyarch::nfl::system::open_file_async(i32 id) {
    lock();

    request_t* request = get_request(id);
    file_t*    file    = request ? get_file(request->file) : nullptr;

    if (!file) {
        unlock();

        return false;
    }

    file_t local_file = *file;

    if (local_file.as.native.driver) {
        bool assigned = false;

        if (!request->driver) {
            request->driver = local_file.as.native.driver;
            assigned        = true;
        }

        unlock();

        return assigned;
    }

    unlock();

    char                bound_path[0x100];
    u8                  local_handle[0x100];
    driver::file_info_t info;

    for (i32 bit = 0; bit < 32; ++bit) {
        const u32 media = local_file.as.native.media & (1 << bit);

        if (!media)
            continue;

        driver::driver_t* driver = get_media_driver(media);

        if (!driver ||
            driver->binding->bind(media, local_file.path, bound_path, sizeof(bound_path)) ||
            driver->file_operations->open(local_handle, bound_path, local_file.as.native.flags, 0) ||
            driver->file_operations->status(local_handle, &info))

            continue;

        lock();

        request = get_request(id);
        file    = nullptr;

        if (request) {
            request->driver = driver;

            file = get_file(request->file);

            if (file) {
                std::memcpy(get_file_handle(request->file), local_handle, file_handle_size);

                file->size             = info.size;
                file->as.native.driver = driver;
                file->as.native.media  = media;
            }
        }

        unlock();

        return request && file;
    }

    return false;
}

// sub_A16AD0
void treyarch::nfl::system::close_file(i32 id, bool deferred) {
    file_t* file = get_file(id);

    if (!file)
        return;

    lock();

    for (i32 request_id = tx::slot_first(&requests_pool); request_id != -1;) {
        request_t* request = get_request(request_id);
        i32        next_id = tx::slot_next(&requests_pool, request_id);

        if (request->file == id) {
            if (deferred)
                ++file->as.native.pending_requests;

            cancel_request(request_id);
        }

        request_id = next_id;
    }

    if (file->file_type) {
        if (file->file_type == file_type_subfile) {
            const i32 parent_id = file->as.subfile.parent;

            --get_file(parent_id)->as.native.child_count;
            close_file(parent_id, false);
        } else
            tl_fatal("nflCloseFile: Invalid filetype");
    } else if (!file->as.native.child_count) {
        // closing now would pull the file out from under requests that are still being canceled
        if (file->as.native.pending_requests && deferred) {
            nfl::log.dbg("close {} \"{}\" -- deferred, {} requests pending",
                         id,
                         file->path,
                         file->as.native.pending_requests);

            file->as.native.flags |= driver::file_flag_deferred_close;
        } else {
            nfl::log.dbg("close {} \"{}\"", id, file->path);

            driver::close_file(id);
        }
    }

    unlock();
}

// sub_A15F20
bool treyarch::nfl::system::validate_destination(u8* destination) {
    *destination = 0;

    return true;
}

// sub_A15EF0
i32 treyarch::nfl::system::allocate_decompress_batch_job() {
    for (decompress_batch_group_t &group : decompress_batch_groups) {
        if (!group.locked) {
            group.locked = 1;

            return group.decompress_group_handle;
        }
    }

    return -1;
}

// sub_A15E60
void treyarch::nfl::system::cancel_request(i32 id) {
    request_t* request = get_request(id);

    if (!request)
        return;

    const i32 state = request->state;

    nfl::log.dbg("cancel {} \"{}\" -- {}",
                 id,
                 file_path(request->file),
                 request_state_text((e_request_state)state));

    // anything with I/O or decompression in flight has to be stopped by the driver first
    if (state == request_state_working || request->active_decompress_jobs) {
        request->state = request_state_canceling;

        SetEvent(cancel_event);
    } else if (state == request_state_io_error_waiting ||
               state == request_state_io_error ||
              (state != request_state_canceling && state != request_state_workdone))

        request->state = request_state_canceled;
}

// sub_A17310
DWORD __stdcall treyarch::nfl::system::update_thread(void* parameter) {
    for (;;) {
        WaitForSingleObject(event, INFINITE);

        lock();

        const bool exit   = thread_exit != 0;
        const i32  result = update();

        unlock();

        if (exit)
            return 0;

        if (!result)
            continue;

        if (alertable_wait) {
            if (result == 1) {
                HANDLE handles[2] = { driver::win32_io_work.io_event, cancel_event };

                WaitForMultipleObjectsEx(2, handles, FALSE, INFINITE, TRUE);
            }
        } else
            Sleep(thread_sleep);

        if (init_params.thread_mode == 1)
            SetEvent(event);
    }
}

// sub_A171A0
i32 treyarch::nfl::system::update() { // 1 while a driver has I/O going, 2 while chunk jobs are pending
    i32 can_schedule[32];
    i32 scheduled[32];

    for (i32 index = 0; index < drivers_count; ++index)
        can_schedule[index] = update_driver(drivers[index]) && driver::find_unlocked_read_buffer(drivers[index]) != -1;

    for (i32 index = 0; index < drivers_count; ++index) {
        scheduled[index] = can_schedule[index] ?
            schedule_request(drivers_count == 1 ? nullptr : drivers[index]) : -1;
    }

    for (i32 index = 0; index < drivers_count; ++index) {
        if (scheduled[index] != -1)
            execute_request(drivers[index], scheduled[index]);
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
i32 treyarch::nfl::system::update_driver(driver::driver_t* driver) { // nonzero when the driver can take a new command
    update_chunk_decompression_jobs(driver);

    if (!driver->io_state)
        return 1;

    // completions run in here
    if (driver->io_operations->update && driver->io_operations->update())
        return 0;

    driver::disk_read_t &disk_read = driver->disk_read;

    const i32  id      = disk_read.io_command.request;
    request_t* request = get_request(id);

    if (request) {
        switch (driver->io_state) {
            case driver::io_state_working:
                if (request->state == request_state_canceling) {
                    if (driver->io_operations->cancel && driver->io_operations->cancel(disk_read.io_command.file_handle))
                        return 0;

                    driver->io_state = driver::io_state_canceling;
                }

                return 0;

            case driver::io_state_workdone:
                if (request->state != request_state_canceling) {
                    if (!disk_read.bytes_completed)
                        request->state = request_state_workdone;
                    else if (request->decompress) {
                        request->state = request->bytes_read == request->disk_size && !request->active_decompress_jobs ?
                            request_state_workdone : request_state_waiting;

                        request->bytes_read += disk_read.bytes_completed;

                        if (request->bytes_scheduled >= request->output_size)
                            driver::unlock_read_buffer(driver, id, request->read_buffer_slot);
                        else {
                            begin_segment_decompress(driver, request);
                            update_chunk_decompression_jobs(driver);
                        }
                    } else {
                        std::memcpy(request->destination + request->bytes_read, disk_read.io_command.buffer, disk_read.bytes_completed);

                        request->bytes_read += disk_read.bytes_completed;

                        request->state = request->bytes_read != request->disk_size ?
                            request_state_waiting : request_state_workdone;

                        driver::unlock_read_buffer(driver, id, request->read_buffer_slot);
                    }

                    break;
                }

                [[fallthrough]];

            case driver::io_state_canceled:
                driver::unlock_read_buffer(driver, id, request->read_buffer_slot);

                if (!request->active_decompress_jobs)
                    request->state = request_state_canceled;

                break;

            case driver::io_state_canceling:
                return 0;

            case driver::io_state_error:
                // stays here, stopping all streaming, until the request is canceled and freed
                if (request->state != request_state_io_error_waiting) {
                    driver::unlock_read_buffer(driver, id, disk_read.io_command.read_buffer_slot);

                    request->state = request_state_io_error;
                }

                return 0;

            default:
                tl_fatal("nfsUpdateDriver: invalid state %s", 0);

                return 0;
        }
    }

    driver->io_state = driver::io_state_idle;

    disk_read.io_command.size             = 0;
    disk_read.io_command.file_handle      = nullptr;
    disk_read.io_command.offset           = 0;
    disk_read.io_command.request          = -1;
    disk_read.io_command.type             = -1;
    disk_read.io_command.read_buffer_slot = -1;

    return 1;
}

// sub_A164E0
i32 treyarch::nfl::system::schedule_request(driver::driver_t* driver) {
    u32 best_priority = 0;
    u32 best_age      = 0xFFFFFFFF;
    i32 best_id       = -1;

    for (i32 next_id = tx::slot_first(&requests_pool); next_id != -1;) {
        const i32  id      = next_id;
        request_t* request = get_request(id);

        next_id = tx::slot_next(&requests_pool, id);

        if (!request->driver && !open_file_async(id)) {
            lock();

            cancel_request(id);

            unlock();

            continue;
        }

        if (request->state != request_state_waiting || request->bytes_read == request->disk_size)
            continue;

        if (driver && get_file_driver(request->file) != driver)
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
nfl::driver::e_error treyarch::nfl::system::execute_request(driver::driver_t* driver, i32 id) {
    if (driver->io_state != driver::io_state_idle)
        return driver::error_invalid_state;

    request_t* request = get_request(id);

    driver::lock_read_buffer(driver, id, driver::find_unlocked_read_buffer(driver));

    file_t* file = get_file(request->file);

    driver::io_command_t command;

    command.offset           = request->bytes_read + request->offset;
    command.type             = request->type;
    command.read_buffer_slot = request->read_buffer_slot;
    command.request          = id;
    command.file_handle      = get_file_handle(request->file);
    command.file_size        = file->size;

    if (request->type) {
        command.buffer = request->destination;
        command.size   = request->output_size;
    } else {
        const driver::buffer_t* buffers = driver->buffers_object;

        command.buffer = buffers->buffers[command.read_buffer_slot].data;
        command.size   = request->disk_size - request->bytes_read;

        if (command.size > buffers->buffer_size)
            command.size = buffers->buffer_size;
    }

    if (file->file_type == file_type_subfile) {
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
        command.file_handle = get_file_handle(file->as.subfile.parent);
    }

    request->state = request_state_working;

    // the one point in a pass where the main thread can get in
    unlock();

    const driver::e_error error = driver::io_execute(driver, &command);

    lock();

    request = get_request(id);

    if (request && request->state == request_state_working && error)
        request->state = request_state_io_error;

    return error;
}

// sub_A16380
void treyarch::nfl::system::begin_segment_decompress(driver::driver_t* driver, request_t* request) { // the read buffer stays locked until its jobs finish
    const driver::io_command_t &io_command = driver->disk_read.io_command;

    request->read_buffer_slot = -1;

    const i32 chunk_count = ((chunk_header*)io_command.buffer)->num_chunks_in_segment;

    if (std::memcmp(((chunk_header*)io_command.buffer)->chunk_id, "NCH", 4))
        nfl::log.err("request {} \"{}\" -- invalid chunk header at 0x{:X}",
                     io_command.request,
                     file_path(request->file),
                     io_command.offset);
    else if (chunk_count <= 0)
        nfl::log.err("request {} \"{}\" -- {} chunks in the segment at 0x{:X}",
                     io_command.request,
                     file_path(request->file),
                     chunk_count,
                     io_command.offset);

    u32 offset = 0;

    for (i32 index = 0; index < chunk_count; ++index) {
        auto* record = (chunk_header*)((u8*)io_command.buffer + offset);

        const i32 job_id = tx::slot_new(&chunk_decompress_job_pool);

        if (job_id == -1)
            nfl::log.err("request {} \"{}\" -- out of chunk jobs ({}), {} chunks in the segment",
                         io_command.request,
                         file_path(request->file),
                         chunk_decompress_job_pool.count,
                         chunk_count);

        chunk_decompress_job* job = get_chunk_decompress_job(job_id);

        job->request = io_command.request;
        job->record  = record;
        job->state   = 0;
        job->output  = request->destination + record->prior_output;
        job->header  = *record;

        ++request->active_decompress_jobs;
        ++driver->chunk_decompress_jobs;

        request->bytes_scheduled += record->output_size;

        offset += record->span;
    }
}

// sub_A16D60
void treyarch::nfl::system::update_chunk_decompression_jobs(driver::driver_t* driver) {
    tx::slot_pool* jobs_pool = &chunk_decompress_job_pool;

    for (i32 next_id = tx::slot_first(jobs_pool); next_id != -1;) {
        const i32             id      = next_id;
        chunk_decompress_job* job     = get_chunk_decompress_job(id);
        request_t*            request = get_request(job->request);

        next_id = tx::slot_next(jobs_pool, id);

        if (job->state == 0) {
            create_jq_decompress_job(job, request);

            job->state = 1;
        } else if (job->state != 1)
            continue;

        // the whole group has to finish, so all of a request's jobs complete in the same walk
        if (decompress_on_jq) {
            const i32 batch_count = retail::sub_A14260(request->group_handle); // jq_get_batch_count

            if (batch_count + retail::sub_A14280(request->group_handle)) // jq_get_executing_batch_count
                continue;
        }

        --request->active_decompress_jobs;
        --driver->chunk_decompress_jobs;

        request->bytes_decompressed += job->header.output_size;

        if (!request->active_decompress_jobs) {
            for (i32 slot = 0; slot < 2; ++slot) {
                if (request->read_buffers_locked[slot] && slot != request->read_buffer_slot)
                    driver::unlock_read_buffer(driver, job->request, slot);
            }

            if ((request->bytes_decompressed == request->output_size && request->bytes_read == request->disk_size) ||
                request->state == request_state_canceling) {

                if (request->state == request_state_waiting && request->bytes_read == request->disk_size)
                    request->state = request_state_workdone;
                else if (request->read_buffer_slot == -1)
                    request->state = request_state_canceled;
            }
        }

        tx::slot_free(jobs_pool, id);
    }
}

// sub_A16D00
void treyarch::nfl::system::create_jq_decompress_job(chunk_decompress_job* job, request_t* request) {
    jq::batch batch; // payload[2], tag and unk_24 stay uninitialized

    batch.payload[0] = job->record;
    batch.payload[1] = job->output;
    batch.count      = 0;
    batch.unk_10[0]  = 0;
    batch.unk_10[1]  = 0;
    batch.descriptor = &nfl_jq_decompress_descriptor;
    batch.group      = request->group_handle;

    if (decompress_on_jq)
        retail::sub_A143E0((u64*)&batch, 0); // jq_add_batch
    else
        nfl_jq_decompress_module(&batch);
}

const char* treyarch::nfl::system::request_state_text(e_request_state state) {
    switch (state) {
        case request_state_waiting:
            return "NFS_REQUEST_STATE_WAITING";

        case request_state_working:
            return "NFS_REQUEST_STATE_WORKING";

        case request_state_workdone:
            return "NFS_REQUEST_STATE_WORKDONE";

        case request_state_canceling:
            return "NFS_REQUEST_STATE_CANCELING";

        case request_state_canceled:
            return "NFS_REQUEST_STATE_CANCELED";

        case request_state_timeout:
            return "NFS_REQUEST_STATE_TIMEOUT";

        case request_state_io_error:
            return "NFS_REQUEST_STATE_IO_ERROR";

        case request_state_io_error_waiting:
            return "NFS_REQUEST_STATE_IO_ERROR_WAITING";

        default:
            return "NFS_REQUEST_STATE_???";
    }
}

const char* treyarch::nfl::system::file_path(i32 id) { // "?" for a dead id
    const file_t* file = get_file(id);

    return file ? file->path : "?";
}

const char* treyarch::nfl::system::request_path(i32 id) { // "?" for a dead id
    const request_t* request = get_request(id);

    return request ? file_path(request->file) : "?";
}

const char* treyarch::nfl::system::file_handle_path(const void* handle) { // from a file's slot in the handle space
    const u32 index = ((const u8*)handle - file_handles) / file_handle_size;

    return files[index].path;
}
