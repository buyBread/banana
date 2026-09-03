#pragma once

#include "treyarch/ngl/fx/effect.hh"

namespace treyarch { namespace ngl { namespace fx {
    void apply_pass(effect* effect_data, pass* value);
    void finish_pass(effect* effect_data, pass* value);
}}} // treyarch::ngl::fx
