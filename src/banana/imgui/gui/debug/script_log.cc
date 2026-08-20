/*
    VIBECODED: either refactor or replace
*/

#include <imgui.h>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <string>
#include <unordered_map>

#include "banana/imgui/manager.hh"
#include "banana/logging.hh"
#include "treyarch/chuck/chuck.hh"

using namespace banana;

class gui_script_load_log final : public i_gui {
    using clock = std::chrono::steady_clock;

    struct script_info {
        uintptr_t   address       = 0;
        uint64_t    instance_id   = 0;
        uint32_t    resource_hash = 0;
        std::string name;
    };

    struct log_entry {
        bool              loaded = false;
        uint64_t          instance_id = 0;
        uintptr_t         address = 0;
        std::string       name;
        clock::time_point born;
    };

    // This is intentionally not a GUI. It is just debug text emitted into the
    // foreground draw list every frame from logic().
    static constexpr size_t max_visible_entries = 25;
    static constexpr float  hold_time            = 5.0f;
    static constexpr float  time_fade_duration   = 1.5f;
    static constexpr float  background_alpha     = 0.77f;

    static constexpr uint32_t max_executables = 4096;
    static constexpr uint32_t max_name_length = 192;

    std::unordered_map<uintptr_t, script_info> m_previous;
    std::unordered_map<uintptr_t, script_info> m_current;
    std::deque<log_entry>                      m_log;
    uint64_t                                   m_next_instance_id = 1;
    bool                                       m_have_baseline = false;

    static std::string executable_name(const treyarch::chuck::vm::script_executable* executable) {
        if (!executable->name || !executable->name_length)
            return "<unnamed>";

        const uint32_t length = (std::min)(executable->name_length, max_name_length);
        std::string result(executable->name, executable->name + length);

        while (!result.empty() && result.back() == '\0')
            result.pop_back();

        for (char& c : result) {
            if (c == '\r' || c == '\n' || c == '\t')
                c = ' ';
        }

        return result.empty() ? "<unnamed>" : result;
    }

    static bool same_executable(const script_info& a, const script_info& b) {
        return a.resource_hash == b.resource_hash && a.name == b.name;
    }

    bool snapshot(std::unordered_map<uintptr_t, script_info>& out) {
        auto* manager = treyarch::chuck::vm::references::script_manager.read();
        if (!manager)
            return false;

        treyarch::engine_lock_scope lock(&manager->loaded_executables_lock);

        auto* tree = manager->loaded_executables;
        if (!tree || !tree->head || tree->count > max_executables)
            return false;

        out.clear();
        out.reserve(tree->count);

        auto* const end = tree->end();
        auto* node = tree->begin();

        for (uint32_t i = 0; node != end && i < tree->count; ++i, node = node->successor()) {
            if (!node || !node->entry)
                break;

            auto* executable = node->get();
            if (!executable)
                continue;

            const uintptr_t address = reinterpret_cast<uintptr_t>(executable);

            out[address] = {
                address,
                0,
                executable->resource_hash,
                executable_name(executable)
            };
        }

        return true;
    }

    void add_entry(bool loaded, const script_info& script, clock::time_point now) {
        m_log.push_back({
            loaded,
            script.instance_id,
            script.address,
            script.name,
            now
        });

        // Anything beyond the spatial fade range is invisible anyway.
        while (m_log.size() > max_visible_entries)
            m_log.pop_front();
    }

    uint64_t allocate_instance_id() {
        return m_next_instance_id++;
    }

    void establish_baseline() {
        for (auto& [address, script] : m_current)
            script.instance_id = allocate_instance_id();
    }

    void diff(clock::time_point now) {
        // First carry forward identities for objects that are still the same
        // executable. If an address was reused for different executable metadata,
        // the new object deliberately gets a fresh debug ID.
        for (auto& [address, new_script] : m_current) {
            const auto previous = m_previous.find(address);

            if (previous != m_previous.end() && same_executable(previous->second, new_script))
                new_script.instance_id = previous->second.instance_id;
            else
                new_script.instance_id = allocate_instance_id();
        }

        // Unloads first so pointer reuse reads naturally as:
        //   - #41 old_script @ 0x...
        //   + #57 new_script @ 0x...
        for (const auto& [address, old_script] : m_previous) {
            const auto current = m_current.find(address);

            if (current == m_current.end() || !same_executable(old_script, current->second))
                add_entry(false, old_script, now);
        }

        for (const auto& [address, new_script] : m_current) {
            const auto previous = m_previous.find(address);

            if (previous == m_previous.end() || !same_executable(new_script, previous->second))
                add_entry(true, new_script, now);
        }
    }

