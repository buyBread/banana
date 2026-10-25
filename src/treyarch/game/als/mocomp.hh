#pragma once

#include "treyarch/game/als/als.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/types.hh"

namespace treyarch { namespace als {
    class animation_logic_system;

    // moves and turns the actor the way its animation asks; built in the als's own storage
    class motion_compensator : public mash::mash_virtual_base {

    public:
        virtual void activate(animation_logic_system* the_als_core, layer_id layer) = 0;
        virtual void deactivate() = 0;
        virtual void transition() = 0;
        virtual void pre_anim_action(f32 delta_t) = 0;
        virtual void post_anim_action(f32 delta_t) = 0;
    };
}} // treyarch::als
