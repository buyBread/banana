#include <windows.h>
#include <psapi.h>

#include "treyarch/mm/mm.hh"

using namespace treyarch;

namespace treyarch { namespace mm { namespace references {
    util::memory_reference<u32> virtual_total { 0x00FFDA60 };
    util::memory_reference<u32> virtual_used  { 0x00FFDA64 };
}}} // treyarch::mm::references

// sub_5F8930
void mm::load_module_information() {
    HANDLE  process = GetCurrentProcess();
    HMODULE modules[1024];
    DWORD   needed  = 0;

    EnumProcessModules(process, modules, sizeof(modules), &needed);

    for (DWORD index = 0; index < needed; ++index) {
        MODULEINFO information;

        GetModuleInformation(process, modules[index], &information, sizeof(information));
    }
}

// sub_5F8990
// inlined @ sub_5F89C0
void mm::get_virtual_memory_usage(u32* total, u32* used) {
    MEMORYSTATUS status;

    GlobalMemoryStatus(&status);

    *total = (u32)status.dwTotalVirtual;
    *used  = (u32)(status.dwTotalVirtual - status.dwAvailVirtual);
}

// sub_5F89C0
i32 mm::memory_space::map_memory(u32 bytes, void* user_data) {
    memory_space* space = (memory_space*)user_data;

    if ((space->max_size && bytes + space->size > space->max_size) || space->type != type_virtual)
        return -1;

    DWORD allocation_type = MEM_COMMIT | MEM_RESERVE;

    if (space->hint_flags & hint_top)
        allocation_type |= MEM_TOP_DOWN;

    void* address = VirtualAlloc(nullptr, bytes, allocation_type, PAGE_READWRITE);

    if (address)
        space->size += bytes;

    get_virtual_memory_usage(&references::virtual_total.get(), &references::virtual_used.get());

    return address ? (i32)address : -1;
}

// sub_5F8A50
i32 mm::memory_space::unmap_memory(void* address, u32 bytes, void* user_data) {
    memory_space* space = (memory_space*)user_data;

    if (space->type != type_virtual)
        return -1;

    // the region goes back one VirtualAlloc reservation at a time
    for (u8* region = (u8*)address; bytes;) {
        MEMORY_BASIC_INFORMATION information;

        if (!VirtualQuery(region, &information, sizeof(information)) ||
            information.BaseAddress    != region                     ||
            information.AllocationBase != region                     ||
            information.State          != MEM_COMMIT                 ||
            information.RegionSize     >  bytes                      ||
            !VirtualFree(region, 0, MEM_RELEASE)) {

            return -1;
        }

        space->size -= (u32)information.RegionSize;
        bytes       -= (u32)information.RegionSize;
        region      += information.RegionSize;
    }

    get_virtual_memory_usage(&references::virtual_total.get(), &references::virtual_used.get());

    return 0;
}

// sub_5F8B00
void mm::memory_space::lock() {
    while (_InterlockedCompareExchange(&spin_lock, 1, 0))
        Sleep(0);
}

// sub_5F8B40
void mm::memory_space::unlock() {
    while (_InterlockedCompareExchange(&spin_lock, 0, 1) != 1)
        Sleep(0);
}
