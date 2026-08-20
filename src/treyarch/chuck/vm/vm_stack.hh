#pragma once

#include <cstring>

#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    struct vm_stack;
    struct vm_thread;

    namespace fn {
        inline auto stack_push_bytes =
            (void(__thiscall*)
            (vm_stack*, const void*, i32))
            0x004E4F70;
    } // fn

    struct vm_stack {
        u8*        cursor;     // next free byte
        void*      allocation; // the pool allocation handle/base
        u8*        data;       // current buffer base, may change after growth
        u32        capacity;
        vm_thread* thread;

        bool valid() const noexcept {
            if (!cursor || !data)
                return false;

            u32 cursor_address = (u32)cursor;
            u32 data_address   = (u32)data;

            return
                cursor_address                >= data_address &&
                cursor_address - data_address <= capacity;
        }

        u32 size() const noexcept {
            if (!valid())
                return 0;

            return (u32)((u32)cursor - (u32)data);
        }

        u32 remaining() const noexcept {
            return valid() ? capacity - size() : 0;
        }

        u32 capacity_after_one_growth() const noexcept {
            switch (capacity) {
                case 128: return 284;
                case 284: return 512;
                case 512: return 1024;
                default:  return capacity;
            }
        }

        bool can_push(u32 byte_count) const noexcept {
            if (!valid())
                return false;

            u32 used = size();

            if (byte_count > U32_MAX - used)
                return false;

            u32 required = used + byte_count;

            return required <= capacity_after_one_growth();
        }

        bool push_bytes(const void* source, u32 byte_count) {
            if (!byte_count)
                return valid();

            if (!source || byte_count > I32_MAX || !can_push(byte_count))
                return false;

            u32 previous_size = size();

            fn::stack_push_bytes(this, source, (i32)byte_count);

            return valid() && size() == previous_size + byte_count;
        }

        bool peek_bytes(void* destination, u32 byte_count) const {
            if (!byte_count)
                return valid();

            if (!destination || !valid() || byte_count > size())
                return false;

            std::memcpy(destination, cursor - byte_count, byte_count);

            return true;
        }

        bool pop_bytes(void* destination, u32 byte_count) {
            if (!peek_bytes(destination, byte_count))
                return false;

            cursor -= byte_count;

            return true;
        }

        bool push_number(f32 value) {
            return push_bytes(&value, sizeof(value));
        }

        bool pop_number(f32* value) {
            return pop_bytes(value, sizeof(*value));
        }

        bool push_unsigned(u32 value) {
            return push_bytes(&value, sizeof(value));
        }

        bool pop_unsigned(u32* value) {
            return pop_bytes(value, sizeof(*value));
        }
    };
}}} // treyarch::chuck::vm