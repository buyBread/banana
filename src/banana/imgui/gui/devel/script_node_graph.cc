/*
    VIBECODED: either refactor or replace
*/

#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <chrono>
#include <cstring>

#include "banana/imgui/manager.hh"
#include "banana/imgui/ext/node_graph.hh"
#include "treyarch/chuck/chuck.hh"

using namespace banana;

class gui_script_node_view : public i_gui {
    using node_id                  = imgui::ext::node_graph::node_id;
    using c_graph                  = imgui::ext::node_graph::c_graph;
    using c_node                   = imgui::ext::node_graph::c_node;
    using c_node_style             = imgui::ext::node_graph::c_node_style;
    using c_connection             = imgui::ext::node_graph::c_connection;
    using c_layered_layout_options = imgui::ext::node_graph::c_layered_layout_options;
    using c_row_layout_options     = imgui::ext::node_graph::c_row_layout_options;
    using c_view                   = imgui::ext::node_graph::c_view;
    
    using e_opcode                 = treyarch::chuck::vm::e_opcode;

    static constexpr uint32_t invalid_index = UINT32_MAX;
    static constexpr uint32_t builtin_native_class_count = 32;
    static constexpr uint32_t builtin_native_function_count = 1934;

    enum class e_call_kind : uint8_t {
        script,
        native
    };

    enum class e_indirect_control_kind : uint8_t {
        receiver_dispatch,
        target_thread,
        instance_callback,
        current_callback
    };

    enum class e_graph_edge_kind : uint8_t {
        script_outgoing,
        script_incoming,
        native
    };

    struct c_instruction_view {
        uint16_t* operands;
        uint32_t  operand_words;
        uint32_t  size;
        int32_t   operand_kind;
        e_opcode  opcode;
    };

    struct c_native_coordinate {
        uint32_t class_index;
        uint32_t function_index;
    };

    struct c_native_function_snapshot {
        uint32_t    class_index;
        uint32_t    function_index;
        uint32_t    address;
        uint32_t    callback;
        std::string type_name;
    };

    struct c_native_class_snapshot {
        uint32_t    index;
        uint32_t    address;
        int32_t     value_size;
        uint32_t    first_function;
        uint32_t    function_count;
        std::string type_name;
        std::string parent_name;
    };

    struct c_call_snapshot {
        e_call_kind kind;
        uint32_t    source_executable;
        uint32_t    source_object;
        uint32_t    source_function;
        uint32_t    code_offset;
        uint32_t    target_address;
        uint32_t    native_class_index;
        uint32_t    native_function_index;
    };

    struct c_indirect_control_snapshot {
        e_indirect_control_kind kind;
        e_opcode                opcode;
        uint32_t                source_executable;
        uint32_t                source_object;
        uint32_t                source_function;
        uint32_t                code_offset;
        uint32_t                prototype_address;
        uint32_t                signature_hash;
        uint32_t                candidate_count;
        uint32_t                cross_root_candidate_count;
    };

    struct c_function_snapshot {
        uint32_t executable_index;
        uint32_t object_index;
        uint32_t local_index;
        uint32_t alias_ordinal;
        uint32_t alias_count;
        uint32_t alias_object_count;
        uint32_t object_alias_ordinal;
        uint32_t object_alias_count;
        uint32_t address;
        uint32_t code_address;
        uint32_t code_offset;
        uint32_t code_size;
        uint32_t signature_hash;
        uint32_t function_hash;
        uint32_t stack_metadata;
        uint16_t flags;
        uint32_t first_call;
        uint32_t call_count;
        uint32_t first_indirect_control;
        uint32_t indirect_control_count;
        uint32_t script_call_count;
        uint32_t native_call_count;
        uint32_t live_thread_count;
    };

    struct c_thread_snapshot {
        uint32_t executable_index;
        uint32_t object_index;
        uint32_t instance_address;
        uint32_t address;
        uint32_t id;
        uint32_t function_address;
        uint32_t parent_address;
        uint32_t pc;
        uint32_t current_instance;
        uint32_t constructor_context;
        uint32_t stack_cursor;
        uint32_t stack_allocation;
        uint32_t stack_data;
        uint32_t stack_capacity;
        uint32_t stack_size;
        uint32_t native_recall;
        uint32_t flow_depth;
    };

    struct c_instance_snapshot {
        uint32_t executable_index;
        uint32_t object_index;
        uint32_t address;
        uint32_t flags;
        uint32_t first_thread;
        uint32_t thread_count;
        uint32_t reported_thread_count;
    };

    struct c_object_snapshot {
        uint32_t executable_index;
        uint32_t local_index;
        uint32_t address;
        uint32_t name_hash;
        uint32_t parent_address;
        uint32_t flags;
        uint32_t first_function;
        uint32_t function_count;
        uint32_t first_instance;
        uint32_t instance_count;
        uint32_t live_thread_count;
        uint32_t function_body_count;
        uint32_t function_alias_count;
        uint32_t script_call_count;
        uint32_t native_call_count;
    };

    struct c_executable_snapshot {
        uint32_t    address;
        uint32_t    resource_hash;
        uint32_t    flags;
        uint32_t    runtime_state;
        uint32_t    code_address;
        uint32_t    code_size;
        uint32_t    first_object;
        uint32_t    object_count;
        uint32_t    first_function;
        uint32_t    function_count;
        uint32_t    first_instance;
        uint32_t    instance_count;
        uint32_t    first_thread;
        uint32_t    thread_count;
        uint32_t    function_body_count;
        uint32_t    function_alias_count;
        uint32_t    script_call_count;
        uint32_t    native_call_count;
        uint32_t    indirect_control_count;
        std::string name;
        std::vector<uint8_t> code_image;
    };

    struct c_snapshot {
        uint32_t runtime_address;
        uint32_t tree_address;
        uint32_t registry_address;
        uint32_t reported_executable_count;
        uint32_t function_body_count;
        uint32_t function_alias_count;
        uint32_t script_call_count;
        uint32_t native_call_count;
        uint32_t indirect_control_count;
        uint32_t appended_native_class_count;
        uint32_t appended_native_function_count;
        uint32_t external_native_class_count;
        uint32_t external_native_function_count;
        uint32_t unresolved_native_class_rtti_count;
        uint32_t unresolved_native_function_rtti_count;
        uint64_t code_image_bytes;
        double   native_snapshot_milliseconds;
        double   runtime_lock_wait_milliseconds;
        double   runtime_lock_milliseconds;
        double   code_copy_milliseconds;
        double   instance_thread_snapshot_milliseconds;
        double   post_lock_analysis_milliseconds;
        double   capture_milliseconds;
        bool     runtime_available;

        std::vector<c_executable_snapshot>  executables;
        std::vector<c_object_snapshot>      objects;
        std::vector<c_function_snapshot>    functions;
        std::vector<c_instance_snapshot>    instances;
        std::vector<c_thread_snapshot>      threads;
        std::vector<c_call_snapshot>        calls;
        std::vector<c_indirect_control_snapshot> indirect_controls;
        std::vector<c_native_class_snapshot> native_classes;
        std::vector<c_native_function_snapshot> native_functions;
    };

    struct c_call_chain_step {
        uint32_t function_index;
        uint32_t call_offset;
    };

    struct c_reachable_script_body {
        uint32_t snapshot_function_index;
        uint32_t depth;
        uint32_t recursion_group;
        uint32_t recursion_group_size;
        std::vector<c_call_chain_step> path;
    };

    struct c_reachable_native {
        uint32_t address;
        uint32_t class_index;
        uint32_t function_index;
        uint32_t call_depth;
        uint32_t callsite_count;
        std::vector<c_call_chain_step> path;
    };

    struct c_transitive_analysis {
        uint64_t snapshot_number;
        uint32_t source_function;
        uint32_t reachable_script_callsite_count;
        uint32_t reachable_native_callsite_count;
        uint32_t unresolved_script_callsite_count;
        uint32_t recursive_body_count;
        uint32_t recursion_group_count;
        bool     available;
        bool     source_recursive;
        double   milliseconds;

        std::vector<c_reachable_script_body> script_bodies;
        std::vector<c_reachable_native>      natives;
    };

    c_snapshot m_snapshot {};
    c_graph    m_graph;
    c_view     m_graph_view;

    uint32_t m_selected_executable = 0;
    uint32_t m_selected_function   = 0;
    uint32_t m_selected_native_class = 0;
    uint32_t m_graph_visible_object_count = 0;
    uint32_t m_graph_hidden_object_count = 0;
    uint32_t m_graph_external_object_count = 0;
    uint32_t m_graph_parent_edge_count = 0;
    uint32_t m_graph_call_edge_count = 0;
    uint32_t m_graph_self_call_count = 0;
    bool   m_refresh_requested = true;
    bool   m_auto_refresh      = false;
    bool   m_show_script_calls = true;
    bool   m_show_native_calls = true;
    bool   m_focus_selection   = true;
    float  m_refresh_interval  = 1.0f;
    double m_last_refresh      = 0.0;
    double m_max_runtime_lock_wait_milliseconds = 0.0;
    double m_max_runtime_lock_milliseconds = 0.0;
    double m_max_capture_milliseconds = 0.0;
    uint64_t m_snapshot_count = 0;

    c_transitive_analysis m_transitive_analysis {};
    uint32_t m_selected_transitive_script = 0;
    uint32_t m_selected_transitive_native = 0;
    uint32_t m_selected_indirect_control = invalid_index;

    char m_function_filter[96] {};
    char m_thread_filter[96] {};
    char m_native_filter[96] {};

    [[nodiscard]] inline node_id make_node_id(uint32_t address, uint32_t kind) const {
        return ((node_id)address << 3) | kind;
    }

    [[nodiscard]] inline node_id executable_node_id(uint32_t address) const {
        return make_node_id(address, 1);
    }

    [[nodiscard]] inline node_id object_node_id(uint32_t address) const {
        return make_node_id(address, 2);
    }

    [[nodiscard]] inline node_id native_class_node_id(uint32_t address) const {
        return make_node_id(address, 3);
    }

    [[nodiscard]] inline node_id unresolved_native_node_id(uint32_t address) const {
        return make_node_id(address, 4);
    }

    [[nodiscard]] inline uint32_t graph_node_kind(node_id node) const {
        return (uint32_t)(node & 7);
    }

    [[nodiscard]] inline uint32_t graph_node_address(node_id node) const {
        return (uint32_t)(node >> 3);
    }

    [[nodiscard]] inline c_node_style executable_style() const {
        c_node_style style;

        style.title_color = IM_COL32(203, 67, 72, 255);
        style.body_color  = IM_COL32(105, 42, 47, 255);

        return style;
    }

    [[nodiscard]] inline c_node_style object_style(bool external) const {
        c_node_style style;

        style.title_color = external ?
            IM_COL32(72, 92, 124, 255) :
            IM_COL32(62, 125, 194, 255);
        style.body_color = external ?
            IM_COL32(37, 49, 68, 255) :
            IM_COL32(35, 70, 112, 255);

        return style;
    }

    [[nodiscard]] inline c_node_style native_style(bool unresolved = false) const {
        c_node_style style;

        style.title_color = unresolved ?
            IM_COL32(120, 91, 57, 255) :
            IM_COL32(115, 76, 155, 255);
        style.body_color = unresolved ?
            IM_COL32(66, 49, 31, 255) :
            IM_COL32(61, 40, 83, 255);

        return style;
    }

    [[nodiscard]] inline size_t readable_span(const void* address) const {
        if (!address)
            return 0;

        MEMORY_BASIC_INFORMATION information {};

        if (!VirtualQuery(address, &information, sizeof(information)))
            return 0;

        if (information.State != MEM_COMMIT ||
            information.Protect & PAGE_GUARD ||
            information.Protect & PAGE_NOACCESS)
            return 0;

        uint32_t start = (uint32_t)address;
        uint32_t end   = (uint32_t)information.BaseAddress + information.RegionSize;

        return end > start ? (size_t)(end - start) : 0;
    }

    [[nodiscard]] inline bool in_main_module(const void* address, size_t size) const {
        HMODULE module = GetModuleHandleA(nullptr);

        if (!module || !address)
            return false;

        IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)module;

        if (dos->e_magic != IMAGE_DOS_SIGNATURE)
            return false;

        IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)((uint8_t*)module + dos->e_lfanew);

        if (nt->Signature != IMAGE_NT_SIGNATURE)
            return false;

        uint32_t module_start = (uint32_t)module;
        uint32_t module_end   = module_start + nt->OptionalHeader.SizeOfImage;

        uint32_t start = (uint32_t)address;

        return
            start >= module_start &&
            start <= module_end &&
            size <= module_end - start;
    }

    [[nodiscard]] inline std::string copy_string(const char* value, uint32_t maximum_length = 192) const {
        if (!value || !maximum_length)
            return {};

        size_t available = (std::min)(readable_span(value), (size_t)maximum_length);
        size_t length = 0;

        while (length < available && value[length])
            ++length;

        return std::string(value, length);
    }

    [[nodiscard]] inline std::string rtti_type_name(const void* object) const {
        if (!object || readable_span(object) < sizeof(uint32_t))
            return "<invalid>";

        uint32_t vtable = *(uint32_t*)object;

        if (!in_main_module((void*)vtable, sizeof(uint32_t)) ||
            !in_main_module((void*)(vtable - sizeof(uint32_t)), sizeof(uint32_t)))
            return "<external>";

        uint32_t locator = *(uint32_t*)(vtable - sizeof(uint32_t));

        if (!in_main_module((void*)locator, 0x14))
            return "<unknown>";

        uint32_t type_descriptor = *(uint32_t*)(locator + 0x0C);

        if (!in_main_module((void*)type_descriptor, 9))
            return "<unknown>";

        std::string name = copy_string((const char*)(type_descriptor + 8), 256);

        if (name.starts_with(".?AV") || name.starts_with(".?AU"))
            name.erase(0, 4);

        while (!name.empty() && name.ends_with('@'))
            name.pop_back();

        for (char &character : name) {
            if (character == '@')
                character = ':';
        }

        return name.empty() ? "<unknown>" : name;
    }

    [[nodiscard]] inline const char* builtin_class_name(uint32_t index) const {
        static constexpr std::array<const char*, 32> names {
            "script_library_class",
            "slc_num_t",
            "slc_str_t",
            "slc_uint_t",
            "slc_sound_response_t",
            "slc_beam_t",
            "slc_entity_t",
            "slc_player_entity_t",
            "slc_entity_tracker_t",
            "slc_line_info_t",
            "slc_polytube_t",
            "slc_script_controller_t",
            "slc_arch_base_t",
            "slc_timer_widget_t",
            "slc_trigger_t",
            "slc_vector3d_t",
            "slc_cpu_combat_movement_t",
            "slc_generic_event_callback_t",
            "slc_mutex_t",
            "slc_widget_3d_t",
            "slc_water_box_t",
            "slc_ai_path_t",
            "slc_cut_scene_t",
            "slc_web_wall_t",
            "slc_obstacle_t",
            "slc_debug_menu_t",
            "slc_debug_menu_entry_t",
            "slc_city_life_tracker_t",
            "slc_mission_objective_data_t",
            "slc_mission_stage_data_t",
            "slc_finger_of_god_t",
            "slc_fight_group_t"
        };

        return index < names.size() ? names[index] : "<appended class>";
    }

    [[nodiscard]] inline bool two_word_operand(int32_t kind) const {
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

    [[nodiscard]] inline bool decode_instruction(
        uint8_t* address,
        uint8_t* end,
        c_instruction_view* output) const {

        if (!address || !end || !output || address >= end || end - address < 2)
            return false;

        uint16_t header = *(uint16_t*)address;
        int32_t kind = ((header >> 2) & 0x3F) - 1;
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
        output->opcode = (e_opcode)((header >> 8) & 0x7F);

        return true;
    }

    inline void snapshot_native_registry(
        c_snapshot* snapshot,
        std::unordered_map<uint32_t, c_native_coordinate>* function_lookup) const {

        treyarch::chuck::script_library::script_library_registry* registry =
            treyarch::chuck::script_library::references::script_library_registry.read();

        snapshot->registry_address = (uint32_t)registry;

        if (!registry || !registry->m_classes)
            return;

        uint32_t class_count = registry->class_count();

        if (class_count > 4096)
            return;

        snapshot->appended_native_class_count =
            class_count > builtin_native_class_count ?
                class_count - builtin_native_class_count : 0;

        for (uint32_t class_index = 0; class_index < class_count; ++class_index) {
            treyarch::chuck::script_library::script_library_class* native_class =
                registry->find_class(class_index);

            if (!native_class)
                continue;

            c_native_class_snapshot class_snapshot {};

            class_snapshot.index = class_index;
            class_snapshot.address = (uint32_t)native_class;
            class_snapshot.value_size = native_class->value_size();
            class_snapshot.first_function = (uint32_t)snapshot->native_functions.size();
            class_snapshot.type_name = rtti_type_name(native_class);
            class_snapshot.parent_name = copy_string(native_class->m_parent_name);

            if (class_snapshot.type_name == "<external>")
                ++snapshot->external_native_class_count;
            else if (class_snapshot.type_name == "<unknown>" ||
                class_snapshot.type_name == "<invalid>")
                ++snapshot->unresolved_native_class_rtti_count;

            if (class_snapshot.type_name == "<unknown>" ||
                class_snapshot.type_name == "<invalid>")
                class_snapshot.type_name = builtin_class_name(class_index);

            uint32_t function_count = native_class->function_count();

            if (function_count > 65536)
                function_count = 0;

            for (uint32_t function_index = 0; function_index < function_count; ++function_index) {
                treyarch::chuck::script_library::script_library_function* function =
                    native_class->find_function(function_index);

                if (!function)
                    continue;

                c_native_function_snapshot function_snapshot {};

                function_snapshot.class_index = class_index;
                function_snapshot.function_index = function_index;
                function_snapshot.address = (uint32_t)function;
                function_snapshot.callback = (uint32_t)function->callback();
                function_snapshot.type_name = rtti_type_name(function);

                if (function_snapshot.type_name == "<external>")
                    ++snapshot->external_native_function_count;
                else if (function_snapshot.type_name == "<unknown>" ||
                    function_snapshot.type_name == "<invalid>")
                    ++snapshot->unresolved_native_function_rtti_count;

                (*function_lookup)[function_snapshot.address] = {
                    class_index,
                    function_index
                };

                snapshot->native_functions.push_back(std::move(function_snapshot));
            }

            class_snapshot.function_count =
                (uint32_t)snapshot->native_functions.size() -
                class_snapshot.first_function;

            snapshot->native_classes.push_back(std::move(class_snapshot));
        }

        uint32_t function_count = (uint32_t)snapshot->native_functions.size();

        snapshot->appended_native_function_count =
            function_count > builtin_native_function_count ?
                function_count - builtin_native_function_count : 0;
    }

    inline void snapshot_instance_threads(
        c_snapshot* snapshot,
        treyarch::chuck::vm::script_object* object,
        uint32_t executable_index,
        uint32_t object_index) const {

        if (!object || !object->instance_lock)
            return;

        treyarch::ref_lock_scope object_scope(object->instance_lock);
        treyarch::chuck::vm::script_instance* instance = object->first_instance;

        for (uint32_t instance_ordinal = 0;
            instance && instance_ordinal < 65536;
            ++instance_ordinal) {

            c_instance_snapshot instance_snapshot {};

            instance_snapshot.executable_index = executable_index;
            instance_snapshot.object_index = object_index;
            instance_snapshot.address = (uint32_t)instance;
            instance_snapshot.flags = instance->flags;
            instance_snapshot.first_thread = (uint32_t)snapshot->threads.size();

            treyarch::chuck::vm::script_instance* next_instance = instance->next;

            {
                treyarch::engine_lock_scope instance_scope(&instance->thread_lock);

                instance_snapshot.reported_thread_count = instance->thread_count;

                treyarch::chuck::vm::vm_thread* thread = instance->first_thread;
                uint32_t maximum_threads = instance->thread_count >= 65535 ?
                    65536u : instance->thread_count + 1;

                for (uint32_t thread_ordinal = 0; thread && thread_ordinal < maximum_threads; ++thread_ordinal) {
                    c_thread_snapshot thread_snapshot {};

                    thread_snapshot.executable_index    = executable_index;
                    thread_snapshot.object_index        = object_index;
                    thread_snapshot.instance_address    = (uint32_t)instance;
                    thread_snapshot.address             = (uint32_t)thread;
                    thread_snapshot.id                  = thread->id;
                    thread_snapshot.function_address    = (uint32_t)thread->entry_function;
                    thread_snapshot.parent_address      = (uint32_t)thread->creator;
                    thread_snapshot.pc                  = (uint32_t)thread->pc;
                    thread_snapshot.current_instance    = (uint32_t)thread->current_instance;
                    thread_snapshot.constructor_context = (uint32_t)thread->constructor_context;
                    thread_snapshot.stack_cursor        = (uint32_t)thread->stack.cursor;
                    thread_snapshot.stack_allocation    = (uint32_t)thread->stack.allocation;
                    thread_snapshot.stack_data          = (uint32_t)thread->stack.data;
                    thread_snapshot.stack_capacity      = thread->stack.capacity;
                    thread_snapshot.stack_size          = thread->stack.size();
                    thread_snapshot.native_recall       = thread->native_recall;

                    treyarch::chuck::vm::vm_flow_frame* frame = thread->flow_frame;

                    while (frame && thread_snapshot.flow_depth < 256) {
                        ++thread_snapshot.flow_depth;
                        frame = frame->previous;
                    }

                    snapshot->threads.push_back(thread_snapshot);
                    thread = thread->next;
                }
            }

            instance_snapshot.thread_count =
                (uint32_t)snapshot->threads.size() -
                instance_snapshot.first_thread;

            snapshot->instances.push_back(instance_snapshot);
            instance = next_instance;
        }
    }

    inline void snapshot_runtime_objects_locked(
        c_snapshot* snapshot,
        treyarch::chuck::vm::script_manager* runtime) const {
        treyarch::chuck::vm::script_executable_tree* tree = runtime->loaded_executables;

        snapshot->tree_address = (uint32_t)tree;

        if (!tree || !tree->head || tree->count > 4096)
            return;

        snapshot->reported_executable_count = tree->count;

        treyarch::chuck::vm::script_executable_tree_node* end = tree->end();
        treyarch::chuck::vm::script_executable_tree_node* tree_node = tree->begin();

        for (uint32_t tree_index = 0;
            tree_node != end && tree_index < tree->count;
            ++tree_index, tree_node = tree_node->successor()) {

            if (!tree_node || !tree_node->entry)
                break;

            treyarch::chuck::vm::script_executable* executable = tree_node->get();

            if (!executable)
                continue;

            uint32_t executable_index = (uint32_t)snapshot->executables.size();
            c_executable_snapshot executable_snapshot {};

            executable_snapshot.address = (uint32_t)executable;
            executable_snapshot.resource_hash = executable->resource_hash;
            executable_snapshot.flags = executable->flags;
            executable_snapshot.runtime_state = executable->runtime_state;
            executable_snapshot.code_address = (uint32_t)executable->code;
            executable_snapshot.code_size = executable->code_size;
            executable_snapshot.first_object = (uint32_t)snapshot->objects.size();
            executable_snapshot.first_function = (uint32_t)snapshot->functions.size();
            executable_snapshot.first_instance = (uint32_t)snapshot->instances.size();
            executable_snapshot.first_thread = (uint32_t)snapshot->threads.size();

            auto code_copy_started = std::chrono::steady_clock::now();

            if (executable_snapshot.code_address &&
                executable_snapshot.code_size &&
                readable_span((void*)executable_snapshot.code_address) >=
                    executable_snapshot.code_size) {
                executable_snapshot.code_image.resize(executable_snapshot.code_size);
                std::memcpy(
                    executable_snapshot.code_image.data(),
                    (void*)executable_snapshot.code_address,
                    executable_snapshot.code_size);
                snapshot->code_image_bytes += executable_snapshot.code_size;
            }

            snapshot->code_copy_milliseconds +=
                std::chrono::duration<double, std::milli>(
                    std::chrono::steady_clock::now() - code_copy_started).count();

            uint32_t name_length = (std::min)(executable->name_length, 192u);
            executable_snapshot.name = executable->name ?
                copy_string(executable->name, name_length) :
                "<unnamed executable>";

            uint32_t object_count = (std::min)(executable->object_count, 65536u);

            for (uint32_t local_object_index = 0;
                local_object_index < object_count;
                ++local_object_index) {

                treyarch::chuck::vm::script_object* object =
                    executable->object(local_object_index);

                if (!object)
                    continue;

                uint32_t object_index = (uint32_t)snapshot->objects.size();
                c_object_snapshot object_snapshot {};

                object_snapshot.executable_index = executable_index;
                object_snapshot.local_index = local_object_index;
                object_snapshot.address = (uint32_t)object;
                object_snapshot.name_hash = object->name_hash;
                object_snapshot.parent_address = (uint32_t)object->parent;
                object_snapshot.flags = object->flags;
                object_snapshot.first_function = (uint32_t)snapshot->functions.size();
                object_snapshot.first_instance = (uint32_t)snapshot->instances.size();

                uint32_t function_count = (std::min)(object->function_count, 65536u);

                for (uint32_t local_function_index = 0;
                    local_function_index < function_count;
                    ++local_function_index) {

                    treyarch::chuck::vm::script_function* function =
                        object->function(local_function_index);

                    if (!function)
                        continue;

                    c_function_snapshot function_snapshot {};

                    function_snapshot.executable_index = executable_index;
                    function_snapshot.object_index = object_index;
                    function_snapshot.local_index = local_function_index;
                    function_snapshot.address = (uint32_t)function;
                    function_snapshot.code_address = (uint32_t)function->code;
                    function_snapshot.signature_hash = function->signature_hash;
                    function_snapshot.function_hash = function->function_hash;
                    function_snapshot.stack_metadata = function->stack_metadata;
                    function_snapshot.flags = function->flags;

                    if (function_snapshot.code_address >= executable_snapshot.code_address &&
                        function_snapshot.code_address - executable_snapshot.code_address <= executable_snapshot.code_size)
                        function_snapshot.code_offset =
                            function_snapshot.code_address -
                            executable_snapshot.code_address;

                    snapshot->functions.push_back(function_snapshot);
                }

                auto instance_thread_started = std::chrono::steady_clock::now();

                snapshot_instance_threads(
                    snapshot,
                    object,
                    executable_index,
                    object_index);

                snapshot->instance_thread_snapshot_milliseconds +=
                    std::chrono::duration<double, std::milli>(
                        std::chrono::steady_clock::now() - instance_thread_started).count();

                object_snapshot.function_count =
                    (uint32_t)snapshot->functions.size() -
                    object_snapshot.first_function;
                object_snapshot.instance_count =
                    (uint32_t)snapshot->instances.size() -
                    object_snapshot.first_instance;

                for (uint32_t instance_index = object_snapshot.first_instance;
                    instance_index < snapshot->instances.size();
                    ++instance_index)
                    object_snapshot.live_thread_count +=
                        snapshot->instances[instance_index].thread_count;

                snapshot->objects.push_back(object_snapshot);
            }

            executable_snapshot.object_count =
                (uint32_t)snapshot->objects.size() -
                executable_snapshot.first_object;
            executable_snapshot.function_count =
                (uint32_t)snapshot->functions.size() -
                executable_snapshot.first_function;
            executable_snapshot.instance_count =
                (uint32_t)snapshot->instances.size() -
                executable_snapshot.first_instance;
            executable_snapshot.thread_count =
                (uint32_t)snapshot->threads.size() -
                executable_snapshot.first_thread;

            snapshot->executables.push_back(std::move(executable_snapshot));
        }

    }

    inline void snapshot_runtime_objects(
        c_snapshot* snapshot,
        const std::unordered_map<uint32_t, c_native_coordinate> &native_lookup) const {
        treyarch::chuck::vm::script_manager* runtime = treyarch::chuck::vm::references::script_manager.read();

        snapshot->runtime_address = (uint32_t)runtime;
        snapshot->runtime_available = runtime != nullptr;

        if (!runtime)
            return;

        auto lock_wait_started = std::chrono::steady_clock::now();

        {
            treyarch::engine_lock_scope runtime_scope(&runtime->loaded_executables_lock);
            auto lock_acquired = std::chrono::steady_clock::now();

            snapshot->runtime_lock_wait_milliseconds =
                std::chrono::duration<double, std::milli>(
                    lock_acquired - lock_wait_started).count();

            snapshot_runtime_objects_locked(snapshot, runtime);

            snapshot->runtime_lock_milliseconds =
                std::chrono::duration<double, std::milli>(
                    std::chrono::steady_clock::now() - lock_acquired).count();
        }

        auto analysis_started = std::chrono::steady_clock::now();
        std::unordered_map<uint32_t, uint32_t> function_lookup;

        function_lookup.reserve(snapshot->functions.size());

        for (uint32_t function_index = 0;
            function_index < snapshot->functions.size();
            ++function_index)
            function_lookup[snapshot->functions[function_index].address] = function_index;

        for (const c_thread_snapshot &thread : snapshot->threads) {
            auto function = function_lookup.find(thread.function_address);

            if (function != function_lookup.end())
                ++snapshot->functions[function->second].live_thread_count;
        }

        snapshot_calls(snapshot, native_lookup);

        snapshot->post_lock_analysis_milliseconds =
            std::chrono::duration<double, std::milli>(
                std::chrono::steady_clock::now() - analysis_started).count();
    }

    inline void snapshot_calls(
        c_snapshot* snapshot,
        const std::unordered_map<uint32_t, c_native_coordinate> &native_lookup) const {

        std::unordered_map<uint32_t, uint32_t> function_by_address;
        std::unordered_map<uint32_t, std::vector<uint32_t>> functions_by_signature;
        std::unordered_set<uint64_t> signature_addresses;

        function_by_address.reserve(snapshot->functions.size());
        functions_by_signature.reserve(snapshot->functions.size());
        signature_addresses.reserve(snapshot->functions.size());

        for (uint32_t function_index = 0;
            function_index < snapshot->functions.size();
            ++function_index) {

            const c_function_snapshot &function =
                snapshot->functions[function_index];
            uint64_t signature_address =
                ((uint64_t)function.signature_hash << 32) |
                function.address;

            function_by_address.emplace(function.address, function_index);

            if (signature_addresses.insert(signature_address).second)
                functions_by_signature[function.signature_hash].push_back(
                    function_index);
        }

        for (uint32_t executable_index = 0;
            executable_index < snapshot->executables.size();
            ++executable_index) {

            c_executable_snapshot &executable = snapshot->executables[executable_index];

            if (!executable.code_address ||
                !executable.code_size ||
                executable.code_image.size() != executable.code_size)
                continue;

            uint8_t* code_start = executable.code_image.data();
            uint8_t* code_end = code_start + executable.code_size;
            std::unordered_set<uint32_t> boundaries;
            uint8_t* cursor = code_start;
            bool grammar_valid = true;

            while (cursor < code_end) {
                boundaries.insert((uint32_t)(cursor - code_start));

                c_instruction_view instruction {};

                if (!decode_instruction(cursor, code_end, &instruction)) {
                    grammar_valid = false;
                    break;
                }

                cursor += instruction.size;
            }

            if (!grammar_valid || cursor != code_end)
                continue;

            std::vector<uint32_t> ordered_functions;

            for (uint32_t function_index = executable.first_function;
                function_index < executable.first_function + executable.function_count;
                ++function_index) {

                c_function_snapshot &function = snapshot->functions[function_index];

                if (function.code_address >= executable.code_address &&
                    function.code_address < executable.code_address + executable.code_size)
                    ordered_functions.push_back(function_index);
            }

            std::sort(
                ordered_functions.begin(),
                ordered_functions.end(),
                [snapshot](uint32_t left, uint32_t right) {
                    uint32_t left_offset =
                        snapshot->functions[left].code_offset;
                    uint32_t right_offset =
                        snapshot->functions[right].code_offset;

                    return left_offset != right_offset ?
                        left_offset < right_offset :
                        left < right;
                });

            for (uint32_t group_begin = 0;
                group_begin < ordered_functions.size();) {

                uint32_t group_end = group_begin + 1;
                uint32_t code_offset =
                    snapshot->functions[ordered_functions[group_begin]].code_offset;

                while (
                    group_end < ordered_functions.size() &&
                    snapshot->functions[ordered_functions[group_end]].code_offset == code_offset)
                    ++group_end;

                uint32_t function_end = group_end < ordered_functions.size() ?
                    snapshot->functions[ordered_functions[group_end]].code_offset :
                    executable.code_size;
                uint32_t alias_count = group_end - group_begin;
                std::unordered_map<uint32_t, uint32_t> object_alias_counts;
                std::unordered_map<uint32_t, uint32_t> object_alias_ordinals;

                for (uint32_t alias = group_begin; alias < group_end; ++alias) {
                    const c_function_snapshot &function =
                        snapshot->functions[ordered_functions[alias]];

                    ++object_alias_counts[function.object_index];
                }

                ++executable.function_body_count;
                ++snapshot->function_body_count;
                executable.function_alias_count += alias_count - 1;
                snapshot->function_alias_count += alias_count - 1;

                for (uint32_t alias = group_begin; alias < group_end; ++alias) {
                    c_function_snapshot &function =
                        snapshot->functions[ordered_functions[alias]];

                    function.alias_ordinal = alias - group_begin;
                    function.alias_count = alias_count;
                    function.alias_object_count =
                        (uint32_t)object_alias_counts.size();
                    function.object_alias_ordinal =
                        object_alias_ordinals[function.object_index]++;
                    function.object_alias_count =
                        object_alias_counts[function.object_index];
                    function.code_size = function_end - code_offset;

                    if (function.object_alias_ordinal == 0) {
                        c_object_snapshot &object =
                            snapshot->objects[function.object_index];

                        ++object.function_body_count;
                        object.function_alias_count +=
                            function.object_alias_count - 1;
                    }
                }

                group_begin = group_end;
            }

            for (uint32_t ordered_index = 0;
                ordered_index < ordered_functions.size();
                ++ordered_index) {

                uint32_t function_index = ordered_functions[ordered_index];
                c_function_snapshot &function = snapshot->functions[function_index];
                uint32_t function_end =
                    function.code_offset + function.code_size;
                function.first_call = (uint32_t)snapshot->calls.size();
                function.first_indirect_control =
                    (uint32_t)snapshot->indirect_controls.size();

                if (!boundaries.contains(function.code_offset) ||
                    (function_end != executable.code_size && !boundaries.contains(function_end)))
                    continue;

                uint8_t* function_cursor = code_start + function.code_offset;

                while ((uint32_t)(function_cursor - code_start) < function_end) {
                    c_instruction_view instruction {};

                    if (!decode_instruction(
                        function_cursor,
                        code_start + function_end,
                        &instruction))
                        break;

                    bool script_call =
                        instruction.opcode == e_opcode::BSR &&
                        instruction.operand_kind == 7 &&
                        instruction.operand_words == 2;
                    bool native_call =
                        instruction.opcode == e_opcode::BSL &&
                        instruction.operand_kind == 8 &&
                        instruction.operand_words == 2;
                    bool indirect_control =
                        instruction.operand_kind == 7 &&
                        instruction.operand_words == 2 &&
                        (instruction.opcode == e_opcode::BVSR ||
                            instruction.opcode == e_opcode::BTH ||
                            (instruction.opcode >=
                                e_opcode::ECB &&
                            instruction.opcode <=
                                e_opcode::REGISTER_CALLBACK_TARGET_MODE0_REMAP));

                    if (script_call || native_call) {
                        c_call_snapshot call {};

                        call.kind = script_call ?
                            e_call_kind::script :
                            e_call_kind::native;
                        call.source_executable = executable.address;
                        call.source_function = function.address;
                        call.code_offset =
                            (uint32_t)(function_cursor - code_start);
                        call.target_address =
                            ((uint32_t)instruction.operands[0] << 16) |
                            instruction.operands[1];
                        call.native_class_index = invalid_index;
                        call.native_function_index = invalid_index;

                        c_object_snapshot &object =
                            snapshot->objects[function.object_index];
                        bool object_aggregate =
                            function.object_alias_ordinal == 0;
                        bool physical_aggregate =
                            function.alias_ordinal == 0;

                        call.source_object = object.address;

                        if (script_call) {
                            ++function.script_call_count;

                            if (object_aggregate)
                                ++object.script_call_count;

                            if (physical_aggregate) {
                                ++executable.script_call_count;
                                ++snapshot->script_call_count;
                            }
                        } else {
                            ++function.native_call_count;

                            if (object_aggregate)
                                ++object.native_call_count;

                            if (physical_aggregate) {
                                ++executable.native_call_count;
                                ++snapshot->native_call_count;
                            }

                            auto native = native_lookup.find(call.target_address);

                            if (native != native_lookup.end()) {
                                call.native_class_index = native->second.class_index;
                                call.native_function_index = native->second.function_index;
                            }
                        }

                        snapshot->calls.push_back(call);
                    }

                    if (indirect_control) {
                        c_indirect_control_snapshot control {};

                        control.opcode = instruction.opcode;
                        control.source_executable = executable.address;
                        control.source_function = function.address;
                        control.code_offset =
                            (uint32_t)(function_cursor - code_start);
                        control.prototype_address =
                            ((uint32_t)instruction.operands[0] << 16) |
                            instruction.operands[1];
                        control.source_object =
                            snapshot->objects[function.object_index].address;

                        if (instruction.opcode == e_opcode::BVSR)
                            control.kind =
                                e_indirect_control_kind::receiver_dispatch;
                        else if (instruction.opcode == e_opcode::BTH)
                            control.kind =
                                e_indirect_control_kind::target_thread;
                        else if (instruction.opcode ==
                                e_opcode::SCB ||
                            instruction.opcode ==
                                e_opcode::SCO)
                            control.kind =
                                e_indirect_control_kind::current_callback;
                        else
                            control.kind =
                                e_indirect_control_kind::instance_callback;

                        auto prototype =
                            function_by_address.find(control.prototype_address);

                        if (prototype != function_by_address.end()) {
                            const c_function_snapshot &prototype_function =
                                snapshot->functions[prototype->second];

                            control.signature_hash =
                                prototype_function.signature_hash;

                            if (control.kind ==
                                e_indirect_control_kind::current_callback) {
                                control.candidate_count = 1;
                                control.cross_root_candidate_count =
                                    prototype_function.executable_index !=
                                    function.executable_index ? 1 : 0;
                            } else {
                                auto candidates = functions_by_signature.find(
                                    control.signature_hash);

                                if (candidates != functions_by_signature.end()) {
                                    control.candidate_count =
                                        (uint32_t)candidates->second.size();

                                    for (uint32_t candidate_index :
                                        candidates->second) {
                                        if (snapshot->functions[candidate_index].executable_index !=
                                            function.executable_index)
                                            ++control.cross_root_candidate_count;
                                    }
                                }
                            }
                        }

                        ++function.indirect_control_count;

                        if (function.alias_ordinal == 0) {
                            ++executable.indirect_control_count;
                            ++snapshot->indirect_control_count;
                        }

                        snapshot->indirect_controls.push_back(control);
                    }

                    function_cursor += instruction.size;
                }

                function.call_count =
                    (uint32_t)snapshot->calls.size() -
                    function.first_call;
                function.indirect_control_count =
                    (uint32_t)snapshot->indirect_controls.size() -
                    function.first_indirect_control;
            }
        }
    }

    [[nodiscard]] inline const c_executable_snapshot* find_executable(uint32_t address) const {
        for (const c_executable_snapshot &executable : m_snapshot.executables) {
            if (executable.address == address)
                return &executable;
        }

        return nullptr;
    }

    [[nodiscard]] inline const c_object_snapshot* find_object(uint32_t address) const {
        for (const c_object_snapshot &object : m_snapshot.objects) {
            if (object.address == address)
                return &object;
        }

        return nullptr;
    }

    [[nodiscard]] inline const c_function_snapshot* find_function(uint32_t address) const {
        for (const c_function_snapshot &function : m_snapshot.functions) {
            if (function.address == address)
                return &function;
        }

        return nullptr;
    }

    [[nodiscard]] inline const c_native_class_snapshot* find_native_class(uint32_t index) const {
        for (const c_native_class_snapshot &native_class : m_snapshot.native_classes) {
            if (native_class.index == index)
                return &native_class;
        }

        return nullptr;
    }

    [[nodiscard]] inline const c_native_function_snapshot* find_native_function(
        uint32_t class_index,
        uint32_t function_index) const {

        const c_native_class_snapshot* native_class = find_native_class(class_index);

        if (!native_class)
            return nullptr;

        for (uint32_t index = native_class->first_function;
            index < native_class->first_function + native_class->function_count;
            ++index) {

            const c_native_function_snapshot &function =
                m_snapshot.native_functions[index];

            if (function.function_index == function_index)
                return &function;
        }

        return nullptr;
    }

    [[nodiscard]] inline bool contains_filter(
        const std::string &value,
        const char* filter) const {

        if (!filter || !*filter)
            return true;

        std::string haystack = value;
        std::string needle = filter;

        std::transform(
            haystack.begin(),
            haystack.end(),
            haystack.begin(),
            [](unsigned char character) {
                return (char)std::tolower(character);
            });
        std::transform(
            needle.begin(),
            needle.end(),
            needle.begin(),
            [](unsigned char character) {
                return (char)std::tolower(character);
            });

        return haystack.find(needle) != std::string::npos;
    }

    inline void rebuild_transitive_analysis(
        const c_function_snapshot &source_function) {

        struct c_script_edge {
            uint32_t target_body;
            uint32_t call_offset;
        };

        struct c_native_edge {
            uint32_t address;
            uint32_t class_index;
            uint32_t function_index;
            uint32_t call_offset;
        };

        struct c_body_node {
            uint32_t function_index;
            uint32_t unresolved_script_calls;
            std::vector<c_script_edge> script_edges;
            std::vector<c_native_edge> native_edges;
        };

        struct c_predecessor {
            uint32_t body_index;
            uint32_t call_offset;
        };

        auto started = std::chrono::steady_clock::now();
        c_transitive_analysis next {};

        next.snapshot_number = m_snapshot_count;
        next.source_function = source_function.address;

        std::vector<c_body_node> bodies;
        std::unordered_map<uint64_t, uint32_t> body_by_key;
        std::unordered_map<uint32_t, uint32_t> body_by_function;

        bodies.reserve(m_snapshot.function_body_count);
        body_by_key.reserve(m_snapshot.function_body_count);
        body_by_function.reserve(m_snapshot.functions.size());

        for (uint32_t function_index = 0;
            function_index < m_snapshot.functions.size();
            ++function_index) {

            const c_function_snapshot &function =
                m_snapshot.functions[function_index];

            if (!function.alias_count || !function.code_size)
                continue;

            uint64_t key =
                ((uint64_t)function.executable_index << 32) |
                function.code_offset;
            auto existing = body_by_key.find(key);
            uint32_t body_index = 0;

            if (existing == body_by_key.end()) {
                body_index = (uint32_t)bodies.size();
                body_by_key.emplace(key, body_index);

                c_body_node body {};

                body.function_index = function_index;
                bodies.push_back(std::move(body));
            } else {
                body_index = existing->second;

                const c_function_snapshot &representative =
                    m_snapshot.functions[bodies[body_index].function_index];

                if (function.alias_ordinal < representative.alias_ordinal)
                    bodies[body_index].function_index = function_index;
            }

            body_by_function[function.address] = body_index;
        }

        auto source_body_iterator =
            body_by_function.find(source_function.address);

        if (source_body_iterator == body_by_function.end()) {
            next.milliseconds =
                std::chrono::duration<double, std::milli>(
                    std::chrono::steady_clock::now() - started).count();
            m_transitive_analysis = std::move(next);
            m_selected_transitive_script = 0;
            m_selected_transitive_native = 0;
            return;
        }

        for (c_body_node &body : bodies) {
            const c_function_snapshot &function =
                m_snapshot.functions[body.function_index];
            uint64_t call_end =
                (uint64_t)function.first_call + function.call_count;

            if (call_end > m_snapshot.calls.size())
                continue;

            body.script_edges.reserve(function.script_call_count);
            body.native_edges.reserve(function.native_call_count);

            for (uint32_t call_index = function.first_call;
                call_index < call_end;
                ++call_index) {

                const c_call_snapshot &call = m_snapshot.calls[call_index];

                if (call.kind == e_call_kind::script) {
                    auto target = body_by_function.find(call.target_address);

                    if (target == body_by_function.end()) {
                        ++body.unresolved_script_calls;
                        continue;
                    }

                    body.script_edges.push_back({
                        target->second,
                        call.code_offset
                    });
                } else {
                    body.native_edges.push_back({
                        call.target_address,
                        call.native_class_index,
                        call.native_function_index,
                        call.code_offset
                    });
                }
            }
        }

        uint32_t source_body = source_body_iterator->second;
        std::vector<uint32_t> distance(bodies.size(), invalid_index);
        std::vector<c_predecessor> predecessors(
            bodies.size(),
            { invalid_index, 0 });
        std::vector<uint32_t> queue;
        uint32_t queue_cursor = 0;

        queue.reserve(bodies.size());
        distance[source_body] = 0;
        queue.push_back(source_body);

        while (queue_cursor < queue.size()) {
            uint32_t body_index = queue[queue_cursor++];
            const c_body_node &body = bodies[body_index];

            next.unresolved_script_callsite_count +=
                body.unresolved_script_calls;
            next.reachable_script_callsite_count +=
                (uint32_t)body.script_edges.size();

            for (const c_script_edge &edge : body.script_edges) {
                if (distance[edge.target_body] != invalid_index)
                    continue;

                distance[edge.target_body] = distance[body_index] + 1;
                predecessors[edge.target_body] = {
                    body_index,
                    edge.call_offset
                };
                queue.push_back(edge.target_body);
            }
        }

        std::vector<int32_t> depth_first_index(bodies.size(), -1);
        std::vector<int32_t> low_link(bodies.size(), -1);
        std::vector<uint8_t> on_stack(bodies.size(), 0);
        std::vector<uint32_t> stack;
        std::vector<uint32_t> recursion_group(bodies.size(), 0);
        std::vector<uint32_t> recursion_group_size(bodies.size(), 0);
        int32_t next_depth_first_index = 0;

        stack.reserve(queue.size());

        std::function<void(uint32_t)> visit_body =
            [&](uint32_t body_index) {
                depth_first_index[body_index] = next_depth_first_index;
                low_link[body_index] = next_depth_first_index;
                ++next_depth_first_index;
                stack.push_back(body_index);
                on_stack[body_index] = 1;

                for (const c_script_edge &edge :
                    bodies[body_index].script_edges) {

                    if (distance[edge.target_body] == invalid_index)
                        continue;

                    if (depth_first_index[edge.target_body] == -1) {
                        visit_body(edge.target_body);
                        low_link[body_index] = (std::min)(
                            low_link[body_index],
                            low_link[edge.target_body]);
                    } else if (on_stack[edge.target_body]) {
                        low_link[body_index] = (std::min)(
                            low_link[body_index],
                            depth_first_index[edge.target_body]);
                    }
                }

                if (low_link[body_index] != depth_first_index[body_index])
                    return;

                std::vector<uint32_t> component;

                while (!stack.empty()) {
                    uint32_t member = stack.back();

                    stack.pop_back();
                    on_stack[member] = 0;
                    component.push_back(member);

                    if (member == body_index)
                        break;
                }

                bool recursive = component.size() > 1;

                if (!recursive && !component.empty()) {
                    uint32_t member = component.front();

                    for (const c_script_edge &edge :
                        bodies[member].script_edges) {
                        if (edge.target_body == member) {
                            recursive = true;
                            break;
                        }
                    }
                }

                if (!recursive)
                    return;

                uint32_t group = ++next.recursion_group_count;
                uint32_t group_size = (uint32_t)component.size();

                next.recursive_body_count += group_size;

                for (uint32_t member : component) {
                    recursion_group[member] = group;
                    recursion_group_size[member] = group_size;
                }
            };

        for (uint32_t body_index : queue) {
            if (depth_first_index[body_index] == -1)
                visit_body(body_index);
        }

        next.source_recursive = recursion_group[source_body] != 0;

        auto build_path = [&](uint32_t target_body) {
            std::vector<c_call_chain_step> path;
            uint32_t body_index = target_body;

            while (body_index != source_body) {
                const c_predecessor &predecessor =
                    predecessors[body_index];

                if (predecessor.body_index == invalid_index) {
                    path.clear();
                    break;
                }

                path.push_back({
                    bodies[predecessor.body_index].function_index,
                    predecessor.call_offset
                });
                body_index = predecessor.body_index;
            }

            std::reverse(path.begin(), path.end());

            return path;
        };

        next.script_bodies.reserve(queue.size() > 0 ? queue.size() - 1 : 0);

        for (uint32_t body_index : queue) {
            if (body_index == source_body)
                continue;

            c_reachable_script_body reachable {};

            reachable.snapshot_function_index =
                bodies[body_index].function_index;
            reachable.depth = distance[body_index];
            reachable.recursion_group = recursion_group[body_index];
            reachable.recursion_group_size =
                recursion_group_size[body_index];
            reachable.path = build_path(body_index);

            next.script_bodies.push_back(std::move(reachable));
        }

        std::unordered_map<uint32_t, uint32_t> native_by_address;

        native_by_address.reserve(m_snapshot.native_functions.size());

        for (uint32_t body_index : queue) {
            const c_body_node &body = bodies[body_index];

            next.reachable_native_callsite_count +=
                (uint32_t)body.native_edges.size();

            for (const c_native_edge &edge : body.native_edges) {
                uint32_t call_depth = distance[body_index] + 1;
                auto existing = native_by_address.find(edge.address);

                if (existing == native_by_address.end()) {
                    c_reachable_native reachable {};

                    reachable.address = edge.address;
                    reachable.class_index = edge.class_index;
                    reachable.function_index = edge.function_index;
                    reachable.call_depth = call_depth;
                    reachable.callsite_count = 1;
                    reachable.path = build_path(body_index);
                    reachable.path.push_back({
                        body.function_index,
                        edge.call_offset
                    });

                    uint32_t native_index =
                        (uint32_t)next.natives.size();

                    next.natives.push_back(std::move(reachable));
                    native_by_address.emplace(edge.address, native_index);
                    continue;
                }

                c_reachable_native &reachable =
                    next.natives[existing->second];

                ++reachable.callsite_count;

                if (reachable.class_index == invalid_index &&
                    edge.class_index != invalid_index) {
                    reachable.class_index = edge.class_index;
                    reachable.function_index = edge.function_index;
                }

                if (call_depth >= reachable.call_depth)
                    continue;

                reachable.call_depth = call_depth;
                reachable.path = build_path(body_index);
                reachable.path.push_back({
                    body.function_index,
                    edge.call_offset
                });
            }
        }

        std::sort(
            next.script_bodies.begin(),
            next.script_bodies.end(),
            [this](
                const c_reachable_script_body &left,
                const c_reachable_script_body &right) {

                if (left.depth != right.depth)
                    return left.depth < right.depth;

                const c_function_snapshot &left_function =
                    m_snapshot.functions[left.snapshot_function_index];
                const c_function_snapshot &right_function =
                    m_snapshot.functions[right.snapshot_function_index];

                if (left_function.function_hash != right_function.function_hash)
                    return left_function.function_hash < right_function.function_hash;

                return left_function.address < right_function.address;
            });

        std::sort(
            next.natives.begin(),
            next.natives.end(),
            [](const c_reachable_native &left,
                const c_reachable_native &right) {

                if (left.call_depth != right.call_depth)
                    return left.call_depth < right.call_depth;

                if (left.class_index != right.class_index)
                    return left.class_index < right.class_index;

                if (left.function_index != right.function_index)
                    return left.function_index < right.function_index;

                return left.address < right.address;
            });

        next.available = true;
        next.milliseconds =
            std::chrono::duration<double, std::milli>(
                std::chrono::steady_clock::now() - started).count();

        m_transitive_analysis = std::move(next);

        bool selected_script_found = false;

        for (const c_reachable_script_body &reachable :
            m_transitive_analysis.script_bodies) {
            if (m_snapshot.functions[reachable.snapshot_function_index].address ==
                m_selected_transitive_script) {
                selected_script_found = true;
                break;
            }
        }

        if (!selected_script_found) {
            m_selected_transitive_script =
                m_transitive_analysis.script_bodies.empty() ?
                    0 :
                    m_snapshot.functions[
                        m_transitive_analysis.script_bodies.front().snapshot_function_index
                    ].address;
        }

        bool selected_native_found = false;

        for (const c_reachable_native &reachable :
            m_transitive_analysis.natives) {
            if (reachable.address == m_selected_transitive_native) {
                selected_native_found = true;
                break;
            }
        }

        if (!selected_native_found) {
            m_selected_transitive_native =
                m_transitive_analysis.natives.empty() ?
                    0 : m_transitive_analysis.natives.front().address;
        }
    }

    inline void ensure_transitive_analysis(
        const c_function_snapshot &source_function) {

        if (m_transitive_analysis.snapshot_number != m_snapshot_count ||
            m_transitive_analysis.source_function != source_function.address)
            rebuild_transitive_analysis(source_function);
    }

    inline void add_object_graph_node(
        const c_object_snapshot &object,
        const ImVec2 &position,
        bool external) {

        std::string title = std::format("{}object [{}] 0x{:08X}",
            external ? "external " : "",
            object.local_index,
            object.name_hash);

        c_node &node = m_graph.add_node(
            object_node_id(object.address),
            position,
            310.0f,
            title,
            object_style(external));

        node.add_row(std::format("object 0x{:08X} | flags 0x{:08X}",
            object.address,
            object.flags));
        node.add_row(std::format("functions {} | bodies {} | aliases {}",
            object.function_count,
            object.function_body_count,
            object.function_alias_count));
        node.add_row(std::format("instances {} | threads {}",
            object.instance_count,
            object.live_thread_count));
        node.add_row(std::format("unique BSR {} | BSL {}",
            object.script_call_count,
            object.native_call_count));
        node.add_row(std::format("parent 0x{:08X}", object.parent_address));
    }

    inline void rebuild_graph() {
        m_graph.clear();
        m_graph_visible_object_count = 0;
        m_graph_hidden_object_count = 0;
        m_graph_external_object_count = 0;
        m_graph_parent_edge_count = 0;
        m_graph_call_edge_count = 0;
        m_graph_self_call_count = 0;

        const c_executable_snapshot* executable =
            find_executable(m_selected_executable);

        if (!executable)
            return;

        std::unordered_map<uint32_t, const c_object_snapshot*> objects_by_address;
        std::unordered_map<uint32_t, const c_function_snapshot*> functions_by_address;
        std::unordered_set<uint32_t> owned_objects;
        std::unordered_set<uint32_t> visible_objects;
        std::unordered_set<uint32_t> related_objects;
        std::unordered_set<uint32_t> native_classes;
        std::unordered_set<uint32_t> unresolved_natives;

        objects_by_address.reserve(m_snapshot.objects.size());
        functions_by_address.reserve(m_snapshot.functions.size());

        for (const c_object_snapshot &object : m_snapshot.objects)
            objects_by_address.emplace(object.address, &object);

        for (const c_function_snapshot &function : m_snapshot.functions)
            functions_by_address.emplace(function.address, &function);

        for (uint32_t object_index = executable->first_object;
            object_index < executable->first_object + executable->object_count;
            ++object_index)
            owned_objects.insert(m_snapshot.objects[object_index].address);

        uint32_t selected_object = 0;
        node_id selected_node = m_graph_view.selected_node();

        if (graph_node_kind(selected_node) == 2) {
            uint32_t candidate = graph_node_address(selected_node);

            if (objects_by_address.contains(candidate))
                selected_object = candidate;
        }

        if (selected_object) {
            for (const c_call_snapshot &call : m_snapshot.calls) {
                auto source_function =
                    functions_by_address.find(call.source_function);

                if (
                    source_function == functions_by_address.end() ||
                    source_function->second->object_alias_ordinal != 0)
                    continue;

                if (call.kind == e_call_kind::script && m_show_script_calls) {
                    auto target_function = functions_by_address.find(call.target_address);

                    if (target_function == functions_by_address.end())
                        continue;

                    const c_object_snapshot &target_object =
                        m_snapshot.objects[target_function->second->object_index];
                    bool outgoing = call.source_object == selected_object;
                    bool incoming = target_object.address == selected_object;

                    if (!outgoing && !incoming)
                        continue;

                    if (call.source_object == target_object.address) {
                        ++m_graph_self_call_count;
                        continue;
                    }

                    if (!objects_by_address.contains(call.source_object))
                        continue;

                    related_objects.insert(call.source_object);
                    related_objects.insert(target_object.address);

                    m_graph.add_or_increment_connection(
                        object_node_id(call.source_object),
                        object_node_id(target_object.address),
                        (uint64_t)(outgoing ?
                            e_graph_edge_kind::script_outgoing :
                            e_graph_edge_kind::script_incoming) + 1,
                        0);
                } else if (call.kind == e_call_kind::native &&
                    m_show_native_calls && call.source_object == selected_object) {

                    node_id target = 0;

                    if (call.native_class_index != invalid_index) {
                        const c_native_class_snapshot* native_class =
                            find_native_class(call.native_class_index);

                        if (!native_class)
                            continue;

                        native_classes.insert(native_class->index);
                        target = native_class_node_id(native_class->address);
                    } else {
                        unresolved_natives.insert(call.target_address);
                        target = unresolved_native_node_id(call.target_address);
                    }

                    m_graph.add_or_increment_connection(
                        object_node_id(call.source_object),
                        target,
                        (uint64_t)e_graph_edge_kind::native + 1,
                        0);
                }
            }
        }

        if (m_focus_selection && selected_object) {
            visible_objects.insert(selected_object);
            visible_objects.insert(related_objects.begin(), related_objects.end());

            for (const c_object_snapshot &object : m_snapshot.objects) {
                if (object.parent_address == selected_object)
                    visible_objects.insert(object.address);
            }
        } else {
            visible_objects.insert(owned_objects.begin(), owned_objects.end());
            visible_objects.insert(related_objects.begin(), related_objects.end());
        }

        std::vector<uint32_t> ancestor_queue(
            visible_objects.begin(),
            visible_objects.end());

        for (size_t queue_index = 0;
            queue_index < ancestor_queue.size();
            ++queue_index) {

            auto object = objects_by_address.find(ancestor_queue[queue_index]);

            if (object == objects_by_address.end() || !object->second->parent_address)
                continue;

            uint32_t parent = object->second->parent_address;

            if (objects_by_address.contains(parent) && visible_objects.insert(parent).second)
                ancestor_queue.push_back(parent);
        }

        uint32_t visible_owned_count = 0;

        for (uint32_t address : visible_objects) {
            if (owned_objects.contains(address))
                ++visible_owned_count;
            else
                ++m_graph_external_object_count;
        }

        m_graph_visible_object_count = (uint32_t)visible_objects.size();
        m_graph_hidden_object_count = executable->object_count -
            (std::min)(executable->object_count, visible_owned_count);

        std::unordered_map<uint32_t, uint32_t> object_depths;
        uint32_t maximum_depth = 0;

        for (uint32_t address : visible_objects) {
            uint32_t depth = 0;
            uint32_t cursor = address;
            std::unordered_set<uint32_t> visited;

            while (visited.insert(cursor).second) {
                auto object = objects_by_address.find(cursor);

                if (object == objects_by_address.end() ||
                    !object->second->parent_address ||
                    !visible_objects.contains(object->second->parent_address))
                    break;

                cursor = object->second->parent_address;
                ++depth;
            }

            object_depths[address] = depth;
            maximum_depth = (std::max)(maximum_depth, depth);
        }

        std::vector<std::vector<const c_object_snapshot*>> object_layers(maximum_depth + 1);

        for (uint32_t address : visible_objects)
            object_layers[object_depths[address]].push_back(objects_by_address[address]);

        for (std::vector<const c_object_snapshot*> &layer : object_layers) {
            std::sort(
                layer.begin(),
                layer.end(),
                [&owned_objects, selected_object](
                    const c_object_snapshot* left,
                    const c_object_snapshot* right) {

                    bool left_selected = left->address == selected_object;
                    bool right_selected = right->address == selected_object;

                    if (left_selected != right_selected)
                        return left_selected;

                    if (left->parent_address != right->parent_address)
                        return left->parent_address < right->parent_address;

                    bool left_owned = owned_objects.contains(left->address);
                    bool right_owned = owned_objects.contains(right->address);

                    if (left_owned != right_owned)
                        return left_owned;

                    if (left->local_index != right->local_index)
                        return left->local_index < right->local_index;

                    return left->address < right->address;
                });
        }

        constexpr float object_start_x = 420.0f;
        uint32_t native_node_count =
            (uint32_t)(native_classes.size() + unresolved_natives.size());
        float object_start_y = native_node_count ? 320.0f : 20.0f;
        uint32_t row_limit = m_focus_selection && selected_object ?
            10u : (visible_objects.size() > 160 ? 24u :
                (visible_objects.size() > 60 ? 18u : 12u));
        std::vector<std::vector<node_id>> graph_layers;
        std::unordered_map<node_id, ImVec2> object_positions;
        std::vector<const c_object_snapshot*> positioned_objects;

        graph_layers.reserve(object_layers.size());
        positioned_objects.reserve(visible_objects.size());

        for (const std::vector<const c_object_snapshot*> &layer : object_layers) {
            std::vector<node_id> graph_layer;
            graph_layer.reserve(layer.size());

            for (const c_object_snapshot* object : layer) {
                graph_layer.push_back(object_node_id(object->address));
                positioned_objects.push_back(object);
            }

            graph_layers.push_back(std::move(graph_layer));
        }

        c_layered_layout_options object_layout;
        object_layout.origin = ImVec2(object_start_x, object_start_y);
        object_layout.row_limit = row_limit;
        object_positions = imgui::ext::node_graph::calculate_layered_layout(
            graph_layers,
            object_layout);

        node_id selected_object_node = object_node_id(selected_object);

        if (selected_object && object_positions.contains(selected_object_node)) {
            ImVec2 selected_position = object_positions[selected_object_node];

            for (auto &entry : object_positions) {
                if (entry.first == selected_object_node)
                    continue;

                if (entry.second.x < selected_position.x)
                    entry.second.x = (std::max)(object_start_x, entry.second.x - 110.0f);
                else if (entry.second.x > selected_position.x)
                    entry.second.x += 110.0f;

                if (entry.second.y < selected_position.y)
                    entry.second.y -= 64.0f;
                else if (entry.second.y > selected_position.y)
                    entry.second.y += 64.0f;
            }
        }

        float graph_height = 128.0f;

        for (const auto &entry : object_positions) {
            graph_height = (std::max)(
                graph_height,
                entry.second.y + c_node::content_height(5));
        }

        c_node &root = m_graph.add_node(
            executable_node_id(executable->address),
            ImVec2(20.0f, (std::max)(20.0f, graph_height * 0.5f - 74.0f)),
            340.0f,
            executable->name,
            executable_style());

        root.add_row(std::format("executable 0x{:08X}", executable->address));
        root.add_row(std::format("resource 0x{:08X} | flags 0x{:08X}",
            executable->resource_hash,
            executable->flags));
        root.add_row(std::format("objects {} | functions {}",
            executable->object_count,
            executable->function_count));
        root.add_row(std::format("bodies {} | aliases {} | threads {}",
            executable->function_body_count,
            executable->function_alias_count,
            executable->thread_count));
        root.add_row(std::format("physical BSR {} | BSL {}",
            executable->script_call_count,
            executable->native_call_count));

        for (const c_object_snapshot* object : positioned_objects) {
            add_object_graph_node(
                *object,
                object_positions[object_node_id(object->address)],
                !owned_objects.contains(object->address));
        }

        for (const c_object_snapshot* object : positioned_objects) {
            if (!object->parent_address ||
                !visible_objects.contains(object->parent_address))
                continue;

            m_graph.add_connection(
                object_node_id(object->parent_address),
                object_node_id(object->address),
                IM_COL32(74, 163, 202, 205),
                1.8f,
                true);
            ++m_graph_parent_edge_count;
        }

        float native_center_x = selected_object && object_positions.contains(selected_object_node) ?
            object_positions[selected_object_node].x + 145.0f : object_start_x + 145.0f;
        std::vector<uint32_t> sorted_native_classes(
            native_classes.begin(),
            native_classes.end());
        std::vector<uint32_t> sorted_unresolved_natives(
            unresolved_natives.begin(),
            unresolved_natives.end());

        std::sort(sorted_native_classes.begin(), sorted_native_classes.end());
        std::sort(sorted_unresolved_natives.begin(), sorted_unresolved_natives.end());

        std::vector<node_id> native_nodes;
        native_nodes.reserve(native_node_count);

        for (uint32_t class_index : sorted_native_classes) {
            const c_native_class_snapshot* native_class =
                find_native_class(class_index);

            if (native_class)
                native_nodes.push_back(native_class_node_id(native_class->address));
        }

        for (uint32_t descriptor : sorted_unresolved_natives)
            native_nodes.push_back(unresolved_native_node_id(descriptor));

        c_row_layout_options native_layout;
        native_layout.center = ImVec2(native_center_x, 20.0f);

        std::unordered_map<node_id, ImVec2> native_positions =
            imgui::ext::node_graph::calculate_row_layout(native_nodes, native_layout);

        for (uint32_t class_index : sorted_native_classes) {
            const c_native_class_snapshot* native_class =
                find_native_class(class_index);

            if (!native_class)
                continue;

            node_id native_node = native_class_node_id(native_class->address);

            c_node &node = m_graph.add_node(
                native_node,
                native_positions[native_node],
                300.0f,
                std::format("native [{}] {}",
                    native_class->index,
                    native_class->type_name),
                native_style());

            node.add_row(std::format("class 0x{:08X} | value size {}",
                native_class->address,
                native_class->value_size));
            node.add_row(std::format("functions {}", native_class->function_count));
            node.add_row(native_class->parent_name.empty() ?
                "parent <none>" :
                std::format("parent {}", native_class->parent_name));
        }

        for (uint32_t descriptor : sorted_unresolved_natives) {
            node_id native_node = unresolved_native_node_id(descriptor);

            c_node &node = m_graph.add_node(
                native_node,
                native_positions[native_node],
                300.0f,
                "unresolved native descriptor",
                native_style(true));

            node.add_row(std::format("descriptor 0x{:08X}", descriptor));
            node.add_row("not present in registry snapshot");
        }

        for (c_connection &edge : m_graph.connections()) {
            if (!edge.group)
                continue;

            ImU32 color = IM_COL32(242, 159, 54, 235);
            const char* relation = "outgoing BSR";
            e_graph_edge_kind kind = (e_graph_edge_kind)(edge.group - 1);

            if (kind == e_graph_edge_kind::script_incoming) {
                color = IM_COL32(235, 91, 58, 235);
                relation = "incoming BSR";
            } else if (kind == e_graph_edge_kind::native) {
                color = IM_COL32(196, 119, 235, 230);
                relation = "BSL";
            }

            float thickness = 1.5f +
                (std::min)((float)edge.multiplicity, 12.0f) * 0.28f;

            edge.color = color;
            edge.thickness = thickness;
            edge.label = std::format("{} {} callsite{}",
                edge.multiplicity,
                relation,
                edge.multiplicity == 1 ? "" : "s");
            ++m_graph_call_edge_count;
        }
    }

    inline void refresh_snapshot() {
        c_snapshot next {};
        std::unordered_map<uint32_t, c_native_coordinate> native_lookup;
        auto capture_started = std::chrono::steady_clock::now();
        auto native_started = capture_started;

        next.executables.reserve(m_snapshot.executables.size());
        next.objects.reserve(m_snapshot.objects.size());
        next.functions.reserve(m_snapshot.functions.size());
        next.instances.reserve(m_snapshot.instances.size());
        next.threads.reserve(m_snapshot.threads.size());
        next.calls.reserve(m_snapshot.calls.size());
        next.indirect_controls.reserve(m_snapshot.indirect_controls.size());
        next.native_classes.reserve(m_snapshot.native_classes.size());
        next.native_functions.reserve(m_snapshot.native_functions.size());
        native_lookup.reserve(m_snapshot.native_functions.size());

        snapshot_native_registry(&next, &native_lookup);
        next.native_snapshot_milliseconds =
            std::chrono::duration<double, std::milli>(
                std::chrono::steady_clock::now() - native_started).count();
        snapshot_runtime_objects(&next, native_lookup);
        next.capture_milliseconds =
            std::chrono::duration<double, std::milli>(
                std::chrono::steady_clock::now() - capture_started).count();

        m_max_runtime_lock_milliseconds = (std::max)(
            m_max_runtime_lock_milliseconds,
            next.runtime_lock_milliseconds);
        m_max_runtime_lock_wait_milliseconds = (std::max)(
            m_max_runtime_lock_wait_milliseconds,
            next.runtime_lock_wait_milliseconds);
        m_max_capture_milliseconds = (std::max)(
            m_max_capture_milliseconds,
            next.capture_milliseconds);
        ++m_snapshot_count;

        uint32_t previous_executable = m_selected_executable;

        m_snapshot = std::move(next);
        m_selected_indirect_control = invalid_index;

        if (!find_executable(m_selected_executable))
            m_selected_executable = m_snapshot.executables.empty() ?
                0 : m_snapshot.executables.front().address;

        if (!find_function(m_selected_function)) {
            const c_executable_snapshot* executable =
                find_executable(m_selected_executable);

            m_selected_function = executable && executable->function_count ?
                m_snapshot.functions[executable->first_function].address : 0;
        }

        if (!find_native_class(m_selected_native_class))
            m_selected_native_class = m_snapshot.native_classes.empty() ?
                0 : m_snapshot.native_classes.front().index;

        if (previous_executable != m_selected_executable) {
            m_graph_view.reset_layout();
        }

        rebuild_graph();

        if (previous_executable != m_selected_executable)
            m_graph_view.request_frame();

        m_last_refresh = ImGui::GetTime();
        m_refresh_requested = false;
    }

    inline bool draw_executable_selector(const char* identifier) {
        const c_executable_snapshot* selected =
            find_executable(m_selected_executable);
        const char* preview = selected ?
            selected->name.c_str() :
            "<no executable>";
        bool changed = false;

        ImGui::SetNextItemWidth(360.0f);

        if (ImGui::BeginCombo(identifier, preview)) {
            for (const c_executable_snapshot &executable : m_snapshot.executables) {
                bool is_selected = executable.address == m_selected_executable;
                std::string label = std::format("{}##{:08X}",
                    executable.name,
                    executable.address);

                if (ImGui::Selectable(label.c_str(), is_selected)) {
                    m_selected_executable = executable.address;
                    changed = true;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        return changed;
    }

    inline void draw_graph_tab() {
        if (draw_executable_selector("##graph_executable")) {
            m_graph_view.reset_layout();
            rebuild_graph();
            m_graph_view.request_frame();
        }

        ImGui::SameLine();

        if (ImGui::Button("frame"))
            m_graph_view.request_frame();

        ImGui::SameLine();

        if (ImGui::Button("reset layout"))
            m_graph_view.reset_layout();

        ImGui::SameLine();

        if (ImGui::Checkbox("BSR", &m_show_script_calls))
            rebuild_graph();

        ImGui::SameLine();

        if (ImGui::Checkbox("BSL", &m_show_native_calls))
            rebuild_graph();

        ImGui::SameLine();

        if (ImGui::Checkbox("focus selection", &m_focus_selection)) {
            rebuild_graph();
            m_graph_view.request_frame();
        }

        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::TextUnformatted(
                "Keep the selected object, its ancestors, direct children, and call peers.");
            ImGui::EndTooltip();
        }

        ImGui::TextDisabled(
            "%u objects (%u external, %u hidden) | %u parent links | %u selected call edges | %u unique object-local BSR callsites | %.1f%%",
            m_graph_visible_object_count,
            m_graph_external_object_count,
            m_graph_hidden_object_count,
            m_graph_parent_edge_count,
            m_graph_call_edge_count,
            m_graph_self_call_count,
            m_graph_view.zoom() * 100.0f);

        ImGui::TextColored(ImVec4(0.29f, 0.64f, 0.79f, 1.0f), "parent");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.95f, 0.62f, 0.21f, 1.0f), "outgoing BSR");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.92f, 0.36f, 0.23f, 1.0f), "incoming BSR");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.77f, 0.47f, 0.92f, 1.0f), "selected BSL");

        if (!m_graph_view.selected_node())
            ImGui::TextDisabled("select an object to reveal its BSR and BSL relationships");

        if (m_graph.empty())
            ImGui::TextDisabled("no executable graph available");
        else {
            m_graph_view.draw("##chuck_graph", m_graph);

            if (m_graph_view.take_selection_changed()) {
                node_id selection = m_graph_view.selected_node();
                rebuild_graph();

                if (selection && m_graph.find_node(selection))
                    m_graph_view.request_focus(selection);
                else if (m_focus_selection)
                    m_graph_view.request_frame();
            }
        }
    }

    [[nodiscard]] inline bool function_matches_filter(
        const c_function_snapshot &function) const {

        if (!*m_function_filter)
            return true;

        const c_executable_snapshot &executable =
            m_snapshot.executables[function.executable_index];
        const c_object_snapshot &object =
            m_snapshot.objects[function.object_index];
        std::string searchable = std::format(
            "{} {:08X} {:08X} {:08X} {:08X}",
            executable.name,
            function.address,
            function.signature_hash,
            function.function_hash,
            object.name_hash);

        return contains_filter(searchable, m_function_filter);
    }

    inline void draw_function_calls(const c_function_snapshot &function) {
        ImGui::SeparatorText("direct callsites");

        if (function.alias_count > 1) {
            ImGui::TextDisabled(
                "shared bytecode body: alias %u of %u across %u object%s",
                function.alias_ordinal + 1,
                function.alias_count,
                function.alias_object_count,
                function.alias_object_count == 1 ? "" : "s");
        }

        if (!function.call_count) {
            ImGui::TextDisabled("no direct BSR or BSL callsites");
            return;
        }

        ImGuiTableFlags flags =
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_ScrollY |
            ImGuiTableFlags_SizingStretchProp;

        if (!ImGui::BeginTable("##function_calls", 5, flags, ImVec2(0.0f, 220.0f)))
            return;

        ImGui::TableSetupColumn("kind", ImGuiTableColumnFlags_WidthFixed, 52.0f);
        ImGui::TableSetupColumn("root offset", ImGuiTableColumnFlags_WidthFixed, 92.0f);
        ImGui::TableSetupColumn("target", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("registry", ImGuiTableColumnFlags_WidthFixed, 90.0f);
        ImGui::TableSetupColumn("address", ImGuiTableColumnFlags_WidthFixed, 92.0f);
        ImGui::TableHeadersRow();

        for (uint32_t call_index = function.first_call; call_index < function.first_call + function.call_count; ++call_index) {
            const c_call_snapshot &call = m_snapshot.calls[call_index];

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(call.kind == e_call_kind::script ? "BSR" : "BSL");
            ImGui::TableNextColumn();
            ImGui::Text("+0x%X", call.code_offset);
            ImGui::TableNextColumn();

            if (call.kind == e_call_kind::script) {
                const c_function_snapshot* target = find_function(call.target_address);

                if (target) {
                    std::string label = std::format(
                        "0x{:08X} / sig 0x{:08X}##call_{}",
                        target->function_hash,
                        target->signature_hash,
                        call_index);

                    if (ImGui::Selectable(label.c_str(), false))
                    {
                        m_selected_executable =
                            m_snapshot.executables[target->executable_index].address;
                        m_selected_function = target->address;
                        rebuild_graph();
                    }
                } else {
                    ImGui::TextDisabled("unresolved script function");
                }

                ImGui::TableNextColumn();
                ImGui::TextDisabled("-");
            } else {
                const c_native_function_snapshot* target =
                    find_native_function(
                        call.native_class_index,
                        call.native_function_index);

                if (target)
                    ImGui::TextUnformatted(target->type_name.c_str());
                else
                    ImGui::TextDisabled("unresolved native descriptor");

                ImGui::TableNextColumn();

                if (call.native_class_index != invalid_index)
                    ImGui::Text("%u:%u",
                        call.native_class_index,
                        call.native_function_index);
                else
                    ImGui::TextDisabled("-");
            }

            ImGui::TableNextColumn();
            ImGui::Text("0x%08X", call.target_address);
        }

        ImGui::EndTable();
    }

    [[nodiscard]] inline const char* indirect_control_kind_name(
        e_indirect_control_kind kind) const {

        switch (kind) {
        case e_indirect_control_kind::receiver_dispatch:
            return "receiver call";
        case e_indirect_control_kind::target_thread:
            return "target thread";
        case e_indirect_control_kind::instance_callback:
            return "instance callback";
        case e_indirect_control_kind::current_callback:
            return "current callback";
        }

        return "unknown";
    }

    inline void draw_indirect_control_candidates(
        const c_indirect_control_snapshot &control) {

        ImGui::Text(
            "possible targets for opcode %u at +0x%X",
            (uint32_t)control.opcode,
            control.code_offset);
        ImGui::TextDisabled(
            "signature matches are an over-approximation, not observed call edges");

        if (!control.signature_hash) {
            ImGui::TextDisabled("prototype is unresolved; no signature is available");
            return;
        }

        ImGuiTableFlags flags =
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_ScrollY |
            ImGuiTableFlags_SizingStretchProp;

        if (!ImGui::BeginTable(
            "##indirect_candidates",
            6,
            flags,
            ImVec2(0.0f, 190.0f)))
            return;

        ImGui::TableSetupColumn(
            "executable",
            ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(
            "object",
            ImGuiTableColumnFlags_WidthFixed,
            92.0f);
        ImGui::TableSetupColumn(
            "function",
            ImGuiTableColumnFlags_WidthFixed,
            92.0f);
        ImGui::TableSetupColumn(
            "address",
            ImGuiTableColumnFlags_WidthFixed,
            92.0f);
        ImGui::TableSetupColumn(
            "body",
            ImGuiTableColumnFlags_WidthFixed,
            72.0f);
        ImGui::TableSetupColumn(
            "relation",
            ImGuiTableColumnFlags_WidthFixed,
            82.0f);
        ImGui::TableHeadersRow();

        std::unordered_set<uint32_t> seen_addresses;

        for (const c_function_snapshot &candidate : m_snapshot.functions) {
            bool exact_callback =
                control.kind == e_indirect_control_kind::current_callback;
            bool matches = exact_callback ?
                candidate.address == control.prototype_address :
                candidate.signature_hash == control.signature_hash;

            if (!matches || !seen_addresses.insert(candidate.address).second)
                continue;

            const c_executable_snapshot &candidate_executable =
                m_snapshot.executables[candidate.executable_index];
            const c_object_snapshot &candidate_object =
                m_snapshot.objects[candidate.object_index];
            bool cross_root =
                candidate_executable.address != control.source_executable;

            ImGui::PushID((int)candidate.address);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            if (ImGui::Selectable(
                candidate_executable.name.c_str(),
                false,
                ImGuiSelectableFlags_SpanAllColumns)) {
                m_selected_executable = candidate_executable.address;
                m_selected_function = candidate.address;
                rebuild_graph();
            }

            ImGui::TableNextColumn();
            ImGui::Text("0x%08X", candidate_object.name_hash);
            ImGui::TableNextColumn();
            ImGui::Text("0x%08X", candidate.function_hash);
            ImGui::TableNextColumn();
            ImGui::Text("0x%08X", candidate.address);
            ImGui::TableNextColumn();
            ImGui::Text("+0x%X", candidate.code_offset);
            ImGui::TableNextColumn();

            if (candidate.address == control.prototype_address)
                ImGui::TextUnformatted("prototype");
            else
                ImGui::TextUnformatted(cross_root ? "cross-root" : "same root");

            ImGui::PopID();
        }

        ImGui::EndTable();
    }

    inline void draw_function_indirect_controls(
        const c_function_snapshot &function) {

        ImGui::SeparatorText("indirect control sites");
        ImGui::TextDisabled(
            "BVSR (74), BTH (9), and ECB/SCB/ECO/SCO callback variants (36-41)");

        if (!function.indirect_control_count) {
            ImGui::TextDisabled("no recognized indirect control sites");
            return;
        }

        ImGuiTableFlags flags =
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_ScrollY |
            ImGuiTableFlags_SizingStretchProp;

        if (ImGui::BeginTable(
            "##indirect_controls",
            7,
            flags,
            ImVec2(0.0f, 190.0f))) {
            ImGui::TableSetupColumn(
                "mode",
                ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn(
                "opcode",
                ImGuiTableColumnFlags_WidthFixed,
                58.0f);
            ImGui::TableSetupColumn(
                "root offset",
                ImGuiTableColumnFlags_WidthFixed,
                92.0f);
            ImGui::TableSetupColumn(
                "prototype",
                ImGuiTableColumnFlags_WidthFixed,
                92.0f);
            ImGui::TableSetupColumn(
                "signature",
                ImGuiTableColumnFlags_WidthFixed,
                92.0f);
            ImGui::TableSetupColumn(
                "possible",
                ImGuiTableColumnFlags_WidthFixed,
                64.0f);
            ImGui::TableSetupColumn(
                "cross-root",
                ImGuiTableColumnFlags_WidthFixed,
                72.0f);
            ImGui::TableHeadersRow();

            for (uint32_t control_index = function.first_indirect_control;
                control_index < function.first_indirect_control +
                    function.indirect_control_count;
                ++control_index) {

                const c_indirect_control_snapshot &control =
                    m_snapshot.indirect_controls[control_index];
                const c_function_snapshot* prototype =
                    find_function(control.prototype_address);

                ImGui::PushID((int)control_index);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                if (ImGui::Selectable(
                    indirect_control_kind_name(control.kind),
                    control_index == m_selected_indirect_control))
                    m_selected_indirect_control = control_index;

                ImGui::TableNextColumn();
                ImGui::Text("%u", (uint32_t)control.opcode);
                ImGui::TableNextColumn();
                ImGui::Text("+0x%X", control.code_offset);
                ImGui::TableNextColumn();

                if (prototype) {
                    std::string label = std::format(
                        "0x{:08X}##prototype",
                        prototype->function_hash);

                    if (ImGui::Selectable(label.c_str(), false)) {
                        m_selected_executable =
                            m_snapshot.executables[
                                prototype->executable_index].address;
                        m_selected_function = prototype->address;
                        rebuild_graph();
                    }
                } else {
                    ImGui::TextDisabled("unresolved");
                }

                ImGui::TableNextColumn();

                if (control.signature_hash)
                    ImGui::Text("0x%08X", control.signature_hash);
                else
                    ImGui::TextDisabled("-");

                ImGui::TableNextColumn();
                ImGui::Text("%u", control.candidate_count);
                ImGui::TableNextColumn();
                ImGui::Text("%u", control.cross_root_candidate_count);
                ImGui::PopID();
            }

            ImGui::EndTable();
        }

        if (m_selected_indirect_control >= function.first_indirect_control &&
            m_selected_indirect_control < function.first_indirect_control +
                function.indirect_control_count)
            draw_indirect_control_candidates(
                m_snapshot.indirect_controls[m_selected_indirect_control]);
        else
            ImGui::TextDisabled("select a site to inspect possible targets");
    }

    inline void draw_chain_function_step(
        const c_call_chain_step &step,
        uint32_t ordinal,
        const char* call_kind) const {

        if (step.function_index >= m_snapshot.functions.size())
            return;

        const c_function_snapshot &function =
            m_snapshot.functions[step.function_index];
        const c_executable_snapshot &executable =
            m_snapshot.executables[function.executable_index];
        const c_object_snapshot &object =
            m_snapshot.objects[function.object_index];

        ImGui::TextWrapped(
            "%u. %s | object 0x%08X | function 0x%08X | body +0x%X -- %s +0x%X -->",
            ordinal,
            executable.name.c_str(),
            object.name_hash,
            function.function_hash,
            function.code_offset,
            call_kind,
            step.call_offset);
    }

    inline void draw_script_call_chain(
        const c_reachable_script_body &reachable) const {

        for (uint32_t step_index = 0;
            step_index < reachable.path.size();
            ++step_index)
            draw_chain_function_step(
                reachable.path[step_index],
                step_index + 1,
                "BSR");

        if (reachable.snapshot_function_index >= m_snapshot.functions.size())
            return;

        const c_function_snapshot &target =
            m_snapshot.functions[reachable.snapshot_function_index];
        const c_executable_snapshot &executable =
            m_snapshot.executables[target.executable_index];
        const c_object_snapshot &object =
            m_snapshot.objects[target.object_index];

        ImGui::TextColored(
            ImVec4(0.43f, 0.76f, 1.0f, 1.0f),
            "%u. %s | object 0x%08X | function 0x%08X | body +0x%X",
            (uint32_t)reachable.path.size() + 1,
            executable.name.c_str(),
            object.name_hash,
            target.function_hash,
            target.code_offset);
    }

    inline void draw_native_call_chain(
        const c_reachable_native &reachable) const {

        for (uint32_t step_index = 0;
            step_index < reachable.path.size();
            ++step_index) {

            bool native_call = step_index + 1 == reachable.path.size();

            draw_chain_function_step(
                reachable.path[step_index],
                step_index + 1,
                native_call ? "BSL" : "BSR");
        }

        const c_native_function_snapshot* target =
            find_native_function(
                reachable.class_index,
                reachable.function_index);
        const char* type_name = target ?
            target->type_name.c_str() :
            "<unresolved native descriptor>";

        if (reachable.class_index == invalid_index) {
            ImGui::TextColored(
                ImVec4(0.88f, 0.62f, 0.34f, 1.0f),
                "%u. %s 0x%08X",
                (uint32_t)reachable.path.size() + 1,
                type_name,
                reachable.address);
        } else {
            ImGui::TextColored(
                ImVec4(0.79f, 0.55f, 0.94f, 1.0f),
                "%u. native [%u:%u] %s | descriptor 0x%08X",
                (uint32_t)reachable.path.size() + 1,
                reachable.class_index,
                reachable.function_index,
                type_name,
                reachable.address);
        }
    }

    inline void draw_transitive_natives() {
        ImGuiTableFlags flags =
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_ScrollY |
            ImGuiTableFlags_SizingStretchProp;

        if (ImGui::BeginTable(
            "##transitive_natives",
            7,
            flags,
            ImVec2(0.0f, 220.0f))) {

            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("depth", ImGuiTableColumnFlags_WidthFixed, 48.0f);
            ImGui::TableSetupColumn("registry", ImGuiTableColumnFlags_WidthFixed, 76.0f);
            ImGui::TableSetupColumn("native", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("via function", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableSetupColumn("callsite", ImGuiTableColumnFlags_WidthFixed, 72.0f);
            ImGui::TableSetupColumn("sites", ImGuiTableColumnFlags_WidthFixed, 46.0f);
            ImGui::TableSetupColumn("descriptor", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableHeadersRow();

            ImGuiListClipper clipper;
            clipper.Begin((int)m_transitive_analysis.natives.size());

            while (clipper.Step()) {
                for (int row = clipper.DisplayStart;
                    row < clipper.DisplayEnd;
                    ++row) {

                    const c_reachable_native &reachable =
                        m_transitive_analysis.natives[row];
                    const c_native_function_snapshot* target =
                        find_native_function(
                            reachable.class_index,
                            reachable.function_index);
                    const char* type_name = target ?
                        target->type_name.c_str() :
                        "<unresolved>";
                    const c_call_chain_step* final_step =
                        reachable.path.empty() ?
                            nullptr : &reachable.path.back();
                    const c_function_snapshot* source =
                        final_step && final_step->function_index <
                            m_snapshot.functions.size() ?
                            &m_snapshot.functions[final_step->function_index] :
                            nullptr;

                    ImGui::PushID((int)reachable.address);
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%u", reachable.call_depth);
                    ImGui::TableNextColumn();

                    if (reachable.class_index == invalid_index)
                        ImGui::TextDisabled("-");
                    else
                        ImGui::Text("%u:%u",
                            reachable.class_index,
                            reachable.function_index);

                    ImGui::TableNextColumn();

                    if (ImGui::Selectable(
                        type_name,
                        reachable.address == m_selected_transitive_native,
                        ImGuiSelectableFlags_SpanAllColumns))
                        m_selected_transitive_native = reachable.address;

                    ImGui::TableNextColumn();

                    if (source)
                        ImGui::Text("0x%08X", source->function_hash);
                    else
                        ImGui::TextDisabled("-");

                    ImGui::TableNextColumn();

                    if (final_step)
                        ImGui::Text("+0x%X", final_step->call_offset);
                    else
                        ImGui::TextDisabled("-");

                    ImGui::TableNextColumn();
                    ImGui::Text("%u", reachable.callsite_count);
                    ImGui::TableNextColumn();
                    ImGui::Text("0x%08X", reachable.address);
                    ImGui::PopID();
                }
            }

            ImGui::EndTable();
        }

        const c_reachable_native* selected = nullptr;

        for (const c_reachable_native &reachable :
            m_transitive_analysis.natives) {
            if (reachable.address == m_selected_transitive_native) {
                selected = &reachable;
                break;
            }
        }

        if (!selected)
            return;

        ImGui::SeparatorText("shortest call chain");
        draw_native_call_chain(*selected);
    }

    inline void draw_transitive_scripts() {
        ImGuiTableFlags flags =
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_ScrollY |
            ImGuiTableFlags_SizingStretchProp;

        if (ImGui::BeginTable(
            "##transitive_scripts",
            7,
            flags,
            ImVec2(0.0f, 220.0f))) {

            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("depth", ImGuiTableColumnFlags_WidthFixed, 48.0f);
            ImGui::TableSetupColumn("function", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableSetupColumn("signature", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableSetupColumn("executable", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("object", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableSetupColumn("body", ImGuiTableColumnFlags_WidthFixed, 72.0f);
            ImGui::TableSetupColumn("recursion", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableHeadersRow();

            ImGuiListClipper clipper;
            clipper.Begin((int)m_transitive_analysis.script_bodies.size());

            while (clipper.Step()) {
                for (int row = clipper.DisplayStart;
                    row < clipper.DisplayEnd;
                    ++row) {

                    const c_reachable_script_body &reachable =
                        m_transitive_analysis.script_bodies[row];
                    const c_function_snapshot &function =
                        m_snapshot.functions[reachable.snapshot_function_index];
                    const c_executable_snapshot &executable =
                        m_snapshot.executables[function.executable_index];
                    const c_object_snapshot &object =
                        m_snapshot.objects[function.object_index];

                    ImGui::PushID((int)function.address);
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%u", reachable.depth);
                    ImGui::TableNextColumn();

                    std::string label = std::format(
                        "0x{:08X}",
                        function.function_hash);

                    if (ImGui::Selectable(
                        label.c_str(),
                        function.address == m_selected_transitive_script,
                        ImGuiSelectableFlags_SpanAllColumns))
                        m_selected_transitive_script = function.address;

                    ImGui::TableNextColumn();
                    ImGui::Text("0x%08X", function.signature_hash);
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(executable.name.c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("0x%08X", object.name_hash);
                    ImGui::TableNextColumn();
                    ImGui::Text("+0x%X", function.code_offset);
                    ImGui::TableNextColumn();

                    if (reachable.recursion_group)
                        ImGui::Text("group %u (%u)",
                            reachable.recursion_group,
                            reachable.recursion_group_size);
                    else
                        ImGui::TextDisabled("-");

                    ImGui::PopID();
                }
            }

            ImGui::EndTable();
        }

        const c_reachable_script_body* selected = nullptr;

        for (const c_reachable_script_body &reachable :
            m_transitive_analysis.script_bodies) {
            const c_function_snapshot &function =
                m_snapshot.functions[reachable.snapshot_function_index];

            if (function.address == m_selected_transitive_script) {
                selected = &reachable;
                break;
            }
        }

        if (!selected)
            return;

        ImGui::SeparatorText("shortest call chain");
        draw_script_call_chain(*selected);

        if (ImGui::Button("use selected body as source")) {
            const c_function_snapshot &function =
                m_snapshot.functions[selected->snapshot_function_index];
            const c_executable_snapshot &executable =
                m_snapshot.executables[function.executable_index];
            bool executable_changed =
                executable.address != m_selected_executable;

            m_selected_executable = executable.address;
            m_selected_function = function.address;

            if (executable_changed) {
                m_graph_view.reset_layout();
            }

            rebuild_graph();
        }
    }

    inline void draw_transitive_analysis(
        const c_function_snapshot &source_function) {

        ensure_transitive_analysis(source_function);

        if (!ImGui::CollapsingHeader(
            "transitive reachability",
            ImGuiTreeNodeFlags_DefaultOpen))
            return;

        if (!m_transitive_analysis.available) {
            ImGui::TextDisabled(
                "the selected function does not have a validated bytecode body");
            return;
        }

        ImGui::Text(
            "%u downstream bodies | %u native capabilities across %u callsites | %u BSR callsites | %u unresolved BSR",
            (uint32_t)m_transitive_analysis.script_bodies.size(),
            (uint32_t)m_transitive_analysis.natives.size(),
            m_transitive_analysis.reachable_native_callsite_count,
            m_transitive_analysis.reachable_script_callsite_count,
            m_transitive_analysis.unresolved_script_callsite_count);
        ImGui::TextDisabled(
            "%u recursive bodies in %u group%s%s | shortest-path analysis %.3f ms",
            m_transitive_analysis.recursive_body_count,
            m_transitive_analysis.recursion_group_count,
            m_transitive_analysis.recursion_group_count == 1 ? "" : "s",
            m_transitive_analysis.source_recursive ?
                " | source is recursive" : "",
            m_transitive_analysis.milliseconds);

        if (ImGui::BeginTabBar("##transitive_tabs")) {
            if (ImGui::BeginTabItem("native capabilities")) {
                if (m_transitive_analysis.natives.empty())
                    ImGui::TextDisabled("no reachable BSL callsites");
                else
                    draw_transitive_natives();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("script bodies")) {
                if (m_transitive_analysis.script_bodies.empty())
                    ImGui::TextDisabled("no downstream BSR targets");
                else
                    draw_transitive_scripts();

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    inline void draw_functions_tab() {
        bool executable_changed = draw_executable_selector("##function_executable");

        if (executable_changed) {
            const c_executable_snapshot* executable =
                find_executable(m_selected_executable);

            m_selected_function = executable && executable->function_count ?
                m_snapshot.functions[executable->first_function].address : 0;
            rebuild_graph();
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1.0f);
        ImGui::InputTextWithHint(
            "##function_filter",
            "filter executable, address, object/function/signature hash",
            m_function_filter,
            sizeof(m_function_filter));

        const c_executable_snapshot* executable =
            find_executable(m_selected_executable);

        if (!executable) {
            ImGui::TextDisabled("no executable selected");
            return;
        }

        std::vector<uint32_t> visible_functions;

        for (uint32_t function_index = executable->first_function; function_index < executable->first_function + executable->function_count; ++function_index) {
            if (function_matches_filter(m_snapshot.functions[function_index]))
                visible_functions.push_back(function_index);
        }

        ImGuiTableFlags flags =
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_ScrollY |
            ImGuiTableFlags_SizingStretchProp;

        if (ImGui::BeginTable("##functions", 10, flags, ImVec2(0.0f, 330.0f))) {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("address", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableSetupColumn("object", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableSetupColumn("slot", ImGuiTableColumnFlags_WidthFixed, 42.0f);
            ImGui::TableSetupColumn("function hash", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableSetupColumn("signature", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableSetupColumn("code", ImGuiTableColumnFlags_WidthFixed, 82.0f);
            ImGui::TableSetupColumn("alias", ImGuiTableColumnFlags_WidthFixed, 54.0f);
            ImGui::TableSetupColumn("size", ImGuiTableColumnFlags_WidthFixed, 62.0f);
            ImGui::TableSetupColumn("BSR / BSL / indirect", ImGuiTableColumnFlags_WidthFixed, 126.0f);
            ImGui::TableSetupColumn("threads", ImGuiTableColumnFlags_WidthFixed, 58.0f);
            ImGui::TableHeadersRow();

            ImGuiListClipper clipper;
            clipper.Begin((int)visible_functions.size());

            while (clipper.Step()) {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {
                    const c_function_snapshot &function =
                        m_snapshot.functions[visible_functions[row]];
                    const c_object_snapshot &object =
                        m_snapshot.objects[function.object_index];

                    ImGui::PushID((int)function.address);
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    std::string address = std::format("0x{:08X}", function.address);

                    if (ImGui::Selectable(
                        address.c_str(),
                        function.address == m_selected_function,
                        ImGuiSelectableFlags_SpanAllColumns))
                        m_selected_function = function.address;

                    ImGui::TableNextColumn();
                    ImGui::Text("0x%08X", object.name_hash);
                    ImGui::TableNextColumn();
                    ImGui::Text("%u", function.local_index);
                    ImGui::TableNextColumn();
                    ImGui::Text("0x%08X", function.function_hash);
                    ImGui::TableNextColumn();
                    ImGui::Text("0x%08X", function.signature_hash);
                    ImGui::TableNextColumn();
                    ImGui::Text("+0x%X", function.code_offset);
                    ImGui::TableNextColumn();

                    if (function.alias_count > 1)
                        ImGui::Text("%u/%u",
                            function.alias_ordinal + 1,
                            function.alias_count);
                    else
                        ImGui::TextDisabled("-");

                    ImGui::TableNextColumn();
                    ImGui::Text("0x%X", function.code_size);
                    ImGui::TableNextColumn();
                    ImGui::Text("%u / %u / %u",
                        function.script_call_count,
                        function.native_call_count,
                        function.indirect_control_count);
                    ImGui::TableNextColumn();
                    ImGui::Text("%u", function.live_thread_count);
                    ImGui::PopID();
                }
            }

            ImGui::EndTable();
        }

        const c_function_snapshot* selected =
            find_function(m_selected_function);

        if (!selected)
            return;

        const c_object_snapshot &object =
            m_snapshot.objects[selected->object_index];

        ImGui::Text(
            "script_function 0x%08X | object 0x%08X | body +0x%X (0x%X bytes) | alias %u/%u | stack metadata 0x%08X | flags 0x%04X | live threads %u",
            selected->address,
            object.address,
            selected->code_offset,
            selected->code_size,
            selected->alias_ordinal + 1,
            selected->alias_count,
            selected->stack_metadata,
            selected->flags,
            selected->live_thread_count);

        draw_function_calls(*selected);
        draw_function_indirect_controls(*selected);
        draw_transitive_analysis(*selected);
    }

    [[nodiscard]] inline bool thread_matches_filter(
        const c_thread_snapshot &thread) const {

        if (!*m_thread_filter)
            return true;

        const c_executable_snapshot &executable =
            m_snapshot.executables[thread.executable_index];
        const c_function_snapshot* function =
            find_function(thread.function_address);
        std::string searchable = std::format(
            "{} {:08X} {:08X} {} {:08X}",
            executable.name,
            thread.address,
            thread.function_address,
            thread.id,
            function ? function->function_hash : 0);

        return contains_filter(searchable, m_thread_filter);
    }

    inline void draw_threads_tab() {
        ImGui::SetNextItemWidth(-1.0f);
        ImGui::InputTextWithHint(
            "##thread_filter",
            "filter executable, thread/function address, ID or function hash",
            m_thread_filter,
            sizeof(m_thread_filter));

        std::vector<uint32_t> visible_threads;

        for (uint32_t thread_index = 0; thread_index < m_snapshot.threads.size(); ++thread_index) {
            if (thread_matches_filter(m_snapshot.threads[thread_index]))
                visible_threads.push_back(thread_index);
        }

        ImGui::TextDisabled("%u live threads in %u instances; stack usage is cursor - data from the locked snapshot",
            (uint32_t)m_snapshot.threads.size(),
            (uint32_t)m_snapshot.instances.size());

        ImGuiTableFlags flags =
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_ScrollY |
            ImGuiTableFlags_SizingStretchProp;

        if (!ImGui::BeginTable("##threads", 11, flags))
            return;

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 72.0f);
        ImGui::TableSetupColumn("thread", ImGuiTableColumnFlags_WidthFixed, 92.0f);
        ImGui::TableSetupColumn("executable", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("function", ImGuiTableColumnFlags_WidthFixed, 92.0f);
        ImGui::TableSetupColumn("function hash", ImGuiTableColumnFlags_WidthFixed, 92.0f);
        ImGui::TableSetupColumn("PC", ImGuiTableColumnFlags_WidthFixed, 92.0f);
        ImGui::TableSetupColumn("stack", ImGuiTableColumnFlags_WidthFixed, 78.0f);
        ImGui::TableSetupColumn("base", ImGuiTableColumnFlags_WidthFixed, 92.0f);
        ImGui::TableSetupColumn("recall", ImGuiTableColumnFlags_WidthFixed, 48.0f);
        ImGui::TableSetupColumn("flow", ImGuiTableColumnFlags_WidthFixed, 42.0f);
        ImGui::TableSetupColumn("instance", ImGuiTableColumnFlags_WidthFixed, 92.0f);
        ImGui::TableHeadersRow();

        ImGuiListClipper clipper;
        clipper.Begin((int)visible_threads.size());

        while (clipper.Step()) {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {
                const c_thread_snapshot &thread =
                    m_snapshot.threads[visible_threads[row]];
                const c_executable_snapshot &executable =
                    m_snapshot.executables[thread.executable_index];
                const c_function_snapshot* function =
                    find_function(thread.function_address);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%u", thread.id);
                ImGui::TableNextColumn();
                ImGui::Text("0x%08X", thread.address);
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(executable.name.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("0x%08X", thread.function_address);
                ImGui::TableNextColumn();
                ImGui::Text("0x%08X", function ? function->function_hash : 0);
                ImGui::TableNextColumn();
                ImGui::Text("0x%08X", thread.pc);
                ImGui::TableNextColumn();
                ImGui::Text("%u/%u", thread.stack_size, thread.stack_capacity);
                ImGui::TableNextColumn();
                ImGui::Text("0x%08X", thread.stack_data);
                ImGui::TableNextColumn();
                ImGui::Text("%u", thread.native_recall);
                ImGui::TableNextColumn();
                ImGui::Text("%u", thread.flow_depth);
                ImGui::TableNextColumn();
                ImGui::Text("0x%08X", thread.instance_address);
            }
        }

        ImGui::EndTable();
    }

    [[nodiscard]] inline bool native_matches_filter(
        const c_native_function_snapshot &function) const {

        if (!*m_native_filter)
            return true;

        std::string searchable = std::format(
            "{} {} {} {:08X} {:08X}",
            function.type_name,
            function.class_index,
            function.function_index,
            function.address,
            function.callback);

        return contains_filter(searchable, m_native_filter);
    }

    inline void draw_natives_tab() {
        ImGui::SetNextItemWidth(-1.0f);
        ImGui::InputTextWithHint(
            "##native_filter",
            "filter RTTI name, class:function index, descriptor or callback",
            m_native_filter,
            sizeof(m_native_filter));

        ImGui::TextDisabled("registry 0x%08X | %u classes (%u appended, %u external, %u unresolved RTTI) | %u functions (%u appended, %u external, %u unresolved RTTI)",
            m_snapshot.registry_address,
            (uint32_t)m_snapshot.native_classes.size(),
            m_snapshot.appended_native_class_count,
            m_snapshot.external_native_class_count,
            m_snapshot.unresolved_native_class_rtti_count,
            (uint32_t)m_snapshot.native_functions.size(),
            m_snapshot.appended_native_function_count,
            m_snapshot.external_native_function_count,
            m_snapshot.unresolved_native_function_rtti_count);

        ImGuiTableFlags flags =
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_ScrollY |
            ImGuiTableFlags_SizingStretchProp;

        if (ImGui::BeginTable("##native_classes", 6, flags, ImVec2(0.0f, 230.0f))) {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("index", ImGuiTableColumnFlags_WidthFixed, 48.0f);
            ImGui::TableSetupColumn("RTTI type", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("address", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableSetupColumn("value size", ImGuiTableColumnFlags_WidthFixed, 72.0f);
            ImGui::TableSetupColumn("functions", ImGuiTableColumnFlags_WidthFixed, 62.0f);
            ImGui::TableSetupColumn("parent", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();

            for (const c_native_class_snapshot &native_class : m_snapshot.native_classes) {
                ImGui::PushID((int)native_class.index);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                std::string index = std::to_string(native_class.index);

                if (ImGui::Selectable(
                    index.c_str(),
                    native_class.index == m_selected_native_class,
                    ImGuiSelectableFlags_SpanAllColumns))
                    m_selected_native_class = native_class.index;

                ImGui::TableNextColumn();
                ImGui::TextUnformatted(native_class.type_name.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("0x%08X", native_class.address);
                ImGui::TableNextColumn();
                ImGui::Text("%d", native_class.value_size);
                ImGui::TableNextColumn();
                ImGui::Text("%u", native_class.function_count);
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(native_class.parent_name.empty() ?
                    "<none>" : native_class.parent_name.c_str());
                ImGui::PopID();
            }

            ImGui::EndTable();
        }

        const c_native_class_snapshot* selected =
            find_native_class(m_selected_native_class);

        if (!selected)
            return;

        std::vector<uint32_t> visible_functions;

        for (uint32_t function_index = selected->first_function;
            function_index < selected->first_function + selected->function_count;
            ++function_index) {

            if (native_matches_filter(m_snapshot.native_functions[function_index]))
                visible_functions.push_back(function_index);
        }

        ImGui::SeparatorText(selected->type_name.c_str());

        if (!ImGui::BeginTable("##native_functions", 4, flags))
            return;

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("index", ImGuiTableColumnFlags_WidthFixed, 48.0f);
        ImGui::TableSetupColumn("RTTI type", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("descriptor", ImGuiTableColumnFlags_WidthFixed, 92.0f);
        ImGui::TableSetupColumn("callback", ImGuiTableColumnFlags_WidthFixed, 92.0f);
        ImGui::TableHeadersRow();

        ImGuiListClipper clipper;
        clipper.Begin((int)visible_functions.size());

        while (clipper.Step()) {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {
                const c_native_function_snapshot &function =
                    m_snapshot.native_functions[visible_functions[row]];

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%u", function.function_index);
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(function.type_name.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("0x%08X", function.address);
                ImGui::TableNextColumn();
                ImGui::Text("0x%08X", function.callback);
            }
        }

        ImGui::EndTable();
    }

public:
    bool logic() { return true; }

    void draw() {
        ImGui::SetNextWindowSize(ImVec2(1280.0f, 820.0f), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin("chuckvm")) {
            ImGui::End();
            return;
        }

        double now = ImGui::GetTime();

        if (m_auto_refresh && now - m_last_refresh >= m_refresh_interval)
            m_refresh_requested = true;

        if (m_refresh_requested)
            refresh_snapshot();

        if (ImGui::Button("refresh snapshot"))
            m_refresh_requested = true;

        ImGui::SameLine();
        ImGui::Checkbox("auto", &m_auto_refresh);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(120.0f);
        ImGui::SliderFloat("seconds", &m_refresh_interval, 0.25f, 5.0f, "%.2f");

        ImGui::SameLine();
        ImGui::TextDisabled("runtime 0x%08X | tree 0x%08X | snapshot %.2fs old",
            m_snapshot.runtime_address,
            m_snapshot.tree_address,
            now - m_last_refresh);

        ImGui::Text("%u executables | %u objects | %u functions | %u bodies | %u aliases | %u instances | %u threads | %u physical BSR | %u physical BSL | %u indirect sites",
            (uint32_t)m_snapshot.executables.size(),
            (uint32_t)m_snapshot.objects.size(),
            (uint32_t)m_snapshot.functions.size(),
            m_snapshot.function_body_count,
            m_snapshot.function_alias_count,
            (uint32_t)m_snapshot.instances.size(),
            (uint32_t)m_snapshot.threads.size(),
            m_snapshot.script_call_count,
            m_snapshot.native_call_count,
            m_snapshot.indirect_control_count);

        ImGui::TextDisabled("capture %.3f ms (max %.3f) | native registry %.3f ms | %llu snapshots",
            m_snapshot.capture_milliseconds,
            m_max_capture_milliseconds,
            m_snapshot.native_snapshot_milliseconds,
            m_snapshot_count);
        ImGui::TextDisabled("runtime lock wait %.3f ms (max %.3f) | held %.3f ms (max %.3f) | post-lock analysis %.3f ms",
            m_snapshot.runtime_lock_wait_milliseconds,
            m_max_runtime_lock_wait_milliseconds,
            m_snapshot.runtime_lock_milliseconds,
            m_max_runtime_lock_milliseconds,
            m_snapshot.post_lock_analysis_milliseconds);
        ImGui::TextDisabled("locked work: %.2f MiB code copy %.3f ms | instance/thread traversal %.3f ms | other metadata %.3f ms",
            (double)m_snapshot.code_image_bytes / (1024.0 * 1024.0),
            m_snapshot.code_copy_milliseconds,
            m_snapshot.instance_thread_snapshot_milliseconds,
            (std::max)(
                0.0,
                m_snapshot.runtime_lock_milliseconds -
                    m_snapshot.code_copy_milliseconds -
                    m_snapshot.instance_thread_snapshot_milliseconds));

        if (!m_snapshot.runtime_available)
            ImGui::TextColored(
                ImVec4(1.0f, 0.55f, 0.35f, 1.0f),
                "script runtime is null; native registry data may still be available");

        if (ImGui::BeginTabBar("##chuck_tabs")) {
            if (ImGui::BeginTabItem("call graph")) {
                draw_graph_tab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("functions + calls")) {
                draw_functions_tab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("live threads")) {
                draw_threads_tab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("native registry")) {
                draw_natives_tab();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }
};

#ifndef NDEBUG
    gui_script_node_view gui {};
#endif
