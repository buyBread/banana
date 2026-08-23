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

        banana::log.dbg("redirecting nglInit");
        
        if (!util::redirect_call(0x009CC222, { 0xE8, 0xA9, 0x9A, 0x01, 0x00 }, &treyarch::ngl::init))
            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting app::tick (active)");

        if (!util::redirect_call(0x009CC44C, { 0xE8, 0x3F, 0xD6, 0xA5, 0xFF }, &treyarch::app::tick))
            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting app::tick (inactive)");

        if (!util::redirect_call(0x009CC477, { 0xE8, 0x14, 0xD6, 0xA5, 0xFF }, &treyarch::app::tick))
            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting nglPresent (movie_manager)");

        if (!util::redirect_call(0x006ABC12, { 0xE8, 0xE9, 0xE8, 0x32, 0x00 }, &treyarch::ngl::present))
            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting nglPresent (game::clear_screen)"); // todo: own the `game` class

        if (!util::redirect_call(0x0097B04A, { 0xE8, 0xB1, 0xF4, 0x05, 0x00 }, &treyarch::ngl::present))
            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting nglListSend callsites");

        // todo: own nglSetBufferSize
        if (!util::redirect_call(0x009E8924, { 0xE8, 0xE7, 0xE6, 0xFF, 0xFF }, &treyarch::ngl::list_send))
            FATAL_BREAKPOINT();

        // todo: own the default work-buffer provisioner
        if (!util::redirect_call(0x009E8B14, { 0xE8, 0xF7, 0xE4, 0xFF, 0xFF }, &treyarch::ngl::list_send) ||
            !util::redirect_call(0x009E8BA2, { 0xE8, 0x69, 0xE4, 0xFF, 0xFF }, &treyarch::ngl::list_send) ||
            !util::redirect_call(0x009E8C5F, { 0xE8, 0xAC, 0xE3, 0xFF, 0xFF }, &treyarch::ngl::list_send) ||
            !util::redirect_call(0x009E8D18, { 0xE8, 0xF3, 0xE2, 0xFF, 0xFF }, &treyarch::ngl::list_send))
            
            FATAL_BREAKPOINT();
#endif
    }

    return TRUE;
}
