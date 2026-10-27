#include <cstdlib>

#include "game/als/animation_logic_system.hh"
#include "game/als/expression_animation.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/interface/morph_interface.hh"

using namespace treyarch;

// sub_5D4A30
resource_key als::expression_animation_entry::pick() const {
    if ((i32)animations.size > 0) {
        f32 total = 0.0f;

        for (expression_animation* animation : animations) {
            if (animation->weight > 0.0f)
                total = animation->weight + total;
        }

        if (total > 0.0f) {
            f32 target  = (f32)std::rand() * (1.0f / 32767.0f) * total;
            f32 running = 0.0f;
            i32 index   = 0;

            for (expression_animation* animation : animations) {
                if (animation->weight > 0.0f) {
                    running = animation->weight + running;

                    if (running >= target) {
                        if (index < 0)
                            break;

                        return animations.data[index]->morph;
                    }
                }

                ++index;
            }
        }
    }

    return resource_key {};
}

// sub_5D4B40
void als::expression_animation_data::apply(animation_logic_system* the_als, actor* the_actor, f32 anim_time, f32 duration) const {
    if (!the_actor || !the_actor->has_ifc(entity_ifc_morph))
        return;

    for (expression_animation_entry* entry : entries) {
        f32 start = entry->start;

        if (0.0f > start) {
            start = start + duration;

            if (0.0f > start)
                start = 0.0f;
        }

        f32 end = entry->end;

        if (0.0f > end) {
            end = end + duration;

            if (0.0f > end)
                end = 0.0f;
        }

        if (!(anim_time < start) && end > anim_time) {
            the_als->set_expression(entry);

            return;
        }
    }

    ((morph_interface*)the_actor->my_ifc_storage->get_ifc(entity_ifc_morph))->set_morph(resource_key {});
}
