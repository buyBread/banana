#include "../base.hh"
#include "../ext/node_graph/node_graph.hh"

#include "../../ngl/chuck/script_runtime.hh"

#include <algorithm>

class gui_script_node_view : public c_gui {
    using node_id      = IMGUI_EXT::node_graph::node_id;
    using c_graph      = IMGUI_EXT::node_graph::c_graph;
    using c_node       = IMGUI_EXT::node_graph::c_node;
    using c_node_style = IMGUI_EXT::node_graph::c_node_style;
    using c_view       = IMGUI_EXT::node_graph::c_view;

    struct c_instruction_view {
        uint16_t* operands;
        uint32_t  operand_words;
        uint32_t  size;
        int       operand_kind;
        uint32_t  opcode;
    };

    struct c_object_call {
        NGL_CHUCK::vm::script_object* source;
        NGL_CHUCK::vm::script_object* target;
        uint32_t                      count;
    };

    struct c_graph_statistics {
        uint32_t executable_count;
        uint32_t object_count;
        uint32_t script_count;
        uint32_t object_call_count;
        uint32_t visible_object_count;
        uint32_t visible_script_count;
    };

    struct c_script_layout {
        NGL_CHUCK::vm::vm_executable* script;
        uint32_t                      index;
    };

    struct c_object_layout {
        NGL_CHUCK::vm::script_object* object;
        uint32_t                      index;
        std::vector<c_script_layout>  scripts;
        ImVec2                        position;
        ImVec2                        size;
        uint32_t                      script_columns;
    };

    struct c_executable_layout {
        NGL_CHUCK::vm::script_executable* executable;
        std::vector<c_object_layout>      objects;
        ImVec2                            position;
        ImVec2                            size;
    };

    c_graph                                               m_graph;
    c_view                                                m_graph_view;
    c_graph_statistics                                    m_statistics {};
    std::vector<c_object_call>                            m_object_calls;
    std::unordered_set<NGL_CHUCK::vm::script_executable*> m_expanded_executables;
    std::unordered_set<NGL_CHUCK::vm::script_object*>     m_expanded_objects;
    NGL_CHUCK::vm::script_executable_tree*                m_cached_tree = nullptr;
    uint32_t                                              m_cached_tree_count = 0;
    node_id                                               m_cached_selection = 0;
    bool                                                  m_refresh_requested = true;
    bool                                                  m_layout_requested = true;
    bool                                                  m_show_all_calls = false;
    bool                                                  m_tree_frozen = false;

    inline node_id executable_node_id(
        NGL_CHUCK::vm::script_executable* executable) const {

        return ((node_id)(uintptr_t)executable << 2) | 1;
    }

    inline node_id object_node_id(
        NGL_CHUCK::vm::script_object* object) const {

        return ((node_id)(uintptr_t)object << 2) | 2;
    }

    inline node_id script_node_id(
        NGL_CHUCK::vm::vm_executable* script) const {

        return ((node_id)(uintptr_t)script << 2) | 3;
    }

    inline c_node_style executable_style() const {
        c_node_style style;

        style.title_color      = IM_COL32(227, 54, 54, 255);
        style.body_color       = IM_COL32(163, 42, 42, 255);
        style.border_thickness = 0.f;
        style.rounding         = 0.f;

        return style;
    }

    inline c_node_style object_style() const {
        c_node_style style;

        style.title_color      = IM_COL32(72, 132, 212, 255);
        style.body_color       = IM_COL32(40, 82, 138, 255);
        style.border_thickness = 0.f;
        style.rounding         = 0.f;

        return style;
    }

    inline c_node_style script_style() const {
        c_node_style style;

        style.title_color      = IM_COL32(69, 64, 77, 255);
        style.body_color       = IM_COL32(37, 34, 41, 255);
        style.border_thickness = 0.f;
        style.rounding         = 0.f;

        return style;
    }

    inline std::string executable_name(
        NGL_CHUCK::vm::script_executable* executable) const {

        if (!executable->name)
            return "<unnamed executable>";

        uint32_t length = (std::min)(executable->name_length, 160u);

        return std::string(executable->name, length);
    }

    inline bool two_word_operand(int kind) const {
        switch (kind) {
            case 0:
            case 1:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 16:
            case 17:
            case 18:
            case 25:
            case 26:
            case 29:
            case 30:
            case 31:
            case 33:
            case 34:
                return true;

            default:
                return false;
        }
    }

