#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl {
    struct init_list {
        init_list();
        virtual ~init_list() = default;
        virtual void register_item() = 0;

        init_list* next;
    };

    struct init_list_function : init_list {
        using callback_type = void(__cdecl*)();

        explicit init_list_function(callback_type callback);
        
        void register_item() override;

        callback_type callback;
    };

    namespace references {
        inline util::memory_reference<init_list*> init_list_head { 0x01115B5C };
    } // references

    ASSERT_SIZEOF  (init_list,       0x08);
    ASSERT_OFFSETOF(init_list, next, 0x04);
    
    ASSERT_SIZEOF  (init_list_function,           0x0C);
    ASSERT_OFFSETOF(init_list_function, callback, 0x08);
}} // treyarch::ngl
