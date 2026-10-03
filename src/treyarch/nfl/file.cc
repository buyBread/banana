#include <cstring>

#include "treyarch/nfl/nfl.hh"
#include "treyarch/nfl/system.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

// sub_A15530
i32 treyarch::nfl::nfs_open_file(u32 media_mask, const char* path, u32 flags, u32* size) {
    char bound_path[0x100];

    // every media bit gets its own try, on the first driver that binds it
    for (i32 bit = 0; bit < 32; ++bit) {
        const u32 media = media_mask & (1 << bit);

        if (!media)
            continue;

        i32 driver_index = 0;

        for (; driver_index < references::driver_count.read(); ++driver_index) {
            driver* candidate = references::drivers.read()[driver_index];

            if (candidate && candidate->binding && media & candidate->binding->media_mask)
                break;
        }

        if (driver_index >= references::driver_count.read())
            continue;

        driver* driver = references::drivers.read()[driver_index];

        if (!driver || driver->binding->bind(media, path, bound_path, sizeof(bound_path)))
            continue;

        lock();

        const i32 id = references::file_pool.get().allocate();

        if (id == -1) {
            unlock();

            return -1;
        }

        file* file = get_file(id);

        unlock();

        file->type           = 0;
        file->children       = 0;
        file->pending_closes = 0;
        file->flags          = flags;
        file->driver         = driver;
        file->media          = media;

        std::strcpy(file->path, bound_path);

        if (flags & file_flag_lazy_open) {
            file->size   = 0;
            file->driver = nullptr;
            file->media  = media_mask;

            return id;
        }

        u8* handle = get_file_handle(resolve_file(id));

        driver_file_info info;

        if (!driver->file_operations->open(handle, bound_path, flags, size ? *size : 0) &&
            !driver->file_operations->get_info(handle, &info)) {

            if (size)
                *size = info.size;

            file->size = info.size;

            return id;
        }

        references::file_pool.get().release(id);
    }

    return -1;
}

// sub_A15CE0
i32 treyarch::nfl::open_file(u32 media_mask, const char* path) {
    return nfs_open_file(media_mask, path, file_flag_read, nullptr);
}

// sub_A15D00
void treyarch::nfl::nfd_close_file(i32 id) {
    file* file = get_file(id);

    if (!file || !file->driver)
        return;

    driver* driver = file->driver;
    u8*     handle = get_file_handle(resolve_file(id));

    if (handle)
        driver->file_operations->close(handle);

    references::file_pool.get().release(id);
}

// sub_A16AD0
void treyarch::nfl::nfs_close_file(i32 id, bool deferred) {
    file* file = get_file(id);

    if (!file)
        return;

    lock();

    pool &request_pool = references::request_pool.get();

    for (i32 request_id = request_pool.first_id(); request_id != -1;) {
        request* request = get_request(request_id);
        i32      next_id = request_pool.next_id(request_id);

        if (request->file == id) {
            if (deferred)
                ++file->pending_closes;

            nfs_cancel_request(request_id);
        }

        request_id = next_id;
    }

    if (file->type) {
        if (file->type == 1) {
            const i32 parent_id = (i32)file->driver;

            --get_file(parent_id)->children;
            nfs_close_file(parent_id, false);
        } else
            memory::report("nflCloseFile: Invalid filetype");
    } else if (!file->children) {
        // closing now would pull the file out from under requests that are still being canceled
        if (file->pending_closes && deferred)
            file->flags |= file_flag_deferred_close;
        else
            nfd_close_file(id);
    }

    unlock();
}

// sub_A16CF0
void treyarch::nfl::close_file(i32 id) {
    nfs_close_file(id, true);
}