    inline bool decode_instruction(uint8_t* address, uint8_t* end, c_instruction_view* output) const {
        if (!address || !end || address >= end || end - address < 2)
            return false;

        uint16_t header = *(uint16_t*)address;
        int kind = ((header >> 2) & 0x3f) - 1;
        uint16_t* operands = (uint16_t*)(address + 2);
        uint32_t operand_words = 0;

        if (two_word_operand(kind)) {
            operand_words = 2;
        } else if (kind >= 2 && kind <= 5) {
            operand_words = 1;
        } else if (kind == 15 || kind == 32) {
            if (end - address < 4)
                return false;

            uint32_t count = operands[0];
            operand_words = kind == 15 ? count + 2 : count * 2 + 1;
        }

        uint32_t size = 2 * (operand_words + 1);

        if ((uint32_t)(end - address) < size)
            return false;

        output->operands = operands;
        output->operand_words = operand_words;
        output->size = size;
        output->operand_kind = kind;
        output->opcode = (header >> 8) & 0x7f;

        return true;
    }

    inline void collect_script_ends(
        NGL_CHUCK::vm::script_executable* executable,
        std::unordered_map<
            NGL_CHUCK::vm::vm_executable*,
            uint8_t*>* script_ends) const {

        if (
            !executable->code ||
            !executable->code_size ||
            !executable->objects)
            return;

        auto code_start = (uint32_t)executable->code;
        auto code_end   = code_start + executable->code_size;

        if (code_end < code_start)
            return;

        std::vector<NGL_CHUCK::vm::vm_executable*> scripts;

        for (uint32_t object_index = 0;
             object_index < executable->object_count;
             ++object_index) {

            NGL_CHUCK::vm::script_object* object =
                executable->objects[object_index];

            if (!object || !object->functions)
                continue;

            for (uint32_t script_index = 0;
                 script_index < object->function_count;
                 ++script_index) {

                NGL_CHUCK::vm::vm_executable* script =
                    object->functions[script_index];

                if (!script || !script->code)
                    continue;

                uint32_t script_address = (uint32_t)script->code;

                if (
                    script_address >= code_start &&
                    script_address < code_end)
                    scripts.push_back(script);
            }
        }

        std::sort(
            scripts.begin(),
            scripts.end(),
            [](NGL_CHUCK::vm::vm_executable* left,
               NGL_CHUCK::vm::vm_executable* right) {

                return (uint32_t)left->code < (uint32_t)right->code;
            });

        for (size_t index = 0; index < scripts.size(); ++index) {
            uint32_t script_start = (uint32_t)scripts[index]->code;
            uint32_t script_end = code_end;

            for (size_t next = index + 1; next < scripts.size(); ++next) {
                uint32_t candidate = (uint32_t)scripts[next]->code;

                if (candidate > script_start) {
                    script_end = candidate;
                    break;
                }
            }

            (*script_ends)[scripts[index]] = (uint8_t*)script_end;
        }
    }

    inline void collect_instruction_boundaries(
        NGL_CHUCK::vm::script_executable* executable,
        std::unordered_set<uint8_t*>* boundaries) const {

        if (!executable->code || !executable->code_size)
            return;

        uint8_t* cursor = executable->code;
        uint8_t* end = executable->code + executable->code_size;

        while (cursor < end) {
            boundaries->insert(cursor);

            c_instruction_view instruction {};

            if (!decode_instruction(cursor, end, &instruction))
                return;

            cursor += instruction.size;
        }
    }

    inline void add_object_call(
        std::vector<c_object_call>* calls,
        NGL_CHUCK::vm::script_object* source,
        NGL_CHUCK::vm::script_object* target) const {

        if (!source || !target || source == target)
            return;

        for (c_object_call& call : *calls) {
            if (call.source == source && call.target == target) {
                ++call.count;
                return;
            }
        }

        calls->push_back({ source, target, 1 });
    }

