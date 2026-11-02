#include "retail.hh"
#include "game/colgeom.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/entity/limbo.hh"
#include "game/wds/entity/moved_entities.hh"
#include "game/wds/entity/vehicle.hh"
#include "game/wds/references.hh"
#include "game/wds/region.hh"
#include "treyarch/chuck/vm/script_instance.hh"

using namespace treyarch;

// sub_4E1F00
entity::~entity() {
    finalize(mash::ALLOCATED);

    ((mash::string*)unk_090)->finalize(mash::ALLOCATED);
}

void entity::finalize(mash::allocation_scope scope) {
    ((void (__thiscall*)(entity*, mash::allocation_scope))retail::sub_64B080)(this, scope);
}

// slot 1
void entity::construct_mashed_class() {
    ((void (__thiscall*)(entity*))retail::sub_653FF0)(this);
}

// slot 2
void entity::destruct_mashed_class() {
    ((void (__thiscall*)(entity*))retail::sub_666DB0)(this);
}

// slot 3
void entity::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(entity*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_64D220)(this,
                                                                                                         mash_info,
                                                                                                         containing_class_ptr,
                                                                                                         buffer);
}

// slot 4
mash::virtual_types_key entity::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const entity*))retail::sub_4E2040)(this);
}

// sub_4E2050
bool entity::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (entity_base::get_virtual_type_key() == parent_class)
        return true;

    if (arch_base::get_virtual_type_key() == parent_class)
        return true;

    return mash::mash_virtual_base::get_virtual_type_key() == parent_class;
}

// slot 8
i32 entity::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const entity*))retail::sub_4BF7E0)(this);
}

// slot 11
bool entity::is_an_entity() const {
    return ((bool (__thiscall*)(const entity*))retail::sub_667F80)(this);
}

// sub_645060
void entity::set_visible(bool visible, bool from_conglom) {
    if (is_visible() == visible)
        return;

    if (visible)
        flags |= entity_flag_graphics_visible;
    else
        flags &= ~entity_flag_graphics_visible;

    if (!is_visible())
        occlusion_status = references::occlusion_status_base.read() - 1;

    visibility_changed(from_conglom);
}

// sub_4E1F70
bool entity::was_occluded_last_frame() {
    if (flags & entity_flag_member)
        return get_occluded_last_frame_conglom_member();

    return occlusion_status != references::occlusion_status_base.read();
}

// sub_719110
e_entity_flavor entity::get_flavor() const {
    return entity_flavor_entity;
}

// slot 87
u32 entity::get_entity_size() const {
    return ((u32 (__thiscall*)(const entity*))retail::sub_4BF7E0)(this);
}

// sub_612C20
void entity::clear_region(region* r, i32 i_know_what_i_am_doing) {
    remove_me_from_region(r);

    limbo::update_limbo_entity(this);
}

// sub_612D10
void entity::compute_sector(terrain* ter, bool use_high_res_intersect, entity* original) {
    if (!(flags & entity_flag_region_forced))
        moved_entities::add_moved(my_handle);
}

// sub_612DB0
void entity::force_region(region* r) {
    if (!(flags & entity_flag_region_forced))
        remove_from_regions(true);

    flags |= entity_flag_region_forced;

    if (r) {
        retail::sub_7ABE70((i32)r, (i32)this); // region::add

        limbo::update_limbo_entity(this);
    }
}

// sub_4E1F90
void entity::force_current_region() {
    flags |= entity_flag_region_forced;
}

// sub_612E10
void entity::unforce_regions() {
    if (flags & entity_flag_region_forced) {
        flags &= ~entity_flag_region_forced;

        moved_entities::add_moved(my_handle);
    }
}

// sub_612E30
void entity::force_regions(entity* e) {
    remove_from_regions(false);

    flags |= entity_flag_region_forced;

    container::fixed_vector<region*, 15> copied;

    for (region* r : e->regions)
        copied.push_back(r);

    update_regions(copied.data_, (i32)copied.size());
}

// sub_601E90
void entity::set_family_visible(bool visible) {
    set_visible(visible, false);

    for (entity_base* child = my_child; child; child = child->my_brother)
        if (child->is_an_entity())
            ((entity*)child)->set_family_visible(visible);
}

// sub_601EE0
u32 entity::possibly_render() const {
    return is_still_visible() && (flags & entity_flag_graphics);
}

