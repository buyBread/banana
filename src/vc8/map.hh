#pragma once

#include <functional>
#include <utility>

#include "vc8/tree.hh"
#include "banana/util/macros/sanity_assert.hh"

namespace treyarch { namespace vc8 {
    template<typename K, typename V>
    struct map_key {
        const K &operator()(const std::pair<const K, V> &value) const noexcept {
            return value.first;
        }
    };

    template<typename K, typename V, typename compare_t = std::less<K>>
    class map : public tree<K, std::pair<const K, V>, map_key<K, V>, compare_t> {

    public:
        map() = default;

        explicit map(mash::generating_vtable_version so_do_nothing) noexcept : tree<K, std::pair<const K, V>, map_key<K, V>, compare_t>(so_do_nothing) {}

        // sub_6F4730
        // sub_6F47A0
        // sub_6F48E0
        // sub_6F4950
        // sub_764120
        // sub_764190
        // sub_93DC00
        // sub_93DDB0
        // sub_93DE40
        // sub_93DED0
        // sub_93DF60
        // sub_93DFD0
        // sub_93E040
        // sub_93E1B0
        // sub_958130
        // sub_979870
        // sub_979900
        V &operator[](const K &key) {
            auto* where = this->lower_bound(key);

            if (where == this->end() || this->compare(key, where->value.first))
                where = this->insert(where, std::pair<const K, V>(key, V()));

            return where->value.second;
        }
    };

    using map_size_sanity = map<u32, u32>;

    ASSERT_SIZEOF(map_size_sanity, 0x0C);
}} // treyarch::vc8
