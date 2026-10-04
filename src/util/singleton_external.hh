#pragma once

#include "util/types.hh"

/*
    this is for the game's singletons;
    let's make it ABI compliant one day?
*/

namespace util {
    template <typename T, u32 address>
    class singleton_external {
        
    protected:
        singleton_external() = default;

        static void set(T* instance) {
            *(T**)address = instance;
        }

        static T* instance() {
            return *(T**)address;
        }

    public:
        singleton_external           (const singleton_external&) = delete;
        singleton_external& operator=(const singleton_external&) = delete;
        singleton_external           (singleton_external&&)      = delete;
        singleton_external& operator=(singleton_external&&)      = delete;

        static T &get() {
            T* instance = *(T**)address;
            
            assert(instance);

            return *instance;
        }
    };
} // util