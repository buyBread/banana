#include "retail.hh"
#include "game/frontend/city_life_tracker.hh"
#include "game/frontend/entity_tracker.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"

using namespace treyarch;

// sub_68FB00
void stealth_widget_t::set_active(bool active) {
    // slot 54
    if (!active && meter)
        ((void (__thiscall*)(igo_3d_widget*, i32))((void**)meter->vtable)[54])(meter, 1);

    if (activated == active)
        return;

    if (active) {
        activated = true;

        if (unk_0c)
            unk_0c->set_visible(true);

        if (meter)
            meter->set_visible(true);

        deactivation_timer = 0.0f;
        this->active       = true;
    } else if (!(deactivation_timer >= 0.0f) || this->active) {
        deactivation_timer = 0.15f;
        this->active       = false;
    }
}

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

// sub_692450
void entity_tracker::set_mini_map_active(bool active) {
    mm_icon->is_shown = active;
}

// sub_692470
bool entity_tracker::get_mini_map_active() {
    return mm_icon->is_shown;
}

// sub_70EB80
void entity_tracker::set_poi_icon(i32 kind) {
    u32 mode           = 0;
    f32 carried_health = 0.0f;
    f32 carried_unk    = 0.0f;

    if (poi_icon && kind != poi_icon->kind && kind != 11) {
        mode = poi_icon->mode;

        if (mode == 1) {
            carried_health = -1.0f;
            carried_unk    = -1.0f;
        } else if (mode == 2) {
            carried_health = (f32)retail::sub_68C490((u32*)poi_icon); // poi_icon_widget::get_life_bar_health
            carried_unk    = -1.0f;
        }

        retail::sub_6923F0((u32*)poi_icon, 1); // poi_icon_widget scalar deleting destructor
        poi_icon = nullptr;
    }

    if (mm_icon && kind != mm_icon->kind) {
        retail::sub_6DFB50((u32*)references::frontend.get().igo->radar_widget, (i32)mm_icon); // remove_icon
        delete mm_icon;
        mm_icon = nullptr;
    }

    if (!poi_icon) {
        if (kind != 11) {
            void* allocation = mm::allocate(sizeof(poi_icon_widget));
            poi_icon = allocation ? (poi_icon_widget*)retail::sub_70C4A0((u8*)allocation, kind) : nullptr;
        }

        poi_icon->mode = mode;

        if (mode == 2)
            retail::sub_68C450((u32*)poi_icon, (i32)carried_health, (i32)carried_unk); // poi_icon_widget::set_life_bar_values
    }

    if (!mm_icon) {
        arch_base* tracked = ent.resolve();

        mm_icon = new igo_map_poi(kind,
                                  31,
                                  tracked ? ((entity*)tracked)->my_abs_po->get_position() : vector3d(0.0f, 0.0f, 0.0f));
    }

    if (kind == 3)
        set_fog_obj(kind);
    else if (kind == 0x11 || kind == 2)
        set_gta_widget_active(false, false);
}

// sub_692480
void entity_tracker::set_poi_icon_offset(vector3d offset) {
    poi_icon_offset = offset;
}

// sub_6924B0
void entity_tracker::set_poi_show_distance_info(bool show) {
    poi_show_distance_info = show;
}

// sub_6AAD00
void entity_tracker::set_poi_range(f32 near_clip, f32 far_clip) {
    auto* record = (u8*)retail::sub_92BB10(ent.value); // tracker record lookup

    if (record)
        ((void (__thiscall*)(u8*, f32, f32))retail::sub_905FE0)(record, near_clip, far_clip); // set_clip_range
}

// sub_6C7130
void entity_tracker::set_poi_active(bool active) {
    if (poi_icon) {
        poi_icon->turn_on(active);

        if (stealth_widget && active)
            stealth_widget->set_active(false);

        if (mm_icon) {
            if (active != mm_icon->unk_3c) {
                mm_icon->unk_3c = active;
                mm_icon->unk_38 = 0.0f;
            }

            mm_icon->unk_44 = 2;
        }

        if (poi_icon->kind != 0x11 && poi_icon->kind != 2)
            enable_zlock(active ? 0.0f : -1.0f);
    }

    if (!unk_41 && poi_icon->kind != 0x11 && poi_icon->kind != 2)
        set_gta_widget_active(active, false);

    if (fog_obj) {
        if (auto* fog = (entity*)fog_obj->fog_vh.resolve())
            fog->set_visible(active, false);

        fog_obj->active = active;
    }
}

// sub_6924E0
bool entity_tracker::get_poi_active() {
    return poi_icon && poi_icon->is_shown;
}

