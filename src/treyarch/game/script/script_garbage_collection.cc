#include "retail.hh"
#include "treyarch/game/arch_base.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/frontend/ui_frontend.hh"
#include "treyarch/game/script/script_garbage_collection.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_85CCA0
void script_garbage_collection::entity_trackers(script_instance*, stuff_to_delete_list_t &stuff_to_delete) {
    for (garbage_collection_element* stuff = stuff_to_delete.head; stuff; stuff = stuff->vm_simple_list_next)
        retail::sub_6DFDA0((u32*)references::frontend.get().igo->ui_object_manager, stuff->element);
}

// sub_85AB00
void script_garbage_collection::line_infos(script_instance*, stuff_to_delete_list_t &stuff_to_delete) {
    for (garbage_collection_element* stuff = stuff_to_delete.head; stuff; stuff = stuff->vm_simple_list_next) {
        u8* line_info = (u8*)stuff->element;

        if (line_info) {
            retail::sub_798F10(line_info);
            memory::heap::free(line_info);
        }
    }
}

// sub_854A80
void script_garbage_collection::triggers(script_instance*, stuff_to_delete_list_t &stuff_to_delete) {
    for (garbage_collection_element* stuff = stuff_to_delete.head; stuff; stuff = stuff->vm_simple_list_next) {
        arch_base* trigger = arch_base_vhandle(stuff->element).resolve();

        // slot 0
        if (trigger)
            ((void (__thiscall*)(arch_base*, u32))trigger->vtable[0])(trigger, 1);
    }
}

// sub_843640
void script_garbage_collection::entities(script_instance*, stuff_to_delete_list_t &stuff_to_delete) {
    for (garbage_collection_element* stuff = stuff_to_delete.head; stuff; stuff = stuff->vm_simple_list_next) {
        arch_base* entity = arch_base_vhandle(stuff->element).resolve();

        if (entity)
            retail::sub_7FBF20((u32*)&references::g_world_ptr.read()->ent_mgr, (i32*)entity);
    }
}

// sub_8663E0
void script_garbage_collection::sound_responses(script_instance*, stuff_to_delete_list_t &stuff_to_delete) {
    for (garbage_collection_element* stuff = stuff_to_delete.head; stuff; stuff = stuff->vm_simple_list_next) {
        void* sound_response = (void*)stuff->element;

        if (sound_response) {
            retail::sub_8FFA10(sound_response, 0);
            memory::heap::free(sound_response);
        }
    }
}

// sub_867DD0
void script_garbage_collection::generic_event_callbacks(script_instance*, stuff_to_delete_list_t &stuff_to_delete) {
    for (garbage_collection_element* stuff = stuff_to_delete.head; stuff; stuff = stuff->vm_simple_list_next) {
        i32* callback = (i32*)stuff->element;

        if (callback) {
            retail::sub_83F9E0(callback);
            memory::heap::free(callback);
        }
    }
}

// sub_85C210
void script_garbage_collection::points_of_interest(script_instance*, stuff_to_delete_list_t &stuff_to_delete) {
    for (garbage_collection_element* stuff = stuff_to_delete.head; stuff; stuff = stuff->vm_simple_list_next)
        retail::sub_749BC0(stuff->element);
}

// sub_876810
void script_garbage_collection::mutexes(script_instance*, stuff_to_delete_list_t &stuff_to_delete) {
    for (garbage_collection_element* stuff = stuff_to_delete.head; stuff; stuff = stuff->vm_simple_list_next) {
        void* mutex = (void*)stuff->element;

        if (mutex)
            memory::heap::free(mutex);
    }
}

// sub_85F170
void script_garbage_collection::widgets_3d(script_instance*, stuff_to_delete_list_t &stuff_to_delete) {
    for (garbage_collection_element* stuff = stuff_to_delete.head; stuff; stuff = stuff->vm_simple_list_next)
        retail::sub_6D8270((u32*)references::frontend.get().igo, (i32*)stuff->element);
}

// sub_85CCD0
void script_garbage_collection::city_life_trackers(script_instance*, stuff_to_delete_list_t &stuff_to_delete) {
    for (garbage_collection_element* stuff = stuff_to_delete.head; stuff; stuff = stuff->vm_simple_list_next)
        retail::sub_6E0020((i32)references::frontend.get().igo->ui_object_manager, (i32)stuff->element);
}

// sub_862400
void script_garbage_collection::fingers_of_god(script_instance*, stuff_to_delete_list_t &stuff_to_delete) {
    for (garbage_collection_element* stuff = stuff_to_delete.head; stuff; stuff = stuff->vm_simple_list_next) {
        if (stuff->element)
            retail::sub_6E77C0((u32*)references::frontend.get().igo, (i32)stuff->element);
    }
}

// sub_878200
void script_garbage_collection::fight_groups(script_instance*, stuff_to_delete_list_t &stuff_to_delete) {
    for (garbage_collection_element* stuff = stuff_to_delete.head; stuff; stuff = stuff->vm_simple_list_next) {
        arch_base* fight_group = arch_base_vhandle(stuff->element).resolve();

        if (fight_group)
            retail::sub_4DC390((u32*)fight_group); // fight_group::release_fight_group
    }
}

// sub_8795E0
void script_garbage_collection::obstacles(script_instance*, stuff_to_delete_list_t&) {
    // each obstacle goes to navmesh_obstacle_manager's removal, which is empty in retail (nullsub_2)
}
