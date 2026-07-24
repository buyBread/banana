#pragma once

#include "../../util.hh"
#include "../macros.hh"
#include "../mutex.hh"

#include "vm/opcode.hh"
#include "script_library.hh"

namespace banana { namespace NGL {
namespace chuck {
    namespace vm {
        struct vm_executable;
        struct vm_stack;
        struct vm_thread;
        struct script_runtime;
        struct script_executable;
        struct script_instance;
        struct script_object;

        namespace fn {
            inline auto runtime_find_thread =
                (vm_thread*(__thiscall*)(script_runtime*, uint32_t))
                0x00A1B030;

            inline auto executable_find_thread =
                (vm_thread*(__thiscall*)(script_executable*, uint32_t))
                0x00A1FDE0;

            inline auto executable_find_object_by_hash =
                (script_object*(__thiscall*)(script_executable*, uint32_t, int32_t*))
                0x00A1F6C0;

            inline auto executable_resolve_code_locator =
                (uint8_t*(__thiscall*)(script_executable*, uint32_t))
                0x00A1F650;

            inline auto executable_find_object_by_index =
                (script_object*(__thiscall*)(script_executable*, uint32_t))
                0x00A1F790;

            inline auto object_find_function_by_index =
                (vm_executable*(__thiscall*)(script_object*, uint32_t))
                0x00A1CFE0;

            inline auto instance_add_thread_with_arguments =
                (vm_thread*(__thiscall*)(script_instance*, vm_executable*, const void*, int32_t, void*, int32_t))
                0x00A1E0F0;

            inline auto stack_push_bytes =
                (void(__thiscall*)(vm_stack*, const void*, int32_t))
                0x004E4F70;
        } // fn

        struct script_executable_entry {
            script_executable* executable;
        };

        struct script_executable_tree_node {
            script_executable_tree_node* left;
            script_executable_tree_node* parent;
            script_executable_tree_node* right;
            
            script_executable_entry* entry;

            uint8_t color;
            uint8_t is_nil;
            uint8_t padding_12[2];

            script_executable* get() const {
                return entry->executable;
            }

            script_executable_tree_node* successor() {
                if (is_nil)
                    return this;

                if (!right->is_nil) {
                    script_executable_tree_node* node = right;

                    while (!node->left->is_nil)
                        node = node->left;

                    return node;
                }

                script_executable_tree_node* node = this;
                script_executable_tree_node* next = parent;

                while (!next->is_nil && node == next->right) {
                    node = next;
                    next = next->parent;
                }

                return next;
            }
        };

        struct script_executable_tree {
            uint32_t                     unk_00;
            script_executable_tree_node* head;
            uint32_t                     count;

            script_executable_tree_node* begin() const {
                return head->left;
            }

            script_executable_tree_node* end() const {
                return head;
            }
        };

        struct script_executable {
            script_executable* self;
            uint32_t           name_length;
            char*              name;
            uint32_t           unk_0c;
            uint32_t           resource_hash;
            uint32_t           object_container;
            uint32_t           object_count;
            script_object**    objects;
            uint32_t           unk_20;
            uint32_t           unk_24;
            script_object*     global_object;
            uint32_t           second_container;
            uint32_t           second_count;
            void*              second_table;
            uint32_t           unk_38;
            uint32_t           unk_3c;
            uint32_t           additional_container;
            uint32_t           unk_44;
            void**             strings;
            uint32_t           string_count;
            uint32_t           unk_50;
            uint8_t*           code;
            uint32_t           code_size;
            uint32_t           unk_5c;
            uint32_t           unk_60;
            uint32_t           flags;
            uint32_t           runtime_state;

            vm_thread* find_thread(uint32_t id) {
                return fn::executable_find_thread(this, id);
            }

            [[nodiscard]] script_object* object(uint32_t index) {
                if (!objects || index >= object_count)
                    return nullptr;

                return fn::executable_find_object_by_index(this, index);
            }

            [[nodiscard]] script_object* find_object(uint32_t hash, int32_t* index = nullptr) {
                return fn::executable_find_object_by_hash(this, hash, index);
            }

            [[nodiscard]] uint8_t* resolve_code_locator(uint32_t locator) {
                return fn::executable_resolve_code_locator(this, locator);
            }
        };

        enum class vm_reference_kind : uint16_t {
            dynamic_array                 = 0,
            string                        = 1,
            ignored                       = 2,
            string_dynamic_array          = 3,
            script_instance_dynamic_array = 4
        };

        struct vm_reference_descriptor {
            vm_reference_kind kind;
            uint16_t          offset;
        };

        struct vm_executable {
            uint32_t                 record_size;
            uint32_t                 reference_descriptor_count;
            vm_reference_descriptor* reference_descriptors;
            uint32_t                 unk_0c;
            uint32_t                 signature_hash;
            uint32_t                 function_hash;
            uint32_t                 unk_18;
            script_object*           object;
            uint8_t*                 code;
            uint32_t                 stack_metadata;
            uint16_t                 unk_28;
            uint16_t                 flags;

