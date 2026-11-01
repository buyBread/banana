#include "retail.hh"
#include "game/wds/entity/interface/spiderman_rvb_player_interface.hh"
#include "game/script/script_access.hh"
#include "game/wds/entity/actor.hh"

using namespace treyarch;

string_hash spiderman_rvb_player_interface::my_type_key { 0xFA62ACC8 }; // "spiderman_rvb_player_interface"

// slot 0
spiderman_rvb_player_interface::~spiderman_rvb_player_interface() {
    retail::sub_7219C0((u32*)this);

    // retail's destructor tore down player_interface's members too; empty the button arrays so their source destructors don't free them again
    buttons.array         = nullptr;
    command_buttons.array = nullptr;
}

// slot 1
void spiderman_rvb_player_interface::construct_mashed_class() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9C2470)(this);
}

// slot 2
void spiderman_rvb_player_interface::destruct_mashed_class() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9C2AA0)(this);
}

// slot 3
void spiderman_rvb_player_interface::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_9C25C0)(this,
                                                                                                                                 mash_info,
                                                                                                                                 containing_class_ptr,
                                                                                                                                 buffer);
}

// slot 4
mash::virtual_types_key spiderman_rvb_player_interface::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const spiderman_rvb_player_interface*))retail::sub_721920)(this);
}

// slot 6
bool spiderman_rvb_player_interface::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const spiderman_rvb_player_interface*, mash::virtual_types_key))retail::sub_721930)(this, parent_class);
}

// slot 8
i32 spiderman_rvb_player_interface::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const spiderman_rvb_player_interface*))retail::sub_721970)(this);
}

// slot 9
bool spiderman_rvb_player_interface::get_ifc_num(const string_hash &name, f32* value, bool document) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, const string_hash&, f32*, bool))retail::sub_9B5730)(this, name, value, document);
}

// slot 10
bool spiderman_rvb_player_interface::set_ifc_num(const string_hash &name, f32 value, bool document) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, const string_hash&, f32, bool))retail::sub_9B6060)(this, name, value, document);
}

// slot 11
bool spiderman_rvb_player_interface::get_ifc_vec(const string_hash &name, vector3d* value, bool document) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, const string_hash&, vector3d*, bool))retail::sub_9A67A0)(this, name, value, document);
}

// slot 18
void spiderman_rvb_player_interface::set_owner(actor* owner) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, actor*))retail::sub_9A4280)(this, owner);
}

// slot 21
f32 spiderman_rvb_player_interface::unk_021() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4140)(this);
}

// slot 22
i32 spiderman_rvb_player_interface::unk_022(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_99CE50)(this, unk);
}

// slot 23
bool spiderman_rvb_player_interface::unk_023(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_99CEC0)(this, unk);
}

// sub_9A7610
void spiderman_rvb_player_interface::unk_024(actor* owner) {
    master_object = new script_object_instance();

    arch_base_vhandle owner_handle = owner->my_handle;

    script_arg owner_arg;
    owner_arg.type  = 3;
    owner_arg.value = &owner_handle;

    master_object->init(mash::string("spiderman_rvb"),
                        mash::string("spiderman_rvb_master_object(entity)"),
                        owner->pack_slot,
                        owner,
                        nullptr,
                        true,
                        owner_arg,
                        script_arg(),
                        script_arg(),
                        script_arg(),
                        script_arg(),
                        script_arg(),
                        script_arg(),
                        script_arg());

    master_object->register_function(mash::string("frame_advance(num)"), 0);
}

// slot 25
void* spiderman_rvb_player_interface::unk_025(i32 unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A9550)(this, unk);
}

// slot 26
bool spiderman_rvb_player_interface::unk_026() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7216A0)(this);
}

// slot 27
bool spiderman_rvb_player_interface::unk_027(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7216B0)(this, unk);
}

// slot 28
bool spiderman_rvb_player_interface::unk_028(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7216C0)(this, unk);
}

// slot 29
bool spiderman_rvb_player_interface::unk_029() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7216D0)(this);
}

// slot 30
f32 spiderman_rvb_player_interface::unk_030() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7216E0)(this);
}

// slot 31
i32 spiderman_rvb_player_interface::unk_031(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7216F0)(this, unk);
}

// slot 32
i32 spiderman_rvb_player_interface::unk_032(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721710)(this, unk);
}

// slot 34
i32 spiderman_rvb_player_interface::unk_034(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A73C0)(this, unk);
}

// slot 35
i32 spiderman_rvb_player_interface::unk_035(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6FD0)(this, unk);
}

// slot 36
void* spiderman_rvb_player_interface::unk_036(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9A6FE0)(this, unk);
}

// slot 37
void spiderman_rvb_player_interface::set_zip_override_anchor_num(f32 num) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A6FF0)(this, num);
}

// slot 38
f32 spiderman_rvb_player_interface::unk_038() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A7010)(this);
}

// slot 39
i32 spiderman_rvb_player_interface::unk_039(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A7020)(this, unk);
}

// slot 40
void* spiderman_rvb_player_interface::unk_040(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9A7030)(this, unk);
}

// slot 41
void* spiderman_rvb_player_interface::unk_041(void* unk, i32 unk_2) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_9B7A20)(this, unk, unk_2);
}

// slot 42
i32 spiderman_rvb_player_interface::unk_042() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721220)(this);
}

// slot 43
void spiderman_rvb_player_interface::unk_043(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A70D0)(this, unk);
}

// slot 44
bool spiderman_rvb_player_interface::unk_044(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721200)(this, unk);
}

// slot 45
bool spiderman_rvb_player_interface::unk_045() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7211F0)(this);
}

// slot 46
void spiderman_rvb_player_interface::unk_046(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9B77A0)(this, unk);
}

// slot 47
void spiderman_rvb_player_interface::unk_047(u32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, u32))retail::sub_9B77F0)(this, unk);
}

// slot 48
void* spiderman_rvb_player_interface::unk_048(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9A7050)(this, unk);
}

// slot 49
bool spiderman_rvb_player_interface::unk_049(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9B7B50)(this, unk);
}

// slot 50
i32 spiderman_rvb_player_interface::unk_050(arch_base_vhandle target) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, arch_base_vhandle))retail::sub_9A70A0)(this, target);
}

// slot 51
i32 spiderman_rvb_player_interface::unk_051(i32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32))retail::sub_9B7B10)(this, unk, unk_2, unk_3);
}

// slot 52
void* spiderman_rvb_player_interface::unk_052(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9A7060)(this, unk);
}

