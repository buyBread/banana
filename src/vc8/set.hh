#pragma once

#include <functional>

#include "vc8/tree.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace vc8 {
    template<typename K>
    struct set_key {
        const K &operator()(const K &value) const noexcept {
            return value;
        }
    };

    template<typename K, typename compare_t = std::less<K>>
    class set : public tree<K, K, set_key<K>, compare_t> {};

    using set_size_sanity = set<void*>;

    ASSERT_SIZEOF(set_size_sanity, 0x0C);
}} // treyarch::vc8
