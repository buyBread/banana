#include <algorithm>
#include <cmath>
#include <new>

#include "retail.hh"
#include "game/math/references.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/entity/entity_base.hh"
#include "game/wds/entity/moved_entities.hh"
#include "game/wds/references.hh"
#include "treyarch/jq/native_locks.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_4E1D40
entity_base::~entity_base() {
    finalize(mash::ALLOCATED);

    ((void (__thiscall*)(entity_base**, mash::allocation_scope))retail::sub_689610)(&my_parent, mash::ALLOCATED); // my_parent's destructor
}

void entity_base::finalize(mash::allocation_scope scope) {
    ((void (__thiscall*)(entity_base*, mash::allocation_scope))retail::sub_660F10)(this, scope);
}

// slot 1
void entity_base::construct_mashed_class() {
    ((void (__thiscall*)(entity_base*))retail::sub_653CB0)(this);
}

// slot 2
void entity_base::destruct_mashed_class() {
    ((void (__thiscall*)(entity_base*))retail::sub_666A80)(this);
}

// slot 3
void entity_base::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(entity_base*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_64D140)(this,
                                                                                                               mash_info,
                                                                                                               containing_class_ptr,
                                                                                                               buffer);
}

// slot 4
mash::virtual_types_key entity_base::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const entity_base*))retail::sub_4E1EA0)(this);
}

// sub_4E1EB0
bool entity_base::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (arch_base::get_virtual_type_key() == parent_class)
        return true;

    return mash::mash_virtual_base::get_virtual_type_key() == parent_class;
}

// slot 8
i32 entity_base::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const entity_base*))retail::sub_4E1E90)(this);
}

// slot 10
bool entity_base::is_an_entity_base() const {
    return ((bool (__thiscall*)(const entity_base*))retail::sub_667F80)(this);
}

// sub_667F30
vector3d entity_base::get_last_position() const {
    return my_abs_po->get_position();
}

// slot 54
i32 entity_base::po_changed() {
    return ((i32 (__thiscall*)(entity_base*))retail::sub_A944D0)(this);
}

// slot 55
i32 entity_base::unk_055(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, bool unk_5, bool unk_6, bool unk_7) {
    return ((i32 (__thiscall*)(entity_base*, i32, i32, i32, i32, bool, bool, bool))retail::sub_64AFE0)(this, unk, unk_2, unk_3, unk_4, unk_5, unk_6, unk_7);
}

// slot 56
u32 entity_base::spawn_effect_at_entity_position(string_hash name, const vector4d &position, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, const vector4d&, i32))retail::sub_611F70)(this, name, position, unk);
}

// slot 57
u32 entity_base::spawn_effect(string_hash name, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, i32))retail::sub_611D00)(this, name, unk);
}

// slot 58
u32 entity_base::spawn_effect_at_world_position(string_hash name, const vector4d &position, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, const vector4d&, i32))retail::sub_612060)(this, name, position, unk);
}

// slot 59
u32 entity_base::unk_059(string_hash name, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, i32))retail::sub_611DD0)(this, name, unk);
}

// slot 60
u32 entity_base::spawn_looping_effect_at_entity_position(string_hash name, const vector4d &position, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, const vector4d&, i32))retail::sub_612240)(this, name, position, unk);
}

// slot 61
u32 entity_base::spawn_looping_effect(string_hash name, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, i32))retail::sub_611EA0)(this, name, unk);
}

// slot 62
u32 entity_base::unk_062(string_hash name, const vector4d &position, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, const vector4d&, i32))retail::sub_612150)(this, name, position, unk);
}

// slot 63
void entity_base::stop_looping_effect(u32 effect_id) {
    ((void (__thiscall*)(entity_base*, u32))retail::sub_626DA0)(this, effect_id);
}

// sub_4E1DB0
void entity_base::set_flag_recursive(u32 flag, bool on) {
    if (flags & entity_flag_bone_connector)
        return;

    if (on)
        flags |= flag;
    else
        flags &= ~flag;
}

// sub_4E1DE0
void entity_base::set_ext_flag_recursive(u32 flag, bool on) {
    if (flags & entity_flag_bone_connector)
        return;

    if (on)
        ext_flags |= flag;
    else
        ext_flags &= ~flag;
}

// sub_4E1E10
void entity_base::set_active(bool active) {
    if (active)
        flags |= entity_flag_active;
    else
        flags &= ~entity_flag_active;
}

