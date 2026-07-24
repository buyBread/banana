#pragma once

namespace banana { namespace NGL { namespace soap { namespace storage {
    inline void* instance = nullptr;

    enum e_save_op {
        refresh = 2, // immediately once @ the main menu
        load,
        save,
    };
}}}} // banana::NGL::soap::storage
