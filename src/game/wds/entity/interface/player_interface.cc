#include "retail.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/ai/actions/pick_up_ai_action.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/ai_weapon_inode.hh"
#include "game/wds/ai/info_node.hh"

using namespace treyarch;

// slot 1
void player_interface::construct_mashed_class() {
    ((void (__thiscall*)(player_interface*))retail::sub_99A620)(this);
}

// slot 2
void player_interface::destruct_mashed_class() {
    ((void (__thiscall*)(player_interface*))retail::sub_99A920)(this);
}

// slot 3
void player_interface::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(player_interface*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_99ADB0)(this,
                                                                                                                   mash_info,
                                                                                                                   containing_class_ptr,
                                                                                                                   buffer);
}

// slot 4
mash::virtual_types_key player_interface::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const player_interface*))retail::sub_720160)(this);
}

// slot 6
bool player_interface::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const player_interface*, mash::virtual_types_key))retail::sub_643700)(this, parent_class);
}

// slot 8
i32 player_interface::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const player_interface*))retail::sub_720170)(this);
}

// slot 9
bool player_interface::get_ifc_num(const string_hash &name, f32* value, bool document) {
    return ((bool (__thiscall*)(player_interface*, const string_hash&, f32*, bool))retail::sub_9886C0)(this, name, value, document);
}

// slot 10
bool player_interface::set_ifc_num(const string_hash &name, f32 value, bool document) {
    return ((bool (__thiscall*)(player_interface*, const string_hash&, f32, bool))retail::sub_98C6E0)(this, name, value, document);
}

// slot 11
bool player_interface::get_ifc_vec(const string_hash &name, vector3d* value, bool document) {
    return ((bool (__thiscall*)(player_interface*, const string_hash&, vector3d*, bool))retail::sub_991760)(this, name, value, document);
}

// slot 16
e_entity_ifc player_interface::get_ifc_id() const {
    return ((e_entity_ifc (__thiscall*)(const player_interface*))retail::sub_A99FF0)(this);
}

// slot 18
void player_interface::set_owner(actor* owner) {
    ((void (__thiscall*)(player_interface*, actor*))retail::sub_996520)(this, owner);
}

// slot 19
bool player_interface::is_enabled() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_71F800)(this);
}

// slot 20
void player_interface::set_enabled(bool value) {
    ((void (__thiscall*)(player_interface*, bool))retail::sub_71F810)(this, value);
}

