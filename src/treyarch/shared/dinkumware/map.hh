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
    class map : public tree<K, std::pair<const K, V>, map_key<K, V>, compare_t> {

    public:
        // sub_764120
        // sub_764190
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
}} // treyarch::dinkumware
