#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <imgui.h>

#include "util/types.hh"

namespace banana { namespace imgui {
namespace ext { namespace node_graph {
    using node_id = u64;

    struct c_layered_layout_options {
        ImVec2 origin;
        f32    lane_step;
        f32    row_step;
        f32    layer_gap;
        u32    row_limit;

        c_layered_layout_options();
    };

    struct c_row_layout_options {
        ImVec2 center;
        f32    node_width;
        f32    step;

        c_row_layout_options();
    };

    struct c_node_style {
        u32 title_color;
        u32 body_color;
        u32 border_color;
        u32 selected_border_color;
        u32 title_text_color;
        u32 body_text_color;
        f32 rounding;
        f32 border_thickness;

        c_node_style();
    };

    struct c_node {
        node_id           id;
        ImVec2            position;
        ImVec2            size;
        std::string       title;
        std::vector
            <std::string> rows;
        c_node_style      style;

        static f32 content_height(size_t row_count);
        
        void add_row(const std::string &row);
    };

    struct c_connection {
        node_id     source;
        node_id     target;
        u32         color;
        f32         thickness;
        bool        directed;
        std::string label;
        u64         group;
        u32         multiplicity;
    };

    class c_graph {
        std::vector
            <c_node>          m_nodes;
        std::vector
            <c_connection>    m_connections;
        std::unordered_map
            <node_id, size_t> m_node_indices;
        u64                   m_revision;

    public:
        c_graph();

        c_node &add_node(      node_id       id,
                               ImVec2        position,
                               ImVec2        size,
                         const std::string  &title,
                         const c_node_style &style = c_node_style());
        c_node &add_node(      node_id       id,
                               ImVec2        position,
                               f32           width,
                         const std::string  &title,
                         const c_node_style &style = c_node_style());

        c_connection &add_connection(      node_id       source,
                                           node_id       target,
                                           u32           color,
                                           f32           thickness = 1.0f,
                                           bool          directed  = true,
                                     const std::string  &label     = std::string(),
                                           u64           group     = 0);
        c_connection &add_or_increment_connection(      node_id       source,
                                                        node_id       target,
                                                        u64           group,
                                                        u32           color,
                                                        f32           thickness = 1.0f,
                                                        bool          directed  = true,
                                                  const std::string  &label     = std::string());

              c_node* find_node(node_id id);
        const c_node* find_node(node_id id) const;

              c_connection* find_connection(node_id source, node_id target, u64 group = 0);
        const c_connection* find_connection(node_id source, node_id target, u64 group = 0) const;

        const std::vector<c_node> &nodes() const;

              std::vector<c_connection> &connections();
        const std::vector<c_connection> &connections() const;
        
        void clear();
        bool empty() const;
        
        u64 revision() const;
    };

    class c_view {
        struct c_connection_indices {
            size_t source;
            size_t target;
        };

        std::unordered_map
            <node_id, ImVec2>        m_positions;
        std::unordered_set
            <node_id>                m_manual_positions;
        std::vector
            <ImVec2>                 m_node_positions;
        std::vector
            <u8>                     m_node_visibility;
        std::vector
            <size_t>                 m_visible_nodes;
        std::vector
            <c_connection_indices>   m_connection_indices;
        ImVec2                       m_pan;
        ImVec2                       m_drag_offset;
        f32                          m_zoom;
        node_id                      m_selected_node;
        node_id                      m_dragged_node;
        node_id                      m_double_clicked_node;
        node_id                      m_focus_requested_node;
        size_t                       m_dragged_node_index;
        const c_graph*               m_synchronized_graph;
        u64                          m_synchronized_revision;
        bool                         m_frame_requested;
        bool                         m_selection_changed;

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
        
        f32 zoom() const;
        
        node_id selected_node() const;
        bool    take_selection_changed();
        node_id take_double_clicked_node();
    };

    std::unordered_map<node_id, ImVec2> calculate_layered_layout(
        const std::vector<std::vector<node_id>> &layers,
        const c_layered_layout_options          &options = c_layered_layout_options());
    std::unordered_map<node_id, ImVec2> calculate_row_layout(
        const std::vector<node_id> &nodes,
        const c_row_layout_options &options = c_row_layout_options());
}} // ext::node_graph
}} // banana::imgui
