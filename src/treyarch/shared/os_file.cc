#include <cstring>

#include "treyarch/shared/os_file.hh"

using namespace treyarch;

// sub_9C8C30
os_file::os_file() {
    flags         = (e_mode_flags)0;
    opened        = false;
    eof           = false;
    position      = 0;
    buffer_start  = 0;
    buffer_length = 0;
    fd            = INVALID_HANDLE_VALUE;
    buffer_index  = -1;
    buffered      = references::os_file_default_buffered.read();
}

// sub_9CC4E0
os_file::~os_file() {
    if (opened)
        close();
}

// sub_9C9A70
void os_file::open(const mash::string &file_name, e_mode_flags mode) {
    name = get_full_path(file_name);

    DWORD access      = (DWORD)-1;
    DWORD disposition = (DWORD)-1;
    DWORD share       = (DWORD)-1;

    flags = mode;

    switch (mode) {
        case file_read:
            access      = GENERIC_READ;
            disposition = OPEN_EXISTING;
            share       = FILE_SHARE_READ;

            break;

        case file_write:
            access      = GENERIC_WRITE;
            disposition = CREATE_ALWAYS;
            share       = 0;

            break;

        case file_modify:
            access      = GENERIC_READ | GENERIC_WRITE;
            disposition = OPEN_ALWAYS;
            share       = 0;

            break;

        case file_append:
            access      = GENERIC_WRITE;
            disposition = OPEN_ALWAYS;
            share       = 0;

            break;
    }

    buffer_length = 0;

    fd = CreateFileA(name.c_str(),
                     access,
                     share,
                     nullptr,
                     disposition,
                     FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL,
                     nullptr);

    if (fd == INVALID_HANDLE_VALUE)
        opened = false;
    else {
        if (flags & file_read) {
            size          = GetFileSize(fd, nullptr);
            buffer_length = 0;
        } else {
            size          = 0;
            buffer_length = buffer_size;
        }

        buffer_start = 0;

        if (buffered) {
            u32 index = 0;

            while (references::os_file_buffer_in_use.get()[index]) {
                if (++index >= buffer_count)
                    break;
            }

            if (index < buffer_count)
                buffer_index = (i32)index;

            // with every buffer taken this marks the byte right after the table
            references::os_file_buffer_in_use.get().data()[index] = true;
        }

        opened   = true;
        position = 0;

        if (flags == file_append) {
            if (buffered && buffer_start)
                flush_buffer();

            position = size;
            SetFilePointer(fd, 0, nullptr, FILE_END);

            if (buffered && (flags & file_read)) {
                buffer_start  = position;
                buffer_length = 0;

                fill_buffer();
            }
        }

        eof = position >= size;
    }

    os_file_callback callback = references::os_file_write_open_callback.read();

    if (callback && flags == file_write && opened)
        callback(this);
}

// sub_9C9C00
void os_file::close() {
    if (buffered) {
        if (!(flags & file_read) && references::os_file_buffers.get().data()[buffer_index] && buffer_start)
            flush_buffer();

        references::os_file_buffer_in_use.get().data()[buffer_index] = false;
    }

    CloseHandle(fd);

    fd       = INVALID_HANDLE_VALUE;
    opened   = false;
    eof      = true;
    position = 0;
    size     = 0;
    name     = "";

    buffer_length = 0;
    buffer_start  = 0;
    buffer_index  = -1;
}

// sub_9C9C70
u32 os_file::read(void* data, u32 bytes) {
    if (!bytes)
        return 0;

    if (position + bytes > size)
        bytes = size - position;

    u8* destination = (u8*)data;
    u32 bytes_read  = 0;

    // anything bigger goes through in buffer-sized pieces first
    if (bytes > buffer_size) {
        for (u32 pieces = ((bytes - (buffer_size + 1)) >> 21) + 1; pieces; --pieces) {
            bytes_read  += read(destination, buffer_size);
            destination += buffer_size;
            bytes       -= buffer_size;
        }
    }

    if (buffered) {
        if (size && bytes) {
            u8* buffer = references::os_file_buffers.get().data()[buffer_index];

            if (position + bytes > buffer_start + buffer_length) {
                u32 buffered_tail = buffer_start - position + buffer_length;

                if (buffered_tail) {
                    bytes_read  += read(destination, buffered_tail);
                    destination += buffered_tail;
                    bytes       -= buffered_tail;
                }

                u32 consumed = buffer_length;

                fill_buffer();
                buffer_start += consumed;
            }

            std::memcpy(destination, buffer + (position - buffer_start), bytes);

            bytes_read += bytes;
            position   += bytes;
        }
    } else {
        DWORD read_count = 0;

        ReadFile(fd, destination, bytes, &read_count, nullptr);

        bytes_read += read_count;
        position   += read_count;
    }

    eof = position >= size;

    return bytes_read;
}

// sub_9C8D10
u32 os_file::get_size() const {
    return opened ? size : (u32)-1;
}

// sub_9C8D90
bool os_file::file_exists(const mash::string &file_name) {
    DWORD attributes = GetFileAttributesA(get_full_path(file_name).c_str());

    // a missing file reports every attribute bit, so it counts as a directory here
    return !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

// sub_9C8D40
mash::string os_file::get_full_path(const mash::string &file_name) {
    const char* path = file_name.c_str();

    if (path[0] == '\\' || path[1] == ':')
        return file_name;

    return references::image_root.get() + file_name;
}

// sub_9C8D20
const mash::string &os_file::get_image_root() {
    return references::image_root.get();
}

// sub_9C8D30
const mash::string &os_file::get_data_root() {
    return references::data_root.get();
}

// sub_9C8C70
DWORD os_file::fill_buffer() {
    DWORD read_count = 0;
    u32   count      = buffer_start + buffer_length + buffer_size > size ?
        size - buffer_start - buffer_length : buffer_size;

    ReadFile(fd, references::os_file_buffers.get().data()[buffer_index], count, &read_count, nullptr);

    buffer_length = read_count;

    return read_count;
}

// sub_9C8CD0
DWORD os_file::flush_buffer() {
    DWORD written_count = 0;

    WriteFile(fd, references::os_file_buffers.get().data()[buffer_index], buffer_start, &written_count, nullptr);

    buffer_start = 0;

    return written_count;
}
