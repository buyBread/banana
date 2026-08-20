#pragma once

#include <format>
#include <minhook.h>

#include "banana/hooks/manager.hh"
#include "banana/hooks/signatures.hh"
#include "banana/core.hh"
#include "banana/logging.hh"
#include "util/type_name.hh"
#include "util/macros/unique_name.hh"

#define DEFINE_HOOK(name, signature) class name : public c_hook<name, signature>
#define CONSTRUCT_HOOK(name, address, ...) name() : c_hook(address, __VA_ARGS__)
#define CREATE_HOOK(name) name UNIQUE_NAME(hk) {}

// clunky
#define HK_MSG(str, ...) banana::log.msg("{}: {}", hook_name(), std::format(str, __VA_ARGS__))
#define HK_WRN(str, ...) banana::log.wrn("{}: {}", hook_name(), std::format(str, __VA_ARGS__))
#define HK_ERR(str, ...) banana::log.err("{}: {}", hook_name(), std::format(str, __VA_ARGS__))
#define HK_DBG(str, ...) banana::log.dbg("{}: {}", hook_name(), std::format(str, __VA_ARGS__))
#define HK_MGR_MSG(str, ...) banana::log.msg("hook_manager -> {}: {}", who(), std::format(str, __VA_ARGS__))
#define HK_MGR_WRN(str, ...) banana::log.wrn("hook_manager -> {}: {}", who(), std::format(str, __VA_ARGS__))
#define HK_MGR_ERR(str, ...) banana::log.err("hook_manager -> {}: {}", who(), std::format(str, __VA_ARGS__))
#define HK_MGR_DBG(str, ...) banana::log.dbg("hook_manager -> {}: {}", who(), std::format(str, __VA_ARGS__))

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
    std::string m_category;

    void*   m_address             = nullptr;
    void* (*m_address_resolver)() = nullptr;

    inline static fn_signature m_original = nullptr;

    MH_STATUS m_status;

    e_hook_state m_state = e_hook_state::absent;

