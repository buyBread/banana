#include <cstdio>

#include <windows.h>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/debug/screenshot.hh"
#include "banana/core.hh"
#include "util/types.hh"
#include "util/memory_reference.hh"
#include "util/gimmie/method.hh"

using namespace treyarch;

using save_texture_function = HRESULT(WINAPI*)(const char*,
                                                     i32,
                                                     IDirect3DBaseTexture9*,
                                               const void*);

static util::memory_reference<char> screenshot_name_buffer { 0x01118860 };
static util::memory_reference<u32>  screenshot_index       { 0x011188A0 };

void ngl::debug::capture_screenshot(const char* name) {
    d3d9::wait_for_rendering();

    char* generated_name = &screenshot_name_buffer.get();

    if (!name) {
        std::sprintf(generated_name,
                     "screenshot%4.4d",
                     screenshot_index.get()++);

        name = generated_name;
    }

    char path[MAX_PATH];

    std::sprintf(path, "%s.png", name);

    for (char* cursor = path; *cursor; ++cursor) {
        if (*cursor == '/')
            *cursor = '\\';
    }

    auto D3DXSaveTextureToFileA = (save_texture_function)
        util::gimmie::method(banana::store::handle_d3dx, "D3DXSaveTextureToFileA");

    D3DXSaveTextureToFileA(path,
                           3,
                           d3d9::references::framebuffers.get().back_buffer->gpu_texture.resource,
                           nullptr);
}