// sub_601F10
u32 entity::possibly_collide() const {
    return get_colgeom() && (flags & entity_flag_collisions_active);
}

// sub_601F40
bool entity::possibly_camera_collide() const {
    return get_colgeom() && (flags & entity_flag_collisions_active) && (get_colgeom()->flags & fi_collide_camera);
}

// sub_601F80
bool entity::possibly_walkable() const {
    return get_colgeom() && (flags & entity_flag_collisions_active) && (flags & entity_flag_walkable);
}

// sub_612F60
void entity::region_update_poss_collide() {
    if ((flags & entity_flag_member) || !get_primary_region())
        return;

    if (possibly_collide()) {
        retail::sub_6022E0((u32*)get_primary_region()->collision_proximity_map, (u32*)this); // collision map add

        return;
    }

    collision_map_t* map = get_primary_region()->collision_proximity_map;

    if (flags & entity_flag_needs_collision_proximity_map_update) {
        flags &= ~entity_flag_needs_collision_proximity_map_update;

        retail::sub_975020((i32*)map, (u32)this); // collision map remove
    }
}

// slot 100
bool entity::unk_100() {
    return ((bool (__thiscall*)(entity*))retail::sub_71F870)(this);
}

// sub_6450D0
void entity::unk_101(bool on) {
    if ((bool)(ext_flags & entity_ext_flag_unk_00001000) == on)
        return;

    if (on) {
        ext_flags |= entity_ext_flag_unk_00001000;

        if (!(flags & entity_flag_in_limbo))
            limbo::add_limbo_entity(this);

        flags |= entity_flag_in_limbo;

        stop_owned_effects(this);
    } else {
        ext_flags &= ~entity_ext_flag_unk_00001000;

        clear_limbo();

        moved_entities::add_moved(my_handle);
    }

    visibility_changed(false);
}

// sub_4E1FA0
bool entity::unk_102() const {
    return ext_flags & entity_ext_flag_unk_00001000;
}

// sub_601FE0
void entity::unk_103(bool on) {
    if (on)
        flags |= entity_flag_unk_00200000;
    else
        flags &= ~entity_flag_unk_00200000;

    unk_101(on);
}

// sub_4E1FB0
bool entity::unk_104() const {
    return flags & entity_flag_unk_00200000;
}

// slot 105
void entity::frame_advance(f32 t) {
    ((void (__thiscall*)(entity*, f32))retail::sub_689610)(this, t);
}

// sub_4E1FC0
bool entity::is_still_visible() const {
    return is_visible();
}

// slot 107
void entity::render_skeleton() {
    ((void (__thiscall*)(entity*))retail::sub_5B4A70)(this);
}

// slot 108
void entity::unk_108(f32 unk) {
    ((void (__thiscall*)(entity*, f32))retail::sub_689610)(this, unk);
}

// slot 109
void entity::render(f32 camera_distance, f32 entity_translucency_pct) {
    ((void (__thiscall*)(entity*, f32, f32))retail::sub_AD2D40)(this, camera_distance, entity_translucency_pct);
}

// slot 110
ngl::mesh* entity::get_mesh() const {
    return ((ngl::mesh* (__thiscall*)(const entity*))retail::sub_A944D0)(this);
}

// slot 111
ngl::mesh* entity::unk_111() const {
    return ((ngl::mesh* (__thiscall*)(const entity*))retail::sub_A944D0)(this);
}

// slot 112
bool entity::has_mesh() const {
    return ((bool (__thiscall*)(const entity*))retail::sub_71F870)(this);
}

// sub_601DF0
f32 entity::get_visual_radius() const {
    if (is_a_conglomerate() || (flags & entity_flag_member)) {
        conglomerate* owner = get_conglom_owner();

        if (owner)
            return owner->get_visual_radius();
    }

    return 0.0f;
}

// sub_601E30
vector3d entity::get_visual_center() const {
    if (is_a_conglomerate() || (flags & entity_flag_member)) {
        conglomerate* owner = get_conglom_owner();

        if (owner)
            return owner->get_visual_center();
    }

    return my_abs_po->get_position();
}

// sub_4E1FE0
vector3d entity::unk_115() {
    return get_visual_center();
}

// slot 116
void entity::suspend(bool update_region) {
    ((void (__thiscall*)(entity*, bool))retail::sub_689610)(this, update_region);
}