            [[nodiscard]] uint16_t argument_size() const noexcept {
                return (uint16_t)stack_metadata;
            }
        };

        struct script_object {
            uint32_t                          name_hash;
            script_object*                    parent;
            script_executable*                executable;
            uint8_t                           unk_0c[0x0C];
            uint32_t                          function_count;
            vm_executable**                   functions;
            uint8_t                           unk_20[0x28];
            script_instance*                  first_instance;
            uint8_t                           unk_4c[0x08];
            uint32_t                          flags;
            NGL_MUTEX::engine_activity_guard* instance_guard;

            [[nodiscard]] vm_executable* function(uint32_t index) const {
                if (!functions || index >= function_count)
                    return nullptr;

                return functions[index];
            }

            [[nodiscard]] vm_executable* find_function(uint32_t flattened_index) {
                return fn::object_find_function_by_index(this, flattened_index);
            }
        };

        struct script_instance {
            uint32_t                         flags;
            uint8_t                          unk_04[0x14];
            vm_thread*                       first_thread;
            vm_thread*                       last_thread;
            uint32_t                         thread_count;
            script_object*                   object;
            uint8_t                          unk_28[0x18];
            NGL_MUTEX::engine_recursive_lock thread_lock;
            uint8_t                          unk_4c[0x1C];
            script_instance*                 next;
            uint32_t                         unk_6c;

            [[nodiscard]] vm_thread* add_thread(
                vm_executable* executable,
                const void* arguments = nullptr,
                int32_t argument_size = 0,
                void* context = nullptr,
                int32_t stack_size = 0) {

                if (!executable || argument_size < 0 || (argument_size && !arguments))
                    return nullptr;

                uint32_t maximum_initial_push = 0;

                switch (stack_size) {
                    case 0:
                    case 128:
                        maximum_initial_push = 284;
                        break;

                    case 284:
                        maximum_initial_push = 512;
                        break;

                    case 512:
                    case 1024:
                        maximum_initial_push = 1024;
                        break;

                    default:
                        return nullptr;
                }

                if ((uint32_t)argument_size > maximum_initial_push)
                    return nullptr;

                return fn::instance_add_thread_with_arguments(
                    this,
                    executable,
                    arguments,
                    argument_size,
                    context,
                    stack_size);
            }
        };

        struct vm_stack {
            uint8_t*   cursor;
            void*      allocation;
            uint8_t*   data;
            uint32_t   capacity;
            vm_thread* thread;

            [[nodiscard]] bool valid() const noexcept {
                if (!cursor || !data)
                    return false;

                uint32_t cursor_address = (uint32_t)cursor;
                uint32_t data_address   = (uint32_t)data;

                return
                    cursor_address >= data_address &&
                    cursor_address - data_address <= capacity;
            }

            [[nodiscard]] uint32_t size() const noexcept {
                if (!valid())
                    return 0;

                return (uint32_t)((uint32_t)cursor - (uint32_t)data);
            }

            [[nodiscard]] uint32_t remaining() const noexcept {
                return valid() ? capacity - size() : 0;
            }

            [[nodiscard]] uint32_t capacity_after_one_growth() const noexcept {
                switch (capacity) {
                    case 128: return 284;
                    case 284: return 512;
                    case 512: return 1024;
                    default:  return capacity;
                }
            }

            [[nodiscard]] bool can_push(uint32_t byte_count) const noexcept {
                if (!valid())
                    return false;

                uint32_t used = size();

                if (byte_count > UINT32_MAX - used)
                    return false;

                uint32_t required = used + byte_count;

                return required <= capacity_after_one_growth();
            }

            bool push_bytes(const void* source, uint32_t byte_count) {
                if (!byte_count)
                    return valid();

                if (!source || byte_count > INT32_MAX || !can_push(byte_count))
                    return false;

                uint32_t previous_size = size();

                fn::stack_push_bytes(this, source, (int32_t)byte_count);

                return valid() && size() == previous_size + byte_count;
            }

            bool peek_bytes(void* destination, uint32_t byte_count) const {
                if (!byte_count)
                    return valid();

                if (!destination || !valid() || byte_count > size())
                    return false;

                std::memcpy(destination, cursor - byte_count, byte_count);

                return true;
            }

            bool pop_bytes(void* destination, uint32_t byte_count) {
                if (!peek_bytes(destination, byte_count))
                    return false;

                cursor -= byte_count;

                return true;
            }

            bool push_number(float value) {
                return push_bytes(&value, sizeof(value));
            }

            bool pop_number(float* value) {
                return pop_bytes(value, sizeof(*value));
            }

            bool push_unsigned(uint32_t value) {
                return push_bytes(&value, sizeof(value));
            }

            bool pop_unsigned(uint32_t* value) {
                return pop_bytes(value, sizeof(*value));
            }
        };

