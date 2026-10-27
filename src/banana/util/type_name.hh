#pragma once

#include <string_view>

namespace util {
    template <typename T>
    constexpr std::string_view type_name() {
        std::string_view sig = __FUNCSIG__;

        auto start = sig.find("type_name<") + sizeof("type_name<") - 1;
        auto end   = sig.rfind(">(void)");
        
        std::string_view name = sig.substr(start, end - start);

        if (name.substr(0, 6) == "class ")
            name.remove_prefix(6);
        else if (name.substr(0, 7) == "struct ")
            name.remove_prefix(7);

        return name;
    }
} // util