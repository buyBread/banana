#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch {
    struct tl_init_list {
        tl_init_list();
        virtual ~tl_init_list() = default;
        virtual void register_item() = 0;

        tl_init_list* next;
    };

    struct tl_init_list_function : tl_init_list {
        using callback_type = void(__cdecl*)();

        explicit tl_init_list_function(callback_type callback);

        void register_item() override;

        callback_type callback;
    };

    namespace references {
        inline util::memory_reference<tl_init_list*> tl_init_list_head { 0x01115B5C };
    } // references

    ASSERT_SIZEOF  (tl_init_list,       0x08);
    ASSERT_OFFSETOF(tl_init_list, next, 0x04);

    ASSERT_SIZEOF  (tl_init_list_function,           0x0C);
    ASSERT_OFFSETOF(tl_init_list_function, callback, 0x08);
} // treyarch
