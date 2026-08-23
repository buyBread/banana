#pragma once

#include "treyarch/shared/memory/game_heap.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace container {
    template<typename T>
    struct legacy_list_node {
        legacy_list_node* next;
        legacy_list_node* previous;
        T                 value;
    };

    template<typename T>
    struct legacy_list {
        u32                  allocator_state;
        legacy_list_node<T>* head;
        u32                  size;
    };

    template<typename T>
    void clear_legacy_list(legacy_list<T>* value) {
        legacy_list_node<T>* head = value->head;
        legacy_list_node<T>* node = head->next;

        head->next     = head;
        head->previous = head;
        value->size    = 0;

        while (node != head) {
            legacy_list_node<T>* next = node->next;

            memory::game_heap::free_small_block(node);
            node = next;
        }
    }

    ASSERT_SIZEOF(legacy_list_node<void*>, 0x0C);
    ASSERT_SIZEOF(legacy_list<void*>,      0x0C);
}} // treyarch::container
