#include <algorithm>

#include "node_graph.hh"
#include "imgui/imgui.hh"

namespace banana { namespace imgui {
namespace ext { namespace node_graph {
    constexpr auto minimum_zoom       = 0.005f;
    constexpr auto maximum_zoom       = 2.00f;
    constexpr auto grid_size          = 64.0f;
    constexpr auto invalid_node_index = (size_t)-1;

    uint64_t graph_revision = 0;

    inline uint64_t next_graph_revision() {
        return ++graph_revision;
    }

    inline ImVec2 add(const ImVec2 &left, const ImVec2 &right) {
        return ImVec2(left.x + right.x, left.y + right.y);
    }

    inline ImVec2 subtract(const ImVec2 &left, const ImVec2 &right) {
        return ImVec2(left.x - right.x, left.y - right.y);
    }

    inline ImVec2 multiply(const ImVec2 &value, float scale) {
        return ImVec2(value.x * scale, value.y * scale);
    }

    inline bool point_in_rectangle(const ImVec2& point, const ImVec2 &minimum, const ImVec2 &maximum) {
        return
            point.x >= minimum.x && point.y >= minimum.y &&
            point.x <= maximum.x && point.y <= maximum.y;
    }

    inline ImVec2 rectangle_edge(const ImVec2 &minimum, const ImVec2 &maximum, const ImVec2 &toward) {
        ImVec2 center(
            (minimum.x + maximum.x) * 0.5f,
            (minimum.y + maximum.y) * 0.5f);

        ImVec2 delta       = subtract(toward, center);
        float  half_width  = (maximum.x - minimum.x) * 0.5f;
        float  half_height = (maximum.y - minimum.y) * 0.5f;
        float  horizontal  = half_width > 0.0f ? std::abs(delta.x) / half_width : 0.0f;
        float  vertical    = half_height > 0.0f ? std::abs(delta.y) / half_height : 0.0f;
        float  divisor     = std::max(horizontal, vertical);

        if (divisor <= 0.0f)
            return center;

        return add(center, multiply(delta, 1.0f / divisor));
    }

    inline void connection_controls(const ImVec2 &source, const ImVec2 &target, ImVec2* source_control, ImVec2* target_control) {
        ImVec2 delta = subtract(target, source);

        if (std::abs(delta.x) >= std::abs(delta.y)) {
            float direction = delta.x < 0.0f ? -1.0f : 1.0f;
            float curve = std::clamp(std::abs(delta.x) * 0.5f, 40.0f, 220.0f);

            *source_control = ImVec2(source.x + curve * direction, source.y);
            *target_control = ImVec2(target.x - curve * direction, target.y);
        } else {
            float direction = delta.y < 0.0f ? -1.0f : 1.0f;
            float curve = std::clamp(std::abs(delta.y) * 0.5f, 40.0f, 220.0f);

            *source_control = ImVec2(source.x, source.y + curve * direction);
            *target_control = ImVec2(target.x, target.y - curve * direction);
        }
    }

    inline void draw_arrow(ImDrawList* draw_list, const ImVec2 &tip, const ImVec2 &preceding_control, ImU32 color, float scale) {
        ImVec2 direction = subtract(tip, preceding_control);
            
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length <= 0.001f)
            return;

        direction = multiply(direction, 1.0f / length);

        ImVec2 normal(-direction.y, direction.x);
        float  arrow_length = 11.0f * scale;
        float  arrow_width  = 5.0f * scale;
        ImVec2 base         = subtract(tip, multiply(direction, arrow_length));

        draw_list->AddTriangleFilled(
            tip,
            add(base, multiply(normal, arrow_width)),
            subtract(base, multiply(normal, arrow_width)),
            color);
    }

    c_node_style::c_node_style()
        : title_color(IM_COL32(54, 60, 72, 255)),
          body_color(IM_COL32(38, 42, 51, 255)),
          border_color(IM_COL32(89, 98, 117, 255)),
          selected_border_color(IM_COL32(255, 204, 92, 255)),
          title_text_color(IM_COL32(244, 246, 250, 255)),
          body_text_color(IM_COL32(199, 205, 216, 255)),
          rounding(6.0f),
          border_thickness(1.5f) {}