// slot 117
void entity::unsuspend(bool update_region) {
    ((void (__thiscall*)(entity*, bool))retail::sub_689610)(this, update_region);
}

// slot 118
void entity::set_render_color(color32 color) {
    ((void (__thiscall*)(entity*, color32))retail::sub_689610)(this, color);
}

// sub_4EB5B0
color32 entity::get_render_color() const {
    return color32(0xFF, 0xFF, 0xFF, 0xFF);
}

// slot 120
void entity::set_render_alpha_mod(f32 alpha) {
    ((void (__thiscall*)(entity*, f32))retail::sub_689610)(this, alpha);
}

// slot 121
f32 entity::get_render_alpha_mod() const {
    return ((f32 (__thiscall*)(const entity*))retail::sub_720040)(this);
}

// slot 122
void entity::set_render_scale(const vector3d &scale) {
    ((void (__thiscall*)(entity*, const vector3d&))retail::sub_689610)(this, scale);
}

// sub_4E2000
vector3d entity::get_render_scale() const {
    return vector3d(1.0f, 1.0f, 1.0f);
}

// slot 124
void entity::set_render_zbias(f32 zbias) {
    ((void (__thiscall*)(entity*, f32))retail::sub_689610)(this, zbias);
}

// slot 125
f32 entity::get_render_zbias() const {
    return ((f32 (__thiscall*)(const entity*))retail::sub_720040)(this);
}

// slot 126
light_manager* entity::get_light_set() const {
    return ((light_manager* (__thiscall*)(const entity*))retail::sub_A944D0)(this);
}

// slot 127
bool entity::can_be_a_lego() const {
    return ((bool (__thiscall*)(const entity*))retail::sub_71F870)(this);
}

// slot 128
bool entity::can_be_a_conglom_clone() const {
    return ((bool (__thiscall*)(const entity*))retail::sub_71F870)(this);
}

// slot 129
f32 entity::get_age() const {
    return ((f32 (__thiscall*)(const entity*))retail::sub_720050)(this);
}

// slot 130
void entity::set_age(f32 age) {
    ((void (__thiscall*)(entity*, f32))retail::sub_689610)(this, age);
}

// sub_4E2020
void entity::set_recursive_age(f32 age) {
    set_age(age);
}

// slot 132
void entity::start_recording() {
    ((void (__thiscall*)(entity*))retail::sub_5B4A70)(this);
}

// slot 133
void entity::stop_recording() {
    ((void (__thiscall*)(entity*))retail::sub_5B4A70)(this);
}

// slot 134
bool entity::hates_spiderman() {
    return ((bool (__thiscall*)(entity*))retail::sub_71F870)(this);
}

// sub_601FB0
mash::string entity::get_signaller_owner_name() const {
    return mash::string(mash::string::fmt, "0x%08x", id.source_hash_code);
}

// loc_602080
bool entity::get_occluded_last_frame_conglom_member() {
    return get_conglom_owner()->was_occluded_last_frame();
}

// sub_602050
void entity::set_occluded_last_frame_all() {
    ++references::occlusion_status_base.get();
}

// sub_602060
void entity::set_occluded_last_frame(bool occluded) {
    occlusion_status = references::occlusion_status_base.read() - occluded;
}

// sub_612B70
void entity::add_me_to_region(region* r) { // an interior region goes first
    if (r->flags & region_flag_interior) {
        i32 position = (i32)regions.size();

        regions.push_back(nullptr);

        for (; position > 0; --position)
            regions[position] = regions[position - 1];

        regions[0] = r;
    } else
        regions.push_back(r);
}

// sub_612BD0
void entity::remove_me_from_region(region* r) { // the count drops even for an absent region
    u32 index = (u32)-1;

    for (u32 i = 0; i < regions.size(); ++i) {
        if (regions[i] == r) {
            index = i;

            break;
        }
    }

    regions.erase(index);
}

