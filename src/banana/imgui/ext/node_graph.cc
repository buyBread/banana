#include <algorithm>

#include "banana/imgui/imgui.hh"
#include "banana/imgui/ext/node_graph.hh"
#include "banana/imgui/util/vec2.hh"

namespace banana { namespace imgui {
namespace ext { namespace node_graph {
    constexpr f32  minimum_zoom       = 0.005f;
    constexpr f32  maximum_zoom       = 2.00f;
    constexpr f32  grid_size          = 64.0f;
    constexpr auto invalid_node_index = (size_t)-1; // C4245

    u64 graph_revision = 0;

    u64 next_graph_revision() {
        return ++graph_revision;
    }

    void connection_controls(const ImVec2  &source,
                             const ImVec2  &target,
                                   ImVec2*  source_control,
                                   ImVec2*  target_control) {

        ImVec2 delta = util::vec2::subtract(target, source);

        if (std::abs(delta.x) >= std::abs(delta.y)) {
            f32 direction = delta.x < 0.0f ? -1.0f : 1.0f;
            f32 curve     = std::clamp(std::abs(delta.x) * 0.5f, 40.0f, 220.0f);

            *source_control = ImVec2(source.x + curve * direction, source.y);
            *target_control = ImVec2(target.x - curve * direction, target.y);
        } else {
            f32 direction = delta.y < 0.0f ? -1.0f : 1.0f;
            f32 curve     = std::clamp(std::abs(delta.y) * 0.5f, 40.0f, 220.0f);

            *source_control = ImVec2(source.x, source.y + curve * direction);
            *target_control = ImVec2(target.x, target.y - curve * direction);
        }
    }

    void draw_arrow(      ImDrawList*  draw_list,
                    const ImVec2      &tip,
                    const ImVec2      &preceding_control,
                          u32          color,
                          f32          scale) {

        ImVec2 direction = util::vec2::subtract(tip, preceding_control);
            
        f32 length = std::sqrt(direction.x * direction.x +
                               direction.y * direction.y);

        if (length <= 0.001f)
            return;

        direction = util::vec2::multiply(direction, 1.0f / length);

        f32 arrow_length = 11.0f * scale;
        f32 arrow_width  = 5.0f  * scale;

        ImVec2 normal(-direction.y, direction.x);

        ImVec2 base = util::vec2::subtract(tip, util::vec2::multiply(direction, arrow_length));

        draw_list->AddTriangleFilled(tip,
                                     util::vec2::add     (base, util::vec2::multiply(normal, arrow_width)),
                                     util::vec2::subtract(base, util::vec2::multiply(normal, arrow_width)),
                                     color);
    }

    c_node_style::c_node_style() : title_color          (IM_COL32( 54,  60,  72, 255)),
                                   body_color           (IM_COL32( 38,  42,  51, 255)),
                                   border_color         (IM_COL32( 89,  98, 117, 255)),
                                   selected_border_color(IM_COL32(255, 204,  92, 255)),
                                   title_text_color     (IM_COL32(244, 246, 250, 255)),
                                   body_text_color      (IM_COL32(199, 205, 216, 255)),
                                   rounding             (6.0f),
                                   border_thickness     (1.5f) {}

    c_layered_layout_options::c_layered_layout_options() : origin   (0.0f, 0.0f),
                                                           lane_step(350.0f),
                                                           row_step (178.0f),
                                                           layer_gap(180.0f),
                                                           row_limit(12) {}

    c_row_layout_options::c_row_layout_options() : center    (0.0f, 0.0f),
                                                   node_width(300.0f),
                                                   step      (330.0f) {}

    f32 c_node::content_height(size_t row_count) {
        constexpr f32 title_height = 30.0f;
        constexpr f32 body_padding = 16.0f;
        constexpr f32 row_height   = 19.0f;

        return title_height + body_padding + row_height * row_count;
    }

    void c_node::add_row(const std::string &row) {
        rows.push_back(row);

        size.y = std::max(size.y, content_height(rows.size()));
    }

    c_graph::c_graph() : m_revision(next_graph_revision()) {}