// slot 21
f32 player_interface::unk_021() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 22
i32 player_interface::unk_022(f32 unk) {
    return ((i32 (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 23
bool player_interface::unk_023(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 24
void player_interface::unk_024(actor* owner) {
    ((void (__thiscall*)(player_interface*, actor*))retail::sub_689610)(this, owner);
}

// slot 25
void* player_interface::unk_025(i32 unk) {
    return ((void* (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 26
bool player_interface::unk_026() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 27
bool player_interface::unk_027(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 28
bool player_interface::unk_028(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 29
bool player_interface::unk_029() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 30
f32 player_interface::unk_030() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 31
i32 player_interface::unk_031(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F830)(this, unk);
}

// slot 32
i32 player_interface::unk_032(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F830)(this, unk);
}

// slot 33
void player_interface::unk_033(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 34
i32 player_interface::unk_034(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_9994F0)(this, unk);
}

// slot 35
i32 player_interface::unk_035(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 36
void* player_interface::unk_036(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 37
void player_interface::set_zip_override_anchor_num(f32 num) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, num);
}

// slot 38
f32 player_interface::unk_038() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 39
i32 player_interface::unk_039(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 40
void* player_interface::unk_040(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 41
void* player_interface::unk_041(void* unk, i32 unk_2) {
    return ((void* (__thiscall*)(player_interface*, void*, i32))retail::sub_992E00)(this, unk, unk_2);
}

// slot 42
i32 player_interface::unk_042() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 43
void player_interface::unk_043(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 44
bool player_interface::unk_044(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 45
bool player_interface::unk_045() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_667F80)(this);
}

// slot 46
void player_interface::unk_046(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 47
void player_interface::unk_047(u32 unk) {
    ((void (__thiscall*)(player_interface*, u32))retail::sub_689610)(this, unk);
}

// slot 48
void* player_interface::unk_048(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 49
bool player_interface::unk_049(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_71FF10)(this, unk);
}

// slot 50
i32 player_interface::unk_050(arch_base_vhandle target) {
    return ((i32 (__thiscall*)(player_interface*, arch_base_vhandle))retail::sub_689610)(this, target);
}

// slot 51
i32 player_interface::unk_051(i32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 52
void* player_interface::unk_052(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 53
void* player_interface::unk_053(void* unk, void* unk_2) {
    return ((void* (__thiscall*)(player_interface*, void*, void*))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 54
void player_interface::unk_054(i32 unk, i32 unk_2) {
    ((void (__thiscall*)(player_interface*, i32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 55
void* player_interface::unk_055(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 56
f32 player_interface::unk_056() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 57
i32 player_interface::unk_057() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 58
void player_interface::unk_058(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 59
void* player_interface::unk_059(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 60
i32 player_interface::unk_060(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 61
arch_base_vhandle player_interface::unk_061() {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(player_interface*, arch_base_vhandle*))retail::sub_720180)(this, &result);

    return result;
}

// slot 62
void* player_interface::unk_062(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_689610)(this, unk);
}

// slot 63
i32 player_interface::unk_063(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F850)(this, unk);
}

// slot 64
i32 player_interface::unk_064(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 65
void* player_interface::unk_065(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 66
void player_interface::unk_066(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 67
i32 player_interface::unk_067(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F830)(this, unk);
}

// slot 68
bool player_interface::unk_068(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 69
bool player_interface::unk_069() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 70
i32 player_interface::unk_070(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 71
void* player_interface::unk_071(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 72
bool player_interface::unk_072() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 73
bool player_interface::unk_073() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 74
bool player_interface::unk_074() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 75
void player_interface::set_desired_mode(i32 m) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_988640)(this, m);
}

// slot 76
i32 player_interface::unk_076() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FB70)(this);
}

// slot 77
i32 player_interface::unk_077() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 78
void player_interface::set_swing_override_anchor(arch_base_vhandle anchor) {
    ((void (__thiscall*)(player_interface*, arch_base_vhandle))retail::sub_689610)(this, anchor);
}

// slot 79
void player_interface::set_zip_override_anchor(arch_base_vhandle anchor) {
    ((void (__thiscall*)(player_interface*, arch_base_vhandle))retail::sub_689610)(this, anchor);
}

// slot 80
void* player_interface::unk_080(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 81
void player_interface::unk_081(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 82
i32 player_interface::unk_082(f32 unk) {
    return ((i32 (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 83
i32 player_interface::unk_083() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_991CC0)(this);
}

// slot 84
f32 player_interface::unk_084() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_991CD0)(this);
}

// slot 85
f32 player_interface::unk_085() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_986AE0)(this);
}

// slot 86
void player_interface::unk_086(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 87
bool player_interface::unk_087() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 88
bool player_interface::unk_088(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 89
bool player_interface::map_controls(i32 config, bool force) {
    return ((bool (__thiscall*)(player_interface*, i32, bool))retail::sub_991C00)(this, config, force);
}

// slot 90
void player_interface::render_phase() {
    ((void (__thiscall*)(player_interface*))retail::sub_986AA0)(this);
}

// slot 91
void player_interface::update_interface() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 92
void player_interface::frame_advance(f32 delta_t) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_9978B0)(this, delta_t);
}

// slot 93
void player_interface::frame_advance_disabled(f32 delta_t) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_992120)(this, delta_t);
}

// slot 94
void player_interface::unk_094(f32 delta_t) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, delta_t);
}

// slot 95
vector3d* player_interface::unk_095() {
    return ((vector3d* (__thiscall*)(player_interface*))retail::sub_720190)(this);
}

// slot 96
bool player_interface::unk_096(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_71FF10)(this, unk);
}

// slot 97
bool player_interface::unk_097() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 98
bool player_interface::unk_098(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 99
i32 player_interface::unk_099() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 100
i32 player_interface::unk_100() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 101
i32 player_interface::unk_101() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 102
bool player_interface::unk_102() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_7201A0)(this);
}

// slot 103
bool player_interface::unk_103() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_7201B0)(this);
}

// slot 104
bool player_interface::unk_104() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_7201F0)(this);
}

// slot 105
bool player_interface::unk_105() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_720200)(this);
}

// slot 106
bool player_interface::unk_106() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_720210)(this);
}

// slot 107
i32 player_interface::unk_107() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 108
i32 player_interface::unk_108(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 109
bool player_interface::unk_109() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F880)(this);
}

// slot 110
bool player_interface::unk_110() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F890)(this);
}

// slot 111
bool player_interface::unk_111() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F8A0)(this);
}

// slot 112
bool player_interface::unk_112(bool unk, bool unk_2) {
    return ((bool (__thiscall*)(player_interface*, bool, bool))retail::sub_987580)(this, unk, unk_2);
}

// slot 113
i32 player_interface::unk_113() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 114
bool player_interface::unk_114(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 115
i32 player_interface::unk_115() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 116
bool player_interface::unk_116(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 117
i32 player_interface::unk_117() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F8B0)(this);
}

// slot 118
i32 player_interface::unk_118() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 119
bool player_interface::unk_119(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 120
bool player_interface::crawling() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_720220)(this);
}

// slot 121
i32 player_interface::unk_121() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_720230)(this);
}

// slot 122
i32 player_interface::unk_122() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_720240)(this);
}

// slot 123
i32 player_interface::unk_123() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_720250)(this);
}

// slot 124
bool player_interface::unk_124() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 125
bool player_interface::unk_125() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 126
bool player_interface::unk_126() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 127
i32 player_interface::unk_127() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F900)(this);
}

// slot 128
i32 player_interface::unk_128() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F930)(this);
}

// slot 129
bool player_interface::unk_129(f32 unk) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 130
bool player_interface::unk_130() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 131
bool player_interface::unk_131() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 132
i32 player_interface::unk_132() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F960)(this);
}

// slot 133
i32 player_interface::unk_133() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F990)(this);
}

// slot 134
bool player_interface::unk_134(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 135
bool player_interface::unk_135(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 136
i32 player_interface::unk_136() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 137
i32 player_interface::unk_137() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_667F80)(this);
}

// slot 138
bool player_interface::unk_138() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 139
i32 player_interface::unk_139() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 140
void player_interface::unk_140() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 141
i32 player_interface::unk_141() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 142
bool player_interface::unk_142(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71F9C0)(this, unk);
}

