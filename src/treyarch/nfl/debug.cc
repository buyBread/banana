#include "treyarch/nfl/system.hh"

using namespace treyarch;

const char* treyarch::nfl::nfs_request_state_text(e_nfs_request_state state) {
    switch (state) {
        case nfs_request_state_waiting:
            return "NFS_REQUEST_STATE_WAITING";

        case nfs_request_state_working:
            return "NFS_REQUEST_STATE_WORKING";

        case nfs_request_state_workdone:
            return "NFS_REQUEST_STATE_WORKDONE";

        case nfs_request_state_canceling:
            return "NFS_REQUEST_STATE_CANCELING";

        case nfs_request_state_canceled:
            return "NFS_REQUEST_STATE_CANCELED";

        case nfs_request_state_timeout:
            return "NFS_REQUEST_STATE_TIMEOUT";

        case nfs_request_state_io_error:
            return "NFS_REQUEST_STATE_IO_ERROR";

        case nfs_request_state_io_error_waiting:
            return "NFS_REQUEST_STATE_IO_ERROR_WAITING";

        default:
            return "NFS_REQUEST_STATE_???";
    }
}

const char* treyarch::nfl::nfs_file_path(i32 id) { // "?" for a dead id
    const nfs_file* file = nfs_get_file(id);

    return file ? file->path : "?";
}

const char* treyarch::nfl::nfs_request_path(i32 id) { // "?" for a dead id
    const nfs_request* request = nfs_get_request(id);

    return request ? nfs_file_path(request->file) : "?";
}

const char* treyarch::nfl::nfs_file_handle_path(const void* handle) { // from a file's slot in the handle space
    const u32 index = ((const u8*)handle - references::nfs_file_handles.read()) / references::nfs_file_handle_size.read();

    return references::nfs_files.read()[index].path;
}