    c_node &c_graph::add_node(      node_id       id,
                                    ImVec2        position,
                                    ImVec2        size,
                              const std::string  &title,
                              const c_node_style &node_style) {

        auto existing = m_node_indices.find(id);

        if (existing != m_node_indices.end()) {
            c_node &node = m_nodes[existing->second];

            node.position = position;
            node.size     = size;
            node.title    = title;
            node.rows.clear();
            node.style    = node_style;
            
            m_revision = next_graph_revision();

            return node;
        }

        size_t index = m_nodes.size();
        
        m_nodes.push_back({ id, position, size, title, {}, node_style });
        m_node_indices[id] = index;
        
        m_revision = next_graph_revision();

        return m_nodes.back();
    }

    c_node& c_graph::add_node(      node_id       id,
                                    ImVec2        position,
                                    f32           width,
                              const std::string  &title,
                              const c_node_style &node_style) {

        auto size = ImVec2(width, c_node::content_height(0));

        return add_node(id, position, size, title, node_style);
    }

    c_connection &c_graph::add_connection(      node_id      source,
                                                node_id      target,
                                                u32          color,
                                                f32          thickness,
                                                bool         directed,
                                          const std::string &label,
                                                u64          group) {

        m_connections.push_back({ source, target, color, thickness, directed, label, group, 1 });

        m_revision = next_graph_revision();

        return m_connections.back();
    }

    c_connection &c_graph::add_or_increment_connection(      node_id      source,
                                                             node_id      target,
                                                             u64          group,
                                                             u32          color,
                                                             f32          thickness,
                                                             bool         directed,
                                                       const std::string &label) {

        c_connection* existing = find_connection(source, target, group);

        if (existing) {
            ++existing->multiplicity;
            
            m_revision = next_graph_revision();
            
            return *existing;
        }

        return add_connection(source, target, color, thickness, directed, label, group);
    }

    c_node* c_graph::find_node(node_id id) {
        auto entry = m_node_indices.find(id);

        return entry == m_node_indices.end() ?
            nullptr : &m_nodes[entry->second];
    }

    const c_node* c_graph::find_node(node_id id) const {
        auto entry = m_node_indices.find(id);

        return entry == m_node_indices.end() ?
            nullptr : &m_nodes[entry->second];
    }

    c_connection* c_graph::find_connection(node_id source, node_id target, u64 group) {
        for (c_connection &connection : m_connections) {
            if (connection.source == source &&
                connection.target == target &&
                connection.group  == group)

                return &connection;
        }

        return nullptr;
    }

    const c_connection* c_graph::find_connection(node_id source, node_id target, u64 group) const {
        for (const c_connection &connection : m_connections) {
            if (connection.source == source &&
                connection.target == target &&
                connection.group  == group)

                return &connection;
        }

        return nullptr;
    }

    const std::vector<c_node> &c_graph::nodes() const {
        return m_nodes;
    }

    std::vector<c_connection> &c_graph::connections() {
        return m_connections;
    }

    const std::vector<c_connection> &c_graph::connections() const {
        return m_connections;
    }

    void c_graph::clear() {
        m_nodes       .clear();
        m_connections .clear();
        m_node_indices.clear();
        
        m_revision = next_graph_revision();
    }

    bool c_graph::empty() const {
        return m_nodes.empty();
    }

    u64 c_graph::revision() const {
        return m_revision;
    }

    c_view::c_view() : m_pan                  (0.0f, 0.0f),
                       m_drag_offset          (0.0f, 0.0f),
                       m_zoom                 (1.0f),
                       m_selected_node        (0),
                       m_dragged_node         (0),
                       m_double_clicked_node  (0),
                       m_focus_requested_node (0),
                       m_dragged_node_index   (invalid_node_index),
                       m_synchronized_graph   (nullptr),
                       m_synchronized_revision(0),
                       m_frame_requested      (true),
                       m_selection_changed    (false) {}

    ImVec2 c_view::graph_to_screen(const ImVec2 &point,
                                   const ImVec2 &canvas_position) const {

        return util::vec2::add(util::vec2::add(canvas_position, m_pan),
                               util::vec2::multiply(point, m_zoom));
    }

    ImVec2 c_view::screen_to_graph(const ImVec2 &point,
                                   const ImVec2 &canvas_position) const {

        auto pan = util::vec2::subtract(util::vec2::subtract(point,
                                                             canvas_position),
                                        m_pan);

        return util::vec2::multiply(pan, 1.0f / m_zoom);
    }

