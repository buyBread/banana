#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

#include "../mstring.hh"

namespace banana { namespace NGL {
namespace internals { namespace chuck {
    namespace vm {
        struct vm_stack;
        struct script_library_class;
        struct script_library_function;

        namespace fn {
            inline auto script_library_class_destruct =
                (void(__thiscall*)
                (script_library_class*))
                0x00A21010;

            inline auto script_library_function_construct_for_class =
                (script_library_function*(__thiscall*)
                (script_library_function*, script_library_class*))
                0x00A215E0;

            inline auto script_library_function_construct_global =
                (script_library_function*(__thiscall*)
                (script_library_function*))
                0x00A21650;

            inline auto script_library_class_construct =
                (script_library_class*(__thiscall*)
                (script_library_class*, const char*, int32_t, const char*, bool))
                0x00A21710;

            inline auto script_library_class_find_instance =
                (uint32_t(__thiscall*)
                (script_library_class*, const mash_string&))
                0x00A90D90;
        } // fn

        /*
            script_library_function is a stateless native-call descriptor.
            only has one field, which is the implicit vtable pointer and its only virtual slot invokes the native function.
            script_library_class owns these descriptors and releases their engine allocations directly.
        */

        struct script_library_function {
            using callback_t = bool(__thiscall*)(
                const script_library_function*,
                vm_stack&);

            script_library_function() {
                fn::script_library_function_construct_global(this);
            }

            explicit script_library_function(script_library_class* owner) {
                fn::script_library_function_construct_for_class(this, owner);
            }

            virtual bool operator()(vm_stack&) const {
                return true;
            }

            [[nodiscard]] void** vtable() const noexcept {
                return *(void***)this;
            }

            [[nodiscard]] callback_t callback() const noexcept {
                callback_t* callbacks = (callback_t*)vtable();

                return callbacks ? callbacks[0] : nullptr;
            }
        };

        struct script_library_function_vector {
            uint32_t                  unk_00;
            script_library_function** first;
            script_library_function** last;
            script_library_function** capacity_end;

            [[nodiscard]] uint32_t size() const noexcept {
                if (!first)
                    return 0;

                return (uint32_t)(last - first);
            }

            [[nodiscard]] bool empty() const noexcept {
                return size() == 0;
            }

            [[nodiscard]] script_library_function* operator[](uint32_t index) const noexcept {
                return first[index];
            }
        };

        /*
            script_library_class describes one chuckvm value type.
            it stores the value's byte width and owns the native function descriptors registered for that type.
            it is not the class containing those methods as member methods.
        */

        struct script_library_class {
            int32_t                         m_value_size;
            script_library_function_vector* m_functions;
            const char*                     m_parent_name;

            script_library_class(const char* name, int32_t value_size, const char* parent_name = nullptr, bool skip_registration = false) {
                fn::script_library_class_construct(this, name, value_size, parent_name, skip_registration);
            }

            virtual ~script_library_class() {
                fn::script_library_class_destruct(this);
            }

            virtual uint32_t find_instance(const mash_string& name) {
                return fn::script_library_class_find_instance(this, name);
            }

            [[nodiscard]] int32_t value_size() const noexcept {
                return m_value_size;
            }

            [[nodiscard]] int32_t get_size() const noexcept {
                return m_value_size;
            }

            [[nodiscard]] uint32_t function_count() const noexcept {
                return m_functions ? m_functions->size() : 0;
            }

            [[nodiscard]] script_library_function* function(uint32_t index) const {
                assert(m_functions != nullptr);
                assert(index < m_functions->size());

                return (*m_functions)[index];
            }

            [[nodiscard]] script_library_function* get_func(int32_t index) const {
                assert(index >= 0);

                return function((uint32_t)index);
            }
        };

        static_assert(sizeof(script_library_function)        == 0x04, ASSERT_FAIL_SANITY);
        static_assert(sizeof(script_library_function_vector) == 0x10, ASSERT_FAIL_SANITY);
        static_assert(sizeof(script_library_class)           == 0x10, ASSERT_FAIL_SANITY);

        static_assert(offsetof(script_library_class, m_value_size)  == 0x04, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_library_class, m_functions)   == 0x08, ASSERT_FAIL_SANITY);
        static_assert(offsetof(script_library_class, m_parent_name) == 0x0C, ASSERT_FAIL_SANITY);
    } // vm
}} // internals::chuck
}} // banana::NGL
