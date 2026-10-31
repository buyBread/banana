#include "game/wds/ai/ai_simple_target.hh"

using namespace treyarch;

// sub_4EF7C0
i32 ai_simple_target_collection::prune() {
    i32 remaining = 0;

    for (ai_simple_target** position = targets.begin(); position != targets.end();) {
        ai_simple_target* target = *position;

        if (target->get_type() != simple_target_type_none) {
            ++remaining;
            ++position;

            continue;
        }

        target->destroy();

        position = targets.erase(position);
    }

    return remaining;
}

// sub_4F21E0
void ai_simple_target_collection::clear() {
    for (ai_simple_target* target : targets)
        if (target)
            target->destroy();

    targets.clear();
}