    inline void collect_script_calls(
        NGL_CHUCK::vm::script_executable* executable,
        NGL_CHUCK::vm::vm_executable* script,
        uint8_t* end,
        NGL_CHUCK::vm::script_object* fallback_source,
        const std::unordered_set<uint8_t*> &instruction_boundaries,
        const std::unordered_map<
            NGL_CHUCK::vm::vm_executable*,
            NGL_CHUCK::vm::script_object*> &function_owners,
        std::vector<c_object_call>* calls) const {

        uint8_t* cursor = script ? script->code : nullptr;

        if (!cursor || !end || cursor >= end)
            return;

        uint8_t* executable_end = executable->code + executable->code_size;

        if (
            !instruction_boundaries.contains(cursor) ||
            (end != executable_end &&
             !instruction_boundaries.contains(end)))
            return;

        NGL_CHUCK::vm::script_object* source =
            script->object ? script->object : fallback_source;

        while (cursor < end) {
            c_instruction_view instruction {};

            if (!decode_instruction(cursor, end, &instruction))
                return;

            // opcode 7 embeds a direct vm_executable pointer
            // it is the only call form that can be resolved back to another script object
            if (instruction.opcode == 7 && instruction.operand_kind == 7 && instruction.operand_words == 2) {
                uint32_t target_address =
                    ((uint32_t)instruction.operands[0] << 16) | instruction.operands[1];
                NGL_CHUCK::vm::vm_executable* target_script = (NGL_CHUCK::vm::vm_executable*)target_address;
                auto target = function_owners.find(target_script);

                if (target != function_owners.end())
                    add_object_call(calls, source, target->second);
            }

            cursor += instruction.size;
        }
    }

    inline void collect_function_owners(
        NGL_CHUCK::vm::script_executable_tree* tree,
        std::unordered_map<
            NGL_CHUCK::vm::vm_executable*,
            NGL_CHUCK::vm::script_object*>* function_owners) const {

        if (!tree || !tree->head)
            return;

        NGL_CHUCK::vm::script_executable_tree_node* end = tree->end();
        NGL_CHUCK::vm::script_executable_tree_node* tree_node = tree->begin();

        for (uint32_t tree_index = 0; tree_node != end && tree_index < tree->count; ++tree_index, tree_node = tree_node->successor()) {
            if (!tree_node || !tree_node->entry)
                break;

            NGL_CHUCK::vm::script_executable* executable = tree_node->get();

            if (!executable || !executable->objects)
                continue;

            for (uint32_t object_index = 0; object_index < executable->object_count; ++object_index) {
                NGL_CHUCK::vm::script_object* object =
                    executable->objects[object_index];

                if (!object || !object->functions)
                    continue;

                for (uint32_t script_index = 0; script_index < object->function_count; ++script_index) {
                    NGL_CHUCK::vm::vm_executable* script =
                        object->functions[script_index];

                    if (script)
                        (*function_owners)[script] =
                            script->object ? script->object : object;
                }
            }
        }
    }

    inline void add_executable_node(NGL_CHUCK::vm::script_executable* executable, const ImVec2 &position, bool expanded) {
        c_node &node = m_graph.add_node(
            executable_node_id(executable),
            position,
            ImVec2(270.0f, 108.0f),
            std::format("{} {}",
                expanded ? "[-]" : "[+]",
                executable_name(executable).c_str()),
            executable_style());

        node.rows.push_back(std::format("executable 0x{:08X}",
            (uint32_t)executable));
        node.rows.push_back(std::format("resource 0x{:08X} | objects {}",
            executable->resource_hash,
            executable->object_count));
        node.rows.push_back(std::format("code 0x{:08X} | size 0x{:X}",
            (uint32_t)executable->code,
            executable->code_size));
    }

    inline void add_object_node(NGL_CHUCK::vm::script_object* object, uint32_t object_index, const ImVec2 &position, bool expanded) {
        c_node &node = m_graph.add_node(
            object_node_id(object),
            position,
            ImVec2(280.0f, 108.0f),
            std::format("{} object [{}] 0x{:08X}",
                expanded ? "[-]" : "[+]",
                object_index,
                object->name_hash),
            object_style());

        node.rows.push_back(std::format("object 0x{:08X}",
            (uint32_t)object));
        node.rows.push_back(std::format("functions {} | flags 0x{:08X}",
            object->function_count,
            object->flags));
        node.rows.push_back(std::format("parent object 0x{:08X}",
            (uint32_t)object->parent));
    }

