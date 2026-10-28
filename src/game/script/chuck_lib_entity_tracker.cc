#include "retail.hh"
#include "game/arch_base.hh"
#include "game/frontend/city_life_tracker.hh"
#include "game/frontend/entity_tracker.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/math/references.hh"
#include "game/script/chuck_lib_entity_tracker.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "treyarch/mash/string.hh"

using namespace treyarch;

// sub_85DA60
bool slf__create_entity_tracker__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    arch_base_vhandle ent(stack.local_uint(0x00));
    arch_base*        self   = ent.resolve();
    entity_tracker_id result = 0;

    if (self && self->is_an_entity()) {
        bool ok = false;

        result = references::frontend.get().igo->entity_tracker_mgr->create_entity_tracker(ent, &ok);

        if (ok)
            stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_entity_tracker, result);
        else
            result = 0;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85DB00
bool slf__destroy_entity_tracker__entity_tracker__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    entity_tracker_id id = stack.local_uint(0x00);

    stack.thread->get_instance(-1)->remove_allocated_stuff(chuck::vm::script_garbage_collection_entity_tracker, id);
    references::frontend.get().igo->entity_tracker_mgr->destroy_entity_tracker(id);

    return true;
}

// sub_85DB40
bool slf__create_city_life_tracker__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    u32 id = references::frontend.get().igo->entity_tracker_mgr->create_city_life_tracker((i32)stack.local_num(0x00), 31);

    stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_city_life_tracker, id);
    stack.push(&id, sizeof(id));

    return true;
}

// sub_85DBA0
bool slf__create_city_life_tracker__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    u32 id = references::frontend.get().igo->entity_tracker_mgr->create_city_life_tracker((i32)stack.local_num(0x00),
                                                                                         (i32)stack.local_num(0x04));

    stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_city_life_tracker, id);
    stack.push(&id, sizeof(id));

    return true;
}

// sub_85DC00
bool slf__destroy_city_life_tracker__city_life_tracker__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    u32 id = stack.local_uint(0x00);

    stack.thread->get_instance(-1)->remove_allocated_stuff(chuck::vm::script_garbage_collection_city_life_tracker, id);
    references::frontend.get().igo->entity_tracker_mgr->destroy_city_life_tracker(id);

    return true;
}

// sub_85CD00
bool slf__entity_tracker__set_mini_map_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_mini_map_active(stack.local_num(0x04) != 0.0f);

    return true;
}

// sub_85CD60
bool slf__entity_tracker__set_poi_icon_offset__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_poi_icon_offset(*(const vector3d*)&stack.local_uint(0x04));

    return true;
}

// sub_85CDB0
bool slf__entity_tracker__set_poi_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_poi_icon((i32)stack.local_num(0x04));

    return true;
}

// sub_85CDF0
bool slf__entity_tracker__set_poi_face_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        retail::sub_689610((i32)stack.local_num(0x04)); // entity_tracker::set_poi_face_icon, empty

    return true;
}

// sub_85CE30
bool slf__entity_tracker__set_poi_range__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_poi_range(stack.local_num(0x04), stack.local_num(0x08));

    return true;
}

// sub_85CE70
bool slf__entity_tracker__set_poi_show_distance_info__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_poi_show_distance_info(stack.local_num(0x04) != 0.0f);

    return true;
}

// sub_85CED0
bool slf__entity_tracker__set_poi_is_life_bar__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_poi_is_life_bar(stack.local_num(0x04) != 0.0f, (i32)stack.local_num(0x08), (i32)stack.local_num(0x0C));

    return true;
}

// sub_85CF30
bool slf__entity_tracker__set_poi_is_life_bar__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_poi_is_life_bar(stack.local_num(0x04) != 0.0f, (i32)stack.local_num(0x08), 100);

    return true;
}

// sub_85CFA0
bool slf__entity_tracker__set_poi_is_life_bar__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_poi_is_life_bar(stack.local_num(0x04) != 0.0f, 100, 100);

    return true;
}

// sub_85D000
bool slf__entity_tracker__set_poi_life_bar_values__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_poi_life_bar_values((i32)stack.local_num(0x04), (i32)stack.local_num(0x08));

    return true;
}

// sub_85D040
bool slf__entity_tracker__set_poi_life_bar_values__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_poi_life_bar_values((i32)stack.local_num(0x04), -1);

    return true;
}

// sub_85D080
bool slf__entity_tracker__get_poi_curr_life_bar_health__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));
    f32             result  = -1.0f;

    if (tracker)
        result = (f32)tracker->get_poi_curr_life_bar_health();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85D0D0
bool slf__entity_tracker__set_poi_egg_timer_values__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        ((void (__stdcall*)(f32, f32))retail::sub_AD2D40)(stack.local_num(0x04), stack.local_num(0x08)); // entity_tracker::set_poi_egg_timer_values, empty

    return true;
}

// sub_85D110
bool slf__entity_tracker__set_poi_egg_timer_values__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        ((void (__stdcall*)(f32, f32))retail::sub_AD2D40)(stack.local_num(0x04), -1.0f); // entity_tracker::set_poi_egg_timer_values, empty

    return true;
}

// sub_85D160
bool slf__entity_tracker__set_poi_egg_timer_paused__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        retail::sub_689610(stack.local_num(0x04) != 0.0f); // entity_tracker::set_poi_egg_timer_paused, empty

    return true;
}

// sub_85D1C0
bool slf__entity_tracker__get_poi_curr_egg_timer_time__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));
    f32             result  = -1.0f;

    if (tracker)
        result = (f32)retail::sub_720060(); // entity_tracker::get_poi_curr_egg_timer_time

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85D220
bool slf__entity_tracker__set_poi_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_poi_active(stack.local_num(0x04) != 0.0f);

    return true;
}

