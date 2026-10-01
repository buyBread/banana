#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    /* intrusive list; elements carry their own previous/next links.
       retail elements also point back at the list that owns them (see vm_thread and script_instance). */
    template<typename T>
    struct vm_simple_list {
        T   head;
        T   last;
        i32 size;
    };

    ASSERT_SIZEOF  (vm_simple_list<void*>,       0x0C);
    ASSERT_OFFSETOF(vm_simple_list<void*>, head, 0x00);
    ASSERT_OFFSETOF(vm_simple_list<void*>, last, 0x04);
    ASSERT_OFFSETOF(vm_simple_list<void*>, size, 0x08);
}}} // treyarch::chuck::vm
