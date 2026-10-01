#pragma once

#include "treyarch/chuck/script_library/script_library_function.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/mash/string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace script_library {
    class script_library_class;

    using script_library_class_destructor    = script_library_class* (__thiscall*)(script_library_class* self, u32 flags);
    using script_library_class_find_instance = u32 (__thiscall*)(const script_library_class* self, const mash::string &name);

    struct script_library_class_vtable {
        script_library_class_destructor    scalar_deleting_destructor; // sub_A21010, then the engine heap
        script_library_class_find_instance find_instance;              // resolves CLV operands; base returns 0 (sub_A90D90)
    };

    /* one chuck value type; its script-visible functions are descriptors, not C++ members.
       built by sub_A21710(name, size, parent_name, skip_registration), which does not keep the name. */
    class script_library_class {

    public:
        script_library_class_vtable*    vtable;
        i32                             size;        // value width: 0 for class 0, 12 for vector3d, otherwise 4
        dinkumware::vector
            <script_library_function*>* functions;   // registration order
        const char*                     parent_name;
    };

    ASSERT_SIZEOF(script_library_class_vtable, 0x08);

    ASSERT_SIZEOF  (script_library_class,              0x10);
    ASSERT_OFFSETOF(script_library_class, vtable,      0x00);
    ASSERT_OFFSETOF(script_library_class, size,        0x04);
    ASSERT_OFFSETOF(script_library_class, functions,   0x08);
    ASSERT_OFFSETOF(script_library_class, parent_name, 0x0C);
}}} // treyarch::chuck::script_library
