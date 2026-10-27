#pragma once

#include "treyarch/mash/vector.hh"
#include "treyarch/mash/virtual_base.hh"
#include "game/resource_key.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;

    namespace als {
        class animation_logic_system;

        // registered but never shipped, so the retail vtables and mash slots stay

        // a morph and its weight in the pick
        class expression_animation : public mash::mash_virtual_base {

        public:
            resource_key morph;
            f32          weight; // only positive ones are picked
        };

        // the morphs the actor picks one of while the anim time is in [start, end)
        class expression_animation_entry : public mash::mash_virtual_base {

        public:
            mash::vector
                <expression_animation> animations;
            f32                        start;      // negative counts back from the end
            f32                        end;
            f32                        unk_20;

            // weighted by the positive weights; none without one
            resource_key pick() const;
        };

        // a state's param class: the entry its anim time is in sets the morph, none clears it
        class expression_animation_data : public mash::mash_virtual_base { // param_block_creatable_class

        public:
            mash::vector
                <expression_animation_entry> entries;

            void apply(animation_logic_system* the_als, actor* the_actor, f32 anim_time, f32 duration) const;
        };

        ASSERT_SIZEOF  (expression_animation,         0x10);
        ASSERT_OFFSETOF(expression_animation, morph,  0x04);
        ASSERT_OFFSETOF(expression_animation, weight, 0x0C);

        ASSERT_SIZEOF  (expression_animation_entry,             0x24);
        ASSERT_OFFSETOF(expression_animation_entry, animations, 0x04);
        ASSERT_OFFSETOF(expression_animation_entry, start,      0x18);
        ASSERT_OFFSETOF(expression_animation_entry, end,        0x1C);

        ASSERT_SIZEOF  (expression_animation_data,          0x18);
        ASSERT_OFFSETOF(expression_animation_data, entries, 0x04);
    } // als
} // treyarch