    void c_view::synchronize_positions(const c_graph &graph) {
        if (m_synchronized_graph    == &graph &&
            m_synchronized_revision ==  graph.revision())

            return;

        m_node_positions .resize(graph.nodes().size());
        m_node_visibility.assign(graph.nodes().size(), 0);
        m_visible_nodes  .clear();

        for (size_t index = 0; index < graph.nodes().size(); ++index) {
            const c_node &node = graph.nodes()[index];
            
            auto position = m_positions.find(node.id);

            if (position == m_positions.end() || !m_manual_positions.contains(node.id)) {
                m_positions[node.id]    = node.position;
                m_node_positions[index] = node.position;
            } else
                m_node_positions[index] = position->second;
        }

        for (auto position = m_positions.begin(); position != m_positions.end(); ) {
            if (!graph.find_node(position->first))
                position = m_positions.erase(position);
            else
                ++position;
        }

        for (auto position = m_manual_positions.begin(); position != m_manual_positions.end(); ) {

            if (!graph.find_node(*position))
                position = m_manual_positions.erase(position);
            else
                ++position;
        }

        if (m_selected_node && !graph.find_node(m_selected_node))
            m_selected_node = 0;

        if (m_dragged_node) {
            const c_node* dragged = graph.find_node(m_dragged_node);

            if (dragged)
                m_dragged_node_index = (size_t)(dragged - graph.nodes().data());
            else {
                m_dragged_node       = 0;
                m_dragged_node_index = invalid_node_index;
            }
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
        m_synchronized_revision =  graph.revision();
    }

    void c_view::frame_graph(const c_graph &graph, const ImVec2 &canvas_size) {
        if (graph.empty()) {
            m_pan  = util::vec2::multiply(canvas_size, 0.5f);
            m_zoom = 1.0f;
            
            return;
        }

        ImVec2 minimum(std::numeric_limits<f32>::max(),
                       std::numeric_limits<f32>::max());
        ImVec2 maximum(-std::numeric_limits<f32>::max(),
                       -std::numeric_limits<f32>::max());

        for (size_t index = 0; index < graph.nodes().size(); ++index) {
            const c_node &node = graph.nodes()[index];
            
            ImVec2 position = m_node_positions[index];

            minimum.x = std::min(minimum.x, position.x);
            minimum.y = std::min(minimum.y, position.y);
            maximum.x = std::max(maximum.x, position.x + node.size.x);
            maximum.y = std::max(maximum.y, position.y + node.size.y);
        }

        ImVec2 graph_size = util::vec2::subtract(maximum, minimum);

        f32 horizontal_zoom = (canvas_size.x - 80.0f) / std::max(graph_size.x, 1.0f);
        f32 vertical_zoom   = (canvas_size.y - 80.0f) / std::max(graph_size.y, 1.0f);

        m_zoom = std::clamp(std::min(horizontal_zoom, vertical_zoom),
                            minimum_zoom,
                            1.25f);

        ImVec2 center = util::vec2::multiply(util::vec2::add(minimum, maximum), 0.5f);
        
        m_pan = util::vec2::subtract(util::vec2::multiply(canvas_size, 0.5f),
                                     util::vec2::multiply(center, m_zoom));
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
        ImVec2 canvas_end      = util::vec2::add(canvas_position, size);

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
                ImVec2 focused_center = util::vec2::add(m_node_positions[focused_index],
                                                        util::vec2::multiply(focused->size, 0.5f));

                m_zoom = std::max(m_zoom, 0.65f);

                m_pan = util::vec2::subtract(util::vec2::multiply(size, 0.5f),
                                             util::vec2::multiply(focused_center, m_zoom));
            }

            m_focus_requested_node = 0;
        }

        if (canvas_hovered && imgui::io->MouseWheel != 0.0f) {
            ImVec2 graph_cursor = screen_to_graph(mouse_position,
                                                  canvas_position);
            
            f32 previous_zoom = m_zoom;

            m_zoom = std::clamp(
                m_zoom * std::pow(1.15f, imgui::io->MouseWheel),
                minimum_zoom,
                maximum_zoom);

            ImVec2 cursor_in_canvas = util::vec2::subtract(mouse_position,
                                                           canvas_position);

            m_pan = util::vec2::subtract(cursor_in_canvas, util::vec2::multiply(graph_cursor, m_zoom));

            if (previous_zoom != m_zoom) {
                m_dragged_node = 0;
                m_dragged_node_index = invalid_node_index;
            }
        }