// slot 143
bool player_interface::unk_143() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F9E0)(this);
}

// slot 144
void player_interface::unk_144() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 145
void player_interface::unk_145(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 146
void player_interface::unk_146(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 147
void player_interface::unk_147(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 148
f32 player_interface::unk_148() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 149
f32 player_interface::unk_149() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 150
bool player_interface::unk_150() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 151
void player_interface::unk_151(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 152
bool player_interface::unk_152() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 153
bool player_interface::unk_153(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 154
i32 player_interface::unk_154() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F9F0)(this);
}

// slot 155
i32 player_interface::unk_155(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FA00)(this, unk);
}

// slot 156
bool player_interface::unk_156() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 157
i32 player_interface::unk_157() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 158
i32 player_interface::unk_158(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 159
bool player_interface::unk_159(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_992AC0)(this, unk);
}

// slot 160
void* player_interface::unk_160(i32 unk) {
    return ((void* (__thiscall*)(player_interface*, i32))retail::sub_992B50)(this, unk);
}

// slot 161
i32 player_interface::unk_161() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FA10)(this);
}

// slot 162
i32 player_interface::unk_162(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FA20)(this, unk);
}

// slot 163
void player_interface::unk_163() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 164
void player_interface::unk_164() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 165
i32 player_interface::unk_165() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 166
i32 player_interface::unk_166() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 167
bool player_interface::unk_167(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 168
bool player_interface::unk_168() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 169
bool player_interface::unk_169(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 170
bool player_interface::unk_170() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 171
bool player_interface::unk_171(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 172
bool player_interface::unk_172() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 173
bool player_interface::unk_173(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 174
bool player_interface::unk_174() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 175
bool player_interface::unk_175(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 176
bool player_interface::unk_176() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 177
bool player_interface::unk_177(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 178
bool player_interface::unk_178() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 179
bool player_interface::unk_179(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 180
bool player_interface::unk_180() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 181
i32 player_interface::unk_181(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_986AF0)(this, unk);
}

// slot 182
bool player_interface::unk_182() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FA30)(this);
}

// slot 183
bool player_interface::unk_183(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71FA40)(this, unk);
}

// slot 184
bool player_interface::unk_184() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FA60)(this);
}

// slot 185
bool player_interface::unk_185(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71FA70)(this, unk);
}

// slot 186
bool player_interface::unk_186() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FA90)(this);
}

// slot 187
void player_interface::unk_187(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_71FAA0)(this, unk);
}

// slot 188
bool player_interface::unk_188() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FAC0)(this);
}

// slot 189
bool player_interface::unk_189(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71FAD0)(this, unk);
}

// slot 190
bool player_interface::unk_190() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FAF0)(this);
}

// slot 191
bool player_interface::unk_191(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71FB00)(this, unk);
}

// slot 192
bool player_interface::unk_192() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 193
bool player_interface::unk_193(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 194
bool player_interface::unk_194(f32 unk) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_71FF10)(this, unk);
}

// slot 195
i32 player_interface::unk_195() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 196
i32 player_interface::unk_196(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F830)(this, unk);
}

// slot 197
i32 player_interface::unk_197() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 198
void player_interface::unk_198() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 199
i32 player_interface::unk_199(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F830)(this, unk);
}

// slot 200
void player_interface::unk_200(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 201
i32 player_interface::unk_201(f32 unk) {
    return ((i32 (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 202
i32 player_interface::unk_202() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 203
void player_interface::unk_203() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 204
i32 player_interface::unk_204(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F830)(this, unk);
}

// slot 205
void* player_interface::unk_205(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 206
i32 player_interface::unk_206(f32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(player_interface*, f32, i32, i32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 207
void player_interface::unk_207() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 208
void player_interface::unk_208(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 209
void player_interface::unk_209(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 210
f32 player_interface::unk_210() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720010)(this);
}

// slot 211
i32 player_interface::unk_211(i32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_986C40)(this, unk, unk_2, unk_3);
}

// slot 212
i32 player_interface::unk_212(i32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_986C00)(this, unk, unk_2, unk_3);
}

// slot 213
i32 player_interface::unk_213(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FB20)(this, unk);
}

// slot 214
bool player_interface::unk_214(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_71FB40)(this, unk);
}

// slot 215
i32 player_interface::unk_215() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FB70)(this);
}

// slot 216
i32 player_interface::unk_216() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FB80)(this);
}

// slot 217
i32 player_interface::unk_217() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FB90)(this);
}

// slot 218
i32 player_interface::unk_218() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_99A4B0)(this);
}

// slot 219
void player_interface::unk_219(bool unk) {
    ((void (__thiscall*)(player_interface*, bool))retail::sub_99A570)(this, unk);
}

// slot 220
bool player_interface::unk_220(f32 delta_t, f32 unk_2) {
    return ((bool (__thiscall*)(player_interface*, f32, f32))retail::sub_986C80)(this, delta_t, unk_2);
}

// slot 221
i32 player_interface::unk_221(f32 unk) {
    return ((i32 (__thiscall*)(player_interface*, f32))retail::sub_99AA90)(this, unk);
}

// slot 222
void player_interface::unk_222(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 223
void player_interface::unk_223(i32 unk, i32 unk_2) {
    ((void (__thiscall*)(player_interface*, i32, i32))retail::sub_9922E0)(this, unk, unk_2);
}

// slot 224
i32 player_interface::unk_224(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FBB0)(this, unk);
}

