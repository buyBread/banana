#pragma once

#include <d3d9.h>

#include "../../util.hh"

namespace banana { namespace NGL {
namespace internals { namespace renderer {
    MEMORY_REFERENCE<IDirect3DDevice9*> d3d9_device { 0x011170BC };
} } // internals::renderer
} } // banana::NGL