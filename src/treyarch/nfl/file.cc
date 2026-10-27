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

        i32 driver_index = 0;

        for (; driver_index < references::nfs_drivers_count.read(); ++driver_index) {
            nfd_driver* candidate = references::nfs_drivers.read()[driver_index];

            if (candidate && candidate->binding && media & candidate->binding->media_mask)
                break;
        }

        if (driver_index >= references::nfs_drivers_count.read())
            continue;

        nfd_driver* driver = references::nfs_drivers.read()[driver_index];

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

        file->file_type             = nfs_file_type_native;
        file->as.native.child_count = 0;
        file->pending_closes        = 0;
        file->as.native.flags       = flags;
        file->as.native.driver      = driver;
        file->as.native.media       = media;

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
            !driver->file_operations->get_info(handle, &info)) {

            if (size)
                *size = info.size;

            file->size = info.size;

            return id;
        }

        tx::slot_free(&references::nfs_files_pool.get(), id);
    }

    return -1;
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
                ++file->pending_closes;

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
        if (file->pending_closes && deferred)
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
