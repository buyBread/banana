#pragma once

#include <stdexcept>

#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace vc8 {
    template<typename T>
    class list_allocator {};

    template<typename T>
    struct list_node {
        list_node* next;
        list_node* previous;
        T          value;
    };

    template<typename T, typename allocator_t = list_allocator<T>>
    class list {

        allocator_t   allocator;
        list_node<T>* sentinel;
        u32           count;

    public:
        using node = list_node<T>;

        // sub_79B510
        // sub_7D23C0
        list() : sentinel((node*)mm::allocate(sizeof(node))),
                 count(0) {
                    
            sentinel->next     = sentinel;
            sentinel->previous = sentinel;
        }

        // sub_4ED040
        ~list() {
            clear();
            
            mm::free(sentinel);

            sentinel = nullptr;
        }

        // sub_6F04C0
        list(const list &other) : list() {
            for (node* position = other.begin(); position != other.end(); position = position->next)
                push_back(position->value);
        }

        list &operator=(const list&) = delete;

        node* head()  const noexcept { return sentinel; }
        node* begin() const noexcept { return sentinel->next; }
        node* end()   const noexcept { return sentinel; }

        u32  size()  const noexcept { return count; }
        bool empty() const noexcept { return count == 0; }

        node* insert_before(node* before, const T &value) {
            node* position = (node*)mm::allocate(sizeof(node));

            position->next     = before;
            position->previous = before->previous;

            new (&position->value) T(value);

            increment_size(1);

            before->previous         = position;
            position->previous->next = position;

            return position;
        }

        // sub_5CD230
        // sub_635E30
        // sub_A1AB20
        void increment_size(u32 added) {
            if (0xFFFFFFFF / sizeof(T) - count < added)
                throw std::length_error("list<T> too long");

            count += added;
        }

        node* push_back(const T &value) {
            return insert_before(sentinel, value);
        }

        node* erase(node* position) noexcept {
            node* next = position->next;

            position->previous->next = position->next;
            position->next->previous = position->previous;
            position->value.~T();
            mm::free(position);
            --count;

            return next;
        }

        // sub_4ECF90
        void clear() noexcept {
            node* position = sentinel->next;

            sentinel->next     = sentinel;
            sentinel->previous = sentinel;
            count              = 0;

            while (position != sentinel) {
                node* next = position->next;

                position->value.~T();
                mm::free(position);
                position = next;
            }
        }

        void relink_before(node* position, node* before) noexcept {
            position->previous->next = position->next;
            position->next->previous = position->previous;

            position->next           = before;
            position->previous       = before->previous;
            before->previous->next   = position;
            before->previous         = position;
        }
    };

    ASSERT_SIZEOF(list<void*>, 0x0C);

    ASSERT_SIZEOF(list_node<void*>, 0x0C);
}} // treyarch::vc8
