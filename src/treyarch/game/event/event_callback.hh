#pragma once

#include <cstddef>

#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/dinkumware/list.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class event;

    using code_event_callback_function = void(__cdecl*)(event*            raised_event,
                                                        arch_base_vhandle recipient,
                                                        void*             parameters);

    class event_callback {

    protected:
        void* parameters;
        u32   id;
        bool  disabled;
        bool  one_shot;
        u8    padding_0e[2];

        event_callback(void* parameters, bool one_shot);

    public:
        virtual ~event_callback() = default;
        virtual void spawn(event* raised_event, arch_base_vhandle recipient) = 0;
        virtual bool is_code_callback() const { return false; }
        virtual bool is_script_callback() const { return false; }

        void operator delete(void* allocation) noexcept;

        u32  callback_id() const noexcept { return id; }
        bool is_disabled() const noexcept { return disabled; }
        bool is_one_shot() const noexcept { return one_shot; }
    };

    class code_event_callback final : public event_callback {

        code_event_callback_function function;

    public:
        code_event_callback(code_event_callback_function function,
                            void*                        parameters,
                            bool                         one_shot);

        void spawn(event* raised_event, arch_base_vhandle recipient) override;
        bool is_code_callback() const override { return true; }
    };

    class script_event_callback final : public event_callback {

        chuck::vm::script_instance* instance;
        chuck::vm::script_function* function;

    public:
        script_event_callback(      chuck::vm::script_instance* instance,
                                    chuck::vm::script_function* function,
                              const void*                       parameters,
                                    bool                        one_shot);
        ~script_event_callback() override;

        void spawn(event* raised_event, arch_base_vhandle recipient) override;
        bool is_script_callback() const override { return true; }

        chuck::vm::script_instance* script_instance() const noexcept { return instance; }
        chuck::vm::script_function* script_function() const noexcept { return function; }

    private:
        static void __cdecl on_instance_lifecycle(i32                          reason,
                                                  chuck::vm::script_instance*,
                                                  chuck::vm::vm_thread*,
                                                  void*                        user_data);
    };

    ASSERT_SIZEOF(event_callback,        0x10);
    ASSERT_SIZEOF(code_event_callback,   0x14);
    ASSERT_SIZEOF(script_event_callback, 0x18);

    void dispatch_event_callbacks(event*                             raised_event,
                                  arch_base_vhandle                  recipient,
                                  dinkumware::list<event_callback*>* callbacks);
} // treyarch