    inline void add_script_node(
        NGL_CHUCK::vm::script_executable* executable,
        NGL_CHUCK::vm::vm_executable* script,
        uint32_t script_index,
        const ImVec2 &position) {

        c_node& node = m_graph.add_node(
            script_node_id(script),
            position,
            ImVec2(300.0f, 108.0f),
            std::format("script [{}] 0x{:08X}", script_index, script->function_hash),
            script_style());

        node.rows.push_back(std::format("vm executable 0x{:08X}",
            (uint32_t)script));
        node.rows.push_back(std::format("signature 0x{:08X} | flags 0x{:04X}",
            script->signature_hash,
            script->flags));

        uint32_t code_start   = (uint32_t)executable->code;
        uint32_t script_start = (uint32_t)script->code;

        if (executable->code && script->code &&
            script_start >= code_start &&
            script_start - code_start <= executable->code_size) {

            node.rows.push_back(std::format("code +0x{:X} | stack 0x{:08X}",
                (uint32_t)(script_start - code_start),
                script->stack_metadata));
        } else {
            node.rows.push_back(std::format("code 0x{:08X} | stack 0x{:08X}",
                (uint32_t)script_start,
                script->stack_metadata));
        }
    }

    inline void add_object_call_connections() {
        node_id selected = m_graph_view.selected_node();

        for (const c_object_call &call : m_object_calls) {
            node_id source = object_node_id(call.source);
            node_id target = object_node_id(call.target);

            if (!m_graph.find_node(source) || !m_graph.find_node(target))
                continue;

            if (!m_show_all_calls &&
                selected != source &&
                selected != target)
                
                continue;

            ImU32 color;

            if (call.count >= 8)
                color = IM_COL32(239, 83, 80, 235);
            else if (call.count >= 3)
                color = IM_COL32(244, 146, 58, 235);
            else
                color = IM_COL32(242, 199, 78, 235);

            float thickness = 2.0f +
                (std::min)((float)call.count, 10.0f) * 0.35f;
            std::string label = std::format("{} direct call{}",
                call.count,
                call.count == 1 ? "" : "s");

            m_graph.add_connection(source, target, color, thickness, true, label);
        }
    }

    inline void refresh_object_call_cache(
        NGL_CHUCK::vm::script_executable_tree* tree) {

        m_object_calls.clear();

        std::unordered_map<
            NGL_CHUCK::vm::vm_executable*,
            NGL_CHUCK::vm::script_object*> function_owners;

        collect_function_owners(tree, &function_owners);

        if (!tree || !tree->head)
            return;

        NGL_CHUCK::vm::script_executable_tree_node* end       = tree->end();
        NGL_CHUCK::vm::script_executable_tree_node* tree_node = tree->begin();

        for (uint32_t tree_index = 0;
            tree_node != end && tree_index < tree->count;
            ++tree_index, tree_node = tree_node->successor()) {

            if (!tree_node || !tree_node->entry)
                break;

            NGL_CHUCK::vm::script_executable* executable = tree_node->get();

            if (!executable || !executable->objects)
                continue;

            std::unordered_set<uint8_t*> instruction_boundaries;
            std::unordered_map<
                NGL_CHUCK::vm::vm_executable*,
                uint8_t*> script_ends;

            collect_instruction_boundaries(
                executable,
                &instruction_boundaries);
            collect_script_ends(executable, &script_ends);

            for (uint32_t object_index = 0; object_index < executable->object_count; ++object_index) {
                NGL_CHUCK::vm::script_object* object = executable->objects[object_index];

                if (!object || !object->functions)
                    continue;

                for (uint32_t script_index = 0; script_index < object->function_count; ++script_index) {

                    NGL_CHUCK::vm::vm_executable* script = object->functions[script_index];
                    
                    auto script_end = script_ends.find(script);

                    if (!script || script_end == script_ends.end())
                        continue;

                    collect_script_calls(executable, script, script_end->second, object, instruction_boundaries, function_owners, &m_object_calls);
                }
            }
        }
    }

