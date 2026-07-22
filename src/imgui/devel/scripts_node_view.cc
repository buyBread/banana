#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <unordered_set>

#include "../base.hh"
#include "../ext/node_graph/node_graph.hh"

#include "../../ngl/chuck/script_runtime.hh"

class gui_script_node_view : public c_gui {
    using node_id      = IMGUI_EXT::node_graph::node_id;
    using c_graph      = IMGUI_EXT::node_graph::c_graph;
    using c_node       = IMGUI_EXT::node_graph::c_node;
    using c_node_style = IMGUI_EXT::node_graph::c_node_style;
    using c_view       = IMGUI_EXT::node_graph::c_view;

    static constexpr uint32_t invalid_index = UINT32_MAX;

    enum class e_call_kind : uint8_t {
        script,
        native
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
        uint32_t  opcode;
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

    struct c_function_snapshot {
        uint32_t executable_index;
        uint32_t object_index;
        uint32_t local_index;
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
        uint32_t    script_call_count;
        uint32_t    native_call_count;
        std::string name;
    };

    struct c_snapshot {
        uint32_t runtime_address;
        uint32_t tree_address;
        uint32_t registry_address;
        uint32_t reported_executable_count;
        uint32_t script_call_count;
        uint32_t native_call_count;
        bool     runtime_available;

        std::vector<c_executable_snapshot>  executables;
        std::vector<c_object_snapshot>      objects;
        std::vector<c_function_snapshot>    functions;
        std::vector<c_instance_snapshot>    instances;
        std::vector<c_thread_snapshot>      threads;
        std::vector<c_call_snapshot>        calls;
        std::vector<c_native_class_snapshot> native_classes;
        std::vector<c_native_function_snapshot> native_functions;
    };

    struct c_graph_edge {
        node_id           source;
        node_id           target;
        uint32_t          count;
        e_graph_edge_kind kind;
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
    node_id  m_cached_graph_selection = 0;

    bool   m_refresh_requested = true;
    bool   m_auto_refresh      = false;
    bool   m_show_script_calls = true;
    bool   m_show_native_calls = true;
    bool   m_focus_selection   = true;
    float  m_refresh_interval  = 1.0f;
    double m_last_refresh      = 0.0;

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
        output->opcode = (header >> 8) & 0x7F;

        return true;
    }

