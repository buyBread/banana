#include "banana/core.hh"
#include "banana/hooks/base.hh"
#include "banana/hooks/manager.hh"
#include "util/macros/debug.hh"

#define MGR_MSG(str, ...) banana::log.msg("hook_manager: {}", std::format(str, __VA_ARGS__))
#define MGR_WRN(str, ...) banana::log.wrn("hook_manager: {}", std::format(str, __VA_ARGS__))
#define MGR_ERR(str, ...) banana::log.err("hook_manager: {}", std::format(str, __VA_ARGS__))
#define MGR_DBG(str, ...) banana::log.dbg("hook_manager: {}", std::format(str, __VA_ARGS__))

void s_hook_manager::register_hook(i_hook* hook, const std::string &category) {
    this->m_hooks[category].push_back(hook);
}

void s_hook_manager::install(const std::string &category) {    
    this->init_minhook();

    auto hooks = this->get_category(category);

    if (hooks.empty())
        return;

    MGR_DBG("installing {} hooks for category \"{}\"", hooks.size(), category);

    this->attempt_queue_enable(category, hooks);
}

void s_hook_manager::uninstall(const std::string &category) {
    auto hooks = this->get_category(category);

    if (hooks.empty())
        return;

    MGR_DBG("uninstalling {} hooks ({})", hooks.size(), category);

    /*
        using minhook's queue for uninstallation would need re-architecting.
        for what gain? none, i think.
    */

    for (const auto &hk : hooks)
        hk->uninstall();

    MGR_MSG("uninstalled {} hooks ({})", hooks.size(), category);
}

void s_hook_manager::uninstall(const std::string &category, const std::string &name) {
    auto hk = this->get_hook(category, name);

    if (!hk)
        return;

    hk->uninstall();

    MGR_MSG("uninstalled hook \"{}\" in category \"{}\"", name, category);
}

void s_hook_manager::shutdown() {
    for (const auto &[hk_category, _] : this->m_hooks) {
        this->uninstall(hk_category);
    }
}

void s_hook_manager::enable_hook(const std::string &category, const std::string &name) {
    this->init_minhook();

    auto hk = this->get_hook(category, name);

    if (!hk)
        return;

    if (hk->enable())
        MGR_MSG("enabled hook \"{}\" in category \"{}\"", name, category);
}

void s_hook_manager::enable_hook_category(const std::string &category) {
    this->init_minhook();

    auto hooks = this->get_category(category);

    if (hooks.empty())
        return;
    
    MGR_DBG("enabling category \"{}\"", category);

    attempt_queue_enable(category, hooks);
}

void s_hook_manager::enable_hook_all() {
    for (const auto &[hk_category, _] : this->m_hooks)
        enable_hook_category(hk_category);
}

void s_hook_manager::disable_hook(const std::string &category, const std::string &name) {
    auto hk = this->get_hook(category, name);

    if (!hk)
        return;

    hk->disable();

    MGR_MSG("disabled hook \"{}\" in category \"{}\"", name, category);
}

void s_hook_manager::disable_hook_category(const std::string &category) {
    auto hooks = this->get_category(category);

    if (hooks.empty())
        return;

    MGR_DBG("disabling category \"{}\"", category);

    attempt_queue_disable(category, hooks);
}

void s_hook_manager::disable_hook_all() {
    for (const auto &[hk_category, _] : this->m_hooks)
        disable_hook_category(hk_category);
}

bool s_hook_manager::is_category_enabled(const std::string &category) {    
    auto hooks = this->get_category(category);

    if (hooks.empty())
        return false;

    for (const auto &hk : hooks)
        if (hk->get_state() != e_hook_state::enabled)
            return false;

    return true;
}

bool s_hook_manager::is_hook_enabled(const std::string &category, const std::string &name) {
    auto hk = this->get_hook(category, name);

    if (!hk)
        return false;

    return hk->get_state() == e_hook_state::enabled;
}

void* s_hook_manager::get_hook_detour_ptr(const std::string &category, const std::string &name) {
    auto hk = this->get_hook(category, name);

    if (!hk)
        return nullptr;

    return hk->get_detour_ptr();
}

void* s_hook_manager::get_hook_original_ptr(const std::string &category, const std::string &name) {
    auto hk = this->get_hook(category, name);

    if (!hk)
        return nullptr;

    return hk->get_original_ptr();
}

i_hook* s_hook_manager::get_hook(const std::string &category, const std::string &name) {
    auto hooks = this->get_category(category);

    if (hooks.empty())
        return nullptr;

    for (const auto &hk : hooks)
        if (hk->who() == name)
            return hk;

    MGR_WRN("hook \"{}\" in category \"{}\" doesn't exist", name, category);

    return nullptr;
}

std::vector<i_hook*> s_hook_manager::get_category(const std::string &category) {
    std::lock_guard<std::mutex> lock(this->m_manager_mutex);
    
    const auto it = m_hooks.find(category);

    if (it == m_hooks.end()) {
        if (category != HK_DEFAULT_CATEGORY)
            MGR_WRN("category \"{}\" doesn't exist", category);

        return {};
    }

    return it->second;
}

void s_hook_manager::attempt_queue_enable(const std::string &category, const std::vector<i_hook*> &hooks) {
    auto queued = std::vector<i_hook*>{};

    for (const auto &hk : hooks) {
        if (hk->get_state() == e_hook_state::enabled)
            continue; // because install() will print out a bogus warning

        hk->install();

        if (hk->get_state() != e_hook_state::queued_for_enable) {
            if (category == HK_DEFAULT_CATEGORY) // we don't care to validate generic hooks
                continue;

            MGR_DBG("failed to queue in category \"{}\", uninstalling it", category);

            for (const auto &queued_hk : queued)
                queued_hk->uninstall();
            
            return;
        }

        queued.push_back(hk);
    }

    auto apply_status = MH_ApplyQueued();

    if (apply_status != MH_OK) {
        MGR_DBG("enabling using ApplyQueue failed for category \"{}\", uninstalling it ({})", category, MH_STRING(apply_status));

        for (const auto &hk : queued)
            hk->uninstall();

        return;
    }

    MGR_MSG("enabled category \"{}\"", category);

    for (const auto &hk : queued)
        hk->set_state(e_hook_state::enabled);
}

void s_hook_manager::attempt_queue_disable(const std::string &category, const std::vector<i_hook*> &hooks) {
    /*
        since the loop will literally headshot the process if queueing for disable fails,
        we don't need to make a `queued` vector for post-disabling state switching.
        in fact, we don't need to check for anything, which is amazing, because i hate state management.
    */
    for (const auto &hk : hooks)
        hk->queue_disable();

    auto apply_status = MH_ApplyQueued();

    if (apply_status != MH_OK) {
        MGR_DBG("disabling using ApplyQueue failed for category \"{}\" ({})", category, MH_STRING(apply_status));

        FATAL_BREAKPOINT(); /* we never know when PSR B1919+21 will snipe our process ID specifically
                               ...to be more precise, there's no reason this should happen */

        return;
    }

    MGR_MSG("disabled category \"{}\"", category);

    for (const auto &hk : hooks)
        if (hk->get_state() != e_hook_state::absent) // some unicorn can be absent, so don't desync state for it
            hk->set_state(e_hook_state::created);
}

void s_hook_manager::init_minhook() {
    static std::once_flag init_once;

    std::call_once(init_once, []{
        MH_STATUS status = MH_Initialize();

        if (status != MH_OK) {
            MGR_ERR("failed to initialize minhook ({})", MH_STRING(status));

            FATAL_BREAKPOINT();
        }

        MGR_MSG("MinHook initialized");
    });
}