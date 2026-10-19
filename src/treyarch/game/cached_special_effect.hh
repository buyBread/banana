#pragma once

#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/boolx.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/resource_key.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class fx_cache_ent {

    public:
        f32               life_time;
        f32               alpha_fade_time;
        arch_base_vhandle ent;

        ~fx_cache_ent() {
            finalize(mash::ALLOCATED);
        }

        void construct_mashed_class();

        void destruct_mashed_class() {
            finalize(mash::FROM_MASH);
        }

        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}

    private:
        void finalize(mash::allocation_scope scope);
    };

    // a pool of one effect entity, reused as each copy's life runs out
    class fx_cache {

    public:
        void* self; // the resource handler points this at the cache itself

        void*                      simple_list_vars[3]; // SM3's simple_list_variables<fx_cache*>
        resource_key               fx_name;             // a type-92 entity
        resource_key               cache_name;
        u32                        unk_20;
        f32                        lifetime;
        f32                        alpha_percent;
        mash::vector<fx_cache_ent> cache;
        i32                        cur_fx_index;
        i32                        ref_count;
        boolx                      dynamic;
        u8                         pad_049[0x03];

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (fx_cache_ent,      0x0C);
    ASSERT_OFFSETOF(fx_cache_ent, ent, 0x08);

    ASSERT_SIZEOF  (fx_cache,               0x4C);
    ASSERT_OFFSETOF(fx_cache, fx_name,      0x10);
    ASSERT_OFFSETOF(fx_cache, cache_name,   0x18);
    ASSERT_OFFSETOF(fx_cache, unk_20,       0x20);
    ASSERT_OFFSETOF(fx_cache, lifetime,     0x24);
    ASSERT_OFFSETOF(fx_cache, cache,        0x2C);
    ASSERT_OFFSETOF(fx_cache, cur_fx_index, 0x40);
    ASSERT_OFFSETOF(fx_cache, dynamic,      0x48);
} // treyarch
