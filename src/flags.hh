#pragma once

/*
    this is not a "config file"!
    any additional flags must be manually added to a commit.
*/

// flush all logging types
#define ALWAYS_FLUSH 1

// enables full NGL bootstrap
#define NGL_BOOTSTRAP 1

// whether logger will allocate a console window
#define ALLOCATE_CONSOLE 1

// adjusts a handful of flags automatically
#define OPTIMIZE_FLAGS 1

// ============================ //
//        OPTIMIZE_FLAGS        //
// ============================ //

#if OPTIMIZE_FLAGS
    #ifdef NDEBUG
        #define ALWAYS_FLUSH 0
        #define ALLOCATE_CONSOLE 0
    #endif
#endif