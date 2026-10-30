#pragma once

#include "vc8/hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace vc8 {
    template<typename K, typename V>
    struct hash_pair {
        K key;
        V value;
    };

    template<typename K, typename V>
    struct hash_map_key {
        const K &operator()(const hash_pair<K, V> &value) const noexcept {
            return value.key;
        }
    };

    template<typename K, typename V, typename traits_t = hash_compare<K>>
    class hash_map : public hash<K, hash_pair<K, V>, hash_map_key<K, V>, traits_t> {};

    using hash_pair_size_sanity = hash_pair<u32, void*>;

    ASSERT_SIZEOF(hash_pair_size_sanity, 0x08);

    using hash_map_size_sanity  = hash_map<u32, void*>;

    ASSERT_SIZEOF(hash_map_size_sanity,  0x28);
}} // treyarch::vc8
