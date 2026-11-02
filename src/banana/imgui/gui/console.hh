#pragma once

#include <array>
#include <deque>
#include <string_view>
#include <vector>

#include "banana/util/types.hh"
#include "banana/imgui/manager.hh"
#include "banana/logging.hh"

namespace banana { namespace imgui { namespace gui {
    class console : public i_gui {

    public:
        console();
        
        void draw() override;

    private:

        static constexpr u64 max_local_lines = 12000;
        
        u64 m_cursor = 0;
        u64 m_missed = 0;
        std::deque<log_line> m_lines;
        std::deque<u64>      m_visible;

        std::vector<log_line> m_incoming;
        std::array<char, 160> m_search{};
        std::array<char, 80>  m_category{};

        bool m_show_msg    = true;
        bool m_show_wrn    = true;
        bool m_show_err    = true;
        bool m_show_dbg    = true;
        bool m_auto_scroll = true;
        bool m_paused      = false;

        void poll();
        void rebuild_filter();
        bool matches(const log_line& line) const;
        void copy_filtered() const;
    };
}}} // banana::imgui::gui
