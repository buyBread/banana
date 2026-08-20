#pragma once

#include <cstdlib>

#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/memory/memory.hh"
#include "util/types.hh"

namespace treyarch { namespace container {
    template <typename value_type>
    struct skip_list_node {
        value_type* value;

        skip_list_node* &forward(i32 level) {
            return *(skip_list_node**)((u8*)this + 4 + 4 * level);
        }

        skip_list_node* forward(i32 level) const {
            return *(skip_list_node* const*)((const u8*)this + 4 + 4 * level);
        }
    };

    template <typename value_type, typename key_access>
    struct skip_list {
        using node = skip_list_node<value_type>;

        i32   random_bits;
        i32   randoms_left;
        node* head;
        i32   level;

        void initialize() {
            level = 0;

            head = (node*)memory::allocate(0x44, 8, 0);

            for (i32 index = 0; index < 16; ++index)
                head->forward(index) = nullptr;
        }

        i32 random_level() {
            i32 selected_level = 0;
            i32 bits;

            do {
                bits = random_bits & 3;

                if (!bits)
                    ++selected_level;

                random_bits >>= 2;

                if (randoms_left-- == 1) {
                    random_bits = std::rand();
                    randoms_left = 7;
                }
            } while (!bits);

            return selected_level > 15 ? 15 : selected_level;
        }

        value_type* insert(value_type* value) {
            if (!head)
                initialize();

            node* update[16];
            node* current = head;
            
            string_hash key = key_access::get(value);

            for (i32 current_level = level; current_level >= 0; --current_level) {
                node* next = current->forward(current_level);

                while (next && key_access::get(next->value) < key) {
                    current = next;
                    next = current->forward(current_level);
                }

                update[current_level] = current;
            }

            node* next = current->forward(0);

            if (next && key_access::get(next->value) == key)
                return next->value;

            i32 new_level = random_level();

            if (new_level > level) {
                new_level = level + 1;
                level = new_level;
                update[new_level] = head;
            }

            node* inserted = (node*)memory::allocate(4 * new_level + 8, 8, 0);
            inserted->value = value;

            for (i32 current_level = new_level; current_level >= 0; --current_level) {
                inserted->forward(current_level) = update[current_level]->forward(current_level);
                update[current_level]->forward(current_level) = inserted;
            }

            return nullptr;
        }

        bool erase(value_type* value) {
            if (!head)
                return false;

            node* update[16];
            node* current = head;
            
            string_hash key = key_access::get(value);

            for (i32 current_level = level; current_level >= 0; --current_level) {
                node* next = current->forward(current_level);

                while (next && key_access::get(next->value) < key) {
                    current = next;
                    
                    next = current->forward(current_level);
                }

                update[current_level] = current;
            }

            node* found = current->forward(0);

            if (!found || found->value != value)
                return false;

            for (i32 current_level = 0; current_level <= level; ++current_level) {
                if (update[current_level]->forward(current_level) != found)
                    break;

                update[current_level]->forward(current_level) = found->forward(current_level);
            }

            memory::free(found);

            while (level > 0 && !head->forward(level))
                --level;

            return true;
        }

        value_type* find(string_hash key) const {
            if (!head)
                return nullptr;

            node* current = head;

            for (i32 current_level = level; current_level >= 0; --current_level) {
                node* next = current->forward(current_level);

                while (next && key_access::get(next->value) < key) {
                    current = next;
                    next = current->forward(current_level);
                }
            }

            node* found = current->forward(0);

            if (found && key_access::get(found->value) == key)
                return found->value;

            return nullptr;
        }
    };
}} // treyarch::container
