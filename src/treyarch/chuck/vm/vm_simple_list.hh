#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    /* intrusive list; elements carry their own previous/next links.
       retail elements also point back at the list that owns them (see vm_thread and script_instance). */
    template<typename T>
    struct vm_simple_list {
        T   head;
        T   last;
        i32 size;

        // sub_A1E310 (garbage_collection_element); returns the next element, or null when `element` is on another list.
        // the element's backpointer must be named `list`
        T erase(T element) noexcept {
            if (!element || element->list != this)
                return nullptr;

            T next = element->vm_simple_list_next;

            if (element->vm_simple_list_previous)
                element->vm_simple_list_previous->vm_simple_list_next = element->vm_simple_list_next;
            else {
                head = head->vm_simple_list_next;

                if (head)
                    head->vm_simple_list_previous = nullptr;
            }

            if (element->vm_simple_list_next)
                element->vm_simple_list_next->vm_simple_list_previous = element->vm_simple_list_previous;
            else {
                last = last->vm_simple_list_previous;

                if (last)
                    last->vm_simple_list_next = nullptr;
            }

            element->list                    = nullptr;
            element->vm_simple_list_previous = nullptr;
            element->vm_simple_list_next     = nullptr;

            --size;

            return next;
        }
    };

    ASSERT_SIZEOF  (vm_simple_list<void*>,       0x0C);
    ASSERT_OFFSETOF(vm_simple_list<void*>, head, 0x00);
    ASSERT_OFFSETOF(vm_simple_list<void*>, last, 0x04);
    ASSERT_OFFSETOF(vm_simple_list<void*>, size, 0x08);
}}} // treyarch::chuck::vm
