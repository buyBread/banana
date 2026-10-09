#pragma once

#include "util/memory_reference.hh"
#include "util/types.hh"

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

    // overlay of the engine's singleton base; its one vtable slot is the deleting destructor
    template <class T, u32 address>
    class singleton : public singleton_instance<T, address> {

    public:
        void** vtable;

        singleton           (const singleton&) = delete;
        singleton &operator=(const singleton&) = delete;

    protected:
        singleton() : vtable((void**)&references::singleton_vtable.get()) {}
    };
} // treyarch