    inline void snapshot_native_registry(
        c_snapshot* snapshot,
        std::unordered_map<uint32_t, c_native_coordinate>* function_lookup) const {

        NGL_CHUCK::vm::script_library_registry* registry =
            NGL_CHUCK::script_library_registry.read();

        snapshot->registry_address = (uint32_t)registry;

        if (!registry || !registry->m_classes)
            return;

        uint32_t class_count = registry->class_count();

        if (class_count > 4096)
            return;

        for (uint32_t class_index = 0; class_index < class_count; ++class_index) {
            NGL_CHUCK::vm::script_library_class* native_class =
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

            if (class_snapshot.type_name == "<unknown>" ||
                class_snapshot.type_name == "<invalid>")
                class_snapshot.type_name = builtin_class_name(class_index);

            uint32_t function_count = native_class->function_count();

            if (function_count > 65536)
                function_count = 0;

            for (uint32_t function_index = 0; function_index < function_count; ++function_index) {
                NGL_CHUCK::vm::script_library_function* function =
                    native_class->find_function(function_index);

                if (!function)
                    continue;

                c_native_function_snapshot function_snapshot {};

                function_snapshot.class_index = class_index;
                function_snapshot.function_index = function_index;
                function_snapshot.address = (uint32_t)function;
                function_snapshot.callback = (uint32_t)function->callback();
                function_snapshot.type_name = rtti_type_name(function);

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
    }

    inline void snapshot_instance_threads(
        c_snapshot* snapshot,
        NGL_CHUCK::vm::script_object* object,
        uint32_t executable_index,
        uint32_t object_index) const {

        if (!object || !object->instance_guard)
            return;

        NGL_MUTEX::engine_activity_scope object_scope(object->instance_guard);
        NGL_CHUCK::vm::script_instance* instance = object->first_instance;

        for (uint32_t instance_ordinal = 0;
            instance && instance_ordinal < 65536;
            ++instance_ordinal) {

            c_instance_snapshot instance_snapshot {};

            instance_snapshot.executable_index = executable_index;
            instance_snapshot.object_index = object_index;
            instance_snapshot.address = (uint32_t)instance;
            instance_snapshot.flags = instance->flags;
            instance_snapshot.first_thread = (uint32_t)snapshot->threads.size();

            NGL_CHUCK::vm::script_instance* next_instance = instance->next;

            {
                NGL_MUTEX::engine_lock_scope instance_scope(&instance->thread_lock);

                instance_snapshot.reported_thread_count = instance->thread_count;

                NGL_CHUCK::vm::vm_thread* thread = instance->first_thread;
                uint32_t maximum_threads = instance->thread_count >= 65535 ?
                    65536u : instance->thread_count + 1;

                for (uint32_t thread_ordinal = 0; thread && thread_ordinal < maximum_threads; ++thread_ordinal) {
                    c_thread_snapshot thread_snapshot {};

                    thread_snapshot.executable_index    = executable_index;
                    thread_snapshot.object_index        = object_index;
                    thread_snapshot.instance_address    = (uint32_t)instance;
                    thread_snapshot.address             = (uint32_t)thread;
                    thread_snapshot.id                  = thread->id;
                    thread_snapshot.function_address    = (uint32_t)thread->executable;
                    thread_snapshot.parent_address      = (uint32_t)thread->parent;
                    thread_snapshot.pc                  = (uint32_t)thread->pc;
                    thread_snapshot.current_instance    = (uint32_t)thread->current_instance;
                    thread_snapshot.constructor_context = (uint32_t)thread->constructor_context;
                    thread_snapshot.stack_cursor        = (uint32_t)thread->stack.cursor;
                    thread_snapshot.stack_allocation    = (uint32_t)thread->stack.allocation;
                    thread_snapshot.stack_data          = (uint32_t)thread->stack.data;
                    thread_snapshot.stack_capacity      = thread->stack.capacity;
                    thread_snapshot.stack_size          = thread->stack.size();
                    thread_snapshot.native_recall       = thread->native_recall;

                    NGL_CHUCK::vm::vm_flow_frame* frame = thread->flow_frame;

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

    inline void snapshot_runtime_objects(
        c_snapshot* snapshot,
        const std::unordered_map<uint32_t, c_native_coordinate> &native_lookup) const {
        NGL_CHUCK::vm::script_runtime* runtime = NGL_CHUCK::script_runtime.read();

        snapshot->runtime_address = (uint32_t)runtime;
        snapshot->runtime_available = runtime != nullptr;

        if (!runtime)
            return;

        NGL_MUTEX::engine_lock_scope runtime_scope(&runtime->loaded_executables_lock);
        NGL_CHUCK::vm::script_executable_tree* tree = runtime->loaded_executables;

        snapshot->tree_address = (uint32_t)tree;

        if (!tree || !tree->head || tree->count > 4096)
            return;

        snapshot->reported_executable_count = tree->count;

        NGL_CHUCK::vm::script_executable_tree_node* end = tree->end();
        NGL_CHUCK::vm::script_executable_tree_node* tree_node = tree->begin();

        for (uint32_t tree_index = 0;
            tree_node != end && tree_index < tree->count;
            ++tree_index, tree_node = tree_node->successor()) {

            if (!tree_node || !tree_node->entry)
                break;

            NGL_CHUCK::vm::script_executable* executable = tree_node->get();

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

            uint32_t name_length = (std::min)(executable->name_length, 192u);
            executable_snapshot.name = executable->name ?
                copy_string(executable->name, name_length) :
                "<unnamed executable>";

            uint32_t object_count = (std::min)(executable->object_count, 65536u);

            for (uint32_t local_object_index = 0;
                local_object_index < object_count;
                ++local_object_index) {

                NGL_CHUCK::vm::script_object* object =
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

                    NGL_CHUCK::vm::vm_executable* function =
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

                snapshot_instance_threads(
                    snapshot,
                    object,
                    executable_index,
                    object_index);

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

        std::unordered_map<uint32_t, uint32_t> function_lookup;

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
    }

    inline void snapshot_calls(
        c_snapshot* snapshot,
        const std::unordered_map<uint32_t, c_native_coordinate> &native_lookup) const {

        std::unordered_map<uint32_t, uint32_t> object_lookup;
        std::unordered_map<uint32_t, uint32_t> function_lookup;

        for (uint32_t object_index = 0; object_index < snapshot->objects.size(); ++object_index)
            object_lookup[snapshot->objects[object_index].address] = object_index;

        for (uint32_t function_index = 0; function_index < snapshot->functions.size(); ++function_index)
            function_lookup[snapshot->functions[function_index].address] = function_index;

        for (uint32_t executable_index = 0;
            executable_index < snapshot->executables.size();
            ++executable_index) {

            c_executable_snapshot &executable = snapshot->executables[executable_index];

            if (!executable.code_address || !executable.code_size)
                continue;

            uint8_t* code_start = (uint8_t*)executable.code_address;
            uint8_t* code_end = code_start + executable.code_size;
            std::unordered_set<uint32_t> boundaries;
            uint8_t* cursor = code_start;
            bool grammar_valid = true;

            while (cursor < code_end) {
                boundaries.insert((uint32_t)cursor);

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
                    return
                        snapshot->functions[left].code_address <
                        snapshot->functions[right].code_address;
                });

            for (uint32_t ordered_index = 0;
                ordered_index < ordered_functions.size();
                ++ordered_index) {

                uint32_t function_index = ordered_functions[ordered_index];
                c_function_snapshot &function = snapshot->functions[function_index];
                uint32_t function_end = (uint32_t)code_end;

                for (uint32_t next = ordered_index + 1;
                    next < ordered_functions.size();
                    ++next) {

                    uint32_t candidate =
                        snapshot->functions[ordered_functions[next]].code_address;

                    if (candidate > function.code_address) {
                        function_end = candidate;
                        break;
                    }
                }

                function.code_size = function_end - function.code_address;
                function.first_call = (uint32_t)snapshot->calls.size();

                if (!boundaries.contains(function.code_address) ||
                    (function_end != (uint32_t)code_end && !boundaries.contains(function_end)))
                    continue;

                uint8_t* function_cursor = (uint8_t*)function.code_address;

                while ((uint32_t)function_cursor < function_end) {
                    c_instruction_view instruction {};

                    if (!decode_instruction(function_cursor, (uint8_t*)function_end, &instruction))
                        break;

                    bool script_call =
                        instruction.opcode == 7 &&
                        instruction.operand_kind == 7 &&
                        instruction.operand_words == 2;
                    bool native_call =
                        instruction.opcode == 6 &&
                        instruction.operand_kind == 8 &&
                        instruction.operand_words == 2;

                    if (script_call || native_call) {
                        c_call_snapshot call {};

                        call.kind = script_call ?
                            e_call_kind::script :
                            e_call_kind::native;
                        call.source_executable = executable.address;
                        call.source_function = function.address;
                        call.code_offset =
                            (uint32_t)function_cursor -
                            executable.code_address;
                        call.target_address =
                            ((uint32_t)instruction.operands[0] << 16) |
                            instruction.operands[1];
                        call.native_class_index = invalid_index;
                        call.native_function_index = invalid_index;

                        c_object_snapshot &object =
                            snapshot->objects[function.object_index];

                        call.source_object = object.address;

                        if (script_call) {
                            ++function.script_call_count;
                            ++object.script_call_count;
                            ++executable.script_call_count;
                            ++snapshot->script_call_count;
                        } else {
                            ++function.native_call_count;
                            ++object.native_call_count;
                            ++executable.native_call_count;
                            ++snapshot->native_call_count;

                            auto native = native_lookup.find(call.target_address);

                            if (native != native_lookup.end()) {
                                call.native_class_index = native->second.class_index;
                                call.native_function_index = native->second.function_index;
                            }
                        }

                        snapshot->calls.push_back(call);
                    }

                    function_cursor += instruction.size;
                }

                function.call_count =
                    (uint32_t)snapshot->calls.size() -
                    function.first_call;
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

    inline void add_graph_edge(
        std::vector<c_graph_edge>* edges,
        node_id source,
        node_id target,
        e_graph_edge_kind kind) const {

        for (c_graph_edge &edge : *edges) {
            if (edge.source == source && edge.target == target && edge.kind == kind) {
                ++edge.count;
                return;
            }
        }

        edges->push_back({ source, target, 1, kind });
    }

    inline void add_object_graph_node(
        const c_object_snapshot &object,
        const ImVec2 &position,
        bool external) {

        c_node &node = m_graph.add_node(
            object_node_id(object.address),
            position,
            ImVec2(290.0f, 128.0f),
            std::format("{}object [{}] 0x{:08X}",
                external ? "external " : "",
                object.local_index,
                object.name_hash),
            object_style(external));

        node.rows.push_back(std::format("object 0x{:08X} | flags 0x{:08X}",
            object.address,
            object.flags));
        node.rows.push_back(std::format("functions {} | instances {} | threads {}",
            object.function_count,
            object.instance_count,
            object.live_thread_count));
        node.rows.push_back(std::format("BSR {} | BSL {}",
            object.script_call_count,
            object.native_call_count));
        node.rows.push_back(std::format("parent 0x{:08X}", object.parent_address));
    }

    inline void rebuild_graph() {
        m_graph = c_graph();
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
        std::vector<c_graph_edge> edges;

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

                    add_graph_edge(
                        &edges,
                        object_node_id(call.source_object),
                        object_node_id(target_object.address),
                        outgoing ?
                            e_graph_edge_kind::script_outgoing :
                            e_graph_edge_kind::script_incoming);
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

                    add_graph_edge(
                        &edges,
                        object_node_id(call.source_object),
                        target,
                        e_graph_edge_kind::native);
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
        constexpr float object_step_x = 350.0f;
        constexpr float object_step_y = 158.0f;
        uint32_t native_node_count =
            (uint32_t)(native_classes.size() + unresolved_natives.size());
        float object_start_y = native_node_count ? 320.0f : 20.0f;
        uint32_t row_limit = m_focus_selection && selected_object ?
            10u : (visible_objects.size() > 160 ? 24u :
                (visible_objects.size() > 60 ? 18u : 12u));
        float layer_start_x = object_start_x;
        float graph_height = 128.0f;
        std::unordered_map<uint32_t, ImVec2> object_positions;
        std::vector<const c_object_snapshot*> positioned_objects;

        object_positions.reserve(visible_objects.size());
        positioned_objects.reserve(visible_objects.size());

        for (const std::vector<const c_object_snapshot*> &layer : object_layers) {
            uint32_t lane_count = (std::max)(
                1u,
                ((uint32_t)layer.size() + row_limit - 1) / row_limit);

            for (uint32_t ordinal = 0; ordinal < layer.size(); ++ordinal) {
                uint32_t lane = ordinal / row_limit;
                uint32_t row = ordinal % row_limit;
                const c_object_snapshot* object = layer[ordinal];
                ImVec2 position(
                    layer_start_x + lane * object_step_x,
                    object_start_y + row * object_step_y);

                object_positions[object->address] = position;
                positioned_objects.push_back(object);
                graph_height = (std::max)(graph_height, position.y + 128.0f);
            }

            layer_start_x += lane_count * object_step_x + 180.0f;
        }

        if (selected_object && object_positions.contains(selected_object)) {
            ImVec2 selected_position = object_positions[selected_object];

            for (auto &entry : object_positions) {
                if (entry.first == selected_object)
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

        c_node &root = m_graph.add_node(
            executable_node_id(executable->address),
            ImVec2(20.0f, (std::max)(20.0f, graph_height * 0.5f - 64.0f)),
            ImVec2(320.0f, 128.0f),
            executable->name,
            executable_style());

        root.rows.push_back(std::format("executable 0x{:08X}", executable->address));
        root.rows.push_back(std::format("resource 0x{:08X} | flags 0x{:08X}",
            executable->resource_hash,
            executable->flags));
        root.rows.push_back(std::format("objects {} | functions {} | threads {}",
            executable->object_count,
            executable->function_count,
            executable->thread_count));
        root.rows.push_back(std::format("BSR {} | BSL {}",
            executable->script_call_count,
            executable->native_call_count));

        for (const c_object_snapshot* object : positioned_objects) {
            add_object_graph_node(
                *object,
                object_positions[object->address],
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

        constexpr float native_start_y = 20.0f;
        constexpr float native_step_x = 330.0f;
        float native_center_x = selected_object && object_positions.contains(selected_object) ?
            object_positions[selected_object].x + 145.0f : object_start_x + 145.0f;
        float native_row_width = native_node_count ?
            300.0f + (native_node_count - 1) * native_step_x : 0.0f;
        float native_start_x = native_center_x - native_row_width * 0.5f;
        uint32_t native_ordinal = 0;
        std::vector<uint32_t> sorted_native_classes(
            native_classes.begin(),
            native_classes.end());
        std::vector<uint32_t> sorted_unresolved_natives(
            unresolved_natives.begin(),
            unresolved_natives.end());

        std::sort(sorted_native_classes.begin(), sorted_native_classes.end());
        std::sort(sorted_unresolved_natives.begin(), sorted_unresolved_natives.end());

        for (uint32_t class_index : sorted_native_classes) {
            const c_native_class_snapshot* native_class =
                find_native_class(class_index);

            if (!native_class)
                continue;

            c_node &node = m_graph.add_node(
                native_class_node_id(native_class->address),
                ImVec2(
                    native_start_x + native_ordinal * native_step_x,
                    native_start_y),
                ImVec2(300.0f, 128.0f),
                std::format("native [{}] {}",
                    native_class->index,
                    native_class->type_name),
                native_style());

            node.rows.push_back(std::format("class 0x{:08X} | value size {}",
                native_class->address,
                native_class->value_size));
            node.rows.push_back(std::format("functions {}", native_class->function_count));
            node.rows.push_back(native_class->parent_name.empty() ?
                "parent <none>" :
                std::format("parent {}", native_class->parent_name));
            ++native_ordinal;
        }

        for (uint32_t descriptor : sorted_unresolved_natives) {
            c_node &node = m_graph.add_node(
                unresolved_native_node_id(descriptor),
                ImVec2(
                    native_start_x + native_ordinal * native_step_x,
                    native_start_y),
                ImVec2(300.0f, 92.0f),
                "unresolved native descriptor",
                native_style(true));

            node.rows.push_back(std::format("descriptor 0x{:08X}", descriptor));
            node.rows.push_back("not present in registry snapshot");
            ++native_ordinal;
        }

        for (const c_graph_edge &edge : edges) {
            ImU32 color = IM_COL32(242, 159, 54, 235);
            const char* relation = "outgoing BSR";

            if (edge.kind == e_graph_edge_kind::script_incoming) {
                color = IM_COL32(235, 91, 58, 235);
                relation = "incoming BSR";
            } else if (edge.kind == e_graph_edge_kind::native) {
                color = IM_COL32(196, 119, 235, 230);
                relation = "BSL";
            }

            float thickness = 1.5f +
                (std::min)((float)edge.count, 12.0f) * 0.28f;

            m_graph.add_connection(
                edge.source,
                edge.target,
                color,
                thickness,
                true,
                std::format("{} {} call{}",
                    edge.count,
                    relation,
                    edge.count == 1 ? "" : "s"));
        }

        m_graph_call_edge_count = (uint32_t)edges.size();
    }

    inline void refresh_snapshot() {
        c_snapshot next {};
        std::unordered_map<uint32_t, c_native_coordinate> native_lookup;

        snapshot_native_registry(&next, &native_lookup);
        snapshot_runtime_objects(&next, native_lookup);

        uint32_t previous_executable = m_selected_executable;

        m_snapshot = std::move(next);

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
            m_cached_graph_selection = 0;
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
            m_cached_graph_selection = 0;
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
            "%u objects (%u external, %u hidden) | %u parent links | %u selected call edges | %u object-local BSR callsites | %.1f%%",
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

            node_id selection = m_graph_view.selected_node();

            if (selection != m_cached_graph_selection &&
                !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {

                m_cached_graph_selection = selection;
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
            "filter name, address, object/function/signature hash",
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

        if (ImGui::BeginTable("##functions", 9, flags, ImVec2(0.0f, 330.0f))) {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("address", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableSetupColumn("object", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableSetupColumn("slot", ImGuiTableColumnFlags_WidthFixed, 42.0f);
            ImGui::TableSetupColumn("function hash", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableSetupColumn("signature", ImGuiTableColumnFlags_WidthFixed, 92.0f);
            ImGui::TableSetupColumn("code", ImGuiTableColumnFlags_WidthFixed, 82.0f);
            ImGui::TableSetupColumn("size", ImGuiTableColumnFlags_WidthFixed, 62.0f);
            ImGui::TableSetupColumn("calls", ImGuiTableColumnFlags_WidthFixed, 90.0f);
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
                    ImGui::Text("0x%X", function.code_size);
                    ImGui::TableNextColumn();
                    ImGui::Text("%u / %u",
                        function.script_call_count,
                        function.native_call_count);
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
            "vm_executable 0x%08X | object 0x%08X | stack metadata 0x%08X | flags 0x%04X | live threads %u",
            selected->address,
            object.address,
            selected->stack_metadata,
            selected->flags,
            selected->live_thread_count);

        draw_function_calls(*selected);
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

        ImGui::TextDisabled(
            "registry 0x%08X | %u classes | %u native functions",
            m_snapshot.registry_address,
            (uint32_t)m_snapshot.native_classes.size(),
            (uint32_t)m_snapshot.native_functions.size());

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

        ImGui::Text("%u executables | %u objects | %u functions | %u instances | %u threads | %u BSR | %u BSL",
            (uint32_t)m_snapshot.executables.size(),
            (uint32_t)m_snapshot.objects.size(),
            (uint32_t)m_snapshot.functions.size(),
            (uint32_t)m_snapshot.instances.size(),
            (uint32_t)m_snapshot.threads.size(),
            m_snapshot.script_call_count,
            m_snapshot.native_call_count);

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
