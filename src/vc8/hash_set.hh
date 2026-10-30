#pragma once

#include "vc8/hash.hh"
#include "vc8/set.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace vc8 {
    template<typename K, typename traits_t = hash_compare<K>>
    class hash_set : public hash<K, K, set_key<K>, traits_t> {};

    using hash_set_size_sanity = hash_set<u32>;

    ASSERT_SIZEOF(hash_set_size_sanity, 0x28);

    ASSERT_SIZEOF(hash_set_size_sanity::node, 0x0C);
}} // treyarch::vc8
