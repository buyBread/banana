#pragma once

#include <array>

#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/platform.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    mash::string        get_amalgatoc_filename(e_platform platform);
    const mash::string &get_packs_directory(e_platform platform);

    namespace references {
        inline util::memory_reference<mash::string> packs_directory { 0x010883FC };

        inline util::memory_reference<u32>        packs_directory_guard    { 0x01088408 };
        inline util::memory_reference<e_platform> packs_directory_platform { 0x00E82484 }; // stays 3, matching no platform

        // indexed by e_platform
        inline util::memory_reference<std::array<const char*, 3>> pack_extensions { 0x00E82128 };
    } // references
}} // treyarch::amalga