    static float time_alpha(const log_entry& entry, clock::time_point now) {
        const float age = std::chrono::duration<float>(now - entry.born).count();

        if (age <= hold_time)
            return 1.0f;

        return std::clamp(
            1.0f - ((age - hold_time) / time_fade_duration),
            0.0f,
            1.0f);
    }

    static float stack_alpha(size_t index_from_bottom) {
        // Newest entry: 1.0. Oldest slot in the 25-line stack: 0.0.
        if constexpr (max_visible_entries <= 1)
            return 1.0f;

        const float t = static_cast<float>(index_from_bottom) /
                        static_cast<float>(max_visible_entries - 1);

        return 1.0f - std::clamp(t, 0.0f, 1.0f);
    }

    void expire(clock::time_point now) {
        const float lifetime = hold_time + time_fade_duration;

        while (!m_log.empty()) {
            const float age = std::chrono::duration<float>(now - m_log.front().born).count();
            if (age < lifetime)
                break;

            m_log.pop_front();
        }
    }

    void render_overlay(clock::time_point now) {
        if (m_log.empty())
            return;

        ImDrawList* draw = ImGui::GetForegroundDrawList();
        const ImGuiIO& io = ImGui::GetIO();
        const float font_size = 16.0f;
        ImFont* font = ImGui::GetFont();

        const float line_height = font_size;

        // Literally starts at framebuffer x=0 and grows upward from framebuffer
        // y=DisplaySize.y. There is no ImGui window and therefore no window padding.
        float y = io.DisplaySize.y - line_height;
        size_t index_from_bottom = 0;

        for (auto it = m_log.rbegin(); it != m_log.rend(); ++it, ++index_from_bottom) {
            const float alpha = time_alpha(*it, now) * stack_alpha(index_from_bottom);
            if (alpha <= 0.0f)
                continue;

            char text[384] {};

            std::snprintf(
                text,
                sizeof(text),
                "%c #%llu %s @ %p",
                it->loaded ? '+' : '-',
                static_cast<unsigned long long>(it->instance_id),
                it->name.c_str(),
                reinterpret_cast<void*>(it->address));

            const ImVec2 text_size = font->CalcTextSizeA(
                font_size,
                FLT_MAX,
                0.0f,
                text);

            draw->AddRectFilled(
                ImVec2(0.0f, y),
                ImVec2(text_size.x, y + line_height),
                IM_COL32(0, 0, 0, static_cast<int>(255.0f * background_alpha * alpha)));

            const ImU32 text_color = it->loaded
                ? IM_COL32(120, 255, 140, static_cast<int>(255.0f * alpha))
                : IM_COL32(255, 120, 120, static_cast<int>(255.0f * alpha));

            draw->AddText(
                font,
                font_size,
                ImVec2(0.0f, y),
                text_color,
                text);

            y -= line_height;
            if (y + line_height <= 0.0f)
                break;
        }
    }

public:
    bool logic() override {
        const auto now = clock::now();

        if (snapshot(m_current)) {
            if (!m_have_baseline) {
                // Give already-live executables identities so their eventual unloads
                // are still distinguishable, but don't spam them as fresh loads.
                establish_baseline();
                m_previous.swap(m_current);
                m_current.clear();
                m_have_baseline = true;
            }
            else {
                diff(now);
                m_previous.swap(m_current);
                m_current.clear();
            }
        }

        expire(now);
        render_overlay(now);

        // draw() is intentionally bypassed. This object is an always-on DEBUG
        // overlay, not a toggleable ImGui window.
        return false;
    }

    void draw() override {}
};

#ifdef DEBUG
gui_script_load_log script_load_log {};
#endif
