#pragma once

#include <cstddef>

#include "vc8/vector.hh"
#include "treyarch/chuck/script_library/script_library_function.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mm/heap.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace chuck { namespace script_library {
    /* one chuck value type; its script-visible functions are descriptors, not C++ members.
       the name is not kept; retail passes "" for all of them. */
    class script_library_class {

    public:
        i32                             size;        // value width: 0 for slc_global, 12 for vector3d, otherwise 4
        vc8::vector
            <script_library_function*>* functions;   // registration order
        const char*                     parent_name; // null for none or ""

        // a dummy is neither registered nor given a function list
        script_library_class(const char* name, i32 requested_size, const char* requested_parent_name, bool is_dummy);
        virtual ~script_library_class();

        // resolves CLV operands
        virtual u32 find_instance(const mash::string &name) const;

        script_library_function* get_function(i32 index) const;

        void* operator new(std::size_t size) noexcept {
            return memory::heap::allocate(size);
        }

        // inlined into the deleting destructors
        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }
    };

    class slc_num_t : public script_library_class {

    public:
        slc_num_t(const char* name, i32 requested_size, const char* requested_parent_name);
    };

    class slc_uint_t : public script_library_class {

    public:
        slc_uint_t(const char* name, i32 requested_size, const char* requested_parent_name);
    };

    class slc_str_t : public script_library_class {

    public:
        slc_str_t(const char* name, i32 requested_size, const char* requested_parent_name);
       ~slc_str_t() override;
    };

    namespace references {
        inline util::memory_reference<script_library_class*> slc_global { 0x010F708C };
        inline util::memory_reference<slc_num_t*>            slc_num    { 0x010F7090 };
        inline util::memory_reference<slc_str_t*>            slc_str    { 0x010F7094 };
        inline util::memory_reference<slc_uint_t*>           slc_uint   { 0x010F7098 };
    } // references

    ASSERT_SIZEOF  (script_library_class,              0x10);
    ASSERT_OFFSETOF(script_library_class, size,        0x04);
    ASSERT_OFFSETOF(script_library_class, functions,   0x08);
    ASSERT_OFFSETOF(script_library_class, parent_name, 0x0C);
}}} // treyarch::chuck::script_library
