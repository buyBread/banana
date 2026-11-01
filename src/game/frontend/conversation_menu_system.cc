#include "retail.hh"
#include "game/frontend/conversation_menu_system.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"
#include "game/frontend/ui_frontend.hh"
#include "treyarch/mash/string.hh"

using namespace treyarch;

// sub_6F90E0
conversation_menu_system::~conversation_menu_system() {
    if (name_text) {
        name_text->destroy();
        name_text = nullptr;
    }

    if (text) {
        text->destroy();
        text = nullptr;
    }

    if (replies_text) {
        replies_text->destroy();
        replies_text = nullptr;
    }

    if (updown_button) {
        delete updown_button;
        updown_button = nullptr;
    }

    if (confirm_button) {
        delete confirm_button;
        confirm_button = nullptr;
    }

    if (cancel_button) {
        delete cancel_button;
        cancel_button = nullptr;
    }

    release_entity();

    for (i32 index = 7; index >= 0; --index)
        ((mash::string*)unk_24c[index])->finalize(mash::ALLOCATED);
}

// slot 2
void conversation_menu_system::unk_002(i32 unk) {
    ((void (__thiscall*)(conversation_menu_system*, i32))retail::sub_68C230)(this, unk);
}

// slot 3
void conversation_menu_system::unk_003(i32 unk) {
    ((void (__thiscall*)(conversation_menu_system*, i32))retail::sub_6A0AC0)(this, unk);
}

// slot 4
void conversation_menu_system::unk_004(i32 unk) {
    ((void (__thiscall*)(conversation_menu_system*, i32))retail::sub_6A0AE0)(this, unk);
}

// slot 5
void conversation_menu_system::unk_005(i32 unk) {
    ((void (__thiscall*)(conversation_menu_system*, i32))retail::sub_689620)(this, unk);
}

// slot 6
void conversation_menu_system::unk_006(i32 unk) {
    ((void (__thiscall*)(conversation_menu_system*, i32))retail::sub_ADF440)(this, unk);
}

// slot 7
void conversation_menu_system::unk_007(i32 unk) {
    ((void (__thiscall*)(conversation_menu_system*, i32))retail::sub_717520)(this, unk);
}

// slot 10
void conversation_menu_system::unk_010(i32 unk) {
    ((void (__thiscall*)(conversation_menu_system*, i32))retail::sub_6F9340)(this, unk);
}

// +0x10 slot 2
void conversation_menu_system::update(f32 time_inc) {
    ((void (__thiscall*)(igo_3d_widget*, f32))retail::sub_7177F0)((igo_3d_widget*)this, time_inc);
}

// +0x10 slot 3
void conversation_menu_system::render() {
    ((void (__thiscall*)(igo_3d_widget*))retail::sub_715B30)((igo_3d_widget*)this);
}

// +0x10 slot 42
void conversation_menu_system::spawn_entity() {
    ((void (__thiscall*)(igo_3d_widget*))retail::sub_6FE1A0)((igo_3d_widget*)this);
}

// +0x10 slot 44
i32 conversation_menu_system::unk_044(i32 unk_0, i32 unk_1, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) {
    return ((i32 (__thiscall*)(igo_3d_widget*, i32, i32, void*, i32, i32, i32))retail::sub_6B40F0)((igo_3d_widget*)this,
                                                                                                   unk_0,
                                                                                                   unk_1,
                                                                                                   unk_2,
                                                                                                   unk_3,
                                                                                                   unk_4,
                                                                                                   unk_5);
}

// +0x10 slot 47
i32 conversation_menu_system::unk_047(i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32 unk_4) {
    return ((i32 (__thiscall*)(igo_3d_widget*, i32, i32, i32, i32, i32))retail::sub_68C200)((igo_3d_widget*)this,
                                                                                            unk_0,
                                                                                            unk_1,
                                                                                            unk_2,
                                                                                            unk_3,
                                                                                            unk_4);
}

// sub_68BF40
bool conversation_menu_system::is_conversation_active() {
    if (references::frontend.get().igo->scrapbook->is_active())
        return false;

    return active;
}