    inline c_object_layout make_object_layout(NGL_CHUCK::vm::script_object* object, uint32_t object_index) const {
        constexpr float object_width   = 280.0f;
        constexpr float script_width   = 300.0f;
        constexpr float node_height    = 108.0f;
        constexpr float script_start_y = 168.0f;
        constexpr float script_step_x  = 330.0f;
        constexpr float script_step_y  = 132.0f;

        c_object_layout layout {
            object,
            object_index,
            {},
            ImVec2(0.0f, 0.0f),
            ImVec2(object_width, node_height),
            0
        };

        if (!m_expanded_objects.contains(object) || !object->functions)
            return layout;

        for (uint32_t script_index = 0;
             script_index < object->function_count;
             ++script_index) {

            NGL_CHUCK::vm::vm_executable* script =
                object->functions[script_index];

            if (script)
                layout.scripts.push_back({ script, script_index });
        }

        if (layout.scripts.empty())
            return layout;

        float balanced_columns = std::ceil(std::sqrt(
            (float)layout.scripts.size() *
            script_step_y / script_step_x));

        layout.script_columns = std::clamp(
            (uint32_t)balanced_columns,
            1u,
            20u);

        uint32_t rows = ((uint32_t)layout.scripts.size() + layout.script_columns - 1) / layout.script_columns;

        float script_grid_width =
            (layout.script_columns - 1) * script_step_x + script_width;

        layout.size.x = (std::max)(object_width, script_grid_width);
        layout.size.y = script_start_y +
            (rows - 1) * script_step_y + node_height;

        return layout;
    }

    inline void make_executable_layouts(NGL_CHUCK::vm::script_executable_tree* tree, std::vector<c_executable_layout>* layouts) {
        constexpr float collapsed_width  = 330.0f;
        constexpr float collapsed_height = 160.0f;
        constexpr float content_width    = 5200.0f;
        constexpr float content_start_y  = 200.0f;
        constexpr float content_margin   = 80.0f;
        constexpr float object_gap       = 60.0f;

        if (!tree || !tree->head)
            return;

        NGL_CHUCK::vm::script_executable_tree_node* end       = tree->end();
        NGL_CHUCK::vm::script_executable_tree_node* tree_node = tree->begin();

        for (uint32_t tree_index = 0; tree_node != end && tree_index < tree->count; ++tree_index, tree_node = tree_node->successor()) {
            if (!tree_node || !tree_node->entry)
                break;

            NGL_CHUCK::vm::script_executable* executable = tree_node->get();

            if (!executable)
                continue;

            c_executable_layout layout {
                executable,
                {},
                ImVec2(0.0f, 0.0f),
                ImVec2(collapsed_width, collapsed_height)
            };

            ++m_statistics.executable_count;

            bool expanded    = m_expanded_executables.contains(executable);
            float cursor_x   = 0.0f;
            float cursor_y   = 0.0f;
            float row_height = 0.0f;
            float used_width = 0.0f;

            if (executable->objects) {
                for (uint32_t object_index = 0; object_index < executable->object_count; ++object_index) {
                    NGL_CHUCK::vm::script_object* object = executable->objects[object_index];

                    if (!object)
                        continue;

                    ++m_statistics.object_count;

                    if (object->functions) {
                        for (uint32_t script_index = 0; script_index < object->function_count; ++script_index) {
                            if (object->functions[script_index])
                                ++m_statistics.script_count;
                        }
                    }

                    if (!expanded)
                        continue;

                    c_object_layout object_layout = make_object_layout(object, object_index);

                    if (cursor_x > 0.0f && cursor_x + object_layout.size.x > content_width) {
                        cursor_x   = 0.0f;
                        cursor_y  += row_height + object_gap;
                        row_height = 0.0f;
                    }

                    object_layout.position = ImVec2(cursor_x, cursor_y);
                    cursor_x += object_layout.size.x + object_gap;
                    row_height = (std::max)(row_height, object_layout.size.y);
                    used_width = (std::max)(used_width, cursor_x - object_gap);
                    
                    layout.objects.push_back(std::move(object_layout));
                }
            }

            if (expanded) {
                float content_height = layout.objects.empty()?
                    0.0f : cursor_y + row_height;

                layout.size.x = (std::max)(
                    collapsed_width,
                    used_width + content_margin * 2.0f);
                layout.size.y = (std::max)(
                    collapsed_height,
                    content_start_y + content_height + content_margin);
            }

            layouts->push_back(std::move(layout));
        }
    }

    inline void pack_executable_layouts(std::vector<c_executable_layout>* layouts) const {
        constexpr float world_width = 7200.0f;
        constexpr float cluster_gap = 180.0f;

        float cursor_x   = 0.0f;
        float cursor_y   = 0.0f;
        float row_height = 0.0f;

        for (c_executable_layout& layout : *layouts) {
            if (cursor_x > 0.0f &&
                cursor_x + layout.size.x > world_width) {

                cursor_x   = 0.0f;
                cursor_y  += row_height + cluster_gap;
                row_height = 0.0f;
            }

            layout.position = ImVec2(cursor_x, cursor_y);
            cursor_x += layout.size.x + cluster_gap;
            row_height = (std::max)(row_height, layout.size.y);
        }
    }

