#pragma once

#include "treyarch/shared/mash/string.hh"
#include "treyarch/chuck/script_library/script_library_function.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace script_library {
    struct script_library_class;
    struct script_library_class_vector;

    namespace fn {
        inline auto script_library_class_construct =
            (script_library_class*(__thiscall*)
            (script_library_class*, const char*, i32, const char*, bool))
            0x00A21710;

        inline auto script_library_class_destruct =
            (void(__thiscall*)
            (script_library_class*))
            0x00A21010;

        inline auto script_library_class_find_instance =
            (u32(__thiscall*)
            (script_library_class*, const treyarch::mash::string&))
            0x00A90D90;

        inline auto script_library_class_find_function =
            (script_library_function*(__thiscall*)
            (script_library_class*, u32))
            0x00A20F80;
    } // fn

    /*
        script_library_class describes one chuckvm value type.
        it stores the value's byte width and owns the native function descriptors registered for that type.
        it is not the class containing those methods as member methods.
    */

    struct script_library_class {
        i32                             m_value_size;
        script_library_function_vector* m_functions;
        const char*                     m_parent_name;
        
        // `skip_registration` changes whether construction participates in the registration-order ABI 
        script_library_class(const char* name, i32 value_size, const char* parent_name = nullptr, bool skip_registration = false) {
            fn::script_library_class_construct(this, name, value_size, parent_name, skip_registration);
        }

        virtual ~script_library_class() {
            fn::script_library_class_destruct(this);
        }

        virtual u32 find_instance(const treyarch::mash::string& name) {
            return fn::script_library_class_find_instance(this, name);
        }

        i32 value_size() const noexcept {
            return m_value_size;
        }

        i32 get_size() const noexcept {
            return m_value_size;
        }

        u32 function_count() const noexcept {
            return m_functions ? m_functions->size() : 0;
        }

        script_library_function* function(u32 index) const {
            assert(m_functions != nullptr);
            assert(index < m_functions->size());

            return (*m_functions)[index];
        }

        script_library_function* find_function(u32 index) {
            if (index >= function_count())
                return nullptr;

            return fn::script_library_class_find_function(this, index);
        }

        script_library_function* get_func(i32 index) const {
            assert(index >= 0);

            return function((u32)index);
        }
    };

    struct script_library_class_vector {
        u32                    unk_00;
        script_library_class** first;
        script_library_class** last;
        script_library_class** capacity_end;

        u32 size() const noexcept {
            if (!first)
                return 0;

            return (u32)(last - first);
        }

        bool empty() const noexcept {
            return size() == 0;
        }

        script_library_class* operator[](u32 index) const noexcept {
            return first[index];
        }
    };
}}} // treyarch::chuck::script_library