// sub_692860
void entity_tracker::set_poi_urgent(bool urgent) {
    if (urgent != this->urgent)
        parent->poi_urgent = urgent;

    this->urgent = urgent;
}

// sub_6924C0
void entity_tracker::set_dot_visible(bool visible) {
    if (poi_icon)
        poi_icon->dot_visible = visible;
}

// sub_692500
void entity_tracker::set_poi_shows_in_spidey_sense(bool shows) {
    shows_in_spidey_sense = shows;
}

// sub_692510
void entity_tracker::set_poi_shows_out_of_spidey_sense(bool shows) {
    shows_out_of_spidey_sense = shows;
}

// sub_692520
void entity_tracker::set_poi_is_life_bar(bool is_life_bar, i32 health, i32 unk) {
    if (poi_icon->unk_2c)
        poi_icon->unk_2c->is_life_bar = is_life_bar;

    if (poi_icon->unk_28)
        poi_icon->unk_28->set_visible(is_life_bar);

    set_poi_life_bar_values(health, unk);
}

// sub_692590
void entity_tracker::set_poi_life_bar_values(i32 health, i32 unk) {
    retail::sub_68C450((u32*)poi_icon, health, unk); // poi_icon_widget::set_life_bar_values
}

// sub_6925D0
i32 entity_tracker::get_poi_curr_life_bar_health() {
    return retail::sub_68C490((u32*)poi_icon); // poi_icon_widget::get_life_bar_health
}

// sub_6AAC90
void entity_tracker::enable_zlock(f32 value) { // a negative value turns the lock off
    if (!(value >= 0.0f)) {
        auto* record = (u8*)retail::sub_92BB10(ent.value); // tracker record lookup

        if (record && *(i32*)(record + 0x10) == 4)
            ((void (__thiscall*)(u8*, f32))retail::sub_90D110)(record, -1.0f);
    } else {
        auto* record = (u8*)retail::sub_94D720(ent.value, 4, true); // tracker record lookup, adding one when missing

        if (record && *(i32*)(record + 0x10) == 4)
            ((void (__thiscall*)(u8*, f32))retail::sub_90D110)(record, value);
    }
}

// sub_692670
void entity_tracker::set_gta_widget_active(bool active, bool unk_41) {
    if (gta_widget) {
        gta_widget->active = active;

        if (!active && gta_widget->unk_19 && !gta_widget->unk_20 && !gta_widget->unk_21 &&
            gta_widget->target_ring.resolve()) {

            gta_widget->target_ring.resolve();
            gta_widget->unk_1c = 0.0f;
            gta_widget->unk_21 = 1;
            gta_widget->unk_20 = 0;
        }
    }

    gta_widget_active = active;
    this->unk_41      = unk_41;
}

// sub_6926D0
void entity_tracker::set_gta_widget_pos_offset(vector3d offset) {
    if (gta_widget)
        gta_widget->pos_offset = offset;
}

// sub_692700
void entity_tracker::set_gta_widget_render_scale(f32 scale) {
    if (!gta_widget)
        return;

    if (auto* ring = (entity*)gta_widget->target_ring.resolve())
        ring->set_render_scale(vector3d(scale, scale, scale));

    gta_widget->render_scale = scale;
}

// sub_692800
void entity_tracker::set_gta_widget_inner_ring_radius(f32 radius) {
    gta_inner_ring_radius = 0.0f > radius ? 0.0f : radius;
}

// sub_692820
void entity_tracker::set_gta_widget_outer_ring_radius(f32 radius) { // never below the inner radius
    if (0.0f > radius)
        radius = 0.0f;

    gta_outer_ring_radius = (f64)gta_inner_ring_radius > (f64)radius ? gta_inner_ring_radius : radius;
}

// sub_6AADD0
void entity_tracker::set_gta_widget_color(vector3d color) {
    if (gta_widget)
        gta_widget->set_color(color);
}

// sub_6C71F0
void entity_tracker::set_fog_obj(i32) { // the kind goes unused
    if (entity_fog_obj* old = fog_obj) {
        if (arch_base* fog = old->fog_vh.resolve())
            retail::sub_7FBF20((u32*)&references::g_world_ptr.read()->ent_mgr, (i32*)fog);

        mm::free(old);
        fog_obj = nullptr;
    }

    void* allocation = mm::allocate(sizeof(entity_fog_obj));
    fog_obj = allocation ? (entity_fog_obj*)retail::sub_6AB6A0((u32*)allocation, ent.value) : nullptr;
}

