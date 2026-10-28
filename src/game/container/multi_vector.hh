#pragma once

#include "vc8/list.hh"
#include "vc8/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace container {
    template<typename T>
    class multi_vector { // a list of vectors read as one sequence

    public:
        using vector_t = vc8::vector<T>;

        mutable vc8::list<vector_t> vectors;

        // inlined
        vector_t* get_vector(u32 index) const {
            if (index >= vectors.size())
                return nullptr;

            typename vc8::list<vector_t>::node* position = vectors.begin();

            for (u32 i = 0; i < index; ++i)
                position = position->next;

            return &position->value;
        }

        // inlined; an empty list gets its first vector
        vector_t* get_base_vector() const {
            if (vectors.size() == 0)
                return push_vector();

            return &vectors.begin()->value;
        }

        // sub_66A130
        vector_t* push_vector() const {
            vector_t tmp;
            vectors.push_back(tmp);

            return &vectors.end()->previous->value;
        }
    };

    ASSERT_SIZEOF(multi_vector<void*>, 0x0C);
}} // treyarch::container
