#pragma once

#include "treyarch/mash/string.hh"
#include "banana/util/macros/sanity_assert.hh"

namespace treyarch {
    class wds_script_manager {

    public:
        mash::string global_script_filename;
    };

    ASSERT_SIZEOF  (wds_script_manager,                         0x0C);
    ASSERT_OFFSETOF(wds_script_manager, global_script_filename, 0x00);
} // treyarch
