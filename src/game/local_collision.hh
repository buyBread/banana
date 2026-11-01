#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;

    namespace references {
        // SM3's local_collision filter references; pointers to the filter objects
        inline util::memory_reference<void*> entfilter_reject_all               { 0x00E7B298 };
        inline util::memory_reference<void*> entfilter_entity_no_capsules       { 0x00E7B2A0 };
        inline util::memory_reference<void*> entfilter_blocks_beams_no_capsules { 0x00E7B2B4 };
        inline util::memory_reference<void*> obbfilter_lineseg_test             { 0x00E7B2D0 };
        inline util::memory_reference<void*> obbfilter_sphere_test              { 0x00E7B2DC };

        inline util::memory_reference<void*> entfilter_base_vtable      { 0x00B8A254 };
        inline util::memory_reference<void*> exclude_entfilter_t_vtable { 0x00B9190C }; // exclude_entfilter_t<entfilter_and<entfilter_entity, entfilter_no_capsules>>
    } // references

    // SM3's world collision queries; the filters keep their retail vtables
    namespace local_collision {
        struct entfilter_entity;
        struct entfilter_no_capsules;

        template<typename filter_a, typename filter_b>
        struct entfilter_and;

        struct entfilter_base {
            void* vtable;

            ~entfilter_base() {
                vtable = &references::entfilter_base_vtable.get();
            }
        };

        // a filter that turns one actor away
        template<typename source_filter>
        struct exclude_entfilter_t : entfilter_base {
            actor* excluded;

            explicit exclude_entfilter_t(actor* a);
        };

        // inlined @ sub_552EC0
        template<>
        inline exclude_entfilter_t<entfilter_and<entfilter_entity, entfilter_no_capsules>>::exclude_entfilter_t(actor* a) : excluded(a) {
            vtable = &references::exclude_entfilter_t_vtable.get();
        }

        ASSERT_SIZEOF(entfilter_base, 0x04);
    } // local_collision
} // treyarch
