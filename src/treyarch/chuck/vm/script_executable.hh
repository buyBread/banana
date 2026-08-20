#pragma once

#include "treyarch/shared/mash/string.hh"
#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    namespace fn {
        inline auto executable_find_thread =
            (vm_thread*(__thiscall*)
            (script_executable*, u32))
            0x00A1FDE0;

        inline auto executable_find_object_by_index =
            (script_object*(__thiscall*)
            (script_executable*, u32))
            0x00A1F790;

        inline auto executable_find_object_by_hash =
            (script_object*(__thiscall*)
            (script_executable*, u32, i32*))
            0x00A1F6C0;

        inline auto executable_resolve_code_locator =
            (u8*(__thiscall*)
            (script_executable*, u32))
            0x00A1F650;
    }

    /*
        if you have a pointer to an object of this type,
        expect it to be dangling, as the manager regularly discards executables.
    */
    struct script_executable {
        script_executable* self;
        u32                name_length;
        char*              name;
        u32                unk_0c;
        u32                resource_hash;
        u32                object_container;
        u32                object_count;
        script_object**    objects;
        u32                object_capacity;
        u32                object_state;
        script_object*     global_object;
        u32                initializer_container;
        u32                initializer_count;
        void**             initializers;
        u32                initializer_capacity;
        u32                initializer_state;
        u32                permanent_string_container;
        u32                permanent_string_count;
        mash::string**     permanent_strings;
        u32                permanent_string_capacity;
        u32                permanent_string_state;
        u8*                code;
        u32                code_size;
        u32                unk_5c;
        u32                unk_60;
        u32                flags; //     0x1: linked
                                  //   0x100: skip/defer first-run
                                  //   0x200: first-run called
                                  //  0x8000: scheduler run-needed/last-run-had-threads
                                  // 0x20000: published after manager insertion
        u32                runtime_state;

        vm_thread* find_thread(u32 id) {
            return fn::executable_find_thread(this, id);
        }

        script_object* object(u32 index) {
            if (!objects || index >= object_count)
                return nullptr;

            return fn::executable_find_object_by_index(this, index);
        }

        script_object* find_object(u32 hash, i32* index = nullptr) {
            return fn::executable_find_object_by_hash(this, hash, index);
        }

        u8* resolve_code_locator(u32 locator) {
            return fn::executable_resolve_code_locator(this, locator);
        }
    };
}}} // treyarch::chuck::vm