    c_graph::c_graph()
        : m_revision(next_graph_revision()) {}

    c_node& c_graph::add_node(node_id id, ImVec2 position, ImVec2 size, const std::string &title, const c_node_style &node_style) {
        auto existing = m_node_indices.find(id);

        if (existing != m_node_indices.end()) {
            c_node &node = m_nodes[existing->second];

            node.position = position;
            node.size = size;
            node.title = title;
            node.rows.clear();
            node.style = node_style;
            m_revision = next_graph_revision();

            return node;
        }

        size_t index = m_nodes.size();
        m_nodes.push_back({ id, position, size, title, {}, node_style });
        m_node_indices[id] = index;
        m_revision = next_graph_revision();

        return m_nodes.back();
    }

    c_connection &c_graph::add_connection(node_id source, node_id target, ImU32 color, float thickness, bool directed, const std::string &label) {
        m_connections.push_back({
            source,
            target,
            color,
            thickness,
            directed,
            label
        });

        m_revision = next_graph_revision();

        return m_connections.back();
    }

    c_node* c_graph::find_node(node_id id) {
        auto entry = m_node_indices.find(id);

        return entry == m_node_indices.end() ? nullptr : &m_nodes[entry->second];
    }

    const c_node* c_graph::find_node(node_id id) const {
        auto entry = m_node_indices.find(id);

        return entry == m_node_indices.end() ? nullptr : &m_nodes[entry->second];
    }

    const std::vector<c_node> &c_graph::nodes() const {
        return m_nodes;
    }

    const std::vector<c_connection> &c_graph::connections() const {
        return m_connections;
    }

    bool c_graph::empty() const {
        return m_nodes.empty();
    }

    uint64_t c_graph::revision() const {
        return m_revision;
    }

    c_view::c_view()
        : m_pan(0.0f, 0.0f),
          m_drag_offset(0.0f, 0.0f),
          m_zoom(1.0f),
          m_selected_node(0),
          m_dragged_node(0),
          m_double_clicked_node(0),
          m_focus_requested_node(0),
          m_dragged_node_index(invalid_node_index),
          m_synchronized_graph(nullptr),
          m_synchronized_revision(0),
          m_frame_requested(true) {}

    ImVec2 c_view::graph_to_screen(const ImVec2 &point, const ImVec2 &canvas_position) const {
        return add(add(canvas_position, m_pan), multiply(point, m_zoom));
    }

    ImVec2 c_view::screen_to_graph(const ImVec2 &point, const ImVec2 &canvas_position) const {
        return multiply(
            subtract(subtract(point, canvas_position), m_pan),
            1.0f / m_zoom);
    }

    void c_view::synchronize_positions(const c_graph &graph) {
        if (m_synchronized_graph    == &graph &&
            m_synchronized_revision == graph.revision())

            return;

        m_node_positions.resize(graph.nodes().size());
        m_node_visibility.assign(graph.nodes().size(), 0);
        m_visible_nodes.clear();

        for (size_t index = 0; index < graph.nodes().size(); ++index) {
            const c_node &node = graph.nodes()[index];
            auto position = m_positions.find(node.id);

            if (position == m_positions.end() || !m_manual_positions.contains(node.id)) {
                m_positions[node.id] = node.position;
                m_node_positions[index] = node.position;
            } else {
                m_node_positions[index] = position->second;
            }
        }

        for (auto position = m_positions.begin(); position != m_positions.end();) {
            if (!graph.find_node(position->first))
                position = m_positions.erase(position);
            else
                ++position;
        }

        for (auto position = m_manual_positions.begin();
             position != m_manual_positions.end();) {

            if (!graph.find_node(*position))
                position = m_manual_positions.erase(position);
            else
                ++position;
        }

        if (m_selected_node && !graph.find_node(m_selected_node))
            m_selected_node = 0;

        if (m_dragged_node && !graph.find_node(m_dragged_node)) {
            m_dragged_node = 0;
            m_dragged_node_index = invalid_node_index;
        }

        m_connection_indices.resize(graph.connections().size());

        for (size_t index = 0; index < graph.connections().size(); ++index) {
            const c_connection &connection = graph.connections()[index];
            const c_node* source = graph.find_node(connection.source);
            const c_node* target = graph.find_node(connection.target);

            m_connection_indices[index].source = source ?
                (size_t)(source - graph.nodes().data()) : invalid_node_index;
            m_connection_indices[index].target = target ?
                (size_t)(target - graph.nodes().data()) : invalid_node_index;
        }

        m_synchronized_graph    = &graph;
        m_synchronized_revision = graph.revision();
    }

