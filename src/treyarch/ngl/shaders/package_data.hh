#pragma once

#include <span>

#include "treyarch/ngl/shaders/description.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace package_data {
    std::span<const u8> load(shader_key key);
}}}} // treyarch::ngl::shaders::package_data
