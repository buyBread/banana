#pragma once

#include <filesystem>

#include "util/env.hh"

namespace util {
    inline auto appdata_path() {
        return std::filesystem::path
            (util::get_env(L"LOCALAPPDATA")) / "banana";
    }
} // util