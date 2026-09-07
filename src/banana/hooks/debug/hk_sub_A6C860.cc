#include "banana/hooks/base.hh"
#include "treyarch/shared/mutex.hh"

/*
    replacing this subroutine doesn't actually fix the stuttering properly.
    the only thing that comes to mind for a "proper" fix is too complex to bother with rn...
    (an actual critical section style replacement when i'm busy? no tnx)

    this variant still stutters (even more horribly) at a low MaxCpuCount setting.
    however, it's considerably smoother than retail when i set it to "4" on my six-core.
    and so... since retail has more stutters on "4" for me from my brief testing, we ship it.
    (13 signs of intelligence you can't fake)
*/

DEFINE_HOOK(hk_sub_A6C860, i32(__fastcall*)(treyarch::engine_recursive_lock*, void*, i64)) {

public:
    CONSTRUCT_HOOK(hk_sub_A6C860, (void*)0x00A6C860, "debug") {}

    static i32 __fastcall detour(treyarch::engine_recursive_lock* lock, void*, i64 owner_state) {
        lock->acquire_contended(owner_state);

        return 0;
    }
};

CREATE_HOOK(hk_sub_A6C860);