// slot 53
void* spiderman_rvb_player_interface::unk_053(void* unk, void* unk_2) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*, void*))retail::sub_9A7070)(this, unk, unk_2);
}

// slot 54
void spiderman_rvb_player_interface::unk_054(i32 unk, i32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9B7950)(this, unk, unk_2);
}

// slot 55
void* spiderman_rvb_player_interface::unk_055(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_721240)(this, unk);
}

// slot 56
f32 spiderman_rvb_player_interface::unk_056() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721250)(this);
}

// slot 57
i32 spiderman_rvb_player_interface::unk_057() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721260)(this);
}

// slot 60
i32 spiderman_rvb_player_interface::unk_060(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721280)(this, unk);
}

// slot 61
arch_base_vhandle spiderman_rvb_player_interface::unk_061() {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(spiderman_rvb_player_interface*, arch_base_vhandle*))retail::sub_721270)(this, &result);

    return result;
}

// slot 62
void* spiderman_rvb_player_interface::unk_062(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_721290)(this, unk);
}

// slot 63
i32 spiderman_rvb_player_interface::unk_063(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7212C0)(this, unk);
}

// slot 64
i32 spiderman_rvb_player_interface::unk_064(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7212E0)(this, unk);
}

// slot 65
void* spiderman_rvb_player_interface::unk_065(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_721330)(this, unk);
}

// slot 66
void spiderman_rvb_player_interface::unk_066(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32))retail::sub_7212F0)(this, unk, unk_2, unk_3);
}

// slot 67
i32 spiderman_rvb_player_interface::unk_067(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721350)(this, unk);
}

// slot 68
bool spiderman_rvb_player_interface::unk_068(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721370)(this, unk);
}

// slot 69
bool spiderman_rvb_player_interface::unk_069() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721380)(this);
}

// slot 70
i32 spiderman_rvb_player_interface::unk_070(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721320)(this, unk);
}

// slot 71
void* spiderman_rvb_player_interface::unk_071(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_721340)(this, unk);
}

// slot 72
bool spiderman_rvb_player_interface::unk_072() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_667F80)(this);
}

// slot 74
bool spiderman_rvb_player_interface::unk_074() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_667F80)(this);
}

// slot 76
i32 spiderman_rvb_player_interface::unk_076() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_A944D0)(this);
}

// slot 77
i32 spiderman_rvb_player_interface::unk_077() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_ABD4A0)(this);
}

// slot 78
void spiderman_rvb_player_interface::set_swing_override_anchor(arch_base_vhandle anchor) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, arch_base_vhandle))retail::sub_9A59D0)(this, anchor);
}

// slot 79
void spiderman_rvb_player_interface::set_zip_override_anchor(arch_base_vhandle anchor) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, arch_base_vhandle))retail::sub_9A59E0)(this, anchor);
}

// slot 80
void* spiderman_rvb_player_interface::unk_080(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9A59F0)(this, unk);
}

// slot 81
void spiderman_rvb_player_interface::unk_081(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A9100)(this, unk);
}

// slot 82
i32 spiderman_rvb_player_interface::unk_082(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A88B0)(this, unk);
}

// slot 84
f32 spiderman_rvb_player_interface::unk_084() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4EB0)(this);
}

// slot 86
void spiderman_rvb_player_interface::unk_086(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A5210)(this, unk);
}

// slot 87
bool spiderman_rvb_player_interface::unk_087() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720E10)(this);
}

// slot 88
bool spiderman_rvb_player_interface::unk_088(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720E20)(this, unk);
}

// slot 89
bool spiderman_rvb_player_interface::map_controls(i32 config, bool force) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32, bool))retail::sub_9A47B0)(this, config, force);
}

// slot 90
void spiderman_rvb_player_interface::render_phase() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A47A0)(this);
}

// slot 91
void spiderman_rvb_player_interface::update_interface() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9BB0E0)(this);
}

// slot 92
void spiderman_rvb_player_interface::frame_advance(f32 delta_t) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9BB110)(this, delta_t);
}

// slot 94
void spiderman_rvb_player_interface::unk_094(f32 delta_t) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9BBC80)(this, delta_t);
}

// slot 96
bool spiderman_rvb_player_interface::unk_096(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720DD0)(this, unk);
}

// slot 97
bool spiderman_rvb_player_interface::unk_097() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720DE0)(this);
}

// slot 98
bool spiderman_rvb_player_interface::unk_098(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720DF0)(this, unk);
}

// slot 99
i32 spiderman_rvb_player_interface::unk_099() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4400)(this);
}

// slot 100
i32 spiderman_rvb_player_interface::unk_100() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4440)(this);
}

// slot 101
i32 spiderman_rvb_player_interface::unk_101() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4480)(this);
}

// slot 107
i32 spiderman_rvb_player_interface::unk_107() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720E30)(this);
}

// slot 108
i32 spiderman_rvb_player_interface::unk_108(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720E40)(this, unk);
}

// slot 113
i32 spiderman_rvb_player_interface::unk_113() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A44A0)(this);
}

// slot 114
bool spiderman_rvb_player_interface::unk_114(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A44F0)(this, unk);
}

// slot 115
i32 spiderman_rvb_player_interface::unk_115() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4510)(this);
}

// slot 116
bool spiderman_rvb_player_interface::unk_116(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A4560)(this, unk);
}

// slot 118
i32 spiderman_rvb_player_interface::unk_118() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4580)(this);
}

// slot 119
bool spiderman_rvb_player_interface::unk_119(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A45A0)(this, unk);
}

// slot 121
i32 spiderman_rvb_player_interface::unk_121() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A77B0)(this);
}

// slot 125
bool spiderman_rvb_player_interface::unk_125() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720E50)(this);
}

// slot 126
bool spiderman_rvb_player_interface::unk_126() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720E60)(this);
}

// slot 129
bool spiderman_rvb_player_interface::unk_129(f32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A77D0)(this, unk);
}

// slot 130
bool spiderman_rvb_player_interface::unk_130() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A67B0)(this);
}

// slot 131
bool spiderman_rvb_player_interface::unk_131() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A67C0)(this);
}

// slot 134
bool spiderman_rvb_player_interface::unk_134(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6550)(this, unk);
}

// slot 135
bool spiderman_rvb_player_interface::unk_135(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A65D0)(this, unk);
}

// slot 136
i32 spiderman_rvb_player_interface::unk_136() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9B0A10)(this);
}

// slot 137
i32 spiderman_rvb_player_interface::unk_137() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A45C0)(this);
}

