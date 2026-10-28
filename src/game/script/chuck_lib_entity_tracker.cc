#include "retail.hh"
#include "game/arch_base.hh"
#include "game/frontend/entity_tracker.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/script/chuck_lib_entity_tracker.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/vm_thread.hh"

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

bool slf__create_city_life_tracker__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DB40)(this, stack);
}

bool slf__create_city_life_tracker__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DBA0)(this, stack);
}

bool slf__destroy_city_life_tracker__city_life_tracker__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DC00)(this, stack);
}

bool slf__entity_tracker__set_mini_map_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CD00)(this, stack);
}

bool slf__entity_tracker__set_poi_icon_offset__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CD60)(this, stack);
}

bool slf__entity_tracker__set_poi_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CDB0)(this, stack);
}

bool slf__entity_tracker__set_poi_face_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CDF0)(this, stack);
}

bool slf__entity_tracker__set_poi_range__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CE30)(this, stack);
}

bool slf__entity_tracker__set_poi_show_distance_info__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CE70)(this, stack);
}

bool slf__entity_tracker__set_poi_is_life_bar__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CED0)(this, stack);
}

bool slf__entity_tracker__set_poi_is_life_bar__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CF30)(this, stack);
}

bool slf__entity_tracker__set_poi_is_life_bar__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CFA0)(this, stack);
}

bool slf__entity_tracker__set_poi_life_bar_values__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D000)(this, stack);
}

bool slf__entity_tracker__set_poi_life_bar_values__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D040)(this, stack);
}

bool slf__entity_tracker__get_poi_curr_life_bar_health__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D080)(this, stack);
}

bool slf__entity_tracker__set_poi_egg_timer_values__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D0D0)(this, stack);
}

bool slf__entity_tracker__set_poi_egg_timer_values__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D110)(this, stack);
}

bool slf__entity_tracker__set_poi_egg_timer_paused__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D160)(this, stack);
}

bool slf__entity_tracker__get_poi_curr_egg_timer_time__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D1C0)(this, stack);
}

bool slf__entity_tracker__set_poi_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D220)(this, stack);
}

bool slf__entity_tracker__set_poi_urgent__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D280)(this, stack);
}

bool slf__entity_tracker__set_dot_visible__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D2E0)(this, stack);
}

bool slf__entity_tracker__get_poi_active__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D340)(this, stack);
}

bool slf__entity_tracker__enable_zlock__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D3A0)(this, stack);
}

bool slf__entity_tracker__set_gta_widget_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D400)(this, stack);
}

bool slf__entity_tracker__init_gta_widget__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D460)(this, stack);
}

bool slf__entity_tracker__set_gta_widget_render_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D4C0)(this, stack);
}

bool slf__entity_tracker__set_gta_widget_pos_offset__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D500)(this, stack);
}

bool slf__entity_tracker__set_gta_widget_color__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D550)(this, stack);
}

bool slf__entity_tracker__get_gta_widget_active__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D5A0)(this, stack);
}

bool slf__entity_tracker__set_gta_widget_outer_ring_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D5F0)(this, stack);
}

bool slf__entity_tracker__set_always_draw_up_down__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D630)(this, stack);
}

bool slf__entity_tracker__set_gta_widget_inner_ring_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D680)(this, stack);
}

bool slf__entity_tracker__set_poi_shows_in_spidey_sense__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D6C0)(this, stack);
}

bool slf__entity_tracker__set_poi_shows_out_of_spidey_sense__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D720)(this, stack);
}

bool slf__entity_tracker__set_fog_obj__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D780)(this, stack);
}

bool slf__entity_tracker__set_fog_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D7C0)(this, stack);
}

bool slf__entity_tracker__set_fog_requires_senses__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D820)(this, stack);
}

bool slf__entity_tracker__set_fog_render_color__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D880)(this, stack);
}

bool slf__entity_tracker__set_fog_distance_clip__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D8D0)(this, stack);
}

bool slf__entity_tracker__set_stealth_widget_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D910)(this, stack);
}

bool slf__entity_tracker__set_stealth_widget_offset__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D970)(this, stack);
}

bool slf__entity_tracker__set_stealth_widget_color__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85D9D0)(this, stack);
}

bool slf__entity_tracker__set_stealth_timer_values__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DA20)(this, stack);
}

bool slf__city_life_tracker__register_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DC40)(this, stack);
}

bool slf__city_life_tracker__add_quest_event__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883720)(this, stack);
}

bool slf__city_life_tracker__get_center_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DC90)(this, stack);
}
