#pragma once

#include <functional>
#include <utility>

#include "treyarch/shared/dinkumware/tree.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace dinkumware {
    template<typename K, typename V>
    struct map_key {
        const K &operator()(const std::pair<const K, V> &value) const noexcept {
            return value.first;
        }
    };

    template<typename K, typename V, typename compare_t = std::less<K>>
    class map : public tree<K, std::pair<const K, V>, map_key<K, V>, compare_t> {};

    using map_size_sanity = map<u32, u32>;

    ASSERT_SIZEOF(map_size_sanity, 0x0C);
}} // treyarch::dinkumware