// slot 139
i32 spiderman_rvb_player_interface::unk_139() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A45F0)(this);
}

// slot 141
i32 spiderman_rvb_player_interface::unk_141() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4620)(this);
}

// slot 145
void spiderman_rvb_player_interface::unk_145(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A5280)(this, unk);
}

// slot 146
void spiderman_rvb_player_interface::unk_146(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A52D0)(this, unk);
}

// slot 147
void spiderman_rvb_player_interface::unk_147(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A5310)(this, unk);
}

// slot 148
f32 spiderman_rvb_player_interface::unk_148() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A5350)(this);
}

// slot 149
f32 spiderman_rvb_player_interface::unk_149() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720EA0)(this);
}

// slot 152
bool spiderman_rvb_player_interface::unk_152() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720EB0)(this);
}

// slot 153
bool spiderman_rvb_player_interface::unk_153(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720EC0)(this, unk);
}

// slot 156
bool spiderman_rvb_player_interface::unk_156() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_99CDE0)(this);
}

// slot 157
i32 spiderman_rvb_player_interface::unk_157() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A9A50)(this);
}

// slot 158
i32 spiderman_rvb_player_interface::unk_158(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721980)(this, unk);
}

// slot 163
void spiderman_rvb_player_interface::unk_163() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6F70)(this);
}

// slot 164
void spiderman_rvb_player_interface::unk_164() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6FA0)(this);
}

// slot 165
i32 spiderman_rvb_player_interface::unk_165() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7217E0)(this);
}

// slot 166
i32 spiderman_rvb_player_interface::unk_166() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7217F0)(this);
}

// slot 167
bool spiderman_rvb_player_interface::unk_167(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721880)(this, unk);
}

// slot 168
bool spiderman_rvb_player_interface::unk_168() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721890)(this);
}

// slot 169
bool spiderman_rvb_player_interface::unk_169(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721800)(this, unk);
}

// slot 170
bool spiderman_rvb_player_interface::unk_170() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721810)(this);
}

// slot 171
bool spiderman_rvb_player_interface::unk_171(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721820)(this, unk);
}

// slot 172
bool spiderman_rvb_player_interface::unk_172() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721830)(this);
}

// slot 173
bool spiderman_rvb_player_interface::unk_173(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721840)(this, unk);
}

// slot 174
bool spiderman_rvb_player_interface::unk_174() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721850)(this);
}

// slot 175
bool spiderman_rvb_player_interface::unk_175(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721860)(this, unk);
}

// slot 176
bool spiderman_rvb_player_interface::unk_176() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721870)(this);
}

// slot 177
bool spiderman_rvb_player_interface::unk_177(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7218A0)(this, unk);
}

// slot 178
bool spiderman_rvb_player_interface::unk_178() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7218B0)(this);
}

// slot 179
bool spiderman_rvb_player_interface::unk_179(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7218C0)(this, unk);
}

// slot 180
bool spiderman_rvb_player_interface::unk_180() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7218D0)(this);
}

// slot 188
bool spiderman_rvb_player_interface::unk_188() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6000)(this);
}

// slot 192
bool spiderman_rvb_player_interface::unk_192() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720EE0)(this);
}

// slot 193
bool spiderman_rvb_player_interface::unk_193(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720EF0)(this, unk);
}

// slot 194
bool spiderman_rvb_player_interface::unk_194(f32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A98A0)(this, unk);
}

// slot 195
i32 spiderman_rvb_player_interface::unk_195() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A53B0)(this);
}

// slot 196
i32 spiderman_rvb_player_interface::unk_196(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A53D0)(this, unk);
}

// slot 197
i32 spiderman_rvb_player_interface::unk_197() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A5490)(this);
}

// slot 198
void spiderman_rvb_player_interface::unk_198() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A54A0)(this);
}

// slot 201
i32 spiderman_rvb_player_interface::unk_201(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9AAA50)(this, unk);
}

// slot 202
i32 spiderman_rvb_player_interface::unk_202() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A57D0)(this);
}

// slot 203
void spiderman_rvb_player_interface::unk_203() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A57E0)(this);
}

// slot 204
i32 spiderman_rvb_player_interface::unk_204(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A57F0)(this, unk);
}

// slot 205
void* spiderman_rvb_player_interface::unk_205(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9AA1D0)(this, unk);
}

// slot 206
i32 spiderman_rvb_player_interface::unk_206(f32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32, i32, i32))retail::sub_9AB550)(this, unk, unk_2, unk_3);
}

// slot 207
void spiderman_rvb_player_interface::unk_207() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A5E20)(this);
}

// slot 208
void spiderman_rvb_player_interface::unk_208(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720FE0)(this, unk);
}

// slot 209
void spiderman_rvb_player_interface::unk_209(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721000)(this, unk);
}

// slot 210
f32 spiderman_rvb_player_interface::unk_210() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721020)(this);
}

// slot 215
i32 spiderman_rvb_player_interface::unk_215() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7211E0)(this);
}

// slot 218
i32 spiderman_rvb_player_interface::unk_218() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9B3DC0)(this);
}

// slot 221
i32 spiderman_rvb_player_interface::unk_221(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A6680)(this, unk);
}

// slot 228
i32 spiderman_rvb_player_interface::unk_228() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9B4860)(this);
}

// slot 237
bool spiderman_rvb_player_interface::unk_237(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5, i32 unk_6, i32 unk_7) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32, i32, i32, i32, i32))retail::sub_9B4C00)(this,
                                                                                                                        unk,
                                                                                                                        unk_2,
                                                                                                                        unk_3,
                                                                                                                        unk_4,
                                                                                                                        unk_5,
                                                                                                                        unk_6,
                                                                                                                        unk_7);
}

// slot 243
void* spiderman_rvb_player_interface::unk_243(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_721060)(this, unk);
}

// slot 244
void* spiderman_rvb_player_interface::unk_244(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_721070)(this, unk);
}

// slot 245
i32 spiderman_rvb_player_interface::unk_245() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721080)(this);
}

// slot 246
f32 spiderman_rvb_player_interface::unk_246() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721090)(this);
}

// slot 247
f32 spiderman_rvb_player_interface::unk_247() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7210A0)(this);
}

// slot 248
bool spiderman_rvb_player_interface::unk_248() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7210B0)(this);
}

// slot 249
bool spiderman_rvb_player_interface::unk_249(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7210C0)(this, unk);
}

// slot 250
f32 spiderman_rvb_player_interface::unk_250() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720F00)(this);
}

