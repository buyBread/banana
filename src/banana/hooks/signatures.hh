#pragma once

// suppress annoying dinput warning
#define DIRECTINPUT_VERSION 0x0800

/*
    type includes
*/
#include <d3d9.h>
#include <dinput.h>

namespace banana { namespace hook_signatures {

namespace device_lifecycle {
    using CreateDevice = HRESULT(WINAPI*)(IDirect3D9*, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, IDirect3DDevice9**);
    using Reset = HRESULT(WINAPI*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
} // device_lifecycle

namespace bandaids {
    using sub_9DDF10 = int(__cdecl*)(const DWORD*, IDirect3DPixelShader9**);
    using sub_9EA950 = int(__cdecl*)(int renderer, DWORD* node);
    using DrawIndexedPrimitive = HRESULT(WINAPI*)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
} // bandaids

namespace imgui {
    using GetRawInputData     = UINT(WINAPI*)(HRAWINPUT, UINT, LPVOID, PUINT, UINT);
    using SetCooperativeLevel = HRESULT(WINAPI*)(IDirectInputDevice8*, HWND, DWORD);
    using EndScene            = HRESULT(WINAPI*)(IDirect3DDevice9*);
} // imgui

}} // banana::hook_signatures

using namespace banana;

#undef DIRECTINPUT_VERSION