// sub_6AAD30
void entity_tracker::set_fog_active(bool active) {
    if (!fog_obj)
        return;

    if (auto* fog = (entity*)fog_obj->fog_vh.resolve())
        fog->set_visible(active, false);

    fog_obj->active = active;
}

// sub_6AAD60
void entity_tracker::set_fog_requires_senses(bool requires_senses) {
    if (fog_obj)
        fog_obj->requires_senses = requires_senses;
}

// sub_6C7280
void entity_tracker::set_fog_render_color(vector3d color) {
    if (!fog_obj)
        return;

    auto* fog = (entity*)fog_obj->fog_vh.resolve();

    if (!fog)
        return;

    f32 red   = 0.0f > color.x ? 0.0f : color.x > 1.0f ? 1.0f : color.x;
    f32 green = 0.0f > color.y ? 0.0f : color.y > 1.0f ? 1.0f : color.y;
    f32 blue  = 0.0f > color.z ? 0.0f : color.z > 1.0f ? 1.0f : color.z;

    color32 render_color;

    render_color.set_clamped((i32)((f64)red * 255.0), (i32)((f64)green * 255.0), (i32)((f64)blue * 255.0), 0xFF);
    fog->set_render_color(render_color);
}

// sub_6AAD80
void entity_tracker::set_fog_distance_clip(f32 fade_distance) {
    if (!fog_obj)
        return;

    auto* fog = (entity*)fog_obj->fog_vh.resolve();

    if (!fog)
        return;

    i32 preset = retail::sub_77F090(fade_distance); // the fader preset closest to the distance

    fog->ext_flags = preset | (fog->ext_flags & ~0xFu);
    retail::sub_6123B0((u32*)fog, preset, (i32)0xCBADF00D);
}

// sub_692600
void entity_tracker::set_stealth_widget_active(bool active) {
    if (!stealth_widget)
        return;

    stealth_widget->set_active(active);

    if (poi_icon && active)
        poi_icon->turn_on(false);
}

// sub_6C7350
void entity_tracker::set_stealth_widget_color(vector3d color) { // the channels are already 0..255
    if (!stealth_widget)
        return;

    f32 red   = color.x;
    f32 green = color.y;
    f32 blue  = color.z;

    if (0.0f > red)
        red = 0.0f;

    if ((f64)red > 255.0)
        red = 255.0f;

    if (0.0f > green)
        green = 0.0f;

    if ((f64)green > 255.0)
        green = 255.0f;

    if (0.0f > blue)
        blue = 0.0f;

    if ((f64)blue > 255.0)
        blue = 255.0f;

    stealth_widget->color.set_clamped((i32)red, (i32)green, (i32)blue, 0xFF);

    // slot 62
    if (igo_3d_widget* meter = stealth_widget->meter)
        ((void (__thiscall*)(igo_3d_widget*, color32))((void**)meter->vtable)[62])(meter, stealth_widget->color);
}

// sub_692640
void entity_tracker::set_stealth_timer_values(f32 unk_0, f32 unk_1) {
    // slot 55
    if (igo_3d_widget* meter = stealth_widget->meter)
        ((void (__thiscall*)(igo_3d_widget*, f32, f32))((void**)meter->vtable)[55])(meter, unk_0, unk_1);
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
        tracker->set_poi_icon(11);
        tracker->mm_icon->is_shown = true;
        tracker->set_poi_active(false);
    }
}

// sub_6E0070
city_life_tracker* entity_tracker_manager::find_city_life_tracker(u32 id) {
    auto* found = city_life_trackers.find(id);

    return found != city_life_trackers.end() ? found->value.second : nullptr;
}

// sub_708230
u32 entity_tracker_manager::create_city_life_tracker(i32 kind, i32 unk_14) {
    void* allocation = mm::allocate(sizeof(city_life_tracker));
    auto* tracker    = allocation ? (city_life_tracker*)retail::sub_7000C0((u32*)allocation, kind, unk_14) : nullptr;
    u32   id         = next_city_life_tracker_id;

    city_life_trackers[id] = tracker;
    ++next_city_life_tracker_id;

    return id;
}

// sub_6E0020
void entity_tracker_manager::destroy_city_life_tracker(u32 id) {
    auto* found = city_life_trackers.find(id);

    if (found == city_life_trackers.end())
        return;

    // slot 0
    if (city_life_tracker* tracker = found->value.second)
        ((void (__thiscall*)(city_life_tracker*, u32))tracker->vtable[0])(tracker, 1);

    city_life_trackers.erase(found);
}

// sub_700070
void entity_tracker_manager::register_entity(arch_base_vhandle ent, city_life_tracker* tracker) {
    if (ent.resolve() && tracker)
        tracker->entities.push_back(ent);
}
