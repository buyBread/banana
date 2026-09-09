#pragma once

#include "util/types.hh"

namespace treyarch { namespace mash {
    // normal objects own their allocation; from-mash objects only borrow their bytes from the image
    enum allocation_scope : u32 {
        ALLOCATED = 0,
        FROM_MASH = 1
    };

    enum buffer_type : u32 {
        NORMAL_BUFFER = 0,
        SHARED_BUFFER,
        MAX_BUFFER_TYPES
    };

    enum io_mode : u32 {
        UNKNOWN_MODE = 0,
        MASH_MODE,
        UNMASH_MODE,
        ALIGN_MODE
    };

    enum mash_state : u32 {
        CONSTRUCTED = 0,
        INITIALIZED,
        FINALIZED
    };

    enum mash_sentry_enum : u32 {
        MEMBER_CLASS_SENTRY = 0x1C001ACE,
        CUSTOM_MASH_SENTRY  = 0x15BADBAD
    };

    enum generating_vtable_version : u32 {
        GENERATING_VTABLES_VERSION
    };

    using vtable_key_t      = u32;
    using vtable_value_t    = u32;
    using virtual_types_key = vtable_key_t;

    constexpr vtable_value_t invalid_vtable_value = 0;
    constexpr vtable_key_t   invalid_type_key     = 0;

    constexpr u8 alignment_padding   = 0xA1;
    constexpr u8 member_class_mashed = 0xAF;

    // generated constructors use the pointer as a tag; there is no actual state inside it
    struct from_mash_in_place_constructor {};
}} // treyarch::mash
