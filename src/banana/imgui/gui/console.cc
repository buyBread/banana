#include <algorithm>
#include <cstdio>
#include <format>
#include <string>
#include <imgui.h>

#include "banana/util/types.hh"
#include "banana/imgui/gui/console.hh"

namespace banana { namespace imgui { namespace gui {
    namespace {
        bool contains_insensitive(std::string_view text, std::string_view needle) {
            if (needle.empty())
                return true;

            const auto lower = [](uchar c) {
                return static_cast<uchar>((c >= 'A' && c <= 'Z') ? (c - 'A' + 'a') : c);
            };

            return std::search(text.begin(),
                            text.end(),
                            needle.begin(),
                            needle.end(),
                            [&](char a, char b) {return lower(a) == lower(b); }) != text.end();
        }

        ImVec4 level_color(log_level level) {
            switch (level) {
                case log_level::msg: return ImVec4( .80f, .83f, .87f, 1.0f);
                case log_level::wrn: return ImVec4(1.00f, .77f, .30f, 1.0f);
                case log_level::err: return ImVec4(1.00f, .43f, .50f, 1.0f);
                case log_level::dbg: return ImVec4( .52f, .65f, .75f, 1.0f);
            }
        }

        std::string_view level_label(log_level level) {
            switch (level) {
                case log_level::msg: return "MSG";
                case log_level::wrn: return "WRN";
                case log_level::err: return "ERR";
                case log_level::dbg: return "DBG";
            }
        }
    }

    console::console() : i_gui("console") {
        m_incoming.reserve(4096);
    }

    bool console::matches(const log_line &line) const {
        bool enabled = false;

        switch (line.level) {
            case log_level::msg: enabled = m_show_msg; break;
            case log_level::wrn: enabled = m_show_wrn; break;
            case log_level::err: enabled = m_show_err; break;
            case log_level::dbg: enabled = m_show_dbg; break;
        }

        return enabled &&
            contains_insensitive(line.category, m_category.data()) &&
            contains_insensitive(line.text,     m_search  .data());
    }

    void console::rebuild_filter() {
        m_visible.clear();

        for (const auto &line : m_lines)
            if (matches(line))
                m_visible.push_back(line.sequence);
    }

    void console::poll() {
        if (m_paused)
            return;

        m_incoming.clear();

        const auto lost = s_logging_manager::get().feed().read_since(m_cursor, m_incoming, 4096);

        m_missed += lost;

        for (auto &line : m_incoming) {
            if (!m_lines.empty() && line.sequence != m_lines.back().sequence + 1) {
                m_lines  .clear();
                m_visible.clear();
            }

            if (matches(line))
                m_visible.push_back(line.sequence);
            
            m_lines.emplace_back(std::move(line));

            if (m_lines.size() > max_local_lines) {
                m_lines.pop_front();

                const auto first = m_lines.front().sequence;

                while (!m_visible.empty() && m_visible.front() < first)
                    m_visible.pop_front();
            }
        }
    }

    void console::copy_filtered() const {
        std::string text;
        
        for (const auto seq : m_visible) {
            if (m_lines.empty())
                break;

            const auto index = static_cast<u64>(seq - m_lines.front().sequence);

            if (index >= m_lines.size())
                continue;

            const auto& line = m_lines[index];

            text += std::format("{} [{}] [{}] {}\n",
                                line.time,
                                line.category,
                                level_label(line.level),
                                line.text);
        }
        ImGui::SetClipboardText(text.c_str());
    }

    void console::draw() {
        poll();

        bool change = false;
        ImGui::SetNextItemWidth(220.0f);
        change |= ImGui::InputTextWithHint("##search", "search messages", m_search.data(), m_search.size());
        ImGui::SameLine();
        ImGui::SetNextItemWidth(115.0f);
        change |= ImGui::InputTextWithHint("##category", "category", m_category.data(), m_category.size());
        ImGui::SameLine();
        change |= ImGui::Checkbox("MSG", &m_show_msg);
        ImGui::SameLine();
        change |= ImGui::Checkbox("WRN", &m_show_wrn);
        ImGui::SameLine();
        change |= ImGui::Checkbox("ERR", &m_show_err);
        ImGui::SameLine();
        change |= ImGui::Checkbox("DBG", &m_show_dbg);

        if (change) rebuild_filter();

        if (ImGui::Button(m_paused ? "resume" : "pause"))
            m_paused = !m_paused;

        ImGui::SameLine();
        if (ImGui::Button("clear")) {
            m_lines  .clear();
            m_visible.clear();
            m_cursor = s_logging_manager::get().feed().next_sequence();
            m_missed = 0;
        }

        ImGui::SameLine();
        if (ImGui::Button("copy filtered"))
            copy_filtered();

        ImGui::SameLine();
        ImGui::Checkbox("auto-scroll", &m_auto_scroll);

        ImGui::SameLine();
        ImGui::TextDisabled("%zu shown / %zu cached | missed %llu",
            m_visible.size(), m_lines.size(), static_cast<u64>(m_missed));

        ImGui::Separator();
        if (!ImGui::BeginChild("##log_scroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
            ImGui::EndChild();

            return;
        }

        const bool was_at_bottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 4.0f;

        ImGuiListClipper clipper;
        clipper.Begin(static_cast<i32>(m_visible.size()));

        while (clipper.Step()) {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
                if (m_lines.empty())
                    break;

                const auto sequence = m_visible[static_cast<u64>(i)];
                const auto index    = static_cast<u64>(sequence - m_lines.front().sequence);

                if (index >= m_lines.size())
                    continue;

                const auto &line = m_lines[index];
                const auto  text = std::format("{:%H:%M:%S} [{}] | {} :: {}",
                                               line.time,
                                               level_label(line.level),
                                               line.category,
                                               line.text);

                ImGui::PushStyleColor(ImGuiCol_Text, level_color(line.level));
                ImGui::TextUnformatted(text.c_str());
                ImGui::PopStyleColor();
            }
        }

        if (m_auto_scroll && was_at_bottom)
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
    }

    console gui_console {};
}}} //banana::imgui::gui
