#pragma once

#include <format>
#include <minhook.h>

#include "manager.hh"
#include "function_signatures.hh"
#include "../log.hh"
#include "../util.hh"

#define HK_MSG(str, ...) LOG.msg("{}: {}", hook_name(), FMT(str, __VA_ARGS__))
#define HK_WRN(str, ...) LOG.wrn("{}: {}", hook_name(), FMT(str, __VA_ARGS__))
#define HK_ERR(str, ...) LOG.err("{}: {}", hook_name(), FMT(str, __VA_ARGS__))
#define HK_DBG(str, ...) LOG.dbg("{}: {}", hook_name(), FMT(str, __VA_ARGS__))
#define HK_MGR_MSG(str, ...) LOG.msg("hook_manager -> {}: {}", who(), str, __VA_ARGS__)
#define HK_MGR_WRN(str, ...) LOG.wrn("hook_manager -> {}: {}", who(), str, __VA_ARGS__)
#define HK_MGR_ERR(str, ...) LOG.err("hook_manager -> {}: {}", who(), str, __VA_ARGS__)
#define HK_MGR_DBG(str, ...) LOG.dbg("hook_manager -> {}: {}", who(), str, __VA_ARGS__)

#ifdef DEBUG
    #define DEBUG_HOOK_THREAD HK_DBG("caller={:p} | tid={}", _ReturnAddress(), GetCurrentThreadId())
    #define DEBUG_HOOK_ECX    HK_DBG("caller={:p} | ecx: {:p} | edx: {:p}", _ReturnAddress(), ecx, edx)
#else
    #define DEBUG_HOOK_THREAD
    #define DEBUG_HOOK_ECX
#endif

template <typename derived, typename fn_signature>
class c_hook : public i_hook {

protected:
    bool m_installed = false;

    void* m_address = nullptr;
    // for lambdas
    void* (*m_address_resolver)() = nullptr; 

    inline static fn_signature m_original = nullptr;

    MH_STATUS m_status;

public:
    c_hook(void* address) : m_address(address) {
        s_hook_manager::get().register_hook(this);
    }

    c_hook(void* (*resolver)()) : m_address_resolver(resolver) {
        s_hook_manager::get().register_hook(this);
    }

   ~c_hook() {
        if (this->m_installed)
            this->uninstall();
    }

    static std::string &hook_name() {
        static std::string name = std::string(type_name<derived>()).erase(0, 3);

        return name;
    }

    std::string who() const override {
        return this->hook_name();
    }

    void* get_original_ptr() const override {
        return (void*)m_original;
    }

    void* get_detour_ptr() const override {
        return (void*)&derived::detour;
    }

    void install() override {
        if (this->m_installed) {
            HK_MGR_WRN("can't install an installed hook");

            return;
        }

        if (m_address_resolver)
            m_address = m_address_resolver();

        if (!m_address) {
            HK_MGR_ERR("can't install, target address is null");
            
            return;
        }
        
        static_assert(!std::is_member_function_pointer_v<decltype(&derived::detour)>,
            "detour function must be declared as static");

        static_assert(std::is_same_v<decltype(&derived::detour), fn_signature>,
            "detour signature doesn't match fn_signature");

        this->m_status = MH_CreateHook(
            this->m_address,
            (void*)&derived::detour,
            (void**)&m_original
        );

        if (this->m_status != MH_OK)
            HK_MGR_ERR("installation failed during creation ({})", MH_STRING(this->m_status));
        else {
            this->m_status = MH_EnableHook(this->m_address);

            if (this->m_status != MH_OK)
                HK_MGR_ERR("installation failed during enabling ({})", MH_STRING(this->m_status));
            else {
                this->m_installed = true;

                HK_MGR_MSG("installed");
            }
        }
    }

    void uninstall() override {
        if (!this->m_installed) {
            HK_MGR_WRN("can't uninstall an uninstalled hook");

            return;
        }

        this->m_status = MH_DisableHook(this->m_address);

        if (this->m_status != MH_OK)
            HK_MGR_ERR("uninstallation failed during disabling ({})", MH_STRING(this->m_status));
        else {
            this->m_status = MH_RemoveHook(this->m_address);

            if (this->m_status != MH_OK)
                HK_MGR_ERR("uninstallation failed during removal ({})", MH_STRING(this->m_status));
            else {
                this->m_installed = false;

                HK_MGR_MSG("uninstalled");
            }
        }
    }
};