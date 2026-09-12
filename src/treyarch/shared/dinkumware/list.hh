#pragma once

#include "treyarch/shared/memory/heap.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace dinkumware {
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

        list() : sentinel((node*)memory::heap::allocate(sizeof(node))),
                 count(0) {
                    
            sentinel->next     = sentinel;
            sentinel->previous = sentinel;
        }

        ~list() {
            clear();
            
            memory::heap::free(sentinel);
        }

        list(const list&) = delete;
        list &operator=(const list&) = delete;

        node* head()  const noexcept { return sentinel; }
        node* begin() const noexcept { return sentinel->next; }
        node* end()   const noexcept { return sentinel; }

        u32  size()  const noexcept { return count; }
        bool empty() const noexcept { return count == 0; }

        node* insert_before(node* before, const T &value) {
            node* position = (node*)memory::heap::allocate(sizeof(node));

            position->next     = before;
            position->previous = before->previous;

            new (&position->value) T(value);

            before->previous->next = position;
            before->previous       = position;

            ++count;

            return position;
        }

        node* push_back(const T &value) {
            return insert_before(sentinel, value);
        }

        node* erase(node* position) noexcept {
            node* next = position->next;

            position->previous->next = position->next;
            position->next->previous = position->previous;
            position->value.~T();
            memory::heap::free(position);
            --count;

            return next;
        }

        void clear() noexcept {
            node* position = sentinel->next;

            sentinel->next     = sentinel;
            sentinel->previous = sentinel;
            count              = 0;

            while (position != sentinel) {
                node* next = position->next;

                position->value.~T();
                memory::heap::free(position);
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
}} // treyarch::dinkumware
