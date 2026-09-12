#pragma once

/*
    this is not a "config" file.
    don't push your local flag flips.
*/

// flush all logging types
#define ALWAYS_FLUSH 1

// enables redirecting important calls to source owned variants
#define REL32_GAME_CALLS 1

// whether logger will allocate a console window
#define ALLOCATE_CONSOLE 1

// adjusts a handful of flags automatically
#define OPTIMIZE_FLAGS 1

// use spinlock method over yielding with Sleep(0) when trying to acquire a mutex
#define SPINLOCK_MUTEX 1

// ============================ //
//        OPTIMIZE_FLAGS        //
// ============================ //

#if OPTIMIZE_FLAGS
    #ifdef NDEBUG
        #define ALWAYS_FLUSH 0
        #define ALLOCATE_CONSOLE 0
    #endif
#endif