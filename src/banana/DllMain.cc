/*
    this isn't actually the entry.
    DllMain should be used for when you need to do something hyper-early.
*/

#include <windows.h>

#include "banana/lifecycle.hh"
#include "banana/core.hh"

#define REL32_GAME_CALLS 1

#if REL32_GAME_CALLS
    #include "redirects.hh"
#endif

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) { 
        DisableThreadLibraryCalls(module);

        if (!banana::vibe_check())
            return FALSE;

#if REL32_GAME_CALLS
        treyarch::redirect_game_calls();
#endif

        banana::state::update(e_lifecycle::pending);
    }

    return TRUE;
}