// slot 251
f32 spiderman_rvb_player_interface::unk_251() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720040)(this);
}

// slot 252
void spiderman_rvb_player_interface::unk_252(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720F10)(this, unk);
}

// slot 253
f32 spiderman_rvb_player_interface::unk_253() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720F30)(this);
}

// slot 254
void spiderman_rvb_player_interface::unk_254(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720F40)(this, unk);
}

// slot 255
void spiderman_rvb_player_interface::unk_255(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9B4B20)(this, unk);
}

// slot 256
void spiderman_rvb_player_interface::unk_256() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6690)(this);
}

// slot 257
bool spiderman_rvb_player_interface::unk_257() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7210D0)(this);
}

// slot 258
void* spiderman_rvb_player_interface::unk_258(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_7210E0)(this, unk);
}

// slot 262
i32 spiderman_rvb_player_interface::unk_262(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9B2610)(this, unk, unk_2);
}

// slot 263
bool spiderman_rvb_player_interface::unk_263(void* unk, i32 unk_2) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_9B4E00)(this, unk, unk_2);
}

// slot 264
bool spiderman_rvb_player_interface::unk_264(f32 delta_t, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, f32, void*, i32, i32, i32))retail::sub_9BDEF0)(this, delta_t, unk_2, unk_3, unk_4, unk_5);
}

// slot 265
bool spiderman_rvb_player_interface::unk_265() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9B4A60)(this);
}

// slot 266
void spiderman_rvb_player_interface::unk_266(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32))retail::sub_9A66C0)(this, unk, unk_2, unk_3);
}

// slot 267
void spiderman_rvb_player_interface::unk_267() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6700)(this);
}

// slot 268
bool spiderman_rvb_player_interface::unk_268() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721030)(this);
}

// slot 269
i32 spiderman_rvb_player_interface::unk_269(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721040)(this, unk);
}

// slot 270
bool spiderman_rvb_player_interface::unk_270() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9B49B0)(this);
}

// slot 271
bool spiderman_rvb_player_interface::unk_271() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9B4B50)(this);
}

// slot 272
bool spiderman_rvb_player_interface::unk_272() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9BD1D0)(this);
}

// slot 273
entity* spiderman_rvb_player_interface::unk_273(entity* target) {
    return ((entity* (__thiscall*)(spiderman_rvb_player_interface*, entity*))retail::sub_9A6740)(this, target);
}

// slot 284
bool spiderman_rvb_player_interface::unk_284() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_667F80)(this);
}

// slot 305
bool spiderman_rvb_player_interface::unk_305() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720DA0)(this);
}

// slot 306
i32 spiderman_rvb_player_interface::unk_306() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A43E0)(this);
}

// slot 307
i32 spiderman_rvb_player_interface::unk_307() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A43F0)(this);
}

// slot 308
f32 spiderman_rvb_player_interface::unk_308() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720DB0)(this);
}

// slot 310
f32 spiderman_rvb_player_interface::unk_310() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720DC0)(this);
}

// slot 315
f32 spiderman_rvb_player_interface::unk_315() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721690)(this);
}

// slot 319
bool spiderman_rvb_player_interface::unk_319(f32 delta_t, game_button* button, i32 unk_3) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, f32, game_button*, i32))retail::sub_9BC680)(this, delta_t, button, unk_3);
}

// slot 320
void* spiderman_rvb_player_interface::unk_320() {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7213E0)(this);
}

// slot 321
void* spiderman_rvb_player_interface::unk_321(const line_info* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, const line_info*))retail::sub_7213F0)(this, unk);
}

// slot 322
bool spiderman_rvb_player_interface::unk_322(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A5050)(this, unk);
}

// slot 323
void* spiderman_rvb_player_interface::unk_323(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_7214F0)(this, unk);
}

// slot 324
i32 spiderman_rvb_player_interface::unk_324(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721520)(this, unk);
}

// slot 325
f32 spiderman_rvb_player_interface::unk_325() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4FE0)(this);
}

// slot 326
bool spiderman_rvb_player_interface::unk_326() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721540)(this);
}

// slot 327
bool spiderman_rvb_player_interface::unk_327(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721550)(this, unk);
}

// slot 330
void spiderman_rvb_player_interface::unk_330(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32, i32))retail::sub_9A4EC0)(this, unk, unk_2);
}

// slot 331
void spiderman_rvb_player_interface::unk_331(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32, i32))retail::sub_9A4F00)(this, unk, unk_2);
}

// slot 332
void spiderman_rvb_player_interface::unk_332(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32, i32))retail::sub_9A4F40)(this, unk, unk_2);
}

// slot 333
i32 spiderman_rvb_player_interface::unk_333(f32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32, i32))retail::sub_9A4F80)(this, unk, unk_2);
}

// slot 334
void spiderman_rvb_player_interface::unk_334(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32, i32))retail::sub_9A5070)(this, unk, unk_2);
}

// slot 335
void spiderman_rvb_player_interface::unk_335(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32, i32))retail::sub_9A50B0)(this, unk, unk_2);
}

// slot 336
void spiderman_rvb_player_interface::unk_336(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32, i32))retail::sub_9A50F0)(this, unk, unk_2);
}

// slot 337
i32 spiderman_rvb_player_interface::unk_337() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721400)(this);
}

// slot 338
i32 spiderman_rvb_player_interface::unk_338() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721420)(this);
}

// slot 339
i32 spiderman_rvb_player_interface::unk_339() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721440)(this);
}

// slot 340
bool spiderman_rvb_player_interface::unk_340() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721460)(this);
}

// slot 341
i32 spiderman_rvb_player_interface::unk_341() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721480)(this);
}

// slot 342
i32 spiderman_rvb_player_interface::unk_342() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7214A0)(this);
}

// slot 343
i32 spiderman_rvb_player_interface::unk_343() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7214C0)(this);
}

// slot 344
void spiderman_rvb_player_interface::unk_344(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A6A40)(this, unk);
}

// slot 345
void spiderman_rvb_player_interface::unk_345(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6A60)(this, unk);
}

// slot 346
void spiderman_rvb_player_interface::unk_346(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A6A80)(this, unk);
}

// slot 347
void spiderman_rvb_player_interface::unk_347(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A6AA0)(this, unk);
}

// slot 348
void spiderman_rvb_player_interface::unk_348(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A6AC0)(this, unk);
}

// slot 349
void spiderman_rvb_player_interface::unk_349(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6AE0)(this, unk);
}

// slot 350
void spiderman_rvb_player_interface::unk_350(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6B00)(this, unk);
}