    inline void rebuild_graph(NGL_CHUCK::vm::script_executable_tree* tree) {
        constexpr float executable_width = 270.0f;
        constexpr float object_width     = 280.0f;
        constexpr float script_width     = 300.0f;
        constexpr float content_start_x  = 80.0f;
        constexpr float content_start_y  = 200.0f;
        constexpr float script_start_y   = 168.0f;
        constexpr float script_step_x    = 330.0f;
        constexpr float script_step_y    = 132.0f;

        m_graph = c_graph();
        m_statistics = {};

        std::vector<c_executable_layout> layouts;
        make_executable_layouts(tree, &layouts);
        pack_executable_layouts(&layouts);

        for (const c_executable_layout& executable_layout : layouts) {
            NGL_CHUCK::vm::script_executable* executable = executable_layout.executable;

            bool executable_expanded = m_expanded_executables.contains(executable);
            
            ImVec2 executable_position(
                executable_layout.position.x +
                    (executable_layout.size.x - executable_width) * 0.5f,
                executable_layout.position.y + 40.0f);

            add_executable_node(executable, executable_position, executable_expanded);

            for (const c_object_layout &object_layout : executable_layout.objects) {
                NGL_CHUCK::vm::script_object* object = object_layout.object;

                bool object_expanded = m_expanded_objects.contains(object);

                ImVec2 block_position(
                    executable_layout.position.x + content_start_x +
                        object_layout.position.x,
                    executable_layout.position.y + content_start_y +
                        object_layout.position.y);

                ImVec2 object_position(
                    block_position.x +
                        (object_layout.size.x - object_width) * 0.5f,
                    block_position.y);

                add_object_node(object, object_layout.index, object_position, object_expanded);

                ++m_statistics.visible_object_count;

                NGL_CHUCK::vm::script_executable* owner_executable = object->executable ?
                    object->executable : executable;

                m_graph.add_connection(
                    object_node_id(object),
                    executable_node_id(owner_executable),
                    IM_COL32(63, 181, 222, 210),
                    3.5f);

                for (size_t layout_index = 0;
                     layout_index < object_layout.scripts.size();
                     ++layout_index) {

                    const c_script_layout &script_layout =
                        object_layout.scripts[layout_index];
                    uint32_t column =
                        (uint32_t)layout_index % object_layout.script_columns;
                    uint32_t row =
                        (uint32_t)layout_index / object_layout.script_columns;
                    float script_grid_width =
                        (object_layout.script_columns - 1) * script_step_x +
                        script_width;
                    ImVec2 script_position(
                        block_position.x +
                            (object_layout.size.x - script_grid_width) * 0.5f +
                            column * script_step_x,
                        block_position.y + script_start_y +
                            row * script_step_y);

                    add_script_node(executable, script_layout.script, script_layout.index, script_position);

                    ++m_statistics.visible_script_count;

                    NGL_CHUCK::vm::script_object* owner_object = script_layout.script->object ?
                        script_layout.script->object : object;

                    m_graph.add_connection(
                        script_node_id(script_layout.script),
                        object_node_id(owner_object),
                        IM_COL32(112, 211, 157, 190),
                        1.75f);
                }
            }
        }

        add_object_call_connections();

        m_statistics.object_call_count = (uint32_t)m_object_calls.size();
    }

    inline void toggle_expansion(node_id node) {
        uint32_t type    = (uint32_t)(node & 3);
        uint32_t address = (uint32_t)(node >> 2);

        if (type == 1) {
            NGL_CHUCK::vm::script_executable* executable =
                (NGL_CHUCK::vm::script_executable*)address;

            if (m_expanded_executables.contains(executable))
                m_expanded_executables.erase(executable);
            else
                m_expanded_executables.insert(executable);

            m_layout_requested = true;
        } else if (type == 2) {
            NGL_CHUCK::vm::script_object* object =
                (NGL_CHUCK::vm::script_object*)address;

            if (m_expanded_objects.contains(object))
                m_expanded_objects.erase(object);
            else
                m_expanded_objects.insert(object);

            m_layout_requested = true;
        }
    }

