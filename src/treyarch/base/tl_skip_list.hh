#pragma once

#include <cstdlib>

#include "treyarch/mash/string_hash.hh"
#include "treyarch/base/tl_system.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct _tl_skip_list_base {
        i32 random_bits;
        i32 randoms_left;

        // sub_884160
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
    };

    template <typename data_type, typename key_access>
    struct tl_skip_list : _tl_skip_list_base {
        struct instance {
            data_type* data_ptr;

            instance* &forward(i32 level) {
                return *(instance**)((u8*)this + 4 + 4 * level);
            }

            instance* forward(i32 level) const {
                return *(instance* const*)((const u8*)this + 4 + 4 * level);
            }
        };

        instance* head;
        i32       level;

        // sub_9DC490
        void init() {
            level = 0;

            head = (instance*)tl_mem_alloc(0x44, 8, 0);

            for (i32 index = 0; index < 16; ++index)
                head->forward(index) = nullptr;
        }

        // sub_9DC300
        // sub_9E1110
        // sub_A24DA0
        // sub_8CABA0
        data_type* add(data_type* data_ptr) {
            if (!head)
                init();

            instance* update[16];
            instance* current = head;

            string_hash key = key_access::get(data_ptr);

            for (i32 current_level = level; current_level >= 0; --current_level) {
                instance* next = current->forward(current_level);

                while (next && key_access::get(next->data_ptr) < key) {
                    current = next;
                    next = current->forward(current_level);
                }

                update[current_level] = current;
            }

            instance* next = current->forward(0);

            if (next && key_access::get(next->data_ptr) == key)
                return next->data_ptr;

            i32 new_level = random_level();

            if (new_level > level) {
                new_level = level + 1;
                level = new_level;
                update[new_level] = head;
            }

            instance* inserted = (instance*)tl_mem_alloc(4 * new_level + 8, 8, 0);
            inserted->data_ptr = data_ptr;

            for (i32 current_level = new_level; current_level >= 0; --current_level) {
                inserted->forward(current_level) = update[current_level]->forward(current_level);
                update[current_level]->forward(current_level) = inserted;
            }

            return nullptr;
        }

        // sub_9DC3C0
        // sub_9E11D0
        // sub_9E20A0
        bool del(data_type* data_ptr) {
            if (!head)
                return false;

            instance* update[16];
            instance* current = head;

            string_hash key = key_access::get(data_ptr);

            for (i32 current_level = level; current_level >= 0; --current_level) {
                instance* next = current->forward(current_level);

                while (next && key_access::get(next->data_ptr) < key) {
                    current = next;

                    next = current->forward(current_level);
                }

                update[current_level] = current;
            }

            instance* found = current->forward(0);

            if (!found || found->data_ptr != data_ptr)
                return false;

            for (i32 current_level = 0; current_level <= level; ++current_level) {
                if (update[current_level]->forward(current_level) != found)
                    break;

                update[current_level]->forward(current_level) = found->forward(current_level);
            }

            tl_mem_free(found);

            while (level > 0 && !head->forward(level))
                --level;

            return true;
        }

        // sub_9E2040
        // sub_743CD0
        // sub_9E1FE0
        data_type* find(string_hash key) const {
            if (!head)
                return nullptr;

            instance* current = head;

            for (i32 current_level = level; current_level >= 0; --current_level) {
                instance* next = current->forward(current_level);

                while (next && key_access::get(next->data_ptr) < key) {
                    current = next;
                    next = current->forward(current_level);
                }
            }

            instance* found = current->forward(0);

            if (found && key_access::get(found->data_ptr) == key)
                return found->data_ptr;

            return nullptr;
        }
    };
} // treyarch
