#pragma once

namespace util { namespace gimmie {
    template <typename T>
    inline void* virt(T* ptr, int idx) {
        void** vtable = *(void***)ptr;

        return vtable[idx];
    }
}} // util::gimmie