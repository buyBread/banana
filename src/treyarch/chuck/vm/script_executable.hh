#pragma once

#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    using resolve_signal_callback_t               = void(*)(const char* signal_name, u32* signal_id);
    using resolve_extern_callback_t               = u32(*)(const char* script_object_name, const char* instance_name);
    using get_chuck_client_library_key_callback_t = u32(*)();
    using get_script_executable_folder_callback_t = mash::string(*)();

    namespace references {
        // installed by sub_843300; only resolve_extern_callback has a retail consumer (linker, EI operands)
        inline util::memory_reference<resolve_signal_callback_t>               resolve_signal_callback               { 0x01124C48 };
        inline util::memory_reference<resolve_extern_callback_t>               resolve_extern_callback               { 0x01124C4C };
        inline util::memory_reference<get_chuck_client_library_key_callback_t> get_chuck_client_library_key_callback { 0x01124C50 };
        inline util::memory_reference<get_script_executable_folder_callback_t> get_script_executable_folder_callback { 0x01124C54 };
    } // references

    enum e_script_executable_flags : u32 {
        script_executable_flag_linked                       = 0x00001,
        script_executable_flag_from_mash                    = 0x00002,
        script_executable_flag_un_mashed                    = 0x00004, // set right after fixup (sub_A1FED0)
        script_executable_flag_awaiting_initial_breakpoints = 0x00008,
        script_executable_flag_has_initial_breakpoints      = 0x00010,
        script_executable_flag_stall_for_breakpoints        = 0x00020,
        script_executable_flag_loading_now                  = 0x00040,
        script_executable_flag_linked_for_mash              = 0x00080,
        script_executable_flag_suspended_for_script_vars    = 0x00100, // a variable container was missing at load
        script_executable_flag_first_run_called             = 0x00200,
        script_executable_flag_unk_800                      = 0x00800, // every shipped PC image
        script_executable_flag_unk_1000                     = 0x01000, // every shipped console image
        script_executable_flag_unloading                    = 0x02000,
        script_executable_flag_running                      = 0x04000,
        script_executable_flag_needs_run                    = 0x08000, // the tick only runs roots with this set
        script_executable_flag_published                    = 0x20000
    };

    // first-run initializer record (sub_A1F920 reads offset..uint)
    class script_executable_object_instance_info {

    public:
        mash::string     inst_name;
        string_hash      so_name;
        script_function* parms;
        i32              offset; // into the global instance's data
        i32              id;     // -3 num, -4 made from num, -5 uint, >= 0 permanent string; -1/-2 skipped
        f32              num;
        u32              uint;
        i32              so_index;
    };

    /*
        pointers into a loaded executable dangle once the manager unloads it.
        the wordcode image is the last member of the normal mash stream, followed by up to 15 bytes of 0xA1 fill.
    */
    class script_executable {

    public:
        void*                                                self;
        mash::string                                         name;
        string_hash                                          resource_hash;
        mash::vector<script_object>                          script_objects;
        script_object*                                       global_script_object;
        mash::vector<script_executable_object_instance_info> object_instances;
        mash::vector<mash::string>                           permanent_string_table;
        u16*                                                 exe_image;
        i32                                                  exe_image_size;
        u32                                                  checksum;           // the milestone verifies it against exe_image; retail never reads it
        u32                                                  client_library_key; // 0x6C0AE071 in every shipped script; retail never reads it
        e_script_executable_flags                            flags;
        i32                                                  suspend_count;      // sub_A1FD60 skips the executable while positive

        static void register_callbacks(resolve_signal_callback_t               resolve_signal,
                                       resolve_extern_callback_t               resolve_extern,
                                       get_chuck_client_library_key_callback_t get_chuck_client_library_key,
                                       get_script_executable_folder_callback_t get_script_executable_folder);
    };

    ASSERT_OFFSETOF(script_executable_object_instance_info, offset, 0x14);
    ASSERT_OFFSETOF(script_executable_object_instance_info, id,     0x18);
    ASSERT_OFFSETOF(script_executable_object_instance_info, num,    0x1C);
    ASSERT_OFFSETOF(script_executable_object_instance_info, uint,   0x20);

    ASSERT_SIZEOF  (script_executable,                         0x6C);
    ASSERT_OFFSETOF(script_executable, self,                   0x00);
    ASSERT_OFFSETOF(script_executable, name,                   0x04);
    ASSERT_OFFSETOF(script_executable, resource_hash,          0x10);
    ASSERT_OFFSETOF(script_executable, script_objects,         0x14);
    ASSERT_OFFSETOF(script_executable, global_script_object,   0x28);
    ASSERT_OFFSETOF(script_executable, object_instances,       0x2C);
    ASSERT_OFFSETOF(script_executable, permanent_string_table, 0x40);
    ASSERT_OFFSETOF(script_executable, exe_image,              0x54);
    ASSERT_OFFSETOF(script_executable, exe_image_size,         0x58);
    ASSERT_OFFSETOF(script_executable, checksum,               0x5C);
    ASSERT_OFFSETOF(script_executable, client_library_key,     0x60);
    ASSERT_OFFSETOF(script_executable, flags,                  0x64);
    ASSERT_OFFSETOF(script_executable, suspend_count,          0x68);
}}} // treyarch::chuck::vm