// slot 351
void spiderman_rvb_player_interface::unk_351(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6B60)(this, unk);
}

// slot 352
void spiderman_rvb_player_interface::unk_352(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6B20)(this, unk);
}

// slot 353
void spiderman_rvb_player_interface::unk_353(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6B80)(this, unk);
}

// slot 354
void spiderman_rvb_player_interface::unk_354(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6BA0)(this, unk);
}

// slot 355
void spiderman_rvb_player_interface::unk_355(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6BC0)(this, unk);
}

// slot 356
void spiderman_rvb_player_interface::unk_356(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6B40)(this, unk);
}

// slot 357
void spiderman_rvb_player_interface::unk_357(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6BE0)(this, unk);
}

// slot 358
void spiderman_rvb_player_interface::unk_358(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6C00)(this, unk);
}

// slot 359
void spiderman_rvb_player_interface::unk_359(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6C20)(this, unk);
}

// slot 360
void spiderman_rvb_player_interface::unk_360(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6C40)(this, unk);
}

// slot 361
void spiderman_rvb_player_interface::unk_361(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6C60)(this, unk);
}

// slot 363
void* spiderman_rvb_player_interface::unk_363(void* unk, void* unk_2, void* unk_3, void* unk_4, void* unk_5, void* unk_6) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*, void*, void*, void*, void*, void*))retail::sub_9A5E40)(this,
                                                                                                                                unk,
                                                                                                                                unk_2,
                                                                                                                                unk_3,
                                                                                                                                unk_4,
                                                                                                                                unk_5,
                                                                                                                                unk_6);
}

// slot 364
void* spiderman_rvb_player_interface::unk_364(void* unk, void* unk_2, void* unk_3, void* unk_4, void* unk_5, void* unk_6, void* unk_7) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*, void*, void*, void*, void*, void*, void*))retail::sub_9A5E80)(this,
                                                                                                                                       unk,
                                                                                                                                       unk_2,
                                                                                                                                       unk_3,
                                                                                                                                       unk_4,
                                                                                                                                       unk_5,
                                                                                                                                       unk_6,
                                                                                                                                       unk_7);
}

// slot 365
i32 spiderman_rvb_player_interface::unk_365(void* unk, void* unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, void*))retail::sub_9A5ED0)(this, unk, unk_2);
}

// slot 366
void* spiderman_rvb_player_interface::unk_366(void* unk, void* unk_2, void* unk_3, void* unk_4) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*, void*, void*, void*))retail::sub_9A5F00)(this, unk, unk_2, unk_3, unk_4);
}

// slot 367
i32 spiderman_rvb_player_interface::unk_367(void* unk, void* unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, void*))retail::sub_9A5F30)(this, unk, unk_2);
}

// slot 368
void spiderman_rvb_player_interface::unk_368(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32, i32))retail::sub_9AD300)(this, unk, unk_2);
}

// slot 369
void spiderman_rvb_player_interface::unk_369(i32 unk, i32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9A6130)(this, unk, unk_2);
}

// slot 370
bool spiderman_rvb_player_interface::unk_370(i32 unk, i32 unk_2, i32 unk_3) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32))retail::sub_9A4650)(this, unk, unk_2, unk_3);
}

// slot 371
i32 spiderman_rvb_player_interface::unk_371(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9A46D0)(this, unk, unk_2);
}

// slot 373
i32 spiderman_rvb_player_interface::unk_373() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4740)(this);
}

// slot 374
f32 spiderman_rvb_player_interface::unk_374() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720E70)(this);
}

// slot 375
f32 spiderman_rvb_player_interface::unk_375() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720E90)(this);
}

// slot 376
i32 spiderman_rvb_player_interface::unk_376() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721570)(this);
}

// slot 377
bool spiderman_rvb_player_interface::unk_377(vector3d* point) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, vector3d*))retail::sub_9A5FA0)(this, point);
}

// slot 378
bool spiderman_rvb_player_interface::unk_378() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9AF050)(this);
}

// slot 379
i32 spiderman_rvb_player_interface::unk_379() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7219A0)(this);
}

// slot 380
i32 spiderman_rvb_player_interface::unk_380(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7219B0)(this, unk);
}

// slot 381
f32 spiderman_rvb_player_interface::unk_381(void* unk, i32 unk_2, f32 unk_3) {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*, void*, i32, f32))retail::sub_9AD940)(this, unk, unk_2, unk_3);
}

// slot 382
bool spiderman_rvb_player_interface::unk_382(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9BEFC0)(this, unk);
}

// slot 383
i32 spiderman_rvb_player_interface::unk_383() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721580)(this);
}

// slot 384
bool spiderman_rvb_player_interface::unk_384() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9B0170)(this);
}

// slot 385
i32 spiderman_rvb_player_interface::unk_385() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721580)(this);
}

// slot 386
i32 spiderman_rvb_player_interface::unk_386(f32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32, i32))retail::sub_9B0280)(this, unk, unk_2);
}

// slot 387
void* spiderman_rvb_player_interface::unk_387() {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721590)(this);
}

// slot 388
bool spiderman_rvb_player_interface::unk_388() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7215A0)(this);
}

// slot 389
bool spiderman_rvb_player_interface::unk_389(void* unk, f32 unk_2) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, void*, f32))retail::sub_9BE960)(this, unk, unk_2);
}

// slot 390
bool spiderman_rvb_player_interface::unk_390() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9BC460)(this);
}

// slot 391
bool spiderman_rvb_player_interface::unk_391(i32 unk, i32 unk_2) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9B03A0)(this, unk, unk_2);
}

// slot 392
bool spiderman_rvb_player_interface::unk_392(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9B0510)(this, unk);
}

// slot 393
bool spiderman_rvb_player_interface::unk_393(f32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B0590)(this, unk);
}

// slot 394
i32 spiderman_rvb_player_interface::unk_394() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4780)(this);
}

// slot 395
i32 spiderman_rvb_player_interface::unk_395() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7215E0)(this);
}

// slot 396
i32 spiderman_rvb_player_interface::unk_396(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7215F0)(this, unk);
}

// slot 398
i32 spiderman_rvb_player_interface::unk_398(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A60A0)(this, unk);
}

// slot 399
i32 spiderman_rvb_player_interface::unk_399(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721600)(this, unk);
}

// slot 400
void spiderman_rvb_player_interface::unk_400(polytube* tube, f32 radius) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, polytube*, f32))retail::sub_9A6030)(this, tube, radius);
}

// slot 401
i32 spiderman_rvb_player_interface::unk_401() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6050)(this);
}