        if (canvas_hovered && !m_dragged_node && (ImGui::IsMouseDragging(ImGuiMouseButton_Middle, 0.0f) ||
                                                  ImGui::IsMouseDragging(ImGuiMouseButton_Right,  0.0f)))

            m_pan = util::vec2::add(m_pan, imgui::io->MouseDelta);

        ImVec2 viewport_start  = screen_to_graph(canvas_position, canvas_position);
        ImVec2 viewport_end    = screen_to_graph(canvas_end,      canvas_position);
        f32    viewport_margin = 80.0f / m_zoom;
        ImVec2 viewport_minimum(std::min(viewport_start.x, viewport_end.x) - viewport_margin,
                                std::min(viewport_start.y, viewport_end.y) - viewport_margin);
        ImVec2 viewport_maximum(std::max(viewport_start.x, viewport_end.x) + viewport_margin,
                                std::max(viewport_start.y, viewport_end.y) + viewport_margin);

        std::fill(m_node_visibility.begin(),
                  m_node_visibility.end(),
                  (u8)0);

        m_visible_nodes.clear();

        for (size_t index = 0; index < graph.nodes().size(); ++index) {
            const c_node &node     = graph.nodes()[index];
            const ImVec2 &position = m_node_positions[index];
            
            ImVec2 maximum = util::vec2::add(position, node.size);

            if (maximum.x  < viewport_minimum.x ||
                maximum.y  < viewport_minimum.y ||
                position.x > viewport_maximum.x ||
                position.y > viewport_maximum.y)
                
                continue;

            m_node_visibility[index] = 1;

            m_visible_nodes.push_back(index);
        }

        node_id hovered_node       = 0;
        size_t  hovered_node_index = invalid_node_index;

        for (auto visible = m_visible_nodes.rbegin(); visible != m_visible_nodes.rend(); ++visible) {
            size_t index = *visible;

            const c_node &node = graph.nodes()[index];
            
            ImVec2 minimum = graph_to_screen(m_node_positions[index],canvas_position);
            ImVec2 maximum = util::vec2::add(minimum, util::vec2::multiply(node.size, m_zoom));

            if (util::vec2::point_in_rectangle(mouse_position, minimum, maximum)) {
                hovered_node       = node.id;
                hovered_node_index = index;

                break;
            }
        }

        if (canvas_hovered && hovered_node && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            m_double_clicked_node = hovered_node;

        if (canvas_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            if (m_selected_node != hovered_node)
                m_selection_changed = true;

            m_selected_node      = hovered_node;
            m_dragged_node       = hovered_node;
            m_dragged_node_index = hovered_node_index;

            if (hovered_node_index != invalid_node_index)
                m_drag_offset = util::vec2::subtract(screen_to_graph(mouse_position, canvas_position),
                                                     m_node_positions[hovered_node_index]);
        }

        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            m_dragged_node       = 0;
            m_dragged_node_index = invalid_node_index;
        }

        if (m_dragged_node && m_dragged_node_index != invalid_node_index && ImGui::IsMouseDragging(ImGuiMouseButton_Left, 2.0f)) {
            ImVec2 graph_cursor = screen_to_graph(mouse_position, canvas_position);
            
            ImVec2 position = util::vec2::subtract(graph_cursor, m_drag_offset);

            m_manual_positions.insert(m_dragged_node);
            m_positions     [m_dragged_node]       = position;
            m_node_positions[m_dragged_node_index] = position;
        }

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        
        draw_list->PushClipRect (canvas_position, canvas_end, true);
        draw_list->AddRectFilled(canvas_position, canvas_end, IM_COL32(24, 27, 33, 255));

        f32 scaled_grid_size = grid_size * m_zoom;

        while (scaled_grid_size < 24.0f)
            scaled_grid_size *= 2.0f;

        f32 horizontal_offset = std::fmod(m_pan.x, scaled_grid_size);
        f32 vertical_offset   = std::fmod(m_pan.y, scaled_grid_size);