    void c_view::frame_graph(const c_graph &graph, const ImVec2 &canvas_size) {
        if (graph.empty()) {
            m_pan  = multiply(canvas_size, 0.5f);
            m_zoom = 1.0f;
            
            return;
        }

        ImVec2 minimum(
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max());
        ImVec2 maximum(
            -std::numeric_limits<float>::max(),
            -std::numeric_limits<float>::max());

        for (size_t index = 0; index < graph.nodes().size(); ++index) {
            const c_node &node = graph.nodes()[index];
            ImVec2 position = m_node_positions[index];

            minimum.x = std::min(minimum.x, position.x);
            minimum.y = std::min(minimum.y, position.y);
            maximum.x = std::max(maximum.x, position.x + node.size.x);
            maximum.y = std::max(maximum.y, position.y + node.size.y);
        }

        ImVec2 graph_size = subtract(maximum, minimum);
        float horizontal_zoom = (canvas_size.x - 80.0f) / std::max(graph_size.x, 1.0f);
        float vertical_zoom   = (canvas_size.y - 80.0f) / std::max(graph_size.y, 1.0f);

        m_zoom = std::clamp(
            std::min(horizontal_zoom, vertical_zoom),
            minimum_zoom,
            1.25f);

        ImVec2 center = multiply(add(minimum, maximum), 0.5f);
        m_pan = subtract(multiply(canvas_size, 0.5f), multiply(center, m_zoom));
    }

    void c_view::draw(const char* identifier, const c_graph &graph, ImVec2 size) {
        synchronize_positions(graph);

        ImVec2 available = ImGui::GetContentRegionAvail();

        if (size.x <= 0.0f)
            size.x = available.x;

        if (size.y <= 0.0f)
            size.y = available.y;

        size.x = std::max(size.x, 64.0f);
        size.y = std::max(size.y, 64.0f);

        ImVec2 canvas_position = ImGui::GetCursorScreenPos();
        ImVec2 canvas_end      = add(canvas_position, size);

        ImGui::InvisibleButton(identifier, size);
        
        bool   canvas_hovered = ImGui::IsItemHovered();
        ImVec2 mouse_position = imgui::io->MousePos;

        if (m_frame_requested) {
            frame_graph(graph, size);
            m_frame_requested = false;
        }

        if (m_focus_requested_node) {
            const c_node* focused = graph.find_node(m_focus_requested_node);

            if (focused) {
                size_t focused_index =
                    (size_t)(focused - graph.nodes().data());
                ImVec2 focused_center = add(
                    m_node_positions[focused_index],
                    multiply(focused->size, 0.5f));

                m_zoom = (std::max)(m_zoom, 0.65f);
                m_pan = subtract(
                    multiply(size, 0.5f),
                    multiply(focused_center, m_zoom));
            }

            m_focus_requested_node = 0;
        }

        if (canvas_hovered && imgui::io->MouseWheel != 0.0f) {
            ImVec2 graph_cursor = screen_to_graph(mouse_position, canvas_position);
            float previous_zoom = m_zoom;

            m_zoom = std::clamp(
                m_zoom * std::pow(1.15f, imgui::io->MouseWheel),
                minimum_zoom,
                maximum_zoom);

            ImVec2 cursor_in_canvas = subtract(mouse_position, canvas_position);
            m_pan = subtract(cursor_in_canvas, multiply(graph_cursor, m_zoom));

            if (previous_zoom != m_zoom) {
                m_dragged_node = 0;
                m_dragged_node_index = invalid_node_index;
            }
        }

        if (canvas_hovered && !m_dragged_node &&
            (ImGui::IsMouseDragging(ImGuiMouseButton_Middle, 0.0f) || ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.0f)))

