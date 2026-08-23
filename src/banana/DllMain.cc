/*
    this isn't actually the entry.
    DllMain should be used for when you need to do something hyper-early.
*/

#include <windows.h>
#include "flags.hh"
#include "treyarch/app.hh"
#include "treyarch/ngl/ngl.hh"
#include "banana/logging.hh"
#include "util/redirect_call.hh"
#include "util/macros/debug.hh"

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) { 
        DisableThreadLibraryCalls(module);

#if NGL_BOOTSTRAP
        /*
            whatever's not marked as "todo" is redirected from a boundary we don't care about
        */

        banana::log.dbg("redirecting nglInit (WinMain)");
        
        if (!util::redirect_call(0x009CC222, { 0xE8, 0xA9, 0x9A, 0x01, 0x00 }, &treyarch::ngl::init))
            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting nglSetBufferSize callsites (WinMain)");

        if (!util::redirect_call(0x009CC2D2, { 0xE8, 0x29, 0xC6, 0x01, 0x00 }, &treyarch::ngl::set_buffer_size) ||
            !util::redirect_call(0x009CC311, { 0xE8, 0xEA, 0xC5, 0x01, 0x00 }, &treyarch::ngl::set_buffer_size) ||
            !util::redirect_call(0x009CC321, { 0xE8, 0xDA, 0xC5, 0x01, 0x00 }, &treyarch::ngl::set_buffer_size) ||
            !util::redirect_call(0x009CC334, { 0xE8, 0xC7, 0xC5, 0x01, 0x00 }, &treyarch::ngl::set_buffer_size))
            
            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting app::tick callsites (WinMain)");

        if (!util::redirect_call(0x009CC44C, { 0xE8, 0x3F, 0xD6, 0xA5, 0xFF }, &treyarch::app::tick) ||
            !util::redirect_call(0x009CC477, { 0xE8, 0x14, 0xD6, 0xA5, 0xFF }, &treyarch::app::tick))

            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting nglPresent (movie_manager)");

        if (!util::redirect_call(0x006ABC12, { 0xE8, 0xE9, 0xE8, 0x32, 0x00 }, &treyarch::ngl::present))
            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting nglPresent (game::clear_screen)"); // todo: own the `game` class

        if (!util::redirect_call(0x0097B04A, { 0xE8, 0xB1, 0xF4, 0x05, 0x00 }, &treyarch::ngl::present))
            FATAL_BREAKPOINT();

#endif
    }

    return TRUE;
}
