#pragma once

#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class toa_cutscene {

    public:
        u8   reserved_000[0x7CD];
        bool running;

        static toa_cutscene* get();

        // sub_805390
        bool is_running() const {
            return running;
        }
    };

    namespace references {
        inline util::memory_reference<toa_cutscene> toa_cutscene { 0x01088AA8 };

        inline util::memory_reference<u32> toa_cutscene_guard { 0x0108927C };
    } // references
} // treyarch
