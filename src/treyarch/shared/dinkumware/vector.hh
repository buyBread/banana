#pragma once

#include <utility>

#include "treyarch/shared/memory/heap.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace dinkumware {
    template<typename T>
    class vector_allocator {};

    template<typename T, typename allocator_t = vector_allocator<T>>
    class vector {

        allocator_t allocator;

        T* first;
        T* last;
        T* capacity_end;

    public:
        vector() noexcept : first(nullptr),
                            last(nullptr),
                            capacity_end(nullptr) {}

        ~vector() {
            destroy();
        }

        vector(const vector&) = delete;
        vector &operator=(const vector&) = delete;

              T &operator[](u32 index)       noexcept { return first[index]; }
        const T &operator[](u32 index) const noexcept { return first[index]; }

              T* begin()       noexcept { return first; }
        const T* begin() const noexcept { return first; }

              T* end()       noexcept { return last; }
        const T* end() const noexcept { return last; }

              T &back()       noexcept { return last[-1]; }
        const T &back() const noexcept { return last[-1]; }

        u32 size() const noexcept {
            return first ? (u32)(last - first) : 0;
        }

        u32 capacity() const noexcept {
            return first ? (u32)(capacity_end - first) : 0;
        }

        bool empty() const noexcept { return first == last; }

        void clear() noexcept {
            while (last != first) {
                --last;
                last->~T();
            }
        }

        void destroy() noexcept {
            clear();

            if (first)
                memory::heap::free(first);

            first        = nullptr;
            last         = nullptr;
            capacity_end = nullptr;
        }

        void assign(u32 count, const T &value) {
            if (capacity() < count) {
                destroy();

                first        = (T*)memory::heap::allocate(count * sizeof(T));
                last         = first;
                capacity_end = first + count;
            } else
                clear();

            while (size() < count) {
                new (last) T(value);

                ++last;
            }
        }

        void push_back(const T &value) {
            if (last == capacity_end)
                reserve_for_size(size() + 1);

            new (last) T(value);
            
            ++last;
        }

        void resize(u32 count, const T &value) {
            while (size() > count) {
                --last;
                last->~T();
            }

            if (capacity() < count)
                reserve_for_size(count);

            while (size() < count) {
                new (last) T(value);

                ++last;
            }
        }

        T* erase(T* position) noexcept {
            T* result = position;

            for (T* next = position + 1; next != last; ++position, ++next)
                *position = std::move(*next);

            --last;
            last->~T();

            return result;
        }

    private:
        void reserve_for_size(u32 requested_size) {
            u32 old_size     = size();
            u32 old_capacity = capacity();
            u32 new_capacity = old_capacity + old_capacity / 2;

            if (new_capacity < requested_size)
                new_capacity = requested_size;

            T* allocation = (T*)memory::heap::allocate(new_capacity * sizeof(T));

            for (u32 index = 0; index < old_size; ++index)
                new (allocation + index) T(std::move(first[index]));

            clear();

            if (first)
                memory::heap::free(first);

            first        = allocation;
            last         = allocation + old_size;
            capacity_end = allocation + new_capacity;
        }
    };

    ASSERT_SIZEOF(vector<void*>, 0x10);
}} // treyarch::dinkumware
