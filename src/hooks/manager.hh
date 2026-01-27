#pragma once

#include <vector>
#include <string>
#include <minhook.h>

#include "../singleton.hh"

class i_hook {
    
public:
    virtual std::string who() const = 0;

    virtual void install()   = 0;
    virtual void uninstall() = 0;

    virtual void* get_original_ptr() const = 0;
    virtual void* get_detour_ptr()   const = 0;
};

class s_hook_manager : public c_singleton<s_hook_manager> {

    std::vector<i_hook*> m_hooks;

public:

    void register_hook(i_hook* hook);
    
    i_hook* get_hook(const std::string &name);

    template <typename fn_signature>
    fn_signature get_original(const std::string& name) {
        if (i_hook* hook = get_hook(name)) {
            return (fn_signature)hook->get_original_ptr();
        }

        return nullptr;
    }

    template <typename fn_signature>
    fn_signature get_detour(const std::string& name) {
        if (i_hook* hook = get_hook(name)) {
            return (fn_signature)hook->get_detour_ptr();
        }
        
        return nullptr;
    }

    void install_all();
    void uninstall_all();
};