// slot 402
i32 spiderman_rvb_player_interface::unk_402(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9A4D30)(this, unk, unk_2);
}

// slot 403
f32 spiderman_rvb_player_interface::unk_403() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4EA0)(this);
}

// slot 404
f32 spiderman_rvb_player_interface::unk_404(f32 unk) {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A4C10)(this, unk);
}

// slot 405
f32 spiderman_rvb_player_interface::unk_405(f32 unk) {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A4C60)(this, unk);
}

// slot 406
f32 spiderman_rvb_player_interface::unk_406(f32 unk) {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A4CA0)(this, unk);
}

// slot 407
f32 spiderman_rvb_player_interface::unk_407(f32 unk) {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A4CF0)(this, unk);
}

// slot 408
void spiderman_rvb_player_interface::unk_408(i32 unk, i32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9B0A30)(this, unk, unk_2);
}

// slot 409
void spiderman_rvb_player_interface::unk_409(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32))retail::sub_9B0D00)(this, unk, unk_2, unk_3);
}

// slot 410
void spiderman_rvb_player_interface::unk_410(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32, i32))retail::sub_9B0E60)(this, unk, unk_2, unk_3, unk_4);
}

// slot 411
i32 spiderman_rvb_player_interface::unk_411(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9A6110)(this, unk, unk_2);
}

// slot 412
void spiderman_rvb_player_interface::unk_412(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5, i32 unk_6) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32, i32, i32, i32))retail::sub_9B1100)(this, unk, unk_2, unk_3, unk_4, unk_5, unk_6);
}

// slot 413
i32 spiderman_rvb_player_interface::unk_413(i32 unk, i32 unk_2, void* unk_3) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, void*))retail::sub_9A6180)(this, unk, unk_2, unk_3);
}

// slot 414
f32 spiderman_rvb_player_interface::unk_414() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A67D0)(this);
}

// slot 415
f32 spiderman_rvb_player_interface::unk_415() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A67E0)(this);
}

// slot 416
f32 spiderman_rvb_player_interface::unk_416() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A67F0)(this);
}

// slot 417
i32 spiderman_rvb_player_interface::unk_417() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6440)(this);
}

// slot 418
i32 spiderman_rvb_player_interface::unk_418() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721610)(this);
}

// slot 419
i32 spiderman_rvb_player_interface::unk_419() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721620)(this);
}

// slot 420
i32 spiderman_rvb_player_interface::unk_420() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721630)(this);
}

// slot 421
i32 spiderman_rvb_player_interface::unk_421() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721640)(this);
}

// slot 422
i32 spiderman_rvb_player_interface::unk_422() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721650)(this);
}

// slot 423
i32 spiderman_rvb_player_interface::unk_423() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721660)(this);
}

// slot 425
f32 spiderman_rvb_player_interface::unk_425() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A69A0)(this);
}

// slot 427
f32 spiderman_rvb_player_interface::unk_427() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A69B0)(this);
}

// slot 429
f32 spiderman_rvb_player_interface::unk_429() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A69C0)(this);
}

// slot 432
f32 spiderman_rvb_player_interface::unk_432() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A69D0)(this);
}

// slot 433
f32 spiderman_rvb_player_interface::unk_433() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A69E0)(this);
}

// slot 434
f32 spiderman_rvb_player_interface::unk_434() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4770)(this);
}

// slot 438
f32 spiderman_rvb_player_interface::unk_438() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7218F0)(this);
}

// slot 439
void spiderman_rvb_player_interface::unk_439(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721900)(this, unk);
}

// slot 440
f32 spiderman_rvb_player_interface::unk_440() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6800)(this);
}

// slot 441
f32 spiderman_rvb_player_interface::unk_441() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6810)(this);
}

// slot 442
f32 spiderman_rvb_player_interface::unk_442() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6820)(this);
}

// slot 443
f32 spiderman_rvb_player_interface::unk_443() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6830)(this);
}

// slot 444
f32 spiderman_rvb_player_interface::unk_444() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721670)(this);
}

// slot 445
f32 spiderman_rvb_player_interface::unk_445() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6860)(this);
}

// slot 446
f32 spiderman_rvb_player_interface::unk_446() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6890)(this);
}

// slot 447
f32 spiderman_rvb_player_interface::unk_447() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A68C0)(this);
}

// slot 448
f32 spiderman_rvb_player_interface::unk_448() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A68F0)(this);
}

// slot 449
f32 spiderman_rvb_player_interface::unk_449() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6900)(this);
}

// slot 450
f32 spiderman_rvb_player_interface::unk_450() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6910)(this);
}

// slot 451
f32 spiderman_rvb_player_interface::unk_451() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6920)(this);
}

// slot 452
f32 spiderman_rvb_player_interface::unk_452() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A69F0)(this);
}

// slot 453
i32 spiderman_rvb_player_interface::unk_453(i32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32))retail::sub_9A6A00)(this, unk, unk_2, unk_3);
}

// slot 454
f32 spiderman_rvb_player_interface::unk_454() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6930)(this);
}

// slot 455
i32 spiderman_rvb_player_interface::unk_455(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6940)(this, unk);
}

// slot 456
bool spiderman_rvb_player_interface::unk_456() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6960)(this);
}

// slot 457
bool spiderman_rvb_player_interface::unk_457(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6980)(this, unk);
}

// slot 458
void spiderman_rvb_player_interface::unk_458(void* unk, void* unk_2, f32 unk_3) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, void*, void*, f32))retail::sub_9A6C80)(this, unk, unk_2, unk_3);
}

// slot 459
i32 spiderman_rvb_player_interface::unk_459() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721680)(this);
}

// slot 460
i32 spiderman_rvb_player_interface::unk_460(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B5140)(this, unk);
}

// slot 461
bool spiderman_rvb_player_interface::unk_461() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7210F0)(this);
}

// slot 462
bool spiderman_rvb_player_interface::unk_462() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721110)(this);
}

// slot 463
bool spiderman_rvb_player_interface::unk_463() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721100)(this);
}

// slot 464
i32 spiderman_rvb_player_interface::unk_464() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721150)(this);
}

// slot 465
void spiderman_rvb_player_interface::unk_465() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721160)(this);
}

// slot 466
bool spiderman_rvb_player_interface::unk_466() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721170)(this);
}

// slot 467
void* spiderman_rvb_player_interface::unk_467() {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721180)(this);
}

// slot 468
void spiderman_rvb_player_interface::unk_468() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721190)(this);
}

