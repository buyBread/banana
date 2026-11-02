#include <cstring>

#include "treyarch/nfl/nfl.hh"
#include "treyarch/nfl/system.hh"
#include "treyarch/base/tl_system.hh"

using namespace treyarch;

// sub_A15530
i32 treyarch::nfl::nfs_open_file(u32 media_mask, const char* path, u32 flags, u32* size) {
    char bound_path[0x100];

    // every media bit gets its own try, on the first driver that binds it
    for (i32 bit = 0; bit < 32; ++bit) {
        const u32 media = media_mask & (1 << bit);

        if (!media)
            continue;

        nfd_driver* driver = nfs_get_media_driver(media);

        if (!driver || driver->binding->bind(media, path, bound_path, sizeof(bound_path)))
            continue;

        nfs_lock();

        const i32 id = tx::slot_new(&references::nfs_files_pool.get());

        if (id == -1) {
            nfs_unlock();

            return -1;
        }

        nfs_file* file = nfs_get_file(id);

        nfs_unlock();

        file->file_type                  = nfs_file_type_native;
        file->as.native.child_count      = 0;
        file->as.native.pending_requests = 0;
        file->as.native.flags            = flags;
        file->as.native.driver           = driver;
        file->as.native.media            = media;

        std::strcpy(file->path, bound_path);

        if (flags & nfd_file_flag_lazy_open) {
            file->size             = 0;
            file->as.native.driver = nullptr;
            file->as.native.media  = media_mask;

            return id;
        }

        u8* handle = nfs_get_file_handle(id);

        nfd_file_info info;

        if (!driver->file_operations->open(handle, bound_path, flags, size ? *size : 0) &&
            !driver->file_operations->status(handle, &info)) {

            if (size)
                *size = info.size;

            file->size = info.size;

            return id;
        }

        tx::slot_free(&references::nfs_files_pool.get(), id);
    }

    return -1;
}

// sub_A158E0
bool treyarch::nfl::nfs_open_file_async(i32 id) {
    nfs_lock();

    nfs_request* request = nfs_get_request(id);
    nfs_file*    file    = request ? nfs_get_file(request->file) : nullptr;

    if (!file) {
        nfs_unlock();

        return false;
    }

    nfs_file local_file = *file;

    if (local_file.as.native.driver) {
        bool assigned = false;

        if (!request->driver) {
            request->driver = local_file.as.native.driver;
            assigned        = true;
        }

        nfs_unlock();

        return assigned;
    }

    nfs_unlock();

    char          bound_path[0x100];
    u8            local_handle[0x100];
    nfd_file_info info;

    for (i32 bit = 0; bit < 32; ++bit) {
        const u32 media = local_file.as.native.media & (1 << bit);

        if (!media)
            continue;

        nfd_driver* driver = nfs_get_media_driver(media);

        if (!driver ||
            driver->binding->bind(media, local_file.path, bound_path, sizeof(bound_path)) ||
            driver->file_operations->open(local_handle, bound_path, local_file.as.native.flags, 0) ||
            driver->file_operations->status(local_handle, &info))

            continue;

        nfs_lock();

        request = nfs_get_request(id);
        file    = nullptr;

        if (request) {
            request->driver = driver;

            file = nfs_get_file(request->file);

            if (file) {
                std::memcpy(nfs_get_file_handle(request->file), local_handle, references::nfs_file_handle_size.read());

                file->size             = info.size;
                file->as.native.driver = driver;
                file->as.native.media  = media;
            }
        }

        nfs_unlock();

        return request && file;
    }

    return false;
}

// sub_A15CE0
i32 treyarch::nfl::open_file(u32 media_mask, const char* path) {
    return nfs_open_file(media_mask, path, nfd_file_flag_read, nullptr);
}

// sub_A15D00
void treyarch::nfl::nfd_close_file(i32 id) {
    nfs_file* file = nfs_get_file(id);

    if (!file || !file->as.native.driver)
        return;

    nfd_driver* driver = file->as.native.driver;
    u8*         handle = nfs_get_file_handle(id);

    if (handle)
        driver->file_operations->close(handle);

    tx::slot_free(&references::nfs_files_pool.get(), id);
}

// sub_A16AD0
void treyarch::nfl::nfs_close_file(i32 id, bool deferred) {
    nfs_file* file = nfs_get_file(id);

    if (!file)
        return;

    nfs_lock();

    tx::slot_pool* requests_pool = &references::nfs_requests_pool.get();

    for (i32 request_id = tx::slot_first(requests_pool); request_id != -1;) {
        nfs_request* request = nfs_get_request(request_id);
        i32          next_id = tx::slot_next(requests_pool, request_id);

        if (request->file == id) {
            if (deferred)
                ++file->as.native.pending_requests;

            nfs_cancel_request(request_id);
        }

        request_id = next_id;
    }

    if (file->file_type) {
        if (file->file_type == nfs_file_type_subfile) {
            const i32 parent_id = file->as.subfile.parent;

            --nfs_get_file(parent_id)->as.native.child_count;
            nfs_close_file(parent_id, false);
        } else
            tl_fatal("nflCloseFile: Invalid filetype");
    } else if (!file->as.native.child_count) {
        // closing now would pull the file out from under requests that are still being canceled
        if (file->as.native.pending_requests && deferred)
            file->as.native.flags |= nfd_file_flag_deferred_close;
        else
            nfd_close_file(id);
    }

    nfs_unlock();
}

// sub_A16CF0
void treyarch::nfl::close_file(i32 id) {
    nfs_close_file(id, true);
}
