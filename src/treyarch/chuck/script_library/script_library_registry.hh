#pragma once

#include "treyarch/chuck/script_library/script_library_function.hh"
#include "treyarch/chuck/script_library/script_library_class.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace script_library {
    struct script_library_registry;

    namespace fn {
        inline auto script_library_registry_find_class =
            (script_library_class*(__thiscall*)
            (script_library_registry*, u32))
            0x00A20F90;
    } // fn

    // registration order is part of the bytecode ABI;
    // BSL operands contain literal function indices
    struct script_library_registry {
        script_library_class_vector* m_classes;

        u32 class_count() const noexcept {
            return m_classes ? m_classes->size() : 0;
        }

        script_library_class* class_at(u32 index) const noexcept {
            if (!m_classes || index >= m_classes->size())
                return nullptr;

            return (*m_classes)[index];
        }

        script_library_class* find_class(u32 index) {
            if (index >= class_count())
                return nullptr;

            return fn::script_library_registry_find_class(this, index);
        }
    };
}}} // treyarch::chuck::script_library
