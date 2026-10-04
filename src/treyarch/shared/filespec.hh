#pragma once

#include "treyarch/shared/mash/string.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    // a path split into its folder, bare name and extension
    class filespec {

    public:
        mash::string path;
        mash::string name;
        mash::string ext;

        explicit filespec(const mash::string &source);
    };

    ASSERT_SIZEOF  (filespec,       0x24);
    ASSERT_OFFSETOF(filespec, name, 0x0C);
    ASSERT_OFFSETOF(filespec, ext,  0x18);
} // treyarch
