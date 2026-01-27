#pragma once

/*
    yes, this is just random things
    yes, todo: clean up
*/

#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <cstdint>
#include <format>
#include <string_view>
#include <dinput.h>

#define ASSERT_FAIL_SANITY "sanity check failed: compare offender against earlier commit?"

#define MEMORY_REFERENCE inline mem_ref

#define MH_STRING MH_StatusToString

template <typename T>
constexpr std::string_view type_name() {
    std::string_view sig = __FUNCSIG__;

    auto start = sig.find("type_name<") + sizeof("type_name<") - 1;
    auto end   = sig.rfind(">(void)");
    
    std::string_view name = sig.substr(start, end - start);

    if (name.substr(0, 6) == "class ")
        name.remove_prefix(6);
    else if (name.substr(0, 7) == "struct ")
        name.remove_prefix(7);

    return name;
}

inline void allocate_console() {
    AllocConsole();

    freopen("CONIN$",  "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    HANDLE handle_output = GetStdHandle(STD_OUTPUT_HANDLE);

    if (handle_output == INVALID_HANDLE_VALUE)
        return;

    DWORD mode = 0;
        
    if (!GetConsoleMode(handle_output, &mode))
        return;

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(handle_output, mode);
}

template <typename T>
[[nodiscard]] inline static T get_fn(uint32_t address) noexcept {
    return (T)address;
}

inline std::string fmt_str_c(const char* format, ...) {
    char buf[512];

    va_list args;
    va_start(args, format);
    std::vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    return buf;
}

template <typename T>
inline void* gimmie_virt(T* ptr, int idx) {
    void** vtable = *(void***)ptr;
    
    return vtable[idx];
}

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

inline void* gimmie_virt_dinput8(int idx) {
    static void** vtable = nullptr;

    if (!vtable) {
        IDirectInput8* pDI = nullptr;

        if (SUCCEEDED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDI, NULL))) {
            IDirectInputDevice8* pDevice = nullptr;

            if (SUCCEEDED(pDI->CreateDevice(GUID_SysKeyboard, &pDevice, NULL))) {
                vtable = *(void***)pDevice;

                pDevice->Release();
            }

            pDI->Release();
        }
    }
    
    return vtable ? vtable[idx] : nullptr;
}

inline HMODULE gimmie_dll(std::string dll) {
    char system_directory[MAX_PATH];
    
    GetSystemDirectoryA(system_directory, MAX_PATH);

    return LoadLibrary(std::format("{}\\{}", system_directory, dll).c_str());
}

template <typename T>
class mem_ref {

private:
    T* m_address;
    
public:
    explicit mem_ref(uint32_t address) noexcept :
        m_address((T*)address) {}
    
    [[nodiscard]] T read() const {
        return *m_address;
    }

    void write(const T& value) const {
        *m_address = value;
    }

    [[nodiscard]] T& get() const noexcept {
        return *m_address;
    }
};
