#pragma once

#include <cstddef>

#include "treyarch/game/arch_base.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    class script_instance_shadow : public arch_base {

        chuck::vm::script_instance* m_inst;

    public:
        script_instance_shadow();

        void* operator new(std::size_t size) noexcept;

        chuck::vm::script_instance* get_volatile_instance() { return m_inst; }

        void set_instance(chuck::vm::script_instance* inst) { m_inst = inst; }
    };

    ASSERT_SIZEOF(script_instance_shadow, 0x0C);
} // treyarch