// slot 225
i32 player_interface::unk_225() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FBD0)(this);
}

// slot 226
i32 player_interface::unk_226() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FBE0)(this);
}

// slot 227
i32 player_interface::unk_227(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FC00)(this, unk);
}

// slot 228
i32 player_interface::unk_228() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_986CD0)(this);
}

// slot 229
i32 player_interface::unk_229() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_986D10)(this);
}

// slot 230
void player_interface::unk_230(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_986B00)(this, unk);
}

// slot 231
bool player_interface::unk_231(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5, i32 unk_6) {
    return ((bool (__thiscall*)(player_interface*, i32, i32, i32, i32, i32, i32))retail::sub_99B330)(this, unk, unk_2, unk_3, unk_4, unk_5, unk_6);
}

// slot 232
bool player_interface::unk_232(f32 delta_t) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_986D50)(this, delta_t);
}

// slot 233
i32 player_interface::unk_233() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FC10)(this);
}

// slot 234
i32 player_interface::unk_234(void* unk) {
    return ((i32 (__thiscall*)(player_interface*, void*))retail::sub_71FC20)(this, unk);
}

// slot 235
i32 player_interface::unk_235() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FC30)(this);
}

// slot 236
i32 player_interface::unk_236(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FC40)(this, unk);
}

// slot 237
bool player_interface::unk_237(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5, i32 unk_6, i32 unk_7) {
    return ((bool (__thiscall*)(player_interface*, i32, i32, i32, i32, i32, i32, i32))retail::sub_986D60)(this, unk, unk_2, unk_3, unk_4, unk_5, unk_6, unk_7);
}

// slot 238
i32 player_interface::unk_238(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_986D70)(this, unk);
}

// slot 239
i32 player_interface::unk_239() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FC50)(this);
}

// slot 240
i32 player_interface::unk_240(void* unk) {
    return ((i32 (__thiscall*)(player_interface*, void*))retail::sub_71FC60)(this, unk);
}

// slot 241
i32 player_interface::unk_241() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FC70)(this);
}

// slot 242
i32 player_interface::unk_242(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FC80)(this, unk);
}

// slot 243
void* player_interface::unk_243(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 244
void* player_interface::unk_244(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 245
i32 player_interface::unk_245() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 246
f32 player_interface::unk_246() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 247
f32 player_interface::unk_247() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 248
bool player_interface::unk_248() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 249
bool player_interface::unk_249(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 250
f32 player_interface::unk_250() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 251
f32 player_interface::unk_251() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 252
void player_interface::unk_252(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 253
f32 player_interface::unk_253() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 254
void player_interface::unk_254(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 255
void player_interface::unk_255(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 256
void player_interface::unk_256() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 257
bool player_interface::unk_257() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 258
void* player_interface::unk_258(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 259
bool player_interface::unk_259(f32 delta_t) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_999660)(this, delta_t);
}

// slot 260
bool player_interface::unk_260(f32 delta_t) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_999760)(this, delta_t);
}

// slot 261
bool player_interface::unk_261(void* unk, void* unk_2, void* unk_3) {
    return ((bool (__thiscall*)(player_interface*, void*, void*, void*))retail::sub_999840)(this, unk, unk_2, unk_3);
}

// slot 262
i32 player_interface::unk_262(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 263
bool player_interface::unk_263(void* unk, i32 unk_2) {
    return ((bool (__thiscall*)(player_interface*, void*, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 264
bool player_interface::unk_264(f32 delta_t, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) {
    return ((bool (__thiscall*)(player_interface*, f32, void*, i32, i32, i32))retail::sub_71FC90)(this, delta_t, unk_2, unk_3, unk_4, unk_5);
}

// slot 265
bool player_interface::unk_265() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 266
void player_interface::unk_266(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 267
void player_interface::unk_267() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 268
bool player_interface::unk_268() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 269
i32 player_interface::unk_269(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F830)(this, unk);
}

// slot 270
bool player_interface::unk_270() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 271
bool player_interface::unk_271() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 272
bool player_interface::unk_272() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 273
entity* player_interface::unk_273(entity* target) {
    return ((entity* (__thiscall*)(player_interface*, entity*))retail::sub_9CAAD0)(this, target);
}

// slot 274
bool player_interface::should_pick_up_ai(f32 delta_t) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_9923E0)(this, delta_t);
}

// slot 275
bool player_interface::unk_275(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_996800)(this, unk);
}

// slot 276
void* player_interface::unk_276(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_71FCA0)(this, unk);
}

// sub_9925A0
bool player_interface::unk_277(ai_core* core) {
    auto* action_proc = (ai_action_processor_inode*)core->get_info_node(info_node_type_ai_action_processor);

    if (action_proc->has_actions())
        return false;

    auto* carry = (std_carry_inode*)core->get_info_node(info_node_type_std_carry);

    if (!carry || !carry->desired_slave)
        return false;

    auto* action = new pick_up_ai_action(core);
    action->setup(carry->desired_slave);

    action_proc->add_action(action);
    carry->desired_slave = nullptr;

    return true;
}

// slot 278
bool player_interface::unk_278(f32 delta_t) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_9926D0)(this, delta_t);
}

// slot 279
bool player_interface::unk_279(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_9968A0)(this, unk);
}

// slot 280
void* player_interface::unk_280(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_71FCB0)(this, unk);
}

