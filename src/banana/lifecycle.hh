#pragma once

#include <string_view>
#include <format>
#include <cassert>

enum class e_lifecycle {
    pending,
    ready,      // acquired device
    failed,
    rebuilding, // lost device
    stopping,
};

template <> struct std::formatter<e_lifecycle> : std::formatter<std::string_view> {
    auto format(e_lifecycle v, std::format_context &ctx) const {
        std::string_view as_string;

        switch(v) {
            case e_lifecycle::pending:    as_string = "pending";    break;
            case e_lifecycle::ready:      as_string = "ready";      break;
            case e_lifecycle::failed:     as_string = "failed";     break;
            case e_lifecycle::rebuilding: as_string = "rebuilding"; break;
            case e_lifecycle::stopping:   as_string = "stopping";   break;
        }

        return std::formatter<std::string_view>::format(as_string, ctx);
    }
};