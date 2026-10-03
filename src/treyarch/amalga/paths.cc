#include <new>

#include "retail.hh"
#include "treyarch/amalga/paths.hh"

using namespace treyarch;

// sub_7EFD90
mash::string amalga::get_amalgatoc_filename(e_platform platform) {
    return *(mash::string*)retail::sub_7EFAB0(platform) + "amalga.toc";
}

// sub_7EFCC0
const mash::string &amalga::get_packs_directory(e_platform platform) {
    u32 &guard = references::packs_directory_guard.get();

    if (!(guard & 1)) {
        guard |= 1;

        new (&references::packs_directory.get()) mash::string();
        retail::sub_ADE5FD(retail::sub_B72CA0); // the game's CRT atexit
    }

    // the cached platform is never written, so this always rebuilds
    if (platform == references::packs_directory_platform.read())
        return references::packs_directory.get();

    references::packs_directory.get() = *(mash::string*)retail::sub_7EFAB0(platform) + "packs\\";

    return references::packs_directory.get();
}
