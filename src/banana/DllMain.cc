/*
    this isn't actually the entry.
    DllMain should be used for when you need to do something hyper-early.
*/

#include <windows.h>

#include "flags.hh"
#include "util/redirect_rel32.hh"
#include "util/macros/debug.hh"
#include "banana/logging.hh"
#include "banana/core.hh"

#if REL32_GAME_CALLS
    #include "treyarch/app/app.hh"
    #include "treyarch/game/game.hh"
    #include "treyarch/game/input/input_mgr.hh"
    #include "treyarch/ngl/ngl.hh"
#endif

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) { 
        DisableThreadLibraryCalls(module);

        if (!banana::vibe_check())
            return FALSE;

#if REL32_GAME_CALLS
        banana::log.dbg("DllMain: redirecting nglInit (WinMain)");
        
        if (!util::redirect_rel32(0x009CC222, { 0xE8, 0xA9, 0x9A, 0x01, 0x00 }, &treyarch::ngl::init))
            FATAL_BREAKPOINT();

        banana::log.dbg("DllMain: redirecting nglSetBufferSize callsites (WinMain)");

        if (!util::redirect_rel32(0x009CC2D2, { 0xE8, 0x29, 0xC6, 0x01, 0x00 }, &treyarch::ngl::set_buffer_size) ||
            !util::redirect_rel32(0x009CC311, { 0xE8, 0xEA, 0xC5, 0x01, 0x00 }, &treyarch::ngl::set_buffer_size) ||
            !util::redirect_rel32(0x009CC321, { 0xE8, 0xDA, 0xC5, 0x01, 0x00 }, &treyarch::ngl::set_buffer_size) ||
            !util::redirect_rel32(0x009CC334, { 0xE8, 0xC7, 0xC5, 0x01, 0x00 }, &treyarch::ngl::set_buffer_size))
            
            FATAL_BREAKPOINT();

        banana::log.dbg("DllMain: redirecting app::tick callsites (WinMain)");

        if (!util::redirect_rel32(0x009CC44C, { 0xE8, 0x3F, 0xD6, 0xA5, 0xFF }, &treyarch::app::tick) ||
            !util::redirect_rel32(0x009CC477, { 0xE8, 0x14, 0xD6, 0xA5, 0xFF }, &treyarch::app::tick))

            FATAL_BREAKPOINT();

        banana::log.dbg("DllMain: redirecting input_mgr::poll_devices (movie_manager)");

        if (!util::redirect_rel32(0x006ABCEB, { 0xE8, 0xE0, 0xD5, 0x2A, 0x00 }, &treyarch::input_mgr::poll_devices))
            FATAL_BREAKPOINT();

        banana::log.dbg("DllMain: redirecting nglPresent (movie_manager)");

        if (!util::redirect_rel32(0x006ABC12, { 0xE8, 0xE9, 0xE8, 0x32, 0x00 }, &treyarch::ngl::present))
            FATAL_BREAKPOINT();

        banana::log.dbg("DllMain: redirecting game::clear_screen callsites");

        if (!util::redirect_rel32(0x0076BC14, { 0xE8, 0xF7, 0xF3, 0x20, 0x00 }, &treyarch::game::clear_screen) ||
            !util::redirect_rel32(0x0097B495, { 0xE8, 0x76, 0xFB, 0xFF, 0xFF }, &treyarch::game::clear_screen))

            FATAL_BREAKPOINT();
#endif
    }

    return TRUE;
}
