#include "retail.hh"
#include "game/wds/entity/interface/ise_interface.hh"

using namespace treyarch;

// slot 19
void* ise_interface::unk_019(void* unk, i32 unk_2) {
    return ((void* (__thiscall*)(ise_interface*, void*, i32))retail::sub_6179D0)(this, unk, unk_2);
}

// slot 20
i32 ise_interface::unk_020() {
    return ((i32 (__thiscall*)(ise_interface*))retail::sub_603E50)(this);
}

// slot 21
void ise_interface::stop_all_effects() {
    ((void (__thiscall*)(ise_interface*))retail::sub_603E60)(this);
}

// slot 22
void* ise_interface::unk_022(void* unk) {
    return ((void* (__thiscall*)(ise_interface*, void*))retail::sub_689610)(this, unk);
}

// slot 23
i32 ise_interface::unk_023() {
    return ((i32 (__thiscall*)(ise_interface*))retail::sub_646220)(this);
}
