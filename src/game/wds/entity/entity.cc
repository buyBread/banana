#include "retail.hh"
#include "game/wds/entity/entity.hh"

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

// slot 6
bool entity::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const entity*, mash::virtual_types_key))retail::sub_4E2050)(this, parent_class);
}

// slot 8
i32 entity::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const entity*))retail::sub_4BF7E0)(this);
}

// slot 11
bool entity::is_an_entity() const {
    return ((bool (__thiscall*)(const entity*))retail::sub_667F80)(this);
}

// slot 67
void entity::set_visible(bool visible, bool from_conglom) {
    ((void (__thiscall*)(entity*, bool, bool))retail::sub_645060)(this, visible, from_conglom);
}

// slot 75
bool entity::was_occluded_last_frame() {
    return ((bool (__thiscall*)(entity*))retail::sub_4E1F70)(this);
}

// slot 77
e_entity_flavor entity::get_flavor() const {
    return ((e_entity_flavor (__thiscall*)(const entity*))retail::sub_719110)(this);
}

// slot 87
u32 entity::get_entity_size() const {
    return ((u32 (__thiscall*)(const entity*))retail::sub_4BF7E0)(this);
}

// slot 88
void entity::clear_region(region* r, i32 i_know_what_i_am_doing) {
    ((void (__thiscall*)(entity*, region*, i32))retail::sub_612C20)(this, r, i_know_what_i_am_doing);
}

// slot 89
void entity::compute_sector(terrain* ter, bool use_high_res_intersect, entity* original) {
    ((void (__thiscall*)(entity*, terrain*, bool, entity*))retail::sub_612D10)(this, ter, use_high_res_intersect, original);
}

// slot 90
void entity::force_region(region* r) {
    ((void (__thiscall*)(entity*, region*))retail::sub_612DB0)(this, r);
}

// slot 91
void entity::force_current_region() {
    ((void (__thiscall*)(entity*))retail::sub_4E1F90)(this);
}

// slot 92
void entity::unforce_regions() {
    ((void (__thiscall*)(entity*))retail::sub_612E10)(this);
}

// slot 93
void entity::force_regions(entity* e) {
    ((void (__thiscall*)(entity*, entity*))retail::sub_612E30)(this, e);
}

// slot 94
void entity::set_family_visible(bool visible) {
    ((void (__thiscall*)(entity*, bool))retail::sub_601E90)(this, visible);
}

// slot 95
u32 entity::possibly_render() const {
    return ((u32 (__thiscall*)(const entity*))retail::sub_601EE0)(this);
}

// slot 96
u32 entity::possibly_collide() const {
    return ((u32 (__thiscall*)(const entity*))retail::sub_601F10)(this);
}

// slot 97
bool entity::possibly_camera_collide() const {
    return ((bool (__thiscall*)(const entity*))retail::sub_601F40)(this);
}

// slot 98
bool entity::possibly_walkable() const {
    return ((bool (__thiscall*)(const entity*))retail::sub_601F80)(this);
}

// slot 99
void entity::region_update_poss_collide() {
    ((void (__thiscall*)(entity*))retail::sub_612F60)(this);
}

// slot 100
bool entity::unk_100() {
    return ((bool (__thiscall*)(entity*))retail::sub_71F870)(this);
}

// slot 101
void entity::unk_101(bool on) {
    ((void (__thiscall*)(entity*, bool))retail::sub_6450D0)(this, on);
}

// slot 102
bool entity::unk_102() const {
    return ((bool (__thiscall*)(const entity*))retail::sub_4E1FA0)(this);
}

// slot 103
void entity::unk_103(bool on) {
    ((void (__thiscall*)(entity*, bool))retail::sub_601FE0)(this, on);
}

// slot 104
bool entity::unk_104() const {
    return ((bool (__thiscall*)(const entity*))retail::sub_4E1FB0)(this);
}

// slot 105
void entity::frame_advance(f32 t) {
    ((void (__thiscall*)(entity*, f32))retail::sub_689610)(this, t);
}

// slot 106
bool entity::is_still_visible() const {
    return ((bool (__thiscall*)(const entity*))retail::sub_4E1FC0)(this);
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

// slot 113
f32 entity::get_visual_radius() const {
    return ((f32 (__thiscall*)(const entity*))retail::sub_601DF0)(this);
}

// slot 114
vector3d entity::get_visual_center() const {
    vector3d result;
    ((vector3d* (__thiscall*)(const entity*, vector3d*))retail::sub_601E30)(this, &result);

    return result;
}

// slot 115
vector3d entity::unk_115() {
    vector3d result;
    ((vector3d* (__thiscall*)(entity*, vector3d*))retail::sub_4E1FE0)(this, &result);

    return result;
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

// slot 119
color32 entity::get_render_color() const {
    color32 result;
    ((color32* (__thiscall*)(const entity*, color32*))retail::sub_4EB5B0)(this, &result);

    return result;
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

// slot 123
vector3d entity::get_render_scale() const {
    vector3d result;
    ((vector3d* (__thiscall*)(const entity*, vector3d*))retail::sub_4E2000)(this, &result);

    return result;
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
void* entity::get_light_set() const {
    return ((void* (__thiscall*)(const entity*))retail::sub_A944D0)(this);
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

// slot 131
void entity::set_recursive_age(f32 age) {
    ((void (__thiscall*)(entity*, f32))retail::sub_4E2020)(this, age);
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

// slot 135
mash::string* entity::get_signaller_owner_name(mash::string* result) const {
    return ((mash::string* (__thiscall*)(const entity*, mash::string*))retail::sub_601FB0)(this, result);
}

// sub_612D30
region* entity::get_primary_region() {
    entity* owner = this;

    // flag 0x8000 entities borrow the regions of the first parent without it
    if (owner->flags & 0x8000) {
        do {
            owner = (entity*)retail::sub_601910((u32*)owner);

            if (!owner)
                return nullptr;
        } while (owner->flags & 0x8000);
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
