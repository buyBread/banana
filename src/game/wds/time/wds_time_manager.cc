#include "retail.hh"
#include "game/wds/time/wds_time_manager.hh"

using namespace treyarch;

wds_time_manager::wds_time_manager() {
    retail::sub_95CA90((u32*)this);
}

// sub_95CB60
// inlined @ sub_97A400
wds_time_manager::~wds_time_manager() {
    if (unk_020)
        ((void (__thiscall*)(void*, u32))(*(void***)unk_020)[0])(unk_020, 1); // slot 0

    unk_020 = nullptr;

    retail::sub_757AB0((char**)unk_030);
}
