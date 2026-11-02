#pragma once

#include <stdexcept>

#include "treyarch/mash/types.hh"
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

        // sub_58F4C0
        // sub_79B510
        // sub_7D23C0
        // sub_9CC540
        list() : sentinel((node*)mm::allocate(sizeof(node))),
                 count(0) {
                    
            sentinel->next     = sentinel;
            sentinel->previous = sentinel;
        }

        // leaves the list alone, for a containing class's generating constructor
        explicit list(mash::generating_vtable_version) noexcept {}

        // sub_4ED040
        // inlined @ sub_97A400
        // inlined @ sub_6F88B0
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

        T &front() const noexcept { return sentinel->next->value; }
        T &back()  const noexcept { return sentinel->previous->value; }

        u32  size()  const noexcept { return count; }
        bool empty() const noexcept { return count == 0; }

        // sub_4CC190
        // sub_4ED2B0
        static node* buy_node(node* next, node* previous, const T &value) {
            node* result = (node*)mm::allocate(sizeof(node));

            if (result) {
                result->next     = next;
                result->previous = previous;

                new (&result->value) T(value);
            }

            return result;
        }

        node* insert_before(node* before, const T &value) {
            node* position = buy_node(before, before->previous, value);

            increment_size(1);

            before->previous         = position;
            position->previous->next = position;

            return position;
        }

        // sub_4120D0
        // sub_4B5EA0
        // sub_4CC220
        // sub_5CD230
        // sub_635E30
        // sub_6CBC90
        // sub_6CBD30
        // sub_6CBF00
        // sub_6CC040
        // sub_6CC0E0
        // sub_749E80
        // sub_A1AB20
        // sub_A6B870
        void increment_size(u32 added) {
            if (0xFFFFFFFF / sizeof(T) - count < added)
                throw std::length_error("list<T> too long");

            count += added;
        }

        // sub_6E6120
        // sub_6E6160
        // inlined @ sub_4BEA10
        // inlined @ sub_6E7BB0
        // inlined @ sub_6EDB20
        node* push_back(const T &value) {
            return insert_before(sentinel, value);
        }

        // inlined @ sub_4DE460
        // inlined @ sub_417520
        // inlined @ sub_70AC10
        // inlined @ sub_70AD30
        // inlined @ sub_70AE40
        // inlined @ sub_70AF40
        // inlined @ sub_70B050
        // inlined @ sub_70B290
        node* push_front(const T &value) {
            return insert_before(sentinel->next, value);
        }

        // sub_6D5110
        // inlined @ sub_4175F0
        // inlined @ sub_6D94B0
        // inlined @ sub_6F86E0
        node* erase(node* position) noexcept { // the sentinel is left alone
            node* next = position->next;

            if (position != sentinel) {
                position->previous->next = position->next;
                position->next->previous = position->previous;
                position->value.~T();
                mm::free(position);
                --count;
            }

            return next;
        }

        // sub_5A85E0
        // inlined @ sub_4DE570
        // inlined @ sub_99A570
        // inlined @ sub_4F7470
        void pop_front() noexcept { // nothing on an empty list
            erase(sentinel->next);
        }

        // sub_4ECF90
        // sub_7FB2F0
        // inlined @ sub_97A400
        // inlined @ sub_99A570
        // inlined @ sub_701110
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