// slot 469
bool spiderman_rvb_player_interface::unk_469() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7211A0)(this);
}

// slot 470
bool spiderman_rvb_player_interface::unk_470() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7211B0)(this);
}

// slot 471
i32 spiderman_rvb_player_interface::unk_471() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7211C0)(this);
}

// slot 472
void spiderman_rvb_player_interface::unk_472() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7211D0)(this);
}

// slot 483
void* spiderman_rvb_player_interface::unk_483(void* unk, i32 unk_2) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_9B76B0)(this, unk, unk_2);
}

// slot 484
bool spiderman_rvb_player_interface::unk_484() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A75B0)(this);
}

// slot 485
bool spiderman_rvb_player_interface::unk_485() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4200)(this);
}

// slot 486
void spiderman_rvb_player_interface::unk_486(bool unk, f32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, bool, f32))retail::sub_9A4220)(this, unk, unk_2);
}

// slot 487
bool spiderman_rvb_player_interface::unk_487() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4240)(this);
}

// slot 488
bool spiderman_rvb_player_interface::unk_488(i32 unk, i32 unk_2) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9A4260)(this, unk, unk_2);
}

// slot 489
i32 spiderman_rvb_player_interface::unk_489() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6DE0)(this);
}

// slot 490
i32 spiderman_rvb_player_interface::unk_490() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6E10)(this);
}

// slot 491
f32 spiderman_rvb_player_interface::unk_491() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6DB0)(this);
}

// slot 492
void spiderman_rvb_player_interface::unk_492(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6D80)(this, unk);
}

// slot 493
i32 spiderman_rvb_player_interface::unk_493() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6EA0)(this);
}

// slot 494
i32 spiderman_rvb_player_interface::unk_494() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6ED0)(this);
}

// slot 495
i32 spiderman_rvb_player_interface::unk_495() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6EF0)(this);
}

// slot 496
i32 spiderman_rvb_player_interface::unk_496() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6F00)(this);
}

// slot 497
i32 spiderman_rvb_player_interface::unk_497() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6F10)(this);
}

// slot 498
i32 spiderman_rvb_player_interface::unk_498() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6F20)(this);
}

// slot 499
i32 spiderman_rvb_player_interface::unk_499() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6F30)(this);
}

// slot 500
i32 spiderman_rvb_player_interface::unk_500() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6F40)(this);
}

// slot 501
i32 spiderman_rvb_player_interface::unk_501() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6F60)(this);
}

// slot 502
f32 spiderman_rvb_player_interface::unk_502() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721730)(this);
}

// slot 503
i32 spiderman_rvb_player_interface::unk_503() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721740)(this);
}

// slot 504
bool spiderman_rvb_player_interface::unk_504(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9B7670)(this, unk);
}

// slot 506
void spiderman_rvb_player_interface::unk_506(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6E30)(this, unk);
}

// slot 507
void* spiderman_rvb_player_interface::unk_507(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_721770)(this, unk);
}

// slot 508
i32 spiderman_rvb_player_interface::unk_508(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721750)(this, unk);
}

// slot 509
i32 spiderman_rvb_player_interface::unk_509() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7217A0)(this);
}

// slot 510
void spiderman_rvb_player_interface::unk_510(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6D50)(this, unk);
}

// slot 511
bool spiderman_rvb_player_interface::unk_511() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7217C0)(this);
}

// slot 512
bool spiderman_rvb_player_interface::unk_512(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7217D0)(this, unk);
}

// slot 513
bool spiderman_rvb_player_interface::unk_513(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9BEF90)(this, unk);
}

// slot 514
void* spiderman_rvb_player_interface::unk_514(i32 unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9C2C00)(this, unk);
}

// slot 515
bool spiderman_rvb_player_interface::unk_515() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9ADA50)(this);
}

// slot 517
i32 spiderman_rvb_player_interface::unk_517() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9AA4D0)(this);
}

// slot 518
bool spiderman_rvb_player_interface::unk_518(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721390)(this, unk);
}

// slot 519
bool spiderman_rvb_player_interface::unk_519() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7213A0)(this);
}

// slot 520
bool spiderman_rvb_player_interface::unk_520() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9BDA20)(this);
}

// slot 522
i32 spiderman_rvb_player_interface::unk_522() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7213C0)(this);
}

// slot 523
void spiderman_rvb_player_interface::unk_523(bool unk, bool unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, bool, bool))retail::sub_9A5130)(this, unk, unk_2);
}

// slot 524
bool spiderman_rvb_player_interface::unk_524() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7213D0)(this);
}

// slot 525
i32 spiderman_rvb_player_interface::unk_525(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9A7E90)(this, unk, unk_2);
}

// slot 526
i32 spiderman_rvb_player_interface::unk_526() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A7F00)(this);
}

// slot 527
void spiderman_rvb_player_interface::unk_527(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A7F50)(this, unk);
}

// slot 528
line_info* spiderman_rvb_player_interface::unk_528() {
    return ((line_info* (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7218E0)(this);
}

// slot 529
void* spiderman_rvb_player_interface::unk_529(i32 unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9B1250)(this, unk);
}

// slot 530
bool spiderman_rvb_player_interface::unk_530(f32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B12A0)(this, unk);
}

// slot 531
bool spiderman_rvb_player_interface::unk_531(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9B1360)(this, unk);
}

// slot 532
bool spiderman_rvb_player_interface::unk_532(const line_info* unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, const line_info*))retail::sub_9B14B0)(this, unk);
}

// slot 534
void spiderman_rvb_player_interface::unk_534(i32 unk, f32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, f32))retail::sub_9A94B0)(this, unk, unk_2);
}

// slot 536
bool spiderman_rvb_player_interface::unk_536(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9B3120)(this, unk);
}

// slot 537
bool spiderman_rvb_player_interface::unk_537(void* unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9B1F60)(this, unk);
}

// slot 538
bool spiderman_rvb_player_interface::unk_538(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9B1760)(this, unk);
}

// slot 539
f32 spiderman_rvb_player_interface::unk_539() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720E80)(this);
}

// slot 540
void spiderman_rvb_player_interface::unk_540(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A5230)(this, unk);
}

// slot 541
f32 spiderman_rvb_player_interface::unk_541(i32 unk) {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720F60)(this, unk);
}

// slot 542
void* spiderman_rvb_player_interface::unk_542(void* unk, i32 unk_2) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_720F90)(this, unk, unk_2);
}

// slot 543
bool spiderman_rvb_player_interface::unk_543() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9C08F0)(this);
}

