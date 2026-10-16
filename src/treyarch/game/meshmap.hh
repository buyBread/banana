#pragma once

#include "treyarch/game/reference.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/math/types/vector3.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct meshmap_entry {
        mash::vector
            <reference<ngl::mesh>> mesh;

        vector3 offset;

        // inlined @ sub_7DE5D0
        void construct_mashed_class() {
            mesh.construct_mashed_class();
        }

        // inlined @ sub_7DC6C0
        void destruct_mashed_class() {
            mesh.destruct_mashed_class();
        }

        // inlined @ sub_7DDB80
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            mesh.unmash(mash_info, this, buffer);
        }
    };

    class meshmap {

    public:
        void* self; // the resource handler points this at the meshmap itself

        meshmap_entry a;
        meshmap_entry b;
        meshmap_entry c;
        meshmap_entry d;
        meshmap_entry e;
        meshmap_entry f;
        meshmap_entry g;
        meshmap_entry h;
        meshmap_entry i;
        meshmap_entry j;
        meshmap_entry k;
        meshmap_entry l;
        meshmap_entry m;
        meshmap_entry n;
        meshmap_entry o;
        meshmap_entry p;
        bool          parapet;
        u8            pad_205[3];

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (meshmap_entry,         0x20);
    ASSERT_OFFSETOF(meshmap_entry, offset, 0x14);

    ASSERT_SIZEOF  (meshmap,          0x208);
    ASSERT_OFFSETOF(meshmap, a,       0x004);
    ASSERT_OFFSETOF(meshmap, p,       0x1E4);
    ASSERT_OFFSETOF(meshmap, parapet, 0x204);
} // treyarch
