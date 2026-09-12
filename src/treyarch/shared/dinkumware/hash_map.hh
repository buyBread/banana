#pragma once

#include "treyarch/shared/dinkumware/list.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace dinkumware {
    template<typename K, typename V>
    struct hash_pair {
        K key;
        V value;
    };

    template<typename K>
    class hash_compare {

    public:
        u32 operator()(const K &key) const noexcept {
            return key.hash();
        }

        bool less(const K &left, const K &right) const noexcept {
            return left < right;
        }
    };

    template<typename K, typename V, typename traits_t = hash_compare<K>>
    class hash_map {

        using pair_t = hash_pair<K, V>;
        using node_t = list_node<pair_t>;

    private:
        traits_t        traits;
        list<pair_t>    entries;
        vector<node_t*> bucket_bounds;
        u32             bucket_mask;
        u32             bucket_count;

    public:
        hash_map() : bucket_mask(1),
                     bucket_count(1) {
                        
            bucket_bounds.assign(9, entries.head());
        }

        hash_map(const hash_map&) = delete;
        hash_map &operator=(const hash_map&) = delete;

        node_t* head() const noexcept { return entries.head(); }
        node_t* begin() const noexcept { return entries.begin(); }
        node_t* end() const noexcept { return entries.end(); }

        u32 size() const noexcept { return entries.size(); }
        bool empty() const noexcept { return entries.empty(); }

        node_t* find(const K &key) const noexcept {
            u32 bucket = bucket_for(key);

            node_t* position = bucket_bounds[bucket];
            node_t* finish   = bucket_bounds[bucket + 1];

            while (position != finish && traits.less(position->value.key, key))
                position = position->next;

            if (position == finish || traits.less(key, position->value.key))
                return entries.head();

            return position;
        }

        node_t* insert(const K &key, const V &value) {
            if (bucket_count <= entries.size() >> 2)
                grow_one_bucket();

            u32 bucket = bucket_for(key);

            node_t* position = bucket_bounds[bucket];
            node_t* finish   = bucket_bounds[bucket + 1];

            while (position != finish && traits.less(position->value.key, key))
                position = position->next;

            if (position != finish && !traits.less(key, position->value.key))
                return position;

            pair_t pair { key, value };
            node_t* inserted = entries.insert_before(position, pair);

            if (bucket_bounds[bucket] == position) {
                u32 boundary = bucket;

                for (;;) {
                    if (bucket_bounds[boundary] != position)
                        break;

                    bucket_bounds[boundary] = inserted;

                    if (!boundary)
                        break;

                    --boundary;
                }
            }

            return inserted;
        }

        node_t* erase(node_t* position) noexcept {
            u32 bucket = bucket_for(position->value.key);

            node_t* next = position->next;

            if (bucket_bounds[bucket] == position) {
                u32 boundary = bucket;

                for (;;) {
                    if (bucket_bounds[boundary] != position)
                        break;

                    bucket_bounds[boundary] = next;

                    if (!boundary)
                        break;

                    --boundary;
                }
            }

            return entries.erase(position);
        }

        void clear() noexcept {
            entries.clear();
            bucket_bounds.assign(9, entries.head());

            bucket_mask  = 1;
            bucket_count = 1;
        }

    private:
        u32 bucket_for(const K &key) const noexcept {
            u32 bucket = traits(key) & bucket_mask;

            if (bucket_count <= bucket)
                bucket += -1 - (bucket_mask >> 1);

            return bucket;
        }

        void grow_one_bucket() {
            if (bucket_bounds.size() - 1 <= bucket_count) {
                u32 new_mask = 2 * bucket_bounds.size() - 3;

                bucket_bounds.resize(new_mask + 2, entries.head());
                bucket_mask = new_mask;
            } else if (bucket_mask < bucket_count)
                bucket_mask = 2 * bucket_mask + 1;

            node_t* sentinel = entries.head();
            u32 split_bucket = bucket_count - (bucket_mask >> 1) - 1;
            node_t* position = bucket_bounds[split_bucket];

            while (position != bucket_bounds[split_bucket + 1]) {
                if ((traits(position->value.key) & bucket_mask) == split_bucket) {
                    position = position->next;

                    continue;
                }

                node_t* next = position->next;

                if (next != sentinel) {
                    if (bucket_bounds[split_bucket] == position) {
                        u32 boundary = split_bucket;

                        for (;;) {
                            bucket_bounds[boundary] = next;

                            if (!boundary || bucket_bounds[boundary - 1] != position)
                                break;

                            --boundary;
                        }
                    }

                    entries.relink_before(position, sentinel);
                    bucket_bounds[bucket_count + 1] = sentinel;
                }

                node_t* moved = sentinel->previous;
                u32 boundary  = bucket_count;

                while (split_bucket < boundary && bucket_bounds[boundary] == sentinel) {
                    bucket_bounds[boundary] = moved;
                    --boundary;
                }

                if (next == sentinel)
                    break;

                position = next;
            }

            ++bucket_count;
        }
    };

    using hash_pair_size_sanity = hash_pair<u32, void*>;

    ASSERT_SIZEOF(hash_pair_size_sanity, 0x08);

    using hash_map_size_sanity  = hash_map<u32, void*>;

    ASSERT_SIZEOF(hash_map_size_sanity,  0x28);
}} // treyarch::dinkumware
