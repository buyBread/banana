#pragma once

#include "banana/util/types.hh"

namespace treyarch { namespace container {
    template<typename T>
    class simple_list;

    // the links a simple_list's element keeps, as a member named simple_list_vars
    template<typename T>
    class simple_list_variables {

    public:
        T               next_element;
        T               prev_element;
        simple_list<T>* list_owner;

        void initialize() {
            next_element = nullptr;
            prev_element = nullptr;
            list_owner   = nullptr;
        }
    };

    // an intrusive list of pointers; each element points back at its list
    template<typename T>
    class simple_list {

    public:
        T   first_element;
        T   last_element;
        i32 size;

        // inlined @ sub_4B4B50
        void push_front(T element) {
            element->simple_list_vars.next_element = first_element;
            element->simple_list_vars.prev_element = nullptr;

            if (first_element)
                first_element->simple_list_vars.prev_element = element;

            first_element = element;

            // the tail only moves for an element that ends up alone
            if (!element->simple_list_vars.next_element)
                last_element = element;

            element->simple_list_vars.list_owner = this;

            ++size;
        }

        bool contains(T element) const {
            return element && element->simple_list_vars.list_owner == this;
        }

        // sub_4A4260
        // sub_4A42C0
        bool remove(T element) { // unlinked through the element's own list pointer
            if (!contains(element))
                return false;

            simple_list_variables<T> &links = element->simple_list_vars;

            if (links.prev_element)
                links.prev_element->simple_list_vars.next_element = links.next_element;
            else
                links.list_owner->first_element = links.next_element;

            if (links.next_element)
                links.next_element->simple_list_vars.prev_element = links.prev_element;
            else
                links.list_owner->last_element = links.prev_element;

            --links.list_owner->size;

            links.initialize();

            return true;
        }
    };
}} // treyarch::container