            m_pan = add(m_pan, imgui::io->MouseDelta);

        ImVec2 viewport_start  = screen_to_graph(canvas_position, canvas_position);
        ImVec2 viewport_end    = screen_to_graph(canvas_end,      canvas_position);
        float  viewport_margin = 80.0f / m_zoom;
        ImVec2 viewport_minimum(
            (std::min)(viewport_start.x, viewport_end.x) - viewport_margin,
            (std::min)(viewport_start.y, viewport_end.y) - viewport_margin);
        ImVec2 viewport_maximum(
            (std::max)(viewport_start.x, viewport_end.x) + viewport_margin,
            (std::max)(viewport_start.y, viewport_end.y) + viewport_margin);

        std::fill(
            m_node_visibility.begin(),
            m_node_visibility.end(),
            (uint8_t)0);

        m_visible_nodes.clear();

        for (size_t index = 0; index < graph.nodes().size(); ++index) {
            const c_node &node     = graph.nodes()[index];
            const ImVec2 &position = m_node_positions[index];
            ImVec2       maximum   = add(position, node.size);

            if (maximum.x  < viewport_minimum.x ||
                maximum.y  < viewport_minimum.y ||
                position.x > viewport_maximum.x ||
                position.y > viewport_maximum.y)
                
                continue;

            m_node_visibility[index] = 1;
            m_visible_nodes.push_back(index);
        }

        node_id hovered_node = 0;
        size_t hovered_node_index = invalid_node_index;

        for (auto visible = m_visible_nodes.rbegin(); visible != m_visible_nodes.rend(); ++visible) {
            size_t index = *visible;

            const c_node &node = graph.nodes()[index];
            
            ImVec2 minimum = graph_to_screen(m_node_positions[index],canvas_position);
            ImVec2 maximum = add(minimum, multiply(node.size, m_zoom));

            if (point_in_rectangle(mouse_position, minimum, maximum)) {
                hovered_node = node.id;
                hovered_node_index = index;

                break;
            }
        }

        if (canvas_hovered && hovered_node && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            m_double_clicked_node = hovered_node;

        if (canvas_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            m_selected_node      = hovered_node;
            m_dragged_node       = hovered_node;
            m_dragged_node_index = hovered_node_index;

            if (hovered_node_index != invalid_node_index) {
                ImVec2 graph_cursor = screen_to_graph(mouse_position, canvas_position);

                m_drag_offset = subtract(
                    graph_cursor,
                    m_node_positions[hovered_node_index]);
            }
        }

        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            m_dragged_node       = 0;
            m_dragged_node_index = invalid_node_index;
        }

        if (m_dragged_node && m_dragged_node_index != invalid_node_index &&
            ImGui::IsMouseDragging(ImGuiMouseButton_Left, 2.0f)) {

            ImVec2 graph_cursor = screen_to_graph(mouse_position, canvas_position);
            ImVec2 position     = subtract(graph_cursor, m_drag_offset);

            m_positions[m_dragged_node] = position;
            m_manual_positions.insert(m_dragged_node);
            m_node_positions[m_dragged_node_index] = position;
        }

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        
        draw_list->PushClipRect(canvas_position, canvas_end, true);
        draw_list->AddRectFilled(canvas_position, canvas_end, IM_COL32(24, 27, 33, 255));

        float scaled_grid_size = grid_size * m_zoom;

        while (scaled_grid_size < 24.0f)
            scaled_grid_size *= 2.0f;

