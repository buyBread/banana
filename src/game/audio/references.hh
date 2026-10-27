#pragma once

#include "banana/util/memory_reference.hh"

namespace treyarch { namespace references {
    // read-only and never set; the wwise wrappers skip their calls while it is
    inline util::memory_reference<bool> sound_disabled { 0x00B8870B };
}} // treyarch::references