    inline void draw_legend() const {
        ImGui::TextColored(
            ImVec4(0.25f, 0.71f, 0.87f, 1.0f),
            "---");
        ImGui::SameLine();
        ImGui::TextUnformatted("object -> executable");
        ImGui::SameLine(0.0f, 18.0f);
        ImGui::TextColored(
            ImVec4(0.44f, 0.83f, 0.62f, 1.0f),
            "---");
        ImGui::SameLine();
        ImGui::TextUnformatted("script -> object");
        ImGui::SameLine(0.0f, 18.0f);
        ImGui::TextColored(
            ImVec4(0.95f, 0.65f, 0.25f, 1.0f),
            "---");
        ImGui::SameLine();
        ImGui::TextUnformatted(m_show_all_calls ?
            "all visible cross-object calls (thickness = count)" :
            "selected object's cross-object calls (thickness = count)");
    }

public:
    bool logic() { return true; }

    void draw() {
        ImGui::SetNextWindowSize(ImVec2(1180.0f, 760.0f), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin("chuckvm")) {
            ImGui::End();

            return;
        }

        if (ImGui::Button("frame all"))
            m_graph_view.request_frame();

        ImGui::SameLine();

        if (ImGui::Button("reset layout"))
            m_graph_view.reset_layout();

        ImGui::SameLine();

        if (ImGui::Button("refresh"))
            m_refresh_requested = true;

        ImGui::SameLine();

        if (ImGui::Button("collapse all")) {
            m_expanded_executables.clear();
            m_expanded_objects.clear();
            m_layout_requested = true;
        }

        ImGui::SameLine();

        if (ImGui::Checkbox("all call edges", &m_show_all_calls))
            m_layout_requested = true;

        ImGui::SameLine();

        ImGui::Checkbox("freeze tree", &m_tree_frozen);

        ImGui::TextDisabled("double-click: expand | wheel: zoom | middle/right: pan | left: move | %.1f%%",
            m_graph_view.zoom() * 100.0f);

        auto runtime = NGL_CHUCK::script_runtime.read();

        if (!runtime) {
            ImGui::TextDisabled("script runtime is null");
            ImGui::End();
            
            return;
        }

        {
            NGL_MUTEX::engine_lock_scope runtime_scope(&runtime->loaded_executables_lock);
            NGL_CHUCK::vm::script_executable_tree* tree = runtime->loaded_executables;

            if (!tree) {
                ImGui::TextDisabled("loaded executable tree is null");
                ImGui::End();
                
                return;
            }

            bool tree_changed =
                tree        != m_cached_tree ||
                tree->count != m_cached_tree_count;

            if (tree_changed && m_cached_tree && !m_tree_frozen) {
                m_expanded_executables.clear();
                m_expanded_objects.clear();
                m_graph_view.reset_layout();
            }

            if (m_refresh_requested || (tree_changed && !m_tree_frozen)) {
                refresh_object_call_cache(tree);
                m_layout_requested = true;
                m_refresh_requested = false;
            }

            if (m_layout_requested) {
                rebuild_graph(tree);
                m_layout_requested = false;
            }

            if (!m_tree_frozen) {
                m_cached_tree       = tree;
                m_cached_tree_count = tree->count;
            }
        }

        ImGui::Text("%u executables | %u/%u objects visible | %u/%u scripts visible | %u cross-object call paths",
            m_statistics.executable_count,
            m_statistics.visible_object_count,
            m_statistics.object_count,
            m_statistics.visible_script_count,
            m_statistics.script_count,
            m_statistics.object_call_count);

        draw_legend();

        if (m_graph.empty()) {
            ImGui::TextDisabled("no loaded script nodes");
        } else {
            m_graph_view.draw("##script_graph_canvas", m_graph);

            node_id double_clicked = m_graph_view.take_double_clicked_node();

            if (double_clicked) {
                toggle_expansion(double_clicked);
                m_graph_view.request_focus(double_clicked);
            }

            node_id selection = m_graph_view.selected_node();

            if (selection != m_cached_selection) {
                m_cached_selection = selection;

                if (!m_show_all_calls)
                    m_layout_requested = true;
            }
        }

        ImGui::End();
    }
};

#ifndef NDEBUG
    gui_script_node_view gui {};
#endif
