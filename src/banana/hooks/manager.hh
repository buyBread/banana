#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <unordered_map>
#include <type_traits>
#include <utility>

#include "banana/logging.hh"
#include "util/singleton.hh"

#define MH_STRING MH_StatusToString

#define HK_DEFAULT_CATEGORY "default"

#define MGR_MSG(str, ...) banana::log.msg("hook_manager: {}", std::format(str, __VA_ARGS__))
#define MGR_WRN(str, ...) banana::log.wrn("hook_manager: {}", std::format(str, __VA_ARGS__))
#define MGR_ERR(str, ...) banana::log.err("hook_manager: {}", std::format(str, __VA_ARGS__))
#define MGR_DBG(str, ...) banana::log.dbg("hook_manager: {}", std::format(str, __VA_ARGS__))

enum class e_hook_state {
    absent,
    created,
    queued_for_enable,
    queued_for_disable,
    enabled,
};

class i_hook {
    
public:
    virtual ~i_hook() = default;

    virtual void install()   = 0; // if hook queueing fails, the hook will be removed
    virtual void uninstall() = 0;

    virtual bool enable()        = 0;
    virtual bool disable()       = 0;
    virtual bool queue_enable()  = 0;
    virtual bool queue_disable() = 0;

    virtual std::string who() const = 0;

    virtual void* get_original_ptr() const = 0;
    virtual void* get_detour_ptr()   const = 0;

    virtual void clean_up() {} // optionally implemented in individual hooks;
                               // called on disable() BEFORE MH_DisableHook;
                               // it will not be called on queue_disable().

    virtual e_hook_state get_state() = 0;
    virtual void         set_state(e_hook_state) = 0;
};

class s_hook_manager : public util::singleton<s_hook_manager> {

    std::unordered_map<
        std::string,
        std::vector<i_hook*>> m_hooks;

    i_hook*              get_hook(const std::string &category, const std::string &name);
    std::vector<i_hook*> get_category(const std::string &category);

    /*
        no reason to keep them public
    */
    void* get_hook_detour_ptr(const std::string &category, const std::string &name);
    void* get_hook_original_ptr(const std::string &category, const std::string &name);

    void attempt_queue_enable(const std::string &category, const std::vector<i_hook*> &hooks);
    void attempt_queue_disable(const std::string &category, const std::vector<i_hook*> &hooks);

    std::mutex m_manager_mutex;

    void init_minhook();

public:

    void register_hook(i_hook* hook, const std::string &category = HK_DEFAULT_CATEGORY); // hooks auto-register themselves

    void install(const std::string &category = HK_DEFAULT_CATEGORY);
    // void install(const std::string &category, const std::string &name);
    // void install(const std::vector<i_hook*> &hooks);
    void uninstall(const std::string &category);
    void uninstall(const std::string &category, const std::string &name);
    // void uninstall(const std::vector<i_hook*> &hooks);
    void shutdown();
    void enable_hook(const std::string &category, const std::string &name);
    void enable_hook_category(const std::string &category);
    void enable_hook_all();
    void disable_hook(const std::string &category, const std::string &name);
    void disable_hook_category(const std::string &category);
    void disable_hook_all();

    bool is_category_enabled(const std::string &category);
    bool is_hook_enabled(const std::string &category, const std::string &name);

    // the year 2041, C++41 lets programmers implement template methods in source files

    template <typename fn>
    fn acquire_original(const std::string &category, const std::string &name) {
        static_assert(std::is_pointer_v<fn> && std::is_function_v<std::remove_pointer_t<fn>>,
            "acquire_original: fn must be a function pointer type (matching the hook's calling convention)");

        void* ptr = this->get_hook_original_ptr(category, name);

        if (!ptr)
            MGR_ERR("can't acquire original for {} ({}), pointer is nullptr", name, category);

        return ((fn)ptr);
    }

    template <typename fn>
    fn acquire_detour(const std::string &category, const std::string &name) {
        static_assert(std::is_pointer_v<fn> && std::is_function_v<std::remove_pointer_t<fn>>,
            "acquire_detour: fn must be a function pointer type (matching the hook's calling convention)");

        void* ptr = this->get_hook_detour_ptr(category, name);

        if (!ptr)
            MGR_ERR("can't acquire detour for {} ({}), pointer is nullptr", name, category);

        return ((fn)ptr);
    }
};

namespace banana {
    inline auto &hook_manager = s_hook_manager::get();
} // banana

#undef MGR_MSG
#undef MGR_WRN
#undef MGR_ERR
#undef MGR_DBG