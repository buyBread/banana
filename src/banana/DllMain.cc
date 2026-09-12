/*
    this isn't actually the entry.
    DllMain should be used for when you need to do something hyper-early.
*/

#include <windows.h>

#include "flags.hh"
#include "banana/core.hh"

#if REL32_GAME_CALLS
    #include "treyarch/rel32_calls.hh"
#endif

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) { 
        DisableThreadLibraryCalls(module);

        if (!banana::vibe_check())
            return FALSE;

#if REL32_GAME_CALLS
        treyarch::redirect_game_calls();
#endif
    }

    return TRUE;
}
