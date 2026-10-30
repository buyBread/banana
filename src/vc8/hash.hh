#pragma once

#include <cstdlib>
#include <utility>

#include "vc8/list.hh"
#include "vc8/vector.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace vc8 {
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

    template<>
    class hash_compare<u32> {

    public:
        u32 operator()(const u32 &key) const noexcept {
            std::div_t division = std::div((i32)(key ^ 0xDEADBEEF), 127773);

            i32 hashed = 16807 * division.rem - 2836 * division.quot;

            if (hashed < 0)
                hashed += I32_MAX;

            return (u32)hashed;
        }

        bool less(const u32 &left, const u32 &right) const noexcept {
            return left < right;
        }
    };

    /* VC8 _Hash. one list holds every node, bucket by bucket, each bucket sorted by key;
       bucket_bounds[i] is bucket i's first node and bucket_bounds[i + 1] its end.
       the table grows one bucket at a time once there are four nodes per bucket. */
    template<typename K, typename T, typename key_of_t, typename traits_t>
    class hash {

    public:
        using node = list_node<T>;

    protected:
        traits_t      traits;
        list<T>       entries;
        vector<node*> bucket_bounds;
        u32           bucket_mask;
        u32           bucket_count;

    public:
        // sub_685AC0
        // sub_A6C040
        // sub_A6C130
        hash() : bucket_bounds(9, entries.head()),
                 bucket_mask(1),
                 bucket_count(1) {}

        hash(const hash&) = delete;
        hash &operator=(const hash&) = delete;

        node* head()  const noexcept { return entries.head(); }
        node* begin() const noexcept { return entries.begin(); }
        node* end()   const noexcept { return entries.end(); }

        u32  size()  const noexcept { return entries.size(); }
        bool empty() const noexcept { return entries.empty(); }

        // sub_79FA30
        // sub_A6B910
        node* find(const K &key) const noexcept {
            u32 bucket = bucket_for(key);

            node* position = bucket_bounds[bucket];
            node* finish   = bucket_bounds[bucket + 1];

            while (position != finish && traits.less(key_of_t()(position->value), key))
                position = position->next;

            if (position == finish || traits.less(key, key_of_t()(position->value)))
                return entries.head();

            return position;
        }

        // sub_685C00
        // sub_9DD940
        // sub_9DDBE0
        // sub_A6C220
        // sub_A6C4C0
        std::pair<node*, bool> insert(const T &value) { // unique insert; an equal key already present is returned with `false`
            if (bucket_count <= entries.size() >> 2)
                grow_one_bucket();

            u32   bucket = bucket_for(key_of_t()(value));
            node* where  = bucket_bounds[bucket + 1];

            while (where != bucket_bounds[bucket]) {
                where = where->previous;

                if (!traits.less(key_of_t()(value), key_of_t()(where->value))) {
                    if (!traits.less(key_of_t()(where->value), key_of_t()(value)))
                        return { where, false };

                    where = where->next;

                    break;
                }
            }

            node* inserted = entries.insert_before(where, value);

            while (bucket_bounds[bucket] == where) {
                bucket_bounds[bucket] = inserted;

                if (!bucket)
                    break;

                --bucket;
            }

            return { inserted, true };
        }

        // sub_684E30
        node* erase(node* position) noexcept { // returns the node after the erased one
            u32 bucket = bucket_for(key_of_t()(position->value));

            node* next = position->next;

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

        // sub_685EF0
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
                bucket_mask = 2 * bucket_bounds.size() - 3;

                bucket_bounds.resize(bucket_mask + 2, entries.head());
            } else if (bucket_mask < bucket_count)
                bucket_mask = 2 * bucket_mask + 1;

            node* sentinel     = entries.head();
            u32   split_bucket = bucket_count - (bucket_mask >> 1) - 1;
            node* position     = bucket_bounds[split_bucket];

            while (position != bucket_bounds[split_bucket + 1]) {
                if ((traits(key_of_t()(position->value)) & bucket_mask) == split_bucket) {
                    position = position->next;

                    continue;
                }

                node* next = position->next;

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

                node* moved    = sentinel->previous;
                u32   boundary = bucket_count;

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
}} // treyarch::vc8