// sub_612C40
void entity::update_regions(region** new_regions, i32 count) { // a set equal to the current one only updates limbo
    const bool same = (count == 0 && regions.size() == 0) ||
                      (count == 1 && regions.size() == 1 && new_regions[0] == regions[0]) ||
                      (count == 2 && regions.size() == 2 && new_regions[0] == regions[0] && new_regions[1] == regions[1]);

    if (!same) {
        // each removal starts the scan over
        region** current = regions.begin();

        while (current != regions.end()) {
            i32 i = count - 1;

            while (i >= 0 && new_regions[i] != *current)
                --i;

            if (i >= 0)
                ++current;
            else {
                retail::sub_7A5280((i32)*current, (u32*)this); // region::remove

                current = regions.begin();
            }
        }

        for (i32 i = 0; i < count; ++i)
            if (new_regions[i]->collision_proximity_map)
                retail::sub_7ABE70((i32)new_regions[i], (i32)this); // region::add
    }

    limbo::update_limbo_entity(this);
}

// sub_612D70
// inlined @ sub_612DB0
// inlined @ sub_612E30
void entity::remove_from_regions(bool update_limbo) {
    if (references::g_world_ptr.read()) {
        while (regions.size())
            retail::sub_7A5280((i32)regions.back(), (u32*)this); // region::remove
    }

    if (update_limbo)
        limbo::update_limbo_entity(this);
}

// sub_612EB0
void entity::update_proximity_maps(entity* ent) { // a member answers for its owner, whatever was passed
    entity* target = ent ? ent : this;

    if (flags & entity_flag_member)
        target = get_conglom_owner();

    if (!target)
        return;

    if (target->possibly_render() && !(target->flags & entity_flag_update_regions_every_frame)) {
        for (region** it = target->regions.begin(); it != target->regions.end(); ++it)
            if ((*it)->collision_proximity_map)
                retail::sub_7734F0((u32*)(*it)->visibility_map, (i32)target); // hierarchical_entity_proximity_map::update_entity
    } else {
        for (region** it = target->regions.begin(); it != target->regions.end(); ++it)
            if ((*it)->collision_proximity_map)
                retail::sub_784A90((char*)(*it)->visibility_map, (i32)target); // hierarchical_entity_proximity_map::remove_entity
    }
}

// sub_640D20
void entity::clear_limbo() {
    if (!(flags & entity_flag_in_limbo))
        return;

    flags &= ~entity_flag_in_limbo;

    limbo::remove_limbo_entity(this);

    if (!is_an_actor())
        return;

    auto* self = (actor*)this;

    if (self->adv_ptrs && self->adv_ptrs->my_script)
        self->adv_ptrs->my_script->unsuspend();

    if (!is_a_vehicle())
        return;

    // vehicle::on_exit_limbo
    auto* car = (vehicle*)this;

    if (!car->my_prop && car->physics_enabled) {
        retail::sub_63B370((i32)car, (i64*)car->my_abs_po); // builds my_prop

        retail::sub_58B470((u32*)car->my_ai, (i32)car->my_prop, (i32)car->my_rb, (i32)car->my_rb_model); // ai_vehicle::set_prop
    }
}

// sub_644FD0
void entity::visibility_changed(bool from_conglom) {
    if (flags & entity_flag_member) {
        conglomerate* owner = get_conglom_owner();

        if (owner) {
            if (!from_conglom)
                owner->update_proximity_maps();

            owner->conglomerate_flags = (owner->conglomerate_flags & ~conglomerate_flag_possibly_render_valid) |
                                        conglomerate_flag_visual_radius_dirty;
        }
    } else {
        if (is_a_conglomerate()) {
            auto* self = (conglomerate*)this;

            self->conglomerate_flags = (self->conglomerate_flags & ~conglomerate_flag_possibly_render_valid) |
                                       conglomerate_flag_visual_radius_dirty;
        }

        update_proximity_maps();
    }

    if (has_manip_obj_ifc())
        manip_obj_ifc()->force_status_update();
}

// sub_612D30
region* entity::get_primary_region() {
    entity* owner = this;

    // a member borrows the regions of the first owner that isn't one
    if (owner->flags & entity_flag_member) {
        do {
            owner = owner->get_conglom_owner();

            if (!owner)
                return nullptr;
        } while (owner->flags & entity_flag_member);
    }

    if (owner->regions.size())
        return owner->regions.data_[0];

    return nullptr;
}

// sub_602010
void entity::lock_found_entities() {
    references::found_entities_lock.get().acquire();

    references::found_entities_locked.write(true);
}

// sub_602030
void entity::unlock_found_entities() {
    references::found_entities_locked.write(false);

    references::found_entities_lock.get().release();
}