        float horizontal_offset = std::fmod(m_pan.x, scaled_grid_size);
        float vertical_offset   = std::fmod(m_pan.y, scaled_grid_size);

        if (horizontal_offset < 0.0f)
            horizontal_offset += scaled_grid_size;

        if (vertical_offset < 0.0f)
            vertical_offset += scaled_grid_size;

        for (float x = horizontal_offset; x < size.x; x += scaled_grid_size) {
            draw_list->AddLine(
                ImVec2(canvas_position.x + x, canvas_position.y),
                ImVec2(canvas_position.x + x, canvas_end.y),
                IM_COL32(44, 48, 58, 160));
        }

        for (float y = vertical_offset; y < size.y; y += scaled_grid_size) {
            draw_list->AddLine(
                ImVec2(canvas_position.x, canvas_position.y + y),
                ImVec2(canvas_end.x, canvas_position.y + y),
                IM_COL32(44, 48, 58, 160));
        }

        for (size_t connection_index = 0; connection_index < graph.connections().size(); ++connection_index) {
            const c_connection         &connection = graph.connections()[connection_index];
            const c_connection_indices &indices    = m_connection_indices[connection_index];

            if (indices.source == invalid_node_index || indices.target == invalid_node_index)
                continue;

            if (!m_node_visibility[indices.source] && !m_node_visibility[indices.target])
                continue;

            const c_node &source_node = graph.nodes()[indices.source];
            const c_node &target_node = graph.nodes()[indices.target];

            ImVec2 source_minimum = graph_to_screen(m_node_positions[indices.source], canvas_position);
            ImVec2 source_maximum = add(source_minimum, multiply(source_node.size, m_zoom));

            ImVec2 target_minimum = graph_to_screen(m_node_positions[indices.target], canvas_position);
            ImVec2 target_maximum = add(target_minimum, multiply(target_node.size, m_zoom));
            
            ImVec2 source_center = multiply(add(source_minimum, source_maximum), 0.5f);
            ImVec2 target_center = multiply(add(target_minimum, target_maximum), 0.5f);
            
            ImVec2 source = rectangle_edge(source_minimum, source_maximum, target_center);
            ImVec2 target = rectangle_edge(target_minimum, target_maximum, source_center);
            
            ImVec2 source_control;
            ImVec2 target_control;

            connection_controls(source, target, &source_control, &target_control);

            float thickness = std::clamp(
                connection.thickness * std::sqrt(m_zoom),
                1.0f,
                8.0f);

            draw_list->AddBezierCubic(
                source,
                source_control,
                target_control,
                target,
                connection.color,
                thickness);

            if (connection.directed)
                draw_arrow(draw_list, target, target_control, connection.color, m_zoom);

            bool connection_relevant =
                connection.source == m_selected_node ||
                connection.target == m_selected_node ||
                connection.source == hovered_node    ||
                connection.target == hovered_node;

            if (!connection.label.empty() && m_zoom >= 0.80f &&
                (connection_relevant || m_visible_nodes.size() <= 80)) {

                ImVec2 padding(5.0f * m_zoom, 3.0f * m_zoom);

                ImVec2 label_position = multiply(add(source, target), 0.5f);
                ImVec2 text_size      = ImGui::CalcTextSize(connection.label.c_str());
                text_size             = multiply(text_size, m_zoom);
                ImVec2 label_minimum  = subtract(label_position, multiply(text_size, 0.5f));
                label_minimum         = subtract(label_minimum, padding);
                ImVec2 label_maximum  = add(label_position, multiply(text_size, 0.5f));
                label_maximum         = add(label_maximum, padding);

                draw_list->AddRectFilled(
                    label_minimum,
                    label_maximum,
                    IM_COL32(24, 27, 33, 230),
                    4.0f * m_zoom);
                draw_list->AddText(
                    ImGui::GetFont(),
                    ImGui::GetFontSize() * m_zoom,
                    add(label_minimum, padding),
                    connection.color,
                    connection.label.c_str());
            }
        }

