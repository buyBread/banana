#include "retail.hh"
#include "game/frontend/entity_tracker.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"

using namespace treyarch;

// sub_68FC00
void stealth_widget_t::draw() {
    if (!activated)
        return;

    if (references::frontend.get().igo->zoom_map->blocks_world_rendering())
        return;

    if (meter)
        meter->draw();

    if (state - 1 <= 1 && unk_0c)
        unk_0c->draw();
}

// sub_70E960
entity_tracker::entity_tracker(entity_tracker_manager* parent, arch_base_vhandle ent) {
    poi_show_distance_info    = false;
    shows_in_spidey_sense     = false;
    shows_out_of_spidey_sense = false;
    this->parent              = parent;
    this->ent                 = ent;

    arch_base* tracked = this->ent.resolve();
    i32        kind    = tracked && tracked->is_an_actor() && ((actor*)tracked)->get_ai_core() ? 2 : 10;

    mm_icon = new igo_map_poi(kind,
                              31,
                              tracked ? ((entity*)tracked)->my_abs_po->get_position() : vector3d(0.0f, 0.0f, 0.0f));

    poi_show_distance_info = false;
    poi_icon_offset        = vector3d(0.0f, 0.0f, 0.0f);

    void* allocation = mm::allocate(sizeof(poi_icon_widget));
    poi_icon = allocation ? (poi_icon_widget*)retail::sub_70C4A0((u8*)allocation, 2) : nullptr;

    if (poi_icon->is_shown) {
        poi_icon->is_shown = false;

        if (poi_icon->unk_24)
            poi_icon->unk_24->set_visible(false);

        poi_icon->unk_2c->set_visible(false);
        poi_icon->unk_3c = false;
    }

    poi_icon->unk_1f          = 1;
    stealth_widget_offset     = vector3d(0.0f, 0.0f, 0.0f);
    stealth_widget            = nullptr;
    shows_in_spidey_sense     = true;
    shows_out_of_spidey_sense = true;
    fog_obj                   = nullptr;

    allocation = mm::allocate(sizeof(igo_3d_gta_widget));
    gta_widget = allocation ? (igo_3d_gta_widget*)retail::sub_6A0E40((u32*)allocation) : nullptr;

    gta_widget_active     = false;
    unk_41                = false;
    gta_inner_ring_radius = 50.0f;
    gta_outer_ring_radius = 200.0f;

    if (gta_widget) {
        if (auto* ring = (entity*)gta_widget->target_ring.resolve())
            ring->set_render_scale(vector3d(7.0f, 7.0f, 7.0f));

        gta_widget->render_scale = 7.0f;
    }

    always_draw_up_down = false;
    urgent              = false;
}

// sub_6F4440
entity_tracker::~entity_tracker() {
    if (urgent) {
        parent->poi_urgent = false;
        urgent             = false;
    }

    auto* record = (u8*)retail::sub_92BB10(ent.value); // tracker record lookup

    if (record && *(i32*)(record + 0x10) == 4)
        ((void (__thiscall*)(u8*, f32))retail::sub_90D110)(record, -1.0f); // zlock off

    if (mm_icon) {
        retail::sub_6DFB50((u32*)references::frontend.get().igo->radar_widget, (i32)mm_icon); // remove_icon
        delete mm_icon;
    }

    mm_icon = nullptr;

    delete poi_icon;
    poi_icon = nullptr;

    // slot 0
    if (stealth_widget)
        ((void (__thiscall*)(stealth_widget_t*, u32))stealth_widget->vtable[0])(stealth_widget, 1);

    stealth_widget = nullptr;

    if (fog_obj) {
        if (arch_base* fog = fog_obj->fog_vh.resolve())
            retail::sub_7FBF20((u32*)&references::g_world_ptr.read()->ent_mgr, (i32*)fog);

        mm::free(fog_obj);
    }

    fog_obj = nullptr;

    // slot 0
    if (gta_widget) {
        ((void (__thiscall*)(igo_3d_gta_widget*, u32))gta_widget->vtable[0])(gta_widget, 1);
        gta_widget = nullptr;
    }

    ui_frontend* igo = references::frontend.get().igo;

    if (igo && ent.value)
        retail::sub_6D9260((i32)igo, ent.value);
}

// sub_6D2710
void entity_tracker_manager::render() {
    bool spidey_sense = references::frontend.get().igo->background_effect_mode == 4;

    for (auto* position = tracker_map.begin(); position != tracker_map.end(); position = position->next()) {
        entity_tracker* tracker;

        if (!tracker_slot_pool.get_slot_contents(position->value.second, &tracker))
            continue;

        bool shown = spidey_sense ? tracker->shows_in_spidey_sense : tracker->shows_out_of_spidey_sense;

        if (tracker->ent.resolve() && tracker->poi_icon && tracker->poi_icon->is_shown && shown &&
            (!poi_urgent || tracker->urgent)) {

            tracker->poi_icon->draw();
        } else if (tracker->ent.resolve()) {
            if (tracker->stealth_widget && tracker->stealth_widget->activated)
                tracker->stealth_widget->draw();
        }
    }
}

// sub_6C7440
entity_tracker* entity_tracker_manager::id_to_ptr(entity_tracker_id id) {
    if (!id)
        return nullptr;

    entity_tracker* tracker;

    if (!tracker_slot_pool.get_slot_contents(id, &tracker))
        return nullptr;

    return tracker;
}

// sub_6DFFF0
entity_tracker_id entity_tracker_manager::get_tracker_by_entity(arch_base_vhandle ent) {
    auto* found = tracker_map.find(ent);

    return found != tracker_map.end() ? found->value.second : 0;
}

// sub_70EDB0
entity_tracker_id entity_tracker_manager::create_entity_tracker(arch_base_vhandle ent, bool* ok) {
    *ok = true;

    auto* found = tracker_map.find(ent);

    if (found != tracker_map.end() && found->value.second)
        destroy_entity_tracker(found->value.second);

    if (!tracker_slot_pool.remaining()) {
        *ok = false;

        return 0;
    }

    entity_tracker_id id = tracker_slot_pool.add_to_pool(new entity_tracker(this, ent));

    tracker_map[ent] = id;

    return id;
}

// sub_6DFDA0
void entity_tracker_manager::destroy_entity_tracker(entity_tracker_id id) { // finds the map entry by its id, not its entity
    auto* position = tracker_map.begin();

    while (position != tracker_map.end() && position->value.second != id)
        position = position->next();

    if (position == tracker_map.end())
        return;

    entity_tracker* tracker;

    if (tracker_slot_pool.get_slot_contents(id, &tracker)) {
        tracker_slot_pool.remove_from_pool(id);

        delete tracker;
    }

    tracker_map.erase(position);
}

// sub_70EEE0
void entity_tracker_manager::set_hero_tracker(arch_base_vhandle hero) {
    if (hero_tracker_id != (entity_tracker_id)-1) {
        destroy_entity_tracker(hero_tracker_id);
        hero_tracker_id = (entity_tracker_id)-1;
    }

    bool ok = false;

    hero_tracker_id = create_entity_tracker(hero, &ok);

    if (!ok) {
        hero_tracker_id = (entity_tracker_id)-1;

        return;
    }

    if (entity_tracker* tracker = id_to_ptr(hero_tracker_id)) {
        retail::sub_70EB80((i32)tracker, 11); // entity_tracker::set_poi_icon
        tracker->mm_icon->is_shown = true;
        retail::sub_6C7130((i32)tracker, 0); // entity_tracker::set_poi_active
    }
}
