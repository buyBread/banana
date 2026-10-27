#pragma once

#include "banana/util/types.hh"

// needed, because modern stl picks differently between equal keys

namespace treyarch { namespace vc8 {
    // sub_7B64B0
    template<typename T>
    void push_heap(T* first, i32 hole, i32 top, T value) {
        for (i32 index = (hole - 1) / 2; top < hole && first[index] < value; index = (hole - 1) / 2) {
            first[hole] = first[index];
            hole        = index;
        }

        first[hole] = value;
    }

    // sub_7C2CB0
    template<typename T>
    void adjust_heap(T* first, i32 hole, i32 bottom, T value) {
        i32 top   = hole;
        i32 index = 2 * hole + 2;

        for (; index < bottom; index = 2 * index + 2) {
            if (first[index] < first[index - 1])
                --index;

            first[hole] = first[index];
            hole        = index;
        }

        if (index == bottom) {
            first[hole] = first[bottom - 1];
            hole        = bottom - 1;
        }

        push_heap(first, hole, top, value);
    }

    // sub_7D3660
    template<typename T>
    void make_heap(T* first, T* last) {
        i32 bottom = (i32)(last - first);

        if (bottom < 2)
            return;

        for (i32 hole = bottom / 2; 0 < hole;) {
            --hole;

            adjust_heap(first, hole, bottom, first[hole]);
        }
    }

    // sub_7DABD0
    template<typename T>
    void sort_heap(T* first, T* last) {
        for (; 1 < last - first; --last) {
            T value = last[-1];

            last[-1] = *first;
            adjust_heap(first, 0, (i32)(last - 1 - first), value);
        }
    }

    // sub_7DCAC0
    template<typename T>
    void partial_sort(T* first, T* middle, T* last) {
        make_heap(first, middle);

        for (T* next = middle; next < last; ++next) {
            if (*next < *first) {
                T value = *next;

                *next = *first;
                adjust_heap(first, 0, (i32)(middle - first), value);
            }
        }

        sort_heap(first, middle);
    }
}} // treyarch::vc8
