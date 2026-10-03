#pragma once

#include <array>
#include <windows.h>

#include "treyarch/shared/mash/string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class os_file;

    using os_file_callback = void (__cdecl*)(os_file* file);

    // synchronous win32 file wrapper
    class os_file {

    public:
        enum e_mode_flags : u32 {
            file_read = 1,
            file_write,
            file_modify,
            file_append
        };

        static constexpr u32 buffer_count = 10;
        static constexpr u32 buffer_size  = 0x200000;

        mash::string name;
        e_mode_flags flags;
        bool         opened;
        bool         eof;
        u8           pad_12[2];
        HANDLE       fd;
        u32          position;
        u32          size;
        bool         buffered;
        u8           pad_21[3];
        i32          buffer_index;  // -1 when none
        u32          buffer_start;  // file offset of the buffered bytes; pending bytes when writing
        u32          buffer_length; // buffered bytes; the capacity when writing

        os_file();
       ~os_file();

        void open(const mash::string &file_name, e_mode_flags mode);
        void close();
        u32  read(void* data, u32 bytes);

        u32 get_size() const;

        static bool file_exists(const mash::string &file_name);
        static mash::string get_full_path(const mash::string &file_name);

        static const mash::string &get_image_root();
        static const mash::string &get_data_root();

    private:
        DWORD fill_buffer();
        DWORD flush_buffer();
    };

    namespace references {
        // set before any files are opened (see WinMain); relative names are opened from image_root
        inline util::memory_reference<mash::string> data_root  { 0x01113794 };
        inline util::memory_reference<mash::string> image_root { 0x011137BC };

        // nothing allocates the buffers or sets the flag, so buffering is always off on pc
        inline util::memory_reference<std::array<u8*, os_file::buffer_count>>  os_file_buffers        { 0x01113708 };
        inline util::memory_reference<std::array<bool, os_file::buffer_count>> os_file_buffer_in_use { 0x01113730 };

        inline util::memory_reference<bool> os_file_default_buffered { 0x011136F9 };

        // never set; called after a successful file_write open
        inline util::memory_reference<os_file_callback> os_file_write_open_callback { 0x0111373C };
    } // references

    ASSERT_SIZEOF  (os_file,                0x30);
    ASSERT_OFFSETOF(os_file, flags,         0x0C);
    ASSERT_OFFSETOF(os_file, opened,        0x10);
    ASSERT_OFFSETOF(os_file, eof,           0x11);
    ASSERT_OFFSETOF(os_file, fd,            0x14);
    ASSERT_OFFSETOF(os_file, position,      0x18);
    ASSERT_OFFSETOF(os_file, size,          0x1C);
    ASSERT_OFFSETOF(os_file, buffered,      0x20);
    ASSERT_OFFSETOF(os_file, buffer_index,  0x24);
    ASSERT_OFFSETOF(os_file, buffer_start,  0x28);
    ASSERT_OFFSETOF(os_file, buffer_length, 0x2C);
} // treyarch
