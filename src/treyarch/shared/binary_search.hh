#pragma once

#include "util/types.hh"

namespace treyarch {
    using binary_search_array_cmp_fn = i32 (__cdecl*)(const void* a, const void* b);

    // sub_735CA0
    template<typename T, typename E>
    bool binary_search_array_cmp(const T*                         find_me,
                                 const E*                         the_array,
                                       i32                        begin_idx,
                                 const i32                        array_size,
                                       i32*                       index,
                                       binary_search_array_cmp_fn fn) {

        i32 end_idx = array_size;

        while (begin_idx < end_idx) {
            const i32 mid_point = (begin_idx + end_idx) / 2;
            const i32 result    = fn(find_me, &the_array[mid_point]);

            if (result < 0)
                end_idx = mid_point;
            else if (result > 0)
                begin_idx = mid_point + 1;
            else {
                if (index)
                    *index = mid_point;

                return true;
            }
        }

        // miss reports the end index as is
        if (index)
            *index = end_idx;

        return false;
    }
} // treyarch