        if (horizontal_offset < 0.0f)
            horizontal_offset += scaled_grid_size;

        if (vertical_offset < 0.0f)
            vertical_offset += scaled_grid_size;

        for (f32 x = horizontal_offset; x < size.x; x += scaled_grid_size)
            draw_list->AddLine(ImVec2(canvas_position.x + x, canvas_position.y),
                               ImVec2(canvas_position.x + x, canvas_end.y),
                               IM_COL32(44, 48, 58, 160));

        for (f32 y = vertical_offset; y < size.y; y += scaled_grid_size)
            draw_list->AddLine(ImVec2(canvas_position.x, canvas_position.y + y),
                               ImVec2(canvas_end.x, canvas_position.y + y),
                               IM_COL32(44, 48, 58, 160));

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
            ImVec2 source_maximum = util::vec2::add(source_minimum, util::vec2::multiply(source_node.size, m_zoom));

            ImVec2 target_minimum = graph_to_screen(m_node_positions[indices.target], canvas_position);
            ImVec2 target_maximum = util::vec2::add(target_minimum, util::vec2::multiply(target_node.size, m_zoom));
            
            ImVec2 source_center = util::vec2::multiply(util::vec2::add(source_minimum, source_maximum), 0.5f);
            ImVec2 target_center = util::vec2::multiply(util::vec2::add(target_minimum, target_maximum), 0.5f);
            
            ImVec2 source = util::vec2::rectangle_edge(source_minimum, source_maximum, target_center);
            ImVec2 target = util::vec2::rectangle_edge(target_minimum, target_maximum, source_center);
            
            ImVec2 source_control;
            ImVec2 target_control;

            connection_controls(source, target, &source_control, &target_control);

            f32 thickness = std::clamp(connection.thickness * std::sqrt(m_zoom),
                                       1.0f,
                                       8.0f);

            draw_list->AddBezierCubic(source,
                                      source_control,
                                      target_control,
                                      target,
                                      connection.color,
                                      thickness);

            if (connection.directed)
                draw_arrow(draw_list, target, target_control, connection.color, m_zoom);

            bool connection_relevant = connection.source == m_selected_node ||
                                       connection.target == m_selected_node ||
                                       connection.source == hovered_node    ||
                                       connection.target == hovered_node;

            if (!connection.label.empty() && m_zoom >= 0.80f && (connection_relevant || m_visible_nodes.size() <= 80)) {
                ImVec2 padding(5.0f * m_zoom, 3.0f * m_zoom);

                ImVec2 label_position = util::vec2::multiply(util::vec2::add(source, target), 0.5f);
                ImVec2 text_size      = ImGui::CalcTextSize(connection.label.c_str());
                       text_size      = util::vec2::multiply(text_size, m_zoom);
                ImVec2 label_minimum  = util::vec2::subtract(label_position, util::vec2::multiply(text_size, 0.5f));
                       label_minimum  = util::vec2::subtract(label_minimum, padding);
                ImVec2 label_maximum  = util::vec2::add(label_position, util::vec2::multiply(text_size, 0.5f));
                       label_maximum  = util::vec2::add(label_maximum, padding);

                draw_list->AddRectFilled(label_minimum,
                                         label_maximum,
                                         IM_COL32(24, 27, 33, 230),
                                         4.0f * m_zoom);
                draw_list->AddText(ImGui::GetFont(),
                                   ImGui::GetFontSize() * m_zoom,
                                   util::vec2::add(label_minimum, padding),
                                   connection.color,
                                   connection.label.c_str());
            }
        }