// sub_4E1E30
void entity_base::set_visible(bool visible, bool from_conglom) {
    if (visible)
        flags |= entity_flag_graphics_visible;
    else
        flags &= ~entity_flag_graphics_visible;
}

// slot 68
collision_geometry* entity_base::get_colgeom() const {
    return ((collision_geometry* (__thiscall*)(const entity_base*))retail::sub_A944D0)(this);
}

// slot 69
void entity_base::set_collisions_active(bool active, bool recursive) {
    ((void (__thiscall*)(entity_base*, bool, bool))retail::sub_AD2D40)(this, active, recursive);
}

// slot 70
bool entity_base::are_character_collisions_active() const {
    return ((bool (__thiscall*)(const entity_base*))retail::sub_71F870)(this);
}

// slot 71
bool entity_base::are_moving_terrain_only_collisions_active() const {
    return ((bool (__thiscall*)(const entity_base*))retail::sub_71F870)(this);
}

// sub_4E1E50
void entity_base::set_character_collisions_active(bool active) {
    if (active)
        flags |= entity_flag_physics_collide_characters;
    else
        flags &= ~entity_flag_physics_collide_characters;
}

// sub_4E1E70
void entity_base::set_terrain_collisions_active(bool active) {
    if (active)
        flags |= entity_flag_physics_collide_terrain;
    else
        flags &= ~entity_flag_physics_collide_terrain;
}

// slot 74
void entity_base::set_moving_terrain_only_collisions_active(bool active) {
    ((void (__thiscall*)(entity_base*, bool))retail::sub_689610)(this, active);
}

// slot 75
bool entity_base::was_occluded_last_frame() {
    return ((bool (__thiscall*)(entity_base*))retail::sub_667F80)(this);
}

// slot 76
bool entity_base::is_alive() const {
    return ((bool (__thiscall*)(const entity_base*))retail::sub_667F80)(this);
}

// slot 77
e_entity_flavor entity_base::get_flavor() const {
    return ((e_entity_flavor (__thiscall*)(const entity_base*))retail::sub_8139C0)(this);
}

// slot 78
bool entity_base::is_a_placeholder_vehicle() const {
    return ((bool (__thiscall*)(const entity_base*))retail::sub_71F870)(this);
}

// inlined @ sub_612510
// inlined @ sub_612620
// inlined @ sub_612730
// inlined @ sub_612840
// inlined @ sub_612950
// inlined @ sub_612A60
template <typename ask_t>
bool entity_base::ask_ifcs(ask_t ask) { // the script data interface is held back and asked last; the first true answer ends it
    if (!my_ifc_storage)
        return false;

    engine_lock_scope lock(&my_ifc_storage->ifc_lock);

    generic_interface* script_data = nullptr;

    for (i32 i = 0; i < (i32)my_ifc_storage->interfaces.size; ++i) {
        generic_interface* ifc = my_ifc_storage->interfaces.array[i];

        if (ifc->get_ifc_id() == entity_ifc_script_data)
            script_data = ifc;
        else if (ask(ifc))
            return true;
    }

    return script_data && ask(script_data);
}

// sub_612510
bool entity_base::get_ifc_num(const string_hash &name, f32* value, bool document) {
    return ask_ifcs([&](generic_interface* ifc) { return ifc->get_ifc_num(name, value, document); });
}

// sub_612620
bool entity_base::set_ifc_num(const string_hash &name, f32 value, bool document) {
    return ask_ifcs([&](generic_interface* ifc) { return ifc->set_ifc_num(name, value, document); });
}

// sub_612730
bool entity_base::get_ifc_vec(const string_hash &name, vector3d* value, bool document) {
    return ask_ifcs([&](generic_interface* ifc) { return ifc->get_ifc_vec(name, value, document); });
}

// sub_612840
bool entity_base::set_ifc_vec(const string_hash &name, const vector3d &value, bool document) {
    return ask_ifcs([&](generic_interface* ifc) { return ifc->set_ifc_vec(name, value, document); });
}

// sub_612950
bool entity_base::get_ifc_str(const string_hash &name, const char** value, bool document) {
    return ask_ifcs([&](generic_interface* ifc) { return ifc->get_ifc_str(name, value, document); });
}

// sub_612A60
bool entity_base::set_ifc_str(const string_hash &name, const char* value, bool document) {
    return ask_ifcs([&](generic_interface* ifc) { return ifc->set_ifc_str(name, value, document); });
}

