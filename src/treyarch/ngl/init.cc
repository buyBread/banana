#include <windows.h>

#include "banana/logging.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/version.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "util/gimmie/fn.hh"
#include "util/types.hh"

using namespace treyarch;

ngl::scene* ngl::init(HWND window) {
    banana::log.dbg("Nyarlathotep's Graphics Laboratory"
                    " "
                    NGL_VERSION);

    references::render_window.write(window);

    _controlfp(_PC_24, _MCW_PC);

    ::util::gimmie::fn<void(__cdecl*)()>(0x009E7280)();
    ::util::gimmie::fn<void(__cdecl*)()>(0x009DC6F0)();
    ::util::gimmie::fn<void(__cdecl*)()>(0x009E4680)();

    references::frame_epoch.write(0);

    ::util::gimmie::fn<void(__cdecl*)()>(0x009DCBA0)();
    ::util::gimmie::fn<void(__cdecl*)()>(0x009CCAF0)();
    ::util::gimmie::fn<void(__cdecl*)()>(0x007C2DE0)();
    ::util::gimmie::fn<void(__cdecl*)()>(0x009E2240)();

    ngl::scene* root_scene = ::util::gimmie::fn<ngl::scene*(__cdecl*)()>(0x009DCDE0)();

    references::initialized.write(1);

    return root_scene;
}

bool ngl::util::redirect_nglInit() {    
    constexpr u8 expected_instruction[] { 0xE8, 0xA9, 0x9A, 0x01, 0x00 };

    u8* call = (u8*)0x009CC222; // to nglInit

    if (std::memcmp(call, expected_instruction, sizeof(expected_instruction)) != 0) {
        banana::log.err("unexpected nglInit callsite");

        return false;
    }

    i32 displacement = (u8*)&ngl::init - (call + sizeof(expected_instruction));
        
    DWORD previous_protection {};

    if (!VirtualProtect(call,
                        sizeof(expected_instruction),
                        PAGE_EXECUTE_READWRITE,
                        &previous_protection)) {

        banana::log.err("failed to redirect nglInit");

        return false;
    }

    // say bye bye
    std::memcpy(call + 1, &displacement, sizeof(displacement));
        
    FlushInstructionCache(GetCurrentProcess(),
                          call,
                          sizeof(expected_instruction));

    DWORD unused_protection {};
        
    VirtualProtect(call,
                   sizeof(expected_instruction),
                   previous_protection,
                   &unused_protection);

    return true;
}