// sub_85D280
bool slf__entity_tracker__set_poi_urgent__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_poi_urgent(stack.local_num(0x04) != 0.0f);

    return true;
}

// sub_85D2E0
bool slf__entity_tracker__set_dot_visible__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_dot_visible(stack.local_num(0x04) != 0.0f);

    return true;
}

// sub_85D340
bool slf__entity_tracker__get_poi_active__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));
    f32             result  = 0.0f;

    if (tracker && tracker->get_poi_active())
        result = 1.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85D3A0
bool slf__entity_tracker__enable_zlock__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->enable_zlock(stack.local_num(0x04) > 0.0f ? 0.0f : -1.0f);

    return true;
}

// sub_85D400
bool slf__entity_tracker__set_gta_widget_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_gta_widget_active(stack.local_num(0x04) != 0.0f, true);

    return true;
}

// sub_85D460
bool slf__entity_tracker__init_gta_widget__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker) {
        tracker->set_gta_widget_pos_offset(*(const vector3d*)&stack.local_uint(0x04));
        tracker->set_gta_widget_render_scale(stack.local_num(0x10));
    }

    return true;
}

// sub_85D4C0
bool slf__entity_tracker__set_gta_widget_render_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_gta_widget_render_scale(stack.local_num(0x04));

    return true;
}

// sub_85D500
bool slf__entity_tracker__set_gta_widget_pos_offset__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_gta_widget_pos_offset(*(const vector3d*)&stack.local_uint(0x04));

    return true;
}

// sub_85D550
bool slf__entity_tracker__set_gta_widget_color__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_gta_widget_color(*(const vector3d*)&stack.local_uint(0x04));

    return true;
}

// sub_85D5A0
bool slf__entity_tracker__get_gta_widget_active__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));
    f32             result  = 0.0f;

    if (tracker && tracker->gta_widget_active)
        result = 1.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85D5F0
bool slf__entity_tracker__set_gta_widget_outer_ring_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_gta_widget_outer_ring_radius(stack.local_num(0x04));

    return true;
}

// sub_85D630
bool slf__entity_tracker__set_always_draw_up_down__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->always_draw_up_down = stack.local_num(0x04) != 0.0f;

    return true;
}

// sub_85D680
bool slf__entity_tracker__set_gta_widget_inner_ring_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_gta_widget_inner_ring_radius(stack.local_num(0x04));

    return true;
}

// sub_85D6C0
bool slf__entity_tracker__set_poi_shows_in_spidey_sense__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_poi_shows_in_spidey_sense(stack.local_num(0x04) != 0.0f);

    return true;
}

// sub_85D720
bool slf__entity_tracker__set_poi_shows_out_of_spidey_sense__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_poi_shows_out_of_spidey_sense(stack.local_num(0x04) != 0.0f);

    return true;
}

// sub_85D780
bool slf__entity_tracker__set_fog_obj__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_fog_obj((i32)stack.local_num(0x04));

    return true;
}

// sub_85D7C0
bool slf__entity_tracker__set_fog_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_fog_active(stack.local_num(0x04) != 0.0f);

    return true;
}

// sub_85D820
bool slf__entity_tracker__set_fog_requires_senses__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_fog_requires_senses(stack.local_num(0x04) != 0.0f);

    return true;
}

// sub_85D880
bool slf__entity_tracker__set_fog_render_color__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_fog_render_color(*(const vector3d*)&stack.local_uint(0x04));

    return true;
}

// sub_85D8D0
bool slf__entity_tracker__set_fog_distance_clip__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_fog_distance_clip(stack.local_num(0x04));

    return true;
}

// sub_85D910
bool slf__entity_tracker__set_stealth_widget_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_stealth_widget_active(stack.local_num(0x04) != 0.0f);

    return true;
}

// sub_85D970
bool slf__entity_tracker__set_stealth_widget_offset__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->stealth_widget_offset = *(const vector3d*)&stack.local_uint(0x04);

    return true;
}

// sub_85D9D0
bool slf__entity_tracker__set_stealth_widget_color__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_stealth_widget_color(*(const vector3d*)&stack.local_uint(0x04));

    return true;
}

// sub_85DA20
bool slf__entity_tracker__set_stealth_timer_values__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    entity_tracker* tracker = references::frontend.get().igo->entity_tracker_mgr->id_to_ptr(stack.local_uint(0x00));

    if (tracker)
        tracker->set_stealth_timer_values(stack.local_num(0x04), stack.local_num(0x08));

    return true;
}

// sub_85DC40
bool slf__city_life_tracker__register_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker_manager* manager = references::frontend.get().igo->entity_tracker_mgr;

    if (!manager)
        return true;

    if (city_life_tracker* tracker = manager->find_city_life_tracker(stack.local_uint(0x00)))
        entity_tracker_manager::register_entity(arch_base_vhandle(stack.local_uint(0x04)), tracker);

    return true;
}

// sub_883720
bool slf__city_life_tracker__add_quest_event__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    entity_tracker_manager* manager = references::frontend.get().igo->entity_tracker_mgr;

    if (!manager)
        return true;

    if (city_life_tracker* tracker = manager->find_city_life_tracker(stack.local_uint(0x00)))
        tracker->add_quest_event(mash::string((const char*)stack.local_uint(0x04)));

    return true;
}

// sub_85DC90
bool slf__city_life_tracker__get_center_pos__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    entity_tracker_manager* manager = references::frontend.get().igo->entity_tracker_mgr;
    vector3d                result  = references::unk_010f7140.get();

    if (manager) {
        if (city_life_tracker* tracker = manager->find_city_life_tracker(stack.local_uint(0x00)))
            result = tracker->get_center_pos();
    }

    stack.push(&result, sizeof(result));

    return true;
}
