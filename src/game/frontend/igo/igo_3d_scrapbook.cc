#include "retail.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"

using namespace treyarch;

// sub_6ED680
igo_3d_scrapbook::~igo_3d_scrapbook() {
    for (u32 index = 0; index < stations.size(); ++index) {
        if (stations[index]) {
            delete stations[index];
            stations[index] = nullptr;
        }
    }

    stations.clear();

    if (tab_pages) {
        delete tab_pages;
        tab_pages = nullptr;
    }

    if (sub_tab_pages) {
        delete sub_tab_pages;
        sub_tab_pages = nullptr;
    }

    if (pause_bar_text) {
        pause_bar_text->destroy();
        pause_bar_text = nullptr;
    }

    if (pause_bar) {
        delete pause_bar;
        pause_bar = nullptr;
    }
}

// slot 1
void igo_3d_scrapbook::unk_001(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_7160E0)(this, unk);
}

// slot 2
void igo_3d_scrapbook::unk_002(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC560)(this, unk);
}

// slot 3
void igo_3d_scrapbook::unk_003(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC640)(this, unk);
}

// slot 4
void igo_3d_scrapbook::unk_004(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC6F0)(this, unk);
}

// slot 5
void igo_3d_scrapbook::unk_005(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC730)(this, unk);
}

// slot 6
void igo_3d_scrapbook::unk_006(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC770)(this, unk);
}

// slot 7
void igo_3d_scrapbook::unk_007(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC7B0)(this, unk);
}

// slot 8
void igo_3d_scrapbook::unk_008(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC7F0)(this, unk);
}

// slot 9
void igo_3d_scrapbook::unk_009(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC830)(this, unk);
}

// slot 10
void igo_3d_scrapbook::unk_010(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC870)(this, unk);
}

// slot 11
void igo_3d_scrapbook::unk_011(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC8F0)(this, unk);
}

// slot 12
void igo_3d_scrapbook::unk_012(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC940)(this, unk);
}

// slot 13
void igo_3d_scrapbook::unk_013(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6D1BB0)(this, unk);
}

// slot 14
void igo_3d_scrapbook::unk_014(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6D1D40)(this, unk);
}

// slot 15
void igo_3d_scrapbook::unk_015(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC5D0)(this, unk);
}

// slot 16
void igo_3d_scrapbook::unk_016(i32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, i32))retail::sub_6BC680)(this, unk);
}

// slot 24
void igo_3d_scrapbook::unk_024() {
    ((void (__thiscall*)(igo_3d_scrapbook*))retail::sub_6FB430)(this);
}

// slot 25
void igo_3d_scrapbook::unk_025(f32 unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, f32))retail::sub_6D1170)(this, unk);
}

// slot 26
void igo_3d_scrapbook::draw_pages() {
    ((void (__thiscall*)(igo_3d_scrapbook*))retail::sub_68FF10)(this);
}

// slot 27
void igo_3d_scrapbook::activate(bool unk) {
    ((void (__thiscall*)(igo_3d_scrapbook*, bool))retail::sub_6D15D0)(this, unk);
}

// slot 28
void igo_3d_scrapbook::unk_028() {
    ((void (__thiscall*)(igo_3d_scrapbook*))retail::sub_6D1930)(this);
}

// slot 29
u8 igo_3d_scrapbook::is_active() {
    return ((u8 (__thiscall*)(igo_3d_scrapbook*))retail::sub_6ED7C0)(this);
}

// slot 30
bool igo_3d_scrapbook::unk_030() {
    return ((bool (__thiscall*)(igo_3d_scrapbook*))retail::sub_690030)(this);
}

// slot 31
bool igo_3d_scrapbook::unk_031() {
    return ((bool (__thiscall*)(igo_3d_scrapbook*))retail::sub_68FF90)(this);
}
