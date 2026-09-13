#pragma once

namespace util { namespace thunk {
    template <typename T, typename... args>
    struct ctor {
        static T* __thiscall invoke(T* self, args... a) {
            return new (self) T((args&&)a...);
        }
    };

    template <typename T>
    struct dtor {
        static void __thiscall invoke(T* self) {
            self->~T();
        }
    };
}} // util::thunk