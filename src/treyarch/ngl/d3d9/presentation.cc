#include <windows.h>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/display.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/d3d9/presentation.hh"
#include "treyarch/ngl/ngl.hh"

using namespace treyarch;

static void pump_window_messages() {
    MSG message;

    while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

void ngl::d3d9::flip() {
    IDirect3DDevice9* device = references::device.get();

    device->BeginScene();

    IDirect3DSurface9* device_back_buffer = nullptr;

    device->GetBackBuffer(0,
                          0,
                          D3DBACKBUFFER_TYPE_MONO,
                          &device_back_buffer);
    blit_texture(device_back_buffer,
                 references::framebuffers.get().back_buffer);
    device_back_buffer->Release();

    framebuffer_state &framebuffers = references::framebuffers.get();
    IDirect3DSurface9* front_surface = nullptr;

    ((IDirect3DTexture9*)framebuffers.front_buffer->gpu_texture.resource)
        ->GetSurfaceLevel(0, &front_surface);
    blit_texture(front_surface, framebuffers.back_buffer);
    front_surface->Release();

    device->EndScene();

    if (!references::window_inactive.read()) {
        HRESULT result = device->Present(nullptr, nullptr, nullptr, nullptr);

        if (result == D3DERR_DEVICELOST) {
            Sleep(100);

            if (device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
                reset_device();
        }
    }

    if (references::owns_window.read())
        pump_window_messages();

    ++ngl::references::frame_epoch.get();
}
