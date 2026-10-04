#pragma once

#include <ctime>

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace soap {
    struct storage_article {
        tm       timestamp;
        i32      state;        // 0 none, 1 present; pc never produces 2 or 3
        char*    name;
        u8       unk_02c[0x08];
        wchar_t* display_name;
    };

    class storage;

    using storage_initialize  = void             (__thiscall*)(storage* self, i32 article_count, i32 unk, i32 payload_size);
    using storage_method      = void             (__thiscall*)(storage* self);
    using storage_query       = i32              (__thiscall*)(storage* self);
    using storage_test        = bool             (__thiscall*)(storage* self);
    using storage_article_get = storage_article* (__thiscall*)(storage* self, i32 user, i32 index);
    using storage_flag_set    = void             (__thiscall*)(storage* self, bool value);

    struct storage_vtable {
        void*               reserved_000[4]; // attach, detach, notify, destructor
        storage_initialize  initialize;
        storage_method      frame_advance;
        storage_query       busy;            // the pending service
        void*               reserved_01c[6];
        storage_article_get article;
        void*               reserved_038[3];
        storage_flag_set    set_must_show_active_device_unavailable;
        storage_method      method_048;
        storage_test        test_04c;
    };

    class storage {

    public:
        storage_vtable* vtable;
        u8              reserved_004[0x18];
        bool            must_show_error_box;
        bool            must_show_active_device_unavailable;

        void initialize(i32 article_count, i32 unk, i32 payload_size) {
            vtable->initialize(this, article_count, unk, payload_size);
        }

        void frame_advance() {
            vtable->frame_advance(this);
        }

        i32 busy() {
            return vtable->busy(this);
        }

        storage_article* article(i32 user, i32 index) {
            return vtable->article(this, user, index);
        }

        void set_must_show_active_device_unavailable(bool value) {
            vtable->set_must_show_active_device_unavailable(this, value);
        }

        void method_048() {
            vtable->method_048(this);
        }

        bool test_04c() {
            return vtable->test_04c(this);
        }
    };

    ASSERT_SIZEOF  (storage_article,               0x38);
    ASSERT_OFFSETOF(storage_article, state,        0x24);
    ASSERT_OFFSETOF(storage_article, name,         0x28);
    ASSERT_OFFSETOF(storage_article, display_name, 0x34);

    ASSERT_OFFSETOF(storage_vtable, initialize,                              0x10);
    ASSERT_OFFSETOF(storage_vtable, frame_advance,                           0x14);
    ASSERT_OFFSETOF(storage_vtable, busy,                                    0x18);
    ASSERT_OFFSETOF(storage_vtable, article,                                 0x34);
    ASSERT_OFFSETOF(storage_vtable, set_must_show_active_device_unavailable, 0x44);
    ASSERT_OFFSETOF(storage_vtable, method_048,                              0x48);
    ASSERT_OFFSETOF(storage_vtable, test_04c,                                0x4C);

    ASSERT_OFFSETOF(storage, must_show_error_box,                 0x1C);
    ASSERT_OFFSETOF(storage, must_show_active_device_unavailable, 0x1D);
}} // treyarch::soap