// sub_611A90
info_node* entity_base::get_voice() {
    if (!is_an_actor())
        return nullptr;

    ai_core* core = ((actor*)this)->get_ai_core();

    return core ? core->get_info_node(info_node_type_voice_box) : nullptr;
}

// sub_627160
void entity_base::post_unmash_fixup(amalga::resource_pack_slot* pack_slot) {
    if (my_ifc_storage)
        my_ifc_storage->set_owner(this);
}

// slot 87
u32 entity_base::get_entity_size() const {
    return ((u32 (__thiscall*)(const entity_base*))retail::sub_4E1E90)(this);
}

// sub_601C50
bool entity_base::is_hero() const {
    bool hero = ext_flags & entity_ext_flag_is_hero;

    // the answer is dropped
    if (references::g_world_ptr.read())
        is_an_actor();

    return hero;
}

// sub_601C70
void entity_base::set_hero(bool hero) {
    // the answer is dropped
    if (references::g_world_ptr.read())
        is_an_actor();

    if (hero)
        ext_flags |= entity_ext_flag_is_hero;
    else
        ext_flags &= ~entity_ext_flag_is_hero;
}

// sub_548450
void entity_base::set_invulnerable(bool invulnerable) {
    if (invulnerable)
        ext_flags |= entity_ext_flag_invulnerable;
    else
        ext_flags &= ~entity_ext_flag_invulnerable;
}

// sub_653DA0
void entity_base::set_parent(entity_base* new_parent) {
    engine_lock_scope lock(&jq::references::unk_00ffece0.get());

    if (!new_parent) {
        clear_parent();

        return;
    }

    if (new_parent == my_parent)
        return;

    if (my_parent)
        clear_parent();

    // a child sharing one po gets its own abs po
    if (!(ext_flags & entity_ext_flag_does_not_manage_abs_po) && my_abs_po == my_rel_po)
        my_abs_po = new (mm::allocate(sizeof(po))) po(*my_rel_po);

    if (!new_parent->my_child)
        new_parent->my_child = this;
    else {
        entity_base* last = new_parent->my_child;

        while (last->my_brother)
            last = last->my_brother;

        last->my_brother = this;
    }

    my_parent  = new_parent;
    my_brother = nullptr;

    update_abs_po_from_model_po();

    if (flags & entity_flag_member)
        return;

    entity_base* owner = new_parent->flags & entity_flag_member ? new_parent->get_conglom_owner() : new_parent;

    owner->add_adopted_child(this);
}

// sub_640910
void entity_base::clear_parent() {
    engine_lock_scope lock(&jq::references::unk_00ffece0.get());

    if (!my_parent)
        return;

    if (!(flags & entity_flag_member)) {
        entity_base* owner = my_parent->flags & entity_flag_member ? my_parent->get_conglom_owner() : my_parent;

        if (owner)
            owner->remove_adopted_child(this);
    }

    if (my_parent->my_child == this) {
        my_parent->my_child = my_brother;

        my_brother = nullptr;
        my_parent  = nullptr;
    } else {
        entity_base* previous = my_parent->my_child;
        entity_base* current  = previous->my_brother;

        while (current && current != this) {
            previous = current;
            current  = current->my_brother;
        }

        if (current) {
            previous->my_brother = my_brother;

            my_brother = nullptr;
            my_parent  = nullptr;
        }
    }

    update_abs_po_from_model_po();
}

// sub_601910
conglomerate* entity_base::get_conglom_owner() const {
    if (!(flags & entity_flag_member) && !is_a_conglomerate())
        return nullptr;

    if (my_conglom_root)
        return my_conglom_root;

    engine_lock_scope lock(&jq::references::unk_00ffece0.get());

    if (is_a_conglomerate())
        return (conglomerate*)this;

    if ((flags & entity_flag_member) && my_parent)
        return my_parent->get_conglom_owner();

    return nullptr;
}

// sub_601A10
entity_base* entity_base::get_highest_parent() const {
    engine_lock_scope lock(&jq::references::unk_00ffece0.get());

    if ((flags & entity_flag_member) && my_parent)
        return my_parent->get_highest_parent();

    return (entity_base*)this;
}

// sub_601AB0
bool entity_base::is_a_parent(const entity_base* par) const {
    engine_lock_scope lock(&jq::references::unk_00ffece0.get());

    return my_parent && (my_parent == par || my_parent->is_a_parent(par));
}

