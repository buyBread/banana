#include <windows.h>

/*
    this isn't actually the entry.
    DllMain should be used for when you need to do something hyper-early.
*/

#include "flags.hh"
#include "treyarch/ngl/ngl.hh"

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) { 
        DisableThreadLibraryCalls(module);

#if NGL_BOOTSTRAP
        return treyarch::ngl::util::redirect_nglInit();
#endif
    }

    return TRUE;
}