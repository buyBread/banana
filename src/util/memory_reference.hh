#pragma once

#include "util/types.hh"

namespace util {
    template <typename T>
    class memory_reference {

    private:
        T* m_address;
        
    public:
        explicit memory_reference(u32 address) noexcept :
            m_address((T*)address) {}
        
        T read() const {
            return *m_address;
        }

        void write(const T& value) const {
            *m_address = value;
        }

        T& get() const noexcept {
            return *m_address;
        }
    };
}