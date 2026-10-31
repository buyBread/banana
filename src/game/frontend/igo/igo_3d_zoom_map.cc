#include <new>

#include "retail.hh"
#include "game/event/event_manager.hh"
#include "game/frontend/igo/igo_3d_zoom_map.hh"

using namespace treyarch;

// sub_6A61B0
// inlined @ sub_6A62A0
// inlined @ sub_6A6210
void zoom_map_poi::set_default() {
    ent        = arch_base_vhandle();
    position.x = 0.0f;
    position.y = 0.0f;
    position.z = 0.0f;
    kind       = zoom_map_poi_free;
    unk_1c     = 0;
    is_shown   = true;
    index      = -1;
    meta_title = "";
    meta_text  = "";
    unk_2c     = 0;
}

// sub_6A62A0
void zoom_map_poi::set(vector3d pos, i32 poi_kind) {
    set_default();

    position = pos;
    kind     = poi_kind;
}

// sub_6A6210
void zoom_map_poi::set(arch_base_vhandle tracked, i32 poi_kind) {
    set_default();

    ent      = tracked;
    position = zerovec;
    kind     = poi_kind;
}

// sub_687390
void zoom_map_poi::set_meta_title(mash::string* title) {
    meta_title = *title;

    title->finalize(mash::ALLOCATED);
}

// sub_6873E0
void zoom_map_poi::set_meta_text(mash::string* text) {
    meta_text = *text;

    text->finalize(mash::ALLOCATED);
}

// sub_690A50
i32 igo_3d_zoom_map::get_selected_poi() {
    return selected_poi;
}

// sub_690A10
void igo_3d_zoom_map::unselect_poi() {
    if (selected_poi != -1)
        event_manager::raise_event(references::zoom_map_poi_unselected.read(), arch_base_vhandle());

    selected_poi = -1;
}

// sub_6BE4C0
void igo_3d_zoom_map::feature_current_poi(bool feature) {
    if (poi_selected || is_zooming || !is_open)
        return;

    if (feature && current_poi && selected_poi != current_poi->index)
        retail::sub_6A71D0((i32)this, current_poi->index); // igo_3d_zoom_map select
    else
        unselect_poi();
}

// sub_690AB0
bool igo_3d_zoom_map::is_poi_in_use(u32 index) {
    return index <= 0x1C1 && pois[index].kind != zoom_map_poi_free;
}

// sub_690A80
// inlined @ sub_6D48B0
void igo_3d_zoom_map::set_poi_shown(u32 index, bool shown) {
    if (index <= 0x1C1)
        pois[index].is_shown = shown;
}

// sub_690DE0
void igo_3d_zoom_map::set_marker_state(u32 index, i32 state) {
    if (index <= 0x1C1)
        pois[index].marker_state = state;
}

// sub_690AE0
void igo_3d_zoom_map::set_poi_meta_title(u32 index, mash::string* title) {
    if (index <= 0x1C1) {
        alignas(mash::string) u8 storage[sizeof(mash::string)];

        pois[index].set_meta_title(new (storage) mash::string(*title));
    }

    title->finalize(mash::ALLOCATED);
}

// sub_690B60
void igo_3d_zoom_map::set_poi_meta_text(u32 index, mash::string* text) {
    if (index <= 0x1C1) {
        alignas(mash::string) u8 storage[sizeof(mash::string)];

        pois[index].set_meta_text(new (storage) mash::string(*text));
    }

    text->finalize(mash::ALLOCATED);
}

// sub_6FB6C0
i32 igo_3d_zoom_map::claim_poi() {
    i32 index = next_free_search;

    while (index < 0x1C2 && pois[index].kind != zoom_map_poi_free)
        ++index;

    if (index >= 0x1C2) {
        index = 0;

        while (index < next_free_search && pois[index].kind != zoom_map_poi_free)
            ++index;

        if (index >= next_free_search)
            return -1;
    }

    next_free_search = index + 1;

    poi_list.push_back(&pois[index]);

    pois[index].list_index = (i32)poi_list.size() - 1;

    return index;
}

// sub_6FB8B0
i32 igo_3d_zoom_map::add_poi(vector3d pos, i32 kind, i32 unk) {
    i32 index = claim_poi();

    if ((u32)index > 0x1C1)
        return -1;

    zoom_map_poi &poi = pois[index];

    poi.set(pos, kind);

    poi.unk_2c = unk;
    poi.index  = index;

    return index;
}

// sub_6FB910
i32 igo_3d_zoom_map::add_safe_zone_marker(vector3d pos) {
    return add_poi(pos, zoom_map_poi_safe_zone_marker, 0);
}

// sub_6FB940
i32 igo_3d_zoom_map::add_safe_zone_marker(arch_base_vhandle tracked) {
    i32 index = claim_poi();

    if ((u32)index > 0x1C1)
        return -1;

    zoom_map_poi &poi = pois[index];

    poi.set(tracked, zoom_map_poi_safe_zone_marker);

    poi.index         = index;
    poi.unk_2c        = 0;
    poi.tracks_entity = true;

    return index;
}

// sub_6DFAC0
void igo_3d_zoom_map::remove_poi(u32 index) {
    if (index > 0x1C1)
        return;

    zoom_map_poi &poi = pois[index];

    if (poi.kind == zoom_map_poi_free)
        return;

    poi.set_default();
    poi.is_shown = false;

    i32 place = poi.list_index;

    if (poi_list.size() > 1) {
        poi_list[place]             = poi_list.back();
        poi_list[place]->list_index = place;
    }

    poi_list.pop_back();
}

// sub_6A7000
void igo_3d_zoom_map::snap_to_point(f32 x, f32 z) {
    unk_90f8 = unk_911c;
    unk_9104 = (f32)((f64)x - (f64)unk_90e4.x);
    unk_9108 = (f32)((f64)z - (f64)unk_90e4.z);

    snap_timer.start(0.0f, 1.0f, 0.3f, 0.0f);

    unk_8f92 = true;
}
