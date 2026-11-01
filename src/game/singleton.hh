#pragma once

#include "treyarch/mm/mm.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace references {
        inline util::memory_reference<void*> singleton_vtable { 0x00B88744 };
    } // references

    // the static instance pointer every singleton class keeps, here at a fixed address
    template <class T, u32 address>
    class singleton_instance {

    public:
        static T*& instance() { return *(T**)address; }

        static T*   inst()    { return instance(); }
        static bool is_inst() { return instance() != nullptr; }
    };

    /* the engine's singleton base; its one vtable slot is the deleting destructor,
       and it keeps singleton_instance's statics without deriving from it, so the vptr stays its first word */
    template <class T, u32 address>
    class singleton {

    public:
        static T*& instance() { return singleton_instance<T, address>::instance(); }

        static T*   inst()    { return instance(); }
        static bool is_inst() { return instance() != nullptr; }

        singleton           (const singleton&) = delete;
        singleton &operator=(const singleton&) = delete;

        // sub_428930
        virtual ~singleton() = default;

        // inlined into the deleting destructors
        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

    protected:
        singleton() {
            *(void**)this = &references::singleton_vtable.get();
        }
    };
} // treyarch
