#pragma once

namespace treyarch {
    // SM3 delta_variable
    template<typename T>
    struct delta_variable {
        T previous;
        T current;
    };
} // treyarch
