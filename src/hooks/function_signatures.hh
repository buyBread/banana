#pragma once

// types [includes]
#include <windows.h>
#include <d3d9.h>
#include <dinput.h>
#include <cstdint>

// types [fwd. declares]
struct resource_descriptor_t;
struct vm_executable;
struct vm_thread;
struct script_instance;
struct script_executable;
struct script_object;

namespace banana {

namespace hk_fn_sig {
    // directx
    using BeginScene_t = HRESULT(WINAPI*)(IDirect3DDevice9* pDevice);
    using EndScene_t   = HRESULT(WINAPI*)(IDirect3DDevice9* pDevice);
    using Present_t    = HRESULT(WINAPI*)(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);

    // awsm video game
    using sub_9CC940_t = int(__cdecl*)(const char*, ...);
    using sub_9DA500_t = void(__cdecl*)();
    using sub_408DD0_t = uint32_t(__cdecl*)(const char*);
    using sub_A6C7E0_t = void(__thiscall*)(uint32_t*, bool, const char*, uint32_t);
    using sub_9E8520_t = void*(__fastcall*)(void*, void*);
    using sub_9E78A0_t = void*(__fastcall*)(void*, void*, void*);
    using sub_9F0100_t = bool(__fastcall*)(void*, void*, int);
    using sub_9E9D10_t = void(__cdecl*)();
    using sub_9E9AE0_t = void(__cdecl*)();
    using sub_759EB0_t = void*(__cdecl*)(resource_descriptor_t*);
    using sub_A1FA70_t = void(__thiscall*)(void*);
    using sub_A1E980_t = void*(__thiscall*)(void*, char);
    using sub_825E00_t = void* (__cdecl*)(int, void*, int);
    using sub_A1DFA0_t = vm_thread*(__thiscall*)(script_instance*, vm_executable*, void*, int);
    using sub_A22AC0_t = void(__thiscall*)(vm_thread*);

    // misc.
    using GetRawInputData_t     = UINT(WINAPI*)(HRAWINPUT, UINT, LPVOID, PUINT, UINT);
    using SetCooperativeLevel_t = HRESULT(WINAPI*)(IDirectInputDevice8*, HWND, DWORD);

} // hk_fn_sig

} // banana

using namespace banana;