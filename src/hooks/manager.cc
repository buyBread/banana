#include "banana.hh"
#include "base.hh"
#include "manager.hh"

void s_hook_manager::register_hook(i_hook* hook) {
    m_hooks.push_back(hook);
}

i_hook* s_hook_manager::get_hook(const std::string &name) {
    for (auto* hook : m_hooks) {
        if (hook->who() == name)
            return hook;
    }

    return nullptr;
}

void s_hook_manager::install_all() {
    banana::safe.wait(false);

    LOG.msg("hook_manager: installing {} hooks", m_hooks.size());

    for (auto &hook : m_hooks)
        hook->install();
}

void s_hook_manager::uninstall_all() {
    LOG.msg("hook_manager: uninstalling {} hooks", m_hooks.size());

    for (auto &hook : m_hooks)
        hook->uninstall();
}