// sub_626820
void entity_base::update_abs_po_from_model_po() {
    engine_lock_scope lock(&jq::references::unk_00ffece0.get());

    update_child_abs_po();
}

// sub_611B60
// inlined @ sub_626820
void entity_base::update_child_abs_po() {
    if (my_abs_po != my_rel_po) {
        const vector3d last_position = my_abs_po->get_position();

        if (my_parent)
            fast_po_mul(*my_abs_po, *my_rel_po, *my_parent->my_abs_po);
        else
            *my_abs_po = *my_rel_po;

        // a bone connector squares up and keeps its translation
        if (flags & entity_flag_bone_connector) {
            const vector3d translation = my_abs_po->get_position();

            my_abs_po->fixup();
            my_abs_po->set_position(translation);
        }

        const vector3d &position = my_abs_po->get_position();

        if ((last_position.x != position.x ||
             last_position.y != position.y ||
             last_position.z != position.z) &&
            is_an_entity()                  &&
            !(flags & entity_flag_member))

            moved_entities::add_moved(my_handle);
    }

    for (entity_base* child = my_child; child; child = child->my_brother)
        child->update_child_abs_po();

    if (is_an_actor()) {
        ((actor*)this)->update_colgeom(nullptr);
        ((actor*)this)->radius_changed(true);
    } else
        ext_flags |= entity_ext_flag_bounding_sphere_dirty;

    if (flags & entity_flag_member)
        my_conglom_root->radius_changed(true);
}

// sub_640830
void entity_base::update_attached_abs_pos() {
    engine_lock_scope lock(&jq::references::unk_00ffece0.get());

    if (is_a_conglomerate() && ((conglomerate*)this)->bone_connectors)
        for (entity_base* connector : *((conglomerate*)this)->bone_connectors)
            connector->update_abs_po_from_model_po();

    if (adopted_children)
        for (entity_base* child : *adopted_children) {
            child->update_abs_po_from_model_po();

            moved_entities::add_moved(child->my_handle);
        }
}

// sub_626BE0
void entity_base::set_rel_po(const po &the_po) {
    const vector3d last_position = my_abs_po->get_position();

    *my_rel_po = the_po;

    update_abs_po_from_model_po();

    if (my_abs_po != my_rel_po)
        return;

    const vector3d &position = my_abs_po->get_position();

    if ((last_position.x != position.x ||
         last_position.y != position.y ||
         last_position.z != position.z) &&
        is_an_entity()                  &&
        !(flags & entity_flag_member))

        moved_entities::add_moved(my_handle);
}

// sub_626CA0
void entity_base::set_rel_position(const vector3d &p) {
    const vector3d last_position = my_abs_po->get_position();

    my_rel_po->set_position(p);

    update_abs_po_from_model_po();

    if (my_abs_po != my_rel_po)
        return;

    const vector3d &position = my_abs_po->get_position();

    if ((last_position.x != position.x ||
         last_position.y != position.y ||
         last_position.z != position.z) &&
        is_an_entity()                  &&
        !(flags & entity_flag_member))

        moved_entities::add_moved(my_handle);
}

// sub_626E30
void entity_base::look_at(const vector3d &abs_pos) { // an up that comes out too short leaves the po alone
    const vector3d &from = my_abs_po->get_position();

    const vector3d to_target((f32)((f64)abs_pos.x - from.x),
                             (f32)((f64)abs_pos.y - from.y),
                             (f32)((f64)abs_pos.z - from.z));

    const f32 distance = (f32)std::sqrt((f64)to_target.length2());

    vector3d forward;

    if (distance > 0.001f) {
        const f32 inverse = (f32)(1.0 / distance);

        forward = vector3d((f32)((f64)to_target.x * inverse),
                           (f32)((f64)to_target.y * inverse),
                           (f32)((f64)to_target.z * inverse));
    } else
        forward = references::zvec.get();

    // world up crossed with the forward
    const vector3d level(forward.z, 0.0f, -0.0f - forward.x);

    const f32 level_length = (f32)std::sqrt((f64)level.length2());

    vector3d side;

    if (level_length > 0.00001f) {
        const f32 inverse = (f32)(1.0 / level_length);

        side = vector3d((f32)((f64)level.x * inverse),
                        (f32)((f64)level.y * inverse),
                        (f32)((f64)level.z * inverse));
    } else
        side = references::xvec.get();

    const vector3d up((f32)((f64)side.z * forward.y - (f64)side.y * forward.z),
                      (f32)((f64)side.x * forward.z - (f64)side.z * forward.x),
                      (f32)((f64)side.y * forward.x - (f64)side.x * forward.y));

    if (0.01f > up.length2())
        return;

    po new_po;
    new_po.set_po(side, up, forward, references::unk_00ffec30.get());

    if (my_parent)
        fast_po_mul(new_po, new_po, my_parent->my_abs_po->inverse());

    new_po.fixup();
    new_po.set_position(my_rel_po->get_position());

    set_rel_po(new_po);
}

