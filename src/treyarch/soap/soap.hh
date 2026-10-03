#pragma once

#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace soap { namespace references {
    inline util::memory_reference<u8> enable_online   { 0x00B88728 };
    inline util::memory_reference<u8> enable_profiles { 0x00B88729 };
    inline util::memory_reference<u8> enable_storage  { 0x00B8872A };
}}} // treyarch::soap::references