        for (size_t index : m_visible_nodes) {
            const c_node &node = graph.nodes()[index];

            ImVec2 minimum = graph_to_screen(m_node_positions[index], canvas_position);
            ImVec2 maximum = add(minimum, multiply(node.size, m_zoom));

            float rounding = node.style.rounding * m_zoom;

            float  title_height = 30.0f * m_zoom;
            ImVec2 title_end(maximum.x, minimum.y + title_height);

            draw_list->AddRectFilled(
                minimum,
                maximum,
                node.style.body_color,
                rounding);
            draw_list->AddRectFilled(
                minimum,
                title_end,
                node.style.title_color,
                rounding,
                ImDrawFlags_RoundCornersTop);

            ImU32 border_color = node.id == m_selected_node ?
                node.style.selected_border_color : node.style.border_color;
            float border_thickness = node.style.border_thickness * std::sqrt(m_zoom);

            draw_list->AddRect(
                minimum,
                maximum,
                border_color,
                rounding,
                0,
                border_thickness);

            if (m_zoom >= 0.18f) {
                float font_size = ImGui::GetFontSize() * m_zoom;
                float horizontal_padding = 10.0f * m_zoom;
                
                ImVec4 title_clip(
                    minimum.x + horizontal_padding,
                    minimum.y,
                    maximum.x - horizontal_padding,
                    title_end.y);

                draw_list->AddText(
                    ImGui::GetFont(),
                    font_size,
                    ImVec2(
                        minimum.x + horizontal_padding,
                        minimum.y + (title_height - font_size) * 0.5f),
                    node.style.title_text_color,
                    node.title.c_str(),
                    nullptr,
                    0.0f,
                    &title_clip);
            }

            if (m_zoom >= 0.75f) {
                float font_size = ImGui::GetFontSize() * m_zoom;
                float horizontal_padding = 10.0f * m_zoom;
                float line_height = 19.0f * m_zoom;
                float row_y = title_end.y + 8.0f * m_zoom;
                
                ImVec4 body_clip(
                    minimum.x + horizontal_padding,
                    title_end.y,
                    maximum.x - horizontal_padding,
                    maximum.y - 5.0f * m_zoom);

                for (const std::string &row : node.rows) {
                    if (row_y + font_size > maximum.y)
                        break;

                    draw_list->AddText(
                        ImGui::GetFont(),
                        font_size,
                        ImVec2(minimum.x + horizontal_padding, row_y),
                        node.style.body_text_color,
                        row.c_str(),
                        nullptr,
                        0.0f,
                        &body_clip);

                    row_y += line_height;
                }
            }
        }

        draw_list->AddRect(
            canvas_position,
            canvas_end,
            IM_COL32(73, 80, 95, 255));
        draw_list->PopClipRect();

        if (canvas_hovered && hovered_node_index != invalid_node_index) {
            const c_node &node = graph.nodes()[hovered_node_index];

            ImGui::BeginTooltip();
            ImGui::TextUnformatted(node.title.c_str());

            for (const std::string &row : node.rows)
                ImGui::TextDisabled("%s", row.c_str());

            ImGui::EndTooltip();
        }
    }

    void c_view::request_frame() {
        m_frame_requested = true;
    }

    void c_view::request_focus(node_id node) {
        m_focus_requested_node = node;
    }

    void c_view::reset_layout() {
        m_positions.clear();
        m_manual_positions.clear();
        m_selected_node = 0;
        m_dragged_node = 0;
        m_dragged_node_index = invalid_node_index;
        m_synchronized_graph = nullptr;
        m_synchronized_revision = 0;
        m_frame_requested = true;
    }

    float c_view::zoom() const {
        return m_zoom;
    }

    node_id c_view::selected_node() const {
        return m_selected_node;
    }

    node_id c_view::take_double_clicked_node() {
        node_id node = m_double_clicked_node;

        m_double_clicked_node = 0;

        return node;
    }
}} // ext::node_graph
}} // banana::imgui::node_graph
