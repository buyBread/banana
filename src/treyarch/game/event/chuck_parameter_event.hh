#pragma once

#include "treyarch/game/event/event.hh"
#include "util/memory_reference.hh"

namespace treyarch {
    namespace references {
        inline util::memory_reference<mash::virtual_types_key> chuck_parameter_event_type_key { 0x010F7160 };
    } // references

    class chuck_parameter_event : public event {

    public:
        i32 args_stack_size;
        u8  parameters[0x40];

        explicit chuck_parameter_event(string_hash signal);
        ~chuck_parameter_event() override;

        void* operator new(std::size_t size) noexcept;
        void  operator delete(void* allocation) noexcept;

        void construct_mashed_class() override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32  get_mash_sizeof() const override;
    };

    ASSERT_SIZEOF  (chuck_parameter_event,                  0x54);
    ASSERT_OFFSETOF(chuck_parameter_event, args_stack_size, 0x10);
    ASSERT_OFFSETOF(chuck_parameter_event, parameters,      0x14);
} // treyarch
