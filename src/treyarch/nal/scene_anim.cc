#include "treyarch/nal/anim_instance.hh"
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

// sub_9D1F80
nal::scene_anim_instance* nal::scene_anim::create_instance() {
    scene_anim_instance* instance = new scene_anim_instance;

    instance->t          = 0.0f;
    instance->t_prev     = 0.0f;
    instance->scene_anim = this;
    instance->instances  = nullptr;

    for (anim* value = first_anim; value; value = value->next) {
        anim_instance* first = instance->instances;

        // only ever writes null over null
        if (!value->transport_list)
            value->transport_list = nullptr;

        instance->instances       = new anim_instance(value, 0, nullptr);
        instance->instances->next = first;
    }

    return instance;
}

// sub_9CFC30
void nal::scene_anim_instance::destroy() {
    for (anim_instance* value = instances; value;) {
        anim_instance* next = value->next;

        delete value;

        value = next;
    }
}