        struct vm_flow_frame {
            uint8_t*       return_pc;
            void*          previous_context;
            vm_flow_frame* previous;
        };

        struct vm_thread {
            script_instance* instance;
            vm_executable*   executable;
            vm_thread*       parent;
            vm_stack         stack;
            uint8_t*         pc;
            uint32_t         state_24;
            uint32_t         state_28;
            uint32_t         unk_2c;
            uint32_t         unk_30;
            uint32_t         result;
            vm_flow_frame*   flow_frame;
            e_opcode         opcode;
            uint32_t         operand_selector;
            uint8_t          operand[0x0C];
            uint32_t         operand_width;
            script_instance* current_instance;
            uint32_t         native_recall;
            void*            constructor_context;
            uint32_t         unk_60;
            uint32_t         unk_64;
            uint32_t         id;
            uint32_t         unk_6c;
            uint32_t         unk_70;
            uint32_t         unk_74;
            vm_thread**      list_owner;
            vm_thread*       previous;
            vm_thread*       next;
        };

        struct script_runtime {
            uint32_t                         unk_00;
            uint32_t                         unk_04;
            script_executable_tree*          loaded_executables;
            uint32_t                         unk_0c;
            uint32_t                         unk_10;
            uint32_t                         unk_14;
            void*                            unk_18; // container
            void*                            fixed_callbacks[15];
            void*                            lifecycle_callback;
            void*                            resource_acquisition_callback;
            void*                            service_60;
            void*                            unk_services_64[5];
            NGL_MUTEX::engine_recursive_lock loaded_executables_lock;
            uint32_t                         unk_84;
            NGL_MUTEX::engine_recursive_lock unk_lock_88;
            uint32_t                         unk_94;

            vm_thread* find_thread(uint32_t id) {
                return fn::runtime_find_thread(this, id);
            }
        };

        static_assert(sizeof(e_opcode)                    == 0x04, ASSERT_FAIL_SANITY);
        static_assert(sizeof(vm_reference_descriptor)     == 0x04, ASSERT_FAIL_SANITY);
        static_assert(sizeof(vm_executable)               == 0x2C, ASSERT_FAIL_SANITY);
        static_assert(sizeof(vm_stack)                    == 0x14, ASSERT_FAIL_SANITY);
        static_assert(sizeof(vm_flow_frame)               == 0x0C, ASSERT_FAIL_SANITY);
        static_assert(sizeof(vm_thread)                   == 0x84, ASSERT_FAIL_SANITY);
        static_assert(sizeof(script_executable_tree_node) == 0x14, ASSERT_FAIL_SANITY);
        static_assert(sizeof(script_executable_tree)      == 0x0C, ASSERT_FAIL_SANITY);
        static_assert(sizeof(script_executable)           == 0x6C, ASSERT_FAIL_SANITY);
        static_assert(sizeof(script_instance)             == 0x70, ASSERT_FAIL_SANITY);
        static_assert(sizeof(script_runtime)              == 0x98, ASSERT_FAIL_SANITY);
        static_assert(sizeof(script_library_function)     == 0x04, ASSERT_FAIL_SANITY);

        static_assert(offsetof(vm_executable, reference_descriptor_count) == 0x04, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_executable, reference_descriptors)      == 0x08, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_executable, signature_hash)             == 0x10, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_executable, object)                     == 0x1C, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_executable, code)                       == 0x20, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_executable, stack_metadata)             == 0x24, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_executable, flags)                      == 0x2A, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_stack, cursor)     == 0x00, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_stack, allocation) == 0x04, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_stack, data)       == 0x08, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_stack, capacity)   == 0x0C, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_stack, thread)     == 0x10, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_thread, stack)      == 0x0C, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_thread, pc)         == 0x20, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_thread, flow_frame) == 0x38, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_thread, opcode)     == 0x3C, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_thread, id)         == 0x68, ASSERT_FAIL_SANITY);
        static_assert(offsetof(vm_thread, next)       == 0x80, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_object, function_count) == 0x18, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_object, functions)      == 0x1C, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_object, first_instance) == 0x48, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_object, instance_guard)  == 0x58, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_executable_tree_node, entry)  == 0x0C, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_executable_tree_node, is_nil) == 0x11, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_executable_tree, head)  == 0x04, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_executable_tree, count) == 0x08, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_executable, object_count) == 0x18, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_executable, objects)      == 0x1C, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_executable, code)         == 0x54, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_executable, flags)        == 0x64, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_instance, first_thread) == 0x18, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_instance, object)       == 0x24, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_instance, thread_lock)  == 0x40, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_instance, next)         == 0x68, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_runtime, loaded_executables)      == 0x08, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_runtime, lifecycle_callback)      == 0x58, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_runtime, loaded_executables_lock) == 0x78, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_runtime, unk_lock_88)             == 0x88, ASSERT_FAIL_SANITY);
    } // vm

    MEMORY_REFERENCE<vm::script_runtime*> script_runtime { 0x011248E0 };
} // chuck
} } // banana::NGL
