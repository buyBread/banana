#pragma once

#include "util/memory_reference.hh"

namespace treyarch { namespace ngl {
    struct texture;

    namespace fx {
        struct effect;
    } // fx
}} // treyarch::ngl

namespace treyarch { namespace references {
    inline util::memory_reference<void*> rvb_radar_resource_manager_maybe_idk { 0x010FC5C0 };

    inline util::memory_reference<ngl::fx::effect*> highlight_zprime_shader { 0x010FC598 };
    inline util::memory_reference<ngl::fx::effect*> highlight_shader        { 0x010FC59C };
    inline util::memory_reference<ngl::fx::effect*> highlight_skin_zprime   { 0x010FC5A0 };
    inline util::memory_reference<ngl::fx::effect*> highlight_skin          { 0x010FC5A4 };
    inline util::memory_reference<ngl::fx::effect*> buildinglod_zpass       { 0x010FC5A8 };
    inline util::memory_reference<ngl::fx::effect*> buildinglod_foam_core   { 0x010FC5AC };
    inline util::memory_reference<ngl::fx::effect*> roadlod_foam_core       { 0x010FC5B0 };
    inline util::memory_reference<ngl::fx::effect*> buildinglod_radar       { 0x010FC5B4 };
    inline util::memory_reference<void*>            rvb_radar               { 0x010FC5B8 };
    inline util::memory_reference<ngl::texture*>    radar_stroke            { 0x010FC5BC };
    inline util::memory_reference<ngl::texture*>    dither_texture          { 0x010FC584 };
    inline util::memory_reference<ngl::texture*>    horizon_clouds          { 0x010FC588 };
}} // treyarch::references
