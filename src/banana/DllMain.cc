#include <windows.h>

/*
    this isn't actually the entry.
    DllMain should be used for when you need to do something hyper-early.
*/

#include "flags.hh"
#include "treyarch/ngl/ngl.hh"
#include "banana/logging.hh"
#include "util/redirect_call.hh"
#include "util/macros/debug.hh"

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) { 
        DisableThreadLibraryCalls(module);

#if NGL_BOOTSTRAP
        banana::log.dbg("redirecting nglInit");
        
        if (!util::redirect_call(0x009CC222, { 0xE8, 0xA9, 0x9A, 0x01, 0x00 }, &treyarch::ngl::init))
            FATAL_BREAKPOINT();
#endif
    }

    return TRUE;
}