        for (size_t index : m_visible_nodes) {
            const c_node &node = graph.nodes()[index];

            ImVec2 minimum      = graph_to_screen(m_node_positions[index], canvas_position);
            ImVec2 maximum      = util::vec2::add(minimum, util::vec2::multiply(node.size, m_zoom));
            f32    rounding     = node.style.rounding * m_zoom;
            f32    title_height = 30.0f * m_zoom;
            ImVec2 title_end(maximum.x, minimum.y + title_height);

            draw_list->AddRectFilled(minimum,
                                     maximum,
                                     node.style.body_color,
                                     rounding);
            draw_list->AddRectFilled(minimum,
                                     title_end,
                                     node.style.title_color,
                                     rounding,
                                     ImDrawFlags_RoundCornersTop);

            u32 border_color = node.id == m_selected_node ?
                node.style.selected_border_color : node.style.border_color;
            f32 border_thickness = node.style.border_thickness * std::sqrt(m_zoom);

            draw_list->AddRect(minimum,
                               maximum,
                               border_color,
                               rounding,
                               0,
                               border_thickness);

            if (m_zoom >= 0.18f) {
                f32 font_size = ImGui::GetFontSize() * m_zoom;

                f32 horizontal_padding = 10.0f * m_zoom;
                
                ImVec4 title_clip(minimum.x + horizontal_padding,
                                  minimum.y,
                                  maximum.x - horizontal_padding,
                                  title_end.y);

                draw_list->AddText(ImGui::GetFont(),
                                   font_size,
                                   ImVec2(minimum.x + horizontal_padding,
                                          minimum.y + (title_height - font_size) * 0.5f),
                                   node.style.title_text_color,
                                   node.title.c_str(),
                                   nullptr,
                                   0.0f,
                                   &title_clip);
            }

            if (m_zoom >= 0.75f) {
                f32 font_size          = ImGui::GetFontSize() * m_zoom;
                f32 horizontal_padding = 10.0f * m_zoom;
                f32 line_height        = 19.0f * m_zoom;
                f32 row_y              = title_end.y + 8.0f * m_zoom;
                
                ImVec4 body_clip(minimum.x + horizontal_padding,
                                 title_end.y,
                                 maximum.x - horizontal_padding,
                                 maximum.y - 5.0f * m_zoom);

                for (const std::string &row : node.rows) {
                    if (row_y + font_size > maximum.y)
                        break;

                    draw_list->AddText(ImGui::GetFont(),
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

        draw_list->AddRect(canvas_position,
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
        m_positions             .clear();
        m_manual_positions      .clear();
        m_selected_node         = 0;
        m_dragged_node          = 0;
        m_dragged_node_index    = invalid_node_index;
        m_synchronized_graph    = nullptr;
        m_synchronized_revision = 0;
        m_frame_requested       = true;
    }

    f32 c_view::zoom() const {
        return m_zoom;
    }

    node_id c_view::selected_node() const {
        return m_selected_node;
    }

    bool c_view::take_selection_changed() {
        bool changed = m_selection_changed;

        m_selection_changed = false;

        return changed;
    }

    node_id c_view::take_double_clicked_node() {
        node_id node = m_double_clicked_node;

        m_double_clicked_node = 0;

        return node;
    }

    std::unordered_map
        <node_id, ImVec2> calculate_layered_layout(const std::vector<std::vector<node_id>> &layers,
                                                   const c_layered_layout_options          &options) {

        std::unordered_map<node_id, ImVec2> positions;

        size_t node_count = 0;

        for (const std::vector<node_id> &layer : layers)
            node_count += layer.size();

        positions.reserve(node_count);

        f32 layer_start_x = options.origin.x;
        u32 row_limit     = std::max(options.row_limit, 1u);

        for (const std::vector<node_id> &layer : layers) {
            u32 lane_count = std::max(1u, ((u32)layer.size() + row_limit - 1) / row_limit);

            for (u32 ordinal = 0; ordinal < layer.size(); ++ordinal) {
                u32 lane = ordinal / row_limit;
                u32 row  = ordinal % row_limit;

                positions[layer[ordinal]] = ImVec2(layer_start_x + lane * options.lane_step,
                                                   options.origin.y + row * options.row_step);
            }

            layer_start_x += lane_count * options.lane_step + options.layer_gap;
        }

        return positions;
    }

    std::unordered_map
        <node_id, ImVec2> calculate_row_layout(const std::vector<node_id> &nodes,
                                               const c_row_layout_options &options) {

        std::unordered_map<node_id, ImVec2> positions;

        positions.reserve(nodes.size());

        f32 row_width = nodes.empty() ?
            0.0f : options.node_width + (nodes.size() - 1) * options.step;
        
            f32 start_x = options.center.x - row_width * 0.5f;

        for (size_t index = 0; index < nodes.size(); ++index)
            positions[nodes[index]] = ImVec2(start_x + index * options.step,
                                             options.center.y);

        return positions;
    }
}} // ext::node_graph
}} // banana::imgui