#include "treyarch/nal/nal.hh"
#include "treyarch/nal/scene_anim.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

// sub_9CFB10
void nal::scene_anim::link_transport_lists() {
    for (anim* value = first_anim; value; value = value->next) {
        if (value->transport_list)
            continue;

        transport_list* list = get_skeleton(&value->transport_list_name);

        // the lookup still runs while linking is skipped
        if (references::skip_scene_anim_linking.read())
            continue;

        if (!list)
            memory::report("Skeleton (transport list) %s not present in the directory. Requested by cutscene %s",
                           value->transport_list_name.get_text(),
                           name.get_text());

        value->transport_list = list;
    }
}
