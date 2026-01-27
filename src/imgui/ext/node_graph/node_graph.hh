#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <imgui.h>

namespace banana { namespace imgui {
namespace ext { namespace node_graph {

    using node_id = uint64_t;

    struct c_node_style {
        ImU32 title_color;
        ImU32 body_color;
        ImU32 border_color;
        ImU32 selected_border_color;
        ImU32 title_text_color;
        ImU32 body_text_color;
        float rounding;
        float border_thickness;

        c_node_style();
    };

    struct c_node {
        node_id                  id;
        ImVec2                   position;
        ImVec2                   size;
        std::string              title;
        std::vector<std::string> rows;
        c_node_style             style;
    };

    struct c_connection {
        node_id     source;
        node_id     target;
        ImU32       color;
        float       thickness;
        bool        directed;
        std::string label;
    };

    class c_graph {
        std::vector<c_node>                 m_nodes;
        std::vector<c_connection>           m_connections;
        std::unordered_map<node_id, size_t> m_node_indices;
        uint64_t                            m_revision;

    public:
        c_graph();

        c_node &add_node(
            node_id            id,
            ImVec2             position,
            ImVec2             size,
            const std::string  &title,
            const c_node_style &style = c_node_style());

        c_connection &add_connection(
            node_id            source,
            node_id            target,
            ImU32              color,
            float              thickness = 1.0f,
            bool               directed = true,
            const std::string  &label = std::string());

        c_node* find_node(node_id id);
        const c_node* find_node(node_id id) const;
        const std::vector<c_node> &nodes() const;
        const std::vector<c_connection> &connections() const;
        bool empty() const;
        uint64_t revision() const;
    };

    class c_view {
        struct c_connection_indices {
            size_t source;
            size_t target;
        };

        std::unordered_map<node_id, ImVec2> m_positions;
        std::unordered_set<node_id>         m_manual_positions;
        std::vector<ImVec2>                 m_node_positions;
        std::vector<uint8_t>                m_node_visibility;
        std::vector<size_t>                 m_visible_nodes;
        std::vector<c_connection_indices>   m_connection_indices;
        ImVec2                              m_pan;
        ImVec2                              m_drag_offset;
        float                               m_zoom;
        node_id                             m_selected_node;
        node_id                             m_dragged_node;
        node_id                             m_double_clicked_node;
        node_id                             m_focus_requested_node;
        size_t                              m_dragged_node_index;
        const c_graph*                      m_synchronized_graph;
        uint64_t                            m_synchronized_revision;
        bool                                m_frame_requested;

        ImVec2 graph_to_screen(const ImVec2 &point, const ImVec2 &canvas_position) const;
        ImVec2 screen_to_graph(const ImVec2 &point, const ImVec2 &canvas_position) const;
        void synchronize_positions(const c_graph &graph);
        void frame_graph(const c_graph &graph, const ImVec2 &canvas_size);

    public:
        c_view();

        void draw(const char* identifier, const c_graph &graph, ImVec2 size = ImVec2(0.0f, 0.0f));
        void request_frame();
        void request_focus(node_id node);
        void reset_layout();
        float zoom() const;
        node_id selected_node() const;
        node_id take_double_clicked_node();
    };
}} // ext::node_graph
}} // banana::imgui
