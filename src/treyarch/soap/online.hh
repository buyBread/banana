#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace soap {
    class online;

    using online_method     = void (__thiscall*)(online* self);
    using online_method_arg = void (__thiscall*)(online* self, i32 unk);

    struct online_vtable {
        void*             reserved_000[4]; // subject's attach, detach, notify, the deleting destructor
        online_method     initialize;
        online_method     frame_advance;
        void*             reserved_018[3];
        online_method_arg method_024;      // nullsub on pc; called with 0 right after context writes
    };

    class online {

    public:
        online_vtable* vtable;
        u8             reserved_004[0x28];
        i32            unk_02c;

        static online* inst();

        void initialize() {
            vtable->initialize(this);
        }

        void frame_advance() {
            vtable->frame_advance(this);
        }

        void method_024(i32 unk) {
            vtable->method_024(this, unk);
        }
    };

    namespace references {
        inline util::memory_reference<online*> online { 0x01123D00 };

        inline util::memory_reference<online_vtable> online_pc_vtable { 0x00DBC13C };
    } // references

    ASSERT_OFFSETOF(online_vtable, initialize,    0x10);
    ASSERT_OFFSETOF(online_vtable, frame_advance, 0x14);
    ASSERT_OFFSETOF(online_vtable, method_024,    0x24);

    ASSERT_SIZEOF  (online,          0x30);
    ASSERT_OFFSETOF(online, unk_02c, 0x2C);
}} // treyarch::soap
