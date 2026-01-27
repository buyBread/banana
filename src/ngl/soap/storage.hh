#pragma once

namespace banana { namespace NGL {
namespace internals { namespace soap {
namespace storage {
    inline void* instance = nullptr;

    enum save_op {
        refresh = 2, // immediately once @ the main menu
        load,
        save,
    };
} // storage
} } // internals::soap
} } // banana::NGL
