#include "banana/hooks/base.hh"
#include "game/mutex.hh"

DEFINE_HOOK(hk_sub_A6C860, i32(__fastcall*)(treyarch::engine_recursive_lock*, void*, i64)) {

public:
    CONSTRUCT_HOOK(hk_sub_A6C860, (void*)0x00A6C860, "debug") {}

    static i32 __fastcall detour(treyarch::engine_recursive_lock* lock, void*, i64 owner_state) {
        lock->acquire_contended(owner_state);

        return 0;
    }
};

CREATE_HOOK(hk_sub_A6C860);