// slot 281
bool player_interface::unk_281(ai_core* core) {
    return ((bool (__thiscall*)(player_interface*, ai_core*))retail::sub_992800)(this, core);
}

// slot 282
bool player_interface::unk_282(ai_core* core) {
    return ((bool (__thiscall*)(player_interface*, ai_core*))retail::sub_992930)(this, core);
}

// slot 283
bool player_interface::unk_283(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_986D80)(this, unk);
}

// slot 284
bool player_interface::unk_284() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 285
i32 player_interface::unk_285() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FCC0)(this);
}

// slot 286
i32 player_interface::unk_286() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FCD0)(this);
}

// slot 287
i32 player_interface::unk_287(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_986B90)(this, unk);
}

// slot 288
void player_interface::unk_288(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_996680)(this, unk);
}

// slot 289
i32 player_interface::unk_289(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FCE0)(this, unk);
}

// slot 290
i32 player_interface::unk_290(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FCF0)(this, unk);
}

// slot 291
void player_interface::unk_291(f32 unk, bool unk_2) {
    ((void (__thiscall*)(player_interface*, f32, bool))retail::sub_986BC0)(this, unk, unk_2);
}

// slot 292
i32 player_interface::unk_292() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FD00)(this);
}

// slot 293
i32 player_interface::unk_293() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FD10)(this);
}

// slot 294
i32 player_interface::unk_294() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FD20)(this);
}

// slot 295
f32 player_interface::unk_295() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_71FD30)(this);
}

// slot 296
void player_interface::unk_296(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_71FD40)(this, unk);
}

// slot 297
f32 player_interface::unk_297() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_71FD60)(this);
}

// slot 298
void player_interface::unk_298(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_71FD70)(this, unk);
}

// slot 299
i32 player_interface::unk_299(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FD90)(this, unk);
}

// slot 300
void player_interface::unk_300(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_71FDB0)(this, unk, unk_2, unk_3);
}

// slot 301
i32 player_interface::unk_301(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FDE0)(this, unk);
}

// slot 302
void player_interface::unk_302(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_71FE00)(this, unk, unk_2, unk_3);
}

// slot 303
void* player_interface::unk_303(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_71FE30)(this, unk);
}

// slot 304
i32 player_interface::unk_304(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FE60)(this, unk);
}

// slot 305
bool player_interface::unk_305() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 306
i32 player_interface::unk_306() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 307
i32 player_interface::unk_307() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 308
f32 player_interface::unk_308() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 309
void player_interface::unk_309(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 310
f32 player_interface::unk_310() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 311
bool player_interface::unk_311() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FE80)(this);
}

// slot 312
bool player_interface::unk_312(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71FE90)(this, unk);
}

// slot 313
bool player_interface::unk_313() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FEA0)(this);
}

// slot 314
bool player_interface::unk_314(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71FEB0)(this, unk);
}

// slot 315
f32 player_interface::unk_315() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 316
i32 player_interface::unk_316() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FB70)(this);
}

// slot 317
f32 player_interface::unk_317() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 318
f32 player_interface::unk_318() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 319
bool player_interface::unk_319(f32 delta_t, game_button* button, i32 unk_3) {
    return ((bool (__thiscall*)(player_interface*, f32, game_button*, i32))retail::sub_5FCC20)(this, delta_t, button, unk_3);
}

// slot 320
void* player_interface::unk_320() {
    return ((void* (__thiscall*)(player_interface*))retail::sub_71FEC0)(this);
}

// slot 321
void* player_interface::unk_321(const line_info* unk) {
    return ((void* (__thiscall*)(player_interface*, const line_info*))retail::sub_689610)(this, unk);
}

// slot 322
bool player_interface::unk_322(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 323
void* player_interface::unk_323(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_689610)(this, unk);
}

// slot 324
i32 player_interface::unk_324(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F830)(this, unk);
}

// slot 325
f32 player_interface::unk_325() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 326
bool player_interface::unk_326() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 327
bool player_interface::unk_327(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 328
void* player_interface::unk_328() {
    return ((void* (__thiscall*)(player_interface*))retail::sub_71FED0)(this);
}

// slot 329
void* player_interface::unk_329(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_71FEE0)(this, unk);
}

