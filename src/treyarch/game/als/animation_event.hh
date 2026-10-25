#pragma once

#include "treyarch/game/event/anim_event.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace als {
    namespace references {
        // static event-type ids, hashed from their names by the CRT initializers
        inline util::memory_reference<string_hash> time_dilation_start { 0x0102C71C };
        inline util::memory_reference<string_hash> time_dilation_end   { 0x0102CAC8 };
    } // references

    // raised when the state's animation passes time; mashed ones get their retail vtables from the type registry
    class animation_event : public mash::mash_virtual_base {

    public:
        string_hash name;
        f32         time; // seconds into the animation

        virtual u32         get_param_type() const; // 0 for none
        virtual f32         get_float_param() const;
        virtual i32         get_int_param() const;
        virtual string_hash get_hash_param() const;

        // the param as an anim event carries it
        // inlined @ sub_5D5310
        // inlined @ sub_5D6610
        void get_param(anim_event_param* param) const {
            switch (get_param_type()) {
                case 1:
                    param->type  = 4;
                    param->value = get_int_param();
                    break;

                case 2: {
                    param->type = 2;

                    f32 value = get_float_param();
                    param->value = *(u32*)&value;
                    break;
                }

                case 3:
                    param->type  = 1;
                    param->value = get_hash_param().source_hash_code;
                    break;
            }
        }
    };

    class animation_event_with_param : public animation_event {

    public:
        u32 param_type; // 1 an integer, 2 a float, 3 a string_hash
        u32 param;

        u32         get_param_type() const override;
        f32         get_float_param() const override;
        i32         get_int_param() const override;
        string_hash get_hash_param() const override;
    };

    ASSERT_SIZEOF  (animation_event,       0x0C);
    ASSERT_OFFSETOF(animation_event, name, 0x04);
    ASSERT_OFFSETOF(animation_event, time, 0x08);

    ASSERT_SIZEOF  (animation_event_with_param,             0x14);
    ASSERT_OFFSETOF(animation_event_with_param, param_type, 0x0C);
    ASSERT_OFFSETOF(animation_event_with_param, param,      0x10);
}} // treyarch::als