// sub_583770
// inlined @ sub_66CE80
// inlined @ sub_6AAD80
// inlined @ sub_847920
// inlined @ sub_847990
void entity_base::set_fader_override_preset_closest_to_distance(f32 fade_distance, only_if_you_know_what_you_are_doing magic) {
    const i32 preset_index = retail::sub_77F090(fade_distance); // distance_fader::get_enx_range_index_by_fading_distance

    ext_flags = preset_index | (ext_flags & ~0xFu);

    on_fade_distance_changed(preset_index, magic);
}

// sub_6123B0
// inlined @ sub_66CE80
void entity_base::on_fade_distance_changed(i32 preset_index, only_if_you_know_what_you_are_doing magic) {
    if (!is_an_entity() || !preset_index)
        return;

    if (references::enx_ranges.get()[preset_index & 0xF].fade_min_dist > 250.0) {
        ext_flags |= entity_ext_flag_unk_00200000;

        retail::sub_979700(my_handle.value, magic == i_know_what_i_am_doing); // highlight_list_add
        retail::sub_979700(my_handle.value, false);
    } else
        ext_flags &= ~entity_ext_flag_unk_00200000;
}

// sub_627180
void treyarch::entity_set_abs_po(entity_base* ent, const po &the_po) {
    if (!ent->my_parent) {
        ent->set_rel_po(the_po);

        return;
    }

    po new_po;
    new_po.identity();

    fast_po_mul(new_po, the_po, ent->my_parent->my_abs_po->inverse());

    ent->set_rel_po(new_po);
}

// sub_6271F0
void treyarch::entity_set_abs_position(entity_base* ent, const vector3d &pos) {
    if (!ent->my_parent) {
        ent->set_rel_position(pos);

        return;
    }

    ent->set_rel_position(ent->my_parent->my_abs_po->inverse_xform(pos));
}

// sub_653EB0
void treyarch::entity_set_abs_parent(entity_base* me, entity_base* parent) {
    if (parent) {
        if (me->my_parent == parent)
            return;

        po world_po = *me->my_abs_po;

        me->set_parent(parent);

        fast_po_mul(world_po, world_po, parent->my_abs_po->inverse());

        me->set_rel_po(world_po);
    } else if (me->my_parent) {
        po world_po = *me->my_abs_po;

        me->clear_parent();
        me->set_rel_po(world_po);
    }
}

// sub_42EA60
void treyarch::entity_set_facing_and_up(actor* ent, const vector3d &facing, const vector3d &up, bool force) {
    if (ent->anim_ctrl && retail::sub_5AD250(ent->anim_ctrl)) // animation_controller::scene_animation_playing
        return;

    if ((ent->ext_flags & entity_ext_flag_sync_anim) && !force)
        return;

    if (ent->has_ifc(entity_ifc_physical) && ent->physical_ifc()->is_flagged(physical_interface_flag_biped_running))
        return;

    vector3d new_facing     = facing;
    vector3d new_up         = up;
    bool     facing_from_po = false;

    if (0.0001f > new_facing.length2()) {
        new_facing     = ent->my_abs_po->get_z_facing();
        facing_from_po = true;
    }

    if (0.0001f > new_up.length2())
        new_up = ent->my_abs_po->get_y_facing();

    new_up     = math::unitize(new_up);
    new_facing = math::unitize(new_facing);

    f32 dot = new_facing * new_up;

    if (std::fabs(dot) > 0.9f && !facing_from_po) {
        new_facing = ent->my_abs_po->get_z_facing();
        new_facing.normalize();

        dot = new_facing * new_up;
    }

    // still along the up, so one of the po's own axes stands in
    if (dot > 0.9f) {
        const vector3d &y = ent->my_abs_po->get_y_facing();

        new_facing = vector3d(-0.0f - y.x, -0.0f - y.y, -0.0f - y.z);
    } else if (-0.9f > dot)
        new_facing = ent->my_abs_po->get_y_facing();

    const vector3d &position = ent->my_abs_po->get_position();

    po new_po;
    new_po.set_po(math::vector4(new_facing.x, new_facing.y, new_facing.z, 0.0f),
                  math::vector4(new_up.x,     new_up.y,     new_up.z,     0.0f),
                  math::vector4(position.x,   position.y,   position.z,   0.0f));

    entity_set_abs_po(ent, new_po);
}

