#include "retail.hh"
#include "treyarch/game/cutscene/toa_cutscene.hh"

using namespace treyarch;

// sub_809590
toa_cutscene* toa_cutscene::get() {
    u32 &guard = references::toa_cutscene_guard.get();

    if (!(guard & 1)) {
        guard |= 1;

        retail::sub_807F30((u32*)&references::toa_cutscene.get());
        retail::sub_ADE5FD(retail::sub_B72E70); // the game's CRT atexit
    }

    return &references::toa_cutscene.get();
}
