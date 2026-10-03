#pragma once

#include "treyarch/game/frontend/igo/igo_3d_widget.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class igo_3d_scrapbook : public igo_3d_drawable {
        
    public:
        u8 reserved_004[0x2C];
        u8 active;

        u8 is_active() {
            return vtable->test_074(this);
        }

        void draw_pages() {
            vtable->method_068(this);
        }

        // slot 27
        void activate(u8 unk) {
            ((void (__thiscall*)(igo_3d_scrapbook*, u8))vtable->method_06c)(this, unk);
        }
    };

    ASSERT_OFFSETOF(igo_3d_scrapbook, active, 0x030);
} // treyarch
