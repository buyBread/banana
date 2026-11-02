#pragma once

#include "banana/util/types.hh"
#include "banana/util/singleton.hh"

#include <string>
#include <string_view>
#include <vector>

class i_gui;

class s_gui_manager : public util::singleton<s_gui_manager> {

public:
    void register_GUI(i_gui* gui);
    void unregister_GUI(i_gui* gui) noexcept;

    void run();

private:
    std::vector<i_gui*> m_GUIs;
    
    u64 m_next_id = 1;

    void draw_top_bar();
};

class i_gui {
    friend class s_gui_manager;

    std::string m_title;
    u64         m_id = 0;

    bool m_open;
    bool m_minimized = false;
    bool m_focus     = true;

    bool m_logic_visible = true;

public:
    explicit i_gui(std::string title, bool initially_open = false);
    
    virtual ~i_gui();

    // optional non-ImGui work;
    // called every run(), including while hidden
    virtual bool logic() {
        return true;
    }

    virtual void draw() = 0;

    std::string_view title() const noexcept { return m_title; }
    
    bool is_open()      const noexcept { return m_open; }
    bool is_minimized() const noexcept { return m_minimized; }

    void open() noexcept {
        m_open      = true;
        m_minimized = false;
        m_focus     = true;
    }
    
    void close() noexcept {
        m_open      = false;
        m_minimized = false;
    }

    void minimize() noexcept {
        if (m_open)
            m_minimized = true;
    }

    void restore() noexcept {
        if (m_open) {
            m_minimized = false;
            m_focus     = true;
        }
    }
};

namespace banana::imgui {
    inline s_gui_manager& gui_manager = s_gui_manager::get();
}
