#pragma once

#include <d3d9.h>

#include "../../util.hh"

namespace banana { namespace NGL {  namespace renderer {
    MEMORY_REFERENCE<IDirect3DDevice9*> d3d9_device { 0x011170BC };
}}} // banana::NGL::renderer