public:
    c_hook(void* address) : m_address(address) {
        s_hook_manager::get().register_hook(this);
    }

    c_hook(void* (*resolver)()) : m_address_resolver(resolver) {
        s_hook_manager::get().register_hook(this);
    }

    c_hook(void* address, std::string category) : m_address(address),
                                                  m_category(category) {
        s_hook_manager::get().register_hook(this, category);
    }

    c_hook(void* (*resolver)(), std::string category) : m_address_resolver(resolver),
                                                        m_category(category) {
        s_hook_manager::get().register_hook(this, category);
    }

    void install() override {
        if (this->m_state == e_hook_state::enabled) {
            HK_MGR_DBG("can't install an enabled hook");

            return;
        }

        if (m_address_resolver)
            m_address = m_address_resolver();

        if (!m_address) {
            HK_MGR_ERR("couldn't resolve target address");
            
            return;
        }

        static_assert(!std::is_member_function_pointer_v<decltype(&derived::detour)>,
            "detour function must be declared as static");

        static_assert(std::is_same_v<decltype(&derived::detour), fn_signature>,
            "detour signature doesn't match original signature");

        if (!this->create()) {
            HK_MGR_ERR("failed to install [creation]");

            return;
        }

        if (!this->queue_enable()) {
            HK_MGR_ERR("failed to install [queue enable]");

            this->remove();

            return;
        }
    }

    void uninstall() override {
        if (this->m_state == e_hook_state::absent) {
            HK_MGR_DBG("can't uninstall an absent hook");

            return;
        }

        if (!this->disable()) {
            HK_MGR_ERR("failed to uninstall [disabling]");

            return;
        }

        if (!this->remove()) {
            HK_MGR_ERR("failed to uninstall [removal]");

            return;
        }
    }

    bool create() {
        // could be first-called from `this->enable` via `hook_manager.enable_hook`
        if (!m_address) {
            if (m_address_resolver)
                m_address = m_address_resolver();

            if (!m_address) {
                HK_MGR_ERR("couldn't resolve target address");
                
                return false;
            }
        }

        if (this->m_state >= e_hook_state::created) {
            HK_MGR_DBG("can't create an already created hook");

            return true;
        }

        this->m_status = MH_CreateHook(
            this->m_address,
            (void*)&derived::detour,
            (void**)&m_original
        );

        if (this->m_status != MH_OK) {
            HK_MGR_ERR("failed to create ({})", MH_STRING(this->m_status));

            return false;
        }

        this->m_state = e_hook_state::created;

        HK_MGR_DBG("created");

        return true;
    }

    bool remove() {
        if (this->m_state == e_hook_state::absent) {
            HK_MGR_DBG("can't remove a non-existing hook");

            return true;
        }

        this->m_status = MH_RemoveHook(this->m_address);

        if (this->m_status != MH_OK) {
            HK_MGR_ERR("failed to remove ({})", MH_STRING(this->m_status));

            return false;
        }

        this->m_state = e_hook_state::absent;

        HK_MGR_DBG("removed");

        return true;
    }

    bool enable() override {
        if (this->m_state == e_hook_state::enabled) {
            HK_MGR_DBG("can't enable an enabled hook");

            return true;
        }

        if (this->m_state == e_hook_state::absent) {
            HK_MGR_DBG("can't enable an absent hook, creating it");

            if (!this->create())
                return false;
        }

        this->m_status = MH_EnableHook(this->m_address);

        if (this->m_status != MH_OK) {
            HK_MGR_ERR("failed to enable hook ({})", MH_STRING(this->m_status));
            
            return false;
        }

        this->m_state = e_hook_state::enabled;

        HK_MGR_DBG("enabled");

        return true;
    }

    bool disable() override {
        if (this->m_state != e_hook_state::enabled) {
            HK_MGR_DBG("can't disable a hook that isn't enabled");

            return true;
        }

        this->clean_up(); // semantically, this is more a "reset engine state" before giving back the hook,
                          // so we don't break things when disabling a hook mid-gameplay.

        this->m_status = MH_DisableHook(this->m_address);

        if (this->m_status != MH_OK) {
            HK_MGR_ERR("failed to disable hook ({})", MH_STRING(this->m_status));

            if (banana::state::current() != e_lifecycle::stopping)
                banana::state::update(e_lifecycle::failed); // icky situation
                                                            // it can only really happen if minhook is spazzing out,
                                                            // so it's as good of a "failed" state as any.

            return false;
        }

        this->m_state = e_hook_state::created;

        HK_MGR_DBG("disabled");
        
        return true;
    }

    bool queue_enable() override {
        if (this->m_state == e_hook_state::queued_for_enable ||
            this->m_state == e_hook_state::enabled) {
            
            HK_MGR_DBG("can't queue enable an already queued or enabled hook");

            return true;
        }

        if (this->m_state == e_hook_state::absent) {
            HK_MGR_DBG("can't queue enable an absent hook");

            if (!this->create())
                return false;
        }

        this->m_status = MH_QueueEnableHook(this->m_address);

        if (this->m_status != MH_OK) {
            HK_MGR_ERR("failed to queue enable hook ({})", MH_STRING(this->m_status));
            
            return false;
        }

        this->m_state = e_hook_state::queued_for_enable;

        HK_MGR_DBG("queued for enable");

        return true;
    }

    bool queue_disable() override {
        if (this->m_state == e_hook_state::queued_for_disable ||
            this->m_state == e_hook_state::created) {

            HK_MGR_DBG("can't queue disable an already queued or disabled hook, it will be created");

            return true;
        }

        if (this->m_state == e_hook_state::absent) {
            HK_MGR_DBG("can't queue disable an absent hook");

            return true;
        }

        this->m_status = MH_QueueDisableHook(this->m_address);

        if (this->m_status != MH_OK) {
            HK_MGR_ERR("failed to queue disable hook ({})", MH_STRING(this->m_status));

            if (banana::state::current() != e_lifecycle::stopping)
                banana::state::update(e_lifecycle::failed); // another icky situation
            
            return false;
        }

        this->m_state = e_hook_state::queued_for_disable;

        HK_MGR_DBG("queued for disable");

        return true;
    }

    static std::string &hook_name() {
        static std::string name = std::string(util::type_name<derived>()).erase(0, 3);

        return name;
    }

    std::string who() const override {
        return this->hook_name();
    }

    void* get_original_ptr() const override {
        if (this->m_state == e_hook_state::enabled)
            return (void*)m_original;

        return nullptr;
    }

    void* get_detour_ptr() const override {
        if (this->m_state == e_hook_state::enabled)
            return (void*)&derived::detour;

        return nullptr;
    }

    e_hook_state get_state() override {
        return this->m_state;
    }

    void set_state(e_hook_state next) override {
        this->m_state = next;
    }
};

#undef HK_MGR_MSG
#undef HK_MGR_WRN
#undef HK_MGR_ERR
#undef HK_MGR_DBG