// sub_653CF0
void entity_base::add_adopted_child(entity_base* child) {
    engine_lock_scope lock(&jq::references::unk_00ffece0.get());

    if (!adopted_children)
        adopted_children = new (mm::allocate(sizeof(vc8::vector<entity_base*>))) vc8::vector<entity_base*>();

    if (std::find(adopted_children->begin(), adopted_children->end(), child) == adopted_children->end())
        adopted_children->push_back(child);
}

// sub_6407B0
void entity_base::remove_adopted_child(entity_base* child) {
    engine_lock_scope lock(&jq::references::unk_00ffece0.get());

    if (!adopted_children)
        return;

    entity_base** found = std::find(adopted_children->begin(), adopted_children->end(), child);

    if (found != adopted_children->end())
        adopted_children->erase(found);
}

// sub_65AF10
void entity_base::remove_child(entity_base* bad_kid) {
    engine_lock_scope lock(&jq::references::unk_00ffece0.get());

    if (bad_kid->flags & entity_flag_member)
        bad_kid->clear_parent();
    else
        entity_set_abs_parent(bad_kid, nullptr);
}

// sub_65AD10
void entity_base::clear_adopted_children() { // a child with no parent is never removed, so this spins on it
    engine_lock_scope lock(&jq::references::unk_00ffece0.get());

    if (!adopted_children)
        return;

    while (!adopted_children->empty()) {
        entity_base* child = adopted_children->front();

        if (!child->my_parent)
            continue;

        po world_po = *child->my_abs_po;

        child->clear_parent();
        child->set_rel_po(world_po);
    }

    if (adopted_children) {
        adopted_children->destroy();

        mm::free(adopted_children);
    }

    adopted_children = nullptr;
}

// sub_639270
void entity_base::dirty_adopted_children() {
    engine_lock_scope lock(&jq::references::unk_00ffece0.get());

    if (!adopted_children)
        return;

    for (entity_base* child : *adopted_children)
        if (child->is_an_entity())
            moved_entities::add_moved(child->my_handle);
}

// sub_627420
void entity_base::stop_owned_effects(entity_base* ent) {
    if (ent->has_ise_ifc())
        ent->ise_ifc()->unk_020();

    if (ent->has_tentacle_ifc())
        retail::sub_61FC80((u32*)ent->tentacle_ifc()); // tentacle effects

    if (ent->is_an_actor()) {
        ai_core* core = ((actor*)ent)->get_ai_core();

        if (core)
            core->stop_looping_effects();
    }
}

// sub_601CA0
bool entity_base::set_flag_in_the_moved_list() {
    for (;;) {
        const u32 seen = ext_flags;

        if (seen & entity_ext_flag_added_to_list_of_moved_entities)
            return false;

        if ((u32)_InterlockedCompareExchange((volatile long*)&ext_flags,
                                             (long)(seen | entity_ext_flag_added_to_list_of_moved_entities),
                                             (long)seen) == seen)

            return true;
    }
}

// sub_6267E0
generic_interface* entity_base::find_ifc_by_name(string_hash name) {
    if (!my_ifc_storage)
        return nullptr;

    return my_ifc_storage->find_ifc_by_name(name);
}

script_data_interface* entity_base::create_script_data_ifc() {
    return (script_data_interface*)retail::sub_64A8B0((i32*)this);
}

physical_interface* entity_base::create_physical_ifc() {
    return (physical_interface*)retail::sub_660CB0((i32*)this);
}

damage_interface* entity_base::create_damage_ifc() {
    return (damage_interface*)retail::sub_660B80((i32*)this);
}

time_interface* entity_base::create_time_ifc() {
    return (time_interface*)retail::sub_65ABE0((i32*)this);
}