// slot 544
i32 spiderman_rvb_player_interface::unk_544(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A5DF0)(this, unk);
}

// slot 545
bool spiderman_rvb_player_interface::unk_545() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9C0270)(this);
}

// slot 546
i32 spiderman_rvb_player_interface::unk_546(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A5DD0)(this, unk);
}

// slot 547
bool spiderman_rvb_player_interface::unk_547() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9C0510)(this);
}

// slot 548
f32 spiderman_rvb_player_interface::unk_548(f32 unk) {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A5D00)(this, unk);
}

// slot 549
f32 spiderman_rvb_player_interface::unk_549(f32 unk) {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A5C00)(this, unk);
}

// slot 550
i32 spiderman_rvb_player_interface::unk_550(void* unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_9B7A50)(this, unk, unk_2);
}

// slot 551
void* spiderman_rvb_player_interface::unk_551(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9A7040)(this, unk);
}

// slot 552
i32 spiderman_rvb_player_interface::unk_552(void* unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_9B7AE0)(this, unk, unk_2);
}

// slot 553
void spiderman_rvb_player_interface::unk_553(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32))retail::sub_9B7700)(this, unk, unk_2, unk_3);
}

// slot 554
f32 spiderman_rvb_player_interface::unk_554() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721210)(this);
}

// slot 555
void spiderman_rvb_player_interface::unk_555(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B8560)(this, unk);
}

// slot 556
void* spiderman_rvb_player_interface::unk_556(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9A70C0)(this, unk);
}

// slot 557
void* spiderman_rvb_player_interface::unk_557(i32 unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9C1530)(this, unk);
}

// slot 558
void spiderman_rvb_player_interface::unk_558(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B8610)(this, unk);
}

// slot 559
void* spiderman_rvb_player_interface::unk_559(f32 unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B7E60)(this, unk);
}

// slot 560
void* spiderman_rvb_player_interface::unk_560(f32 unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B7EF0)(this, unk);
}

// slot 561
i32 spiderman_rvb_player_interface::unk_561(void* unk, void* unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, void*))retail::sub_9A71F0)(this, unk, unk_2);
}

// slot 562
f32 spiderman_rvb_player_interface::unk_562(i32 unk) {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9B9440)(this, unk);
}

// slot 563
f32 spiderman_rvb_player_interface::unk_563(f32 unk, f32 unk_2) {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*, f32, f32))retail::sub_9A7250)(this, unk, unk_2);
}

// slot 564
i32 spiderman_rvb_player_interface::unk_564(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B9610)(this, unk);
}

// slot 565
i32 spiderman_rvb_player_interface::unk_565(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9C2810)(this, unk);
}

// slot 566
void spiderman_rvb_player_interface::unk_566(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9C1BE0)(this, unk);
}

// slot 567
void* spiderman_rvb_player_interface::unk_567(f32 unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9C20C0)(this, unk);
}

// slot 568
void spiderman_rvb_player_interface::unk_568() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9B8C10)(this);
}

// slot 569
void spiderman_rvb_player_interface::unk_569() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_5B4A70)(this);
}

// slot 570
bool spiderman_rvb_player_interface::unk_570(i32 unk, void* unk_2, i32 unk_3, i32 unk_4, f32 unk_5) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32, void*, i32, i32, f32))retail::sub_9B89E0)(this, unk, unk_2, unk_3, unk_4, unk_5);
}

// slot 571
i32 spiderman_rvb_player_interface::unk_571(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7213B0)(this, unk);
}

// slot 572
void spiderman_rvb_player_interface::unk_572(f32 unk, bool black) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32, bool))retail::sub_9A51A0)(this, unk, black);
}

// slot 573
i32 spiderman_rvb_player_interface::unk_573() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A8010)(this);
}

// slot 574
bool spiderman_rvb_player_interface::unk_574() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7214E0)(this);
}

// slot 575
bool spiderman_rvb_player_interface::unk_575() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9ADA90)(this);
}

// slot 576
bool spiderman_rvb_player_interface::unk_576() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9AED60)(this);
}

// slot 577
i32 spiderman_rvb_player_interface::unk_577() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9AE460)(this);
}

// slot 578
bool spiderman_rvb_player_interface::unk_578() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9B06F0)(this);
}

// slot 579
i32 spiderman_rvb_player_interface::unk_579(void* unk, void* unk_2, void* unk_3, i32 unk_4, i32 unk_5) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, void*, void*, i32, i32))retail::sub_9ABA60)(this, unk, unk_2, unk_3, unk_4, unk_5);
}

// slot 580
void* spiderman_rvb_player_interface::unk_580(f32 unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A5F60)(this, unk);
}

// slot 581
bool spiderman_rvb_player_interface::unk_581(i32 unk, i32 unk_2) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9AD0E0)(this, unk, unk_2);
}

// slot 582
i32 spiderman_rvb_player_interface::unk_582(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A8310)(this, unk);
}

// slot 583
i32 spiderman_rvb_player_interface::unk_583() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6F50)(this);
}

// slot 584
f32 spiderman_rvb_player_interface::unk_584() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6E60)(this);
}

// slot 585
void spiderman_rvb_player_interface::unk_585() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A53C0)(this);
}

// slot 586
void* spiderman_rvb_player_interface::unk_586(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9AA140)(this, unk);
}

// slot 587
i32 spiderman_rvb_player_interface::unk_587(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9AA110)(this, unk, unk_2);
}

// slot 588
i32 spiderman_rvb_player_interface::unk_588(void* unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_9A5450)(this, unk, unk_2);
}

// slot 589
i32 spiderman_rvb_player_interface::unk_589(i32 unk, f32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, f32))retail::sub_9A54B0)(this, unk, unk_2);
}

// slot 590
void* spiderman_rvb_player_interface::unk_590(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9AA1A0)(this, unk);
}

// slot 591
i32 spiderman_rvb_player_interface::unk_591(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9AA170)(this, unk, unk_2);
}

// slot 592
i32 spiderman_rvb_player_interface::unk_592(void* unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_9A5790)(this, unk, unk_2);
}

// slot 593
void spiderman_rvb_player_interface::unk_593(void* unk, i32 unk_2, f32 unk_3) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, void*, i32, f32))retail::sub_9AA200)(this, unk, unk_2, unk_3);
}

// slot 594
i32 spiderman_rvb_player_interface::unk_594(void* unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_9A5A00)(this, unk, unk_2);
}

// slot 595
i32 spiderman_rvb_player_interface::unk_595(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9BF060)(this, unk);
}