// slot 330
void player_interface::unk_330(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(player_interface*, f32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 331
void player_interface::unk_331(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(player_interface*, f32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 332
void player_interface::unk_332(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(player_interface*, f32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 333
i32 player_interface::unk_333(f32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(player_interface*, f32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 334
void player_interface::unk_334(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(player_interface*, f32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 335
void player_interface::unk_335(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(player_interface*, f32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 336
void player_interface::unk_336(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(player_interface*, f32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 337
i32 player_interface::unk_337() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 338
i32 player_interface::unk_338() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 339
i32 player_interface::unk_339() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 340
bool player_interface::unk_340() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 341
i32 player_interface::unk_341() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 342
i32 player_interface::unk_342() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 343
i32 player_interface::unk_343() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 344
void player_interface::unk_344(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 345
void player_interface::unk_345(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 346
void player_interface::unk_346(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 347
void player_interface::unk_347(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 348
void player_interface::unk_348(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 349
void player_interface::unk_349(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 350
void player_interface::unk_350(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 351
void player_interface::unk_351(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 352
void player_interface::unk_352(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 353
void player_interface::unk_353(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 354
void player_interface::unk_354(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 355
void player_interface::unk_355(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 356
void player_interface::unk_356(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 357
void player_interface::unk_357(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 358
void player_interface::unk_358(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 359
void player_interface::unk_359(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 360
void player_interface::unk_360(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 361
void player_interface::unk_361(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 362
void player_interface::unk_362(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32, i32))retail::sub_9CD780)(this, unk, unk_2, unk_3, unk_4);
}

// slot 363
void* player_interface::unk_363(void* unk, void* unk_2, void* unk_3, void* unk_4, void* unk_5, void* unk_6) {
    return ((void* (__thiscall*)(player_interface*, void*, void*, void*, void*, void*, void*))retail::sub_71FF60)(this, unk, unk_2, unk_3, unk_4, unk_5, unk_6);
}

// slot 364
void* player_interface::unk_364(void* unk, void* unk_2, void* unk_3, void* unk_4, void* unk_5, void* unk_6, void* unk_7) {
    return ((void* (__thiscall*)(player_interface*, void*, void*, void*, void*, void*, void*, void*))retail::sub_71FEF0)(this,
                                                                                                                         unk,
                                                                                                                         unk_2,
                                                                                                                         unk_3,
                                                                                                                         unk_4,
                                                                                                                         unk_5,
                                                                                                                         unk_6,
                                                                                                                         unk_7);
}

// slot 365
i32 player_interface::unk_365(void* unk, void* unk_2) {
    return ((i32 (__thiscall*)(player_interface*, void*, void*))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 366
void* player_interface::unk_366(void* unk, void* unk_2, void* unk_3, void* unk_4) {
    return ((void* (__thiscall*)(player_interface*, void*, void*, void*, void*))retail::sub_9CD780)(this, unk, unk_2, unk_3, unk_4);
}

// slot 367
i32 player_interface::unk_367(void* unk, void* unk_2) {
    return ((i32 (__thiscall*)(player_interface*, void*, void*))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 368
void player_interface::unk_368(f32 unk, i32 unk_2) {
    ((void (__thiscall*)(player_interface*, f32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 369
void player_interface::unk_369(i32 unk, i32 unk_2) {
    ((void (__thiscall*)(player_interface*, i32, i32))retail::sub_5BD2C0)(this, unk, unk_2);
}

// slot 370
bool player_interface::unk_370(i32 unk, i32 unk_2, i32 unk_3) {
    return ((bool (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_5FCC20)(this, unk, unk_2, unk_3);
}

// slot 371
i32 player_interface::unk_371(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 372
entity* player_interface::get_last_anchor() const {
    return ((entity* (__thiscall*)(const player_interface*))retail::sub_A944D0)(this);
}

// slot 373
i32 player_interface::unk_373() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 374
f32 player_interface::unk_374() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 375
f32 player_interface::unk_375() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 376
i32 player_interface::unk_376() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 377
bool player_interface::unk_377(vector3d* point) {
    return ((bool (__thiscall*)(player_interface*, vector3d*))retail::sub_71FF10)(this, point);
}

// slot 378
bool player_interface::unk_378() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 379
i32 player_interface::unk_379() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 380
i32 player_interface::unk_380(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_A90D90)(this, unk);
}

// slot 381
f32 player_interface::unk_381(void* unk, i32 unk_2, f32 unk_3) {
    return ((f32 (__thiscall*)(player_interface*, void*, i32, f32))retail::sub_71FF00)(this, unk, unk_2, unk_3);
}

// slot 382
bool player_interface::unk_382(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_71FF10)(this, unk);
}

// slot 383
i32 player_interface::unk_383() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 384
bool player_interface::unk_384() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 385
i32 player_interface::unk_385() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 386
i32 player_interface::unk_386(f32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(player_interface*, f32, i32))retail::sub_5BD2C0)(this, unk, unk_2);
}

// slot 387
void* player_interface::unk_387() {
    return ((void* (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 388
bool player_interface::unk_388() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 389
bool player_interface::unk_389(void* unk, f32 unk_2) {
    return ((bool (__thiscall*)(player_interface*, void*, f32))retail::sub_5BD2C0)(this, unk, unk_2);
}

// slot 390
bool player_interface::unk_390() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 391
bool player_interface::unk_391(i32 unk, i32 unk_2) {
    return ((bool (__thiscall*)(player_interface*, i32, i32))retail::sub_5BD2C0)(this, unk, unk_2);
}

// slot 392
bool player_interface::unk_392(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_71FF10)(this, unk);
}

// slot 393
bool player_interface::unk_393(f32 unk) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_71FF10)(this, unk);
}

// slot 394
i32 player_interface::unk_394() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 395
i32 player_interface::unk_395() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FB70)(this);
}

// slot 396
i32 player_interface::unk_396(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FF20)(this, unk);
}

// slot 397
i32 player_interface::unk_397() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FF30)(this);
}

// slot 398
i32 player_interface::unk_398(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_A90D90)(this, unk);
}

// slot 399
i32 player_interface::unk_399(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_A90D90)(this, unk);
}

// slot 400
void player_interface::unk_400(polytube* tube, f32 radius) {
    ((void (__thiscall*)(player_interface*, polytube*, f32))retail::sub_AD2D40)(this, tube, radius);
}

// slot 401
i32 player_interface::unk_401() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 402
i32 player_interface::unk_402(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 403
f32 player_interface::unk_403() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 404
f32 player_interface::unk_404(f32 unk) {
    return ((f32 (__thiscall*)(player_interface*, f32))retail::sub_71FF40)(this, unk);
}

// slot 405
f32 player_interface::unk_405(f32 unk) {
    return ((f32 (__thiscall*)(player_interface*, f32))retail::sub_71FF50)(this, unk);
}

// slot 406
f32 player_interface::unk_406(f32 unk) {
    return ((f32 (__thiscall*)(player_interface*, f32))retail::sub_71FF50)(this, unk);
}

// slot 407
f32 player_interface::unk_407(f32 unk) {
    return ((f32 (__thiscall*)(player_interface*, f32))retail::sub_71FF50)(this, unk);
}

// slot 408
void player_interface::unk_408(i32 unk, i32 unk_2) {
    ((void (__thiscall*)(player_interface*, i32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 409
void player_interface::unk_409(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 410
void player_interface::unk_410(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32, i32))retail::sub_9CD780)(this, unk, unk_2, unk_3, unk_4);
}

// slot 411
i32 player_interface::unk_411(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 412
void player_interface::unk_412(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5, i32 unk_6) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32, i32, i32, i32))retail::sub_71FF60)(this, unk, unk_2, unk_3, unk_4, unk_5, unk_6);
}

// slot 413
i32 player_interface::unk_413(i32 unk, i32 unk_2, void* unk_3) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32, void*))retail::sub_71FF70)(this, unk, unk_2, unk_3);
}

// slot 414
f32 player_interface::unk_414() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 415
f32 player_interface::unk_415() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 416
f32 player_interface::unk_416() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 417
i32 player_interface::unk_417() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 418
i32 player_interface::unk_418() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 419
i32 player_interface::unk_419() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 420
i32 player_interface::unk_420() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 421
i32 player_interface::unk_421() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 422
i32 player_interface::unk_422() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 423
i32 player_interface::unk_423() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 424
f32 player_interface::unk_424() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_987000)(this);
}

// slot 425
f32 player_interface::unk_425() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_71FF90)(this);
}

// slot 426
f32 player_interface::unk_426() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_987160)(this);
}

// slot 427
f32 player_interface::unk_427() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_71FFB0)(this);
}

// slot 428
f32 player_interface::unk_428() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_992CB0)(this);
}

// slot 429
f32 player_interface::unk_429() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_71FFD0)(this);
}

// slot 430
void player_interface::unk_430() {
    ((void (__thiscall*)(player_interface*))retail::sub_9872D0)(this);
}

// slot 431
f32 player_interface::unk_431() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_71FFF0)(this);
}

// slot 432
f32 player_interface::unk_432() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 433
f32 player_interface::unk_433() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 434
f32 player_interface::unk_434() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 435
f32 player_interface::unk_435() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720010)(this);
}

// slot 436
f32 player_interface::unk_436() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 437
f32 player_interface::unk_437() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720020)(this);
}

// slot 438
f32 player_interface::unk_438() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720020)(this);
}

// slot 439
void player_interface::unk_439(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 440
f32 player_interface::unk_440() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 441
f32 player_interface::unk_441() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 442
f32 player_interface::unk_442() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720030)(this);
}

// slot 443
f32 player_interface::unk_443() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 444
f32 player_interface::unk_444() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 445
f32 player_interface::unk_445() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 446
f32 player_interface::unk_446() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 447
f32 player_interface::unk_447() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 448
f32 player_interface::unk_448() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 449
f32 player_interface::unk_449() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 450
f32 player_interface::unk_450() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 451
f32 player_interface::unk_451() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 452
f32 player_interface::unk_452() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 453
i32 player_interface::unk_453(i32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 454
f32 player_interface::unk_454() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720060)(this);
}

// slot 455
i32 player_interface::unk_455(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 456
bool player_interface::unk_456() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 457
bool player_interface::unk_457(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 458
void player_interface::unk_458(void* unk, void* unk_2, f32 unk_3) {
    ((void (__thiscall*)(player_interface*, void*, void*, f32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 459
i32 player_interface::unk_459() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_720070)(this);
}

// slot 460
i32 player_interface::unk_460(f32 unk) {
    return ((i32 (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 461
bool player_interface::unk_461() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 462
bool player_interface::unk_462() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 463
bool player_interface::unk_463() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 464
i32 player_interface::unk_464() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 465
void player_interface::unk_465() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 466
bool player_interface::unk_466() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 467
void* player_interface::unk_467() {
    return ((void* (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 468
void player_interface::unk_468() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 469
bool player_interface::unk_469() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 470
bool player_interface::unk_470() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 471
i32 player_interface::unk_471() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 472
void player_interface::unk_472() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 473
arch_base_vhandle player_interface::unk_473() {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(player_interface*, arch_base_vhandle*))retail::sub_720080)(this, &result);

    return result;
}

// slot 474
i32 player_interface::unk_474(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_720260)(this, unk);
}

// slot 475
f32 player_interface::unk_475() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720090)(this);
}

// slot 476
void player_interface::unk_476(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_7200A0)(this, unk);
}

// slot 477
i32 player_interface::unk_477(i32 unk, bool unk_2) {
    return ((i32 (__thiscall*)(player_interface*, i32, bool))retail::sub_7200C0)(this, unk, unk_2);
}

// slot 478
bool player_interface::unk_478(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_7200F0)(this, unk);
}

// slot 479
bool player_interface::unk_479() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_720100)(this);
}

// slot 480
bool player_interface::unk_480(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_720110)(this, unk);
}

// slot 481
bool player_interface::unk_481() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_720130)(this);
}

// slot 482
bool player_interface::unk_482(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_720140)(this, unk);
}

// slot 483
void* player_interface::unk_483(void* unk, i32 unk_2) {
    return ((void* (__thiscall*)(player_interface*, void*, i32))retail::sub_992E00)(this, unk, unk_2);
}

// slot 484
bool player_interface::unk_484() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 485
bool player_interface::unk_485() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 486
void player_interface::unk_486(bool unk, f32 unk_2) {
    ((void (__thiscall*)(player_interface*, bool, f32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 487
bool player_interface::unk_487() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 488
bool player_interface::unk_488(i32 unk, i32 unk_2) {
    return ((bool (__thiscall*)(player_interface*, i32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 489
i32 player_interface::unk_489() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 490
i32 player_interface::unk_490() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 491
f32 player_interface::unk_491() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 492
void player_interface::unk_492(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 493
i32 player_interface::unk_493() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 494
i32 player_interface::unk_494() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 495
i32 player_interface::unk_495() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 496
i32 player_interface::unk_496() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 497
i32 player_interface::unk_497() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 498
i32 player_interface::unk_498() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 499
i32 player_interface::unk_499() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 500
i32 player_interface::unk_500() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 501
i32 player_interface::unk_501() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 502
f32 player_interface::unk_502() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 503
i32 player_interface::unk_503() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 504
bool player_interface::unk_504(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_71FF10)(this, unk);
}

// slot 505
f32 player_interface::unk_505() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 506
void player_interface::unk_506(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 507
void* player_interface::unk_507(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_689610)(this, unk);
}

// slot 508
i32 player_interface::unk_508(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_9874C0)(this, unk);
}

// slot 509
i32 player_interface::unk_509() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 510
void player_interface::unk_510(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 511
bool player_interface::unk_511() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 512
bool player_interface::unk_512(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 513
bool player_interface::unk_513(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_71FF10)(this, unk);
}

// slot 514
void* player_interface::unk_514(i32 unk) {
    return ((void* (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 515
bool player_interface::unk_515() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 516
i32 player_interface::unk_516(i32 unk, void* unk_2, bool unk_3) {
    return ((i32 (__thiscall*)(player_interface*, i32, void*, bool))retail::sub_992E10)(this, unk, unk_2, unk_3);
}

// slot 517
i32 player_interface::unk_517() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 518
bool player_interface::unk_518(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 519
bool player_interface::unk_519() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 520
bool player_interface::unk_520() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 521
void player_interface::unk_521() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 522
i32 player_interface::unk_522() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 523
void player_interface::unk_523(bool unk, bool unk_2) {
    ((void (__thiscall*)(player_interface*, bool, bool))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 524
bool player_interface::unk_524() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 525
i32 player_interface::unk_525(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 526
i32 player_interface::unk_526() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 527
void player_interface::unk_527(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 528
line_info* player_interface::unk_528() {
    return ((line_info* (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 529
void* player_interface::unk_529(i32 unk) {
    return ((void* (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 530
bool player_interface::unk_530(f32 unk) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_71FF10)(this, unk);
}

// slot 531
bool player_interface::unk_531(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_71FF10)(this, unk);
}

// slot 532
bool player_interface::unk_532(const line_info* unk) {
    return ((bool (__thiscall*)(player_interface*, const line_info*))retail::sub_71FF10)(this, unk);
}

// slot 533
i32 player_interface::unk_533(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_9A6650)(this, unk);
}

// slot 534
void player_interface::unk_534(i32 unk, f32 unk_2) {
    ((void (__thiscall*)(player_interface*, i32, f32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 535
i32 player_interface::unk_535(void* unk, i32 unk_2, i32 unk_3, i32 unk_4, void* unk_5, void* unk_6) {
    return ((i32 (__thiscall*)(player_interface*, void*, i32, i32, i32, void*, void*))retail::sub_99AFE0)(this, unk, unk_2, unk_3, unk_4, unk_5, unk_6);
}

// sub_783750
void player_interface::clear_buttons() { // every button but the first of each array
    for (i32 ctrl = 1; ctrl < 25; ++ctrl)
        buttons[ctrl].clear();

    for (i32 cmd = 1; cmd < 89; ++cmd)
        command_buttons[cmd].clear();
}

void player_interface::clear_controller(bool unk) {
    retail::sub_991FC0((i32*)this, unk);
}

// sub_83D9E0
void player_interface::override_button(i32 ctrl, f32 value, f32 t) {
    buttons[ctrl].update_state(t, value, value);
}

// sub_83DA10
void player_interface::override_command(i32 cmd, f32 value, f32 t) {
    command_buttons[cmd].update_state(t, value, value);
}

// sub_83DA40
void player_interface::set_button(i32 ctrl, f32 value) {
    buttons[ctrl].state  = value;
    buttons[ctrl].unk_1c = value;
}

// sub_83DA70
void player_interface::set_axis(i32 ctrl, bool ud, f32 value) {
    game_button* button = ud ? control_axis[ctrl]->axis_ud : control_axis[ctrl]->axis_lr;

    button->state  = value;
    button->unk_1c = value;
}
