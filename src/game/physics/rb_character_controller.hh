#pragma once

#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class rb_character_controller;

    namespace references {
        inline util::memory_reference<u8> rb_character_controller_pool { 0x00E82DB0 };
    } // references

    void rb_character_controller_pool_free(rb_character_controller* controller);
} // treyarch
