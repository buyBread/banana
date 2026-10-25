#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/game/script/script_access.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/shared/container/fixed_vector.hh"

using namespace treyarch;

// sub_825600
void script::update_script_instance_shadow(chuck::vm::script_instance* inst) {
    if (!inst || inst->client_space)
        return;

    script_instance_shadow* inst_shadow = new script_instance_shadow();
    inst_shadow->set_instance(inst);

    inst->client_space = inst_shadow;
}

// sub_824F20
i32 script::find_function(string_hash fullname, chuck::vm::script_object* object) {
    if (!object)
        return -1;

    i32 index = object->find_func(fullname);

    return index < 0 ? -1 : index;
}

// sub_824F80
i32 script::find_instance_function(string_hash fullname, chuck::vm::script_instance* inst) {
    if (!inst)
        return -1;

    return find_function(fullname, inst->parent);
}

// sub_842C00
chuck::vm::vm_thread* script::create_thread(i32 function_index, chuck::vm::script_instance* inst, u32 unk_60) {
    if (!inst || function_index < 0)
        return nullptr;

    chuck::vm::vm_thread* thread = inst->add_thread(inst->parent->get_function_ptr(function_index), nullptr, 0);

    if (thread)
        thread->unk_60 = unk_60;

    // global functions take no instance
    if (inst != references::master_global_instance.read())
        thread->dstack.push(&inst, sizeof(inst));

    return thread;
}

// sub_842DD0
chuck::vm::vm_thread* script::create_thread(string_hash fullname, chuck::vm::script_instance* inst, u32 unk_60) {
    i32 function_index = find_function(fullname, inst->parent);

    if (function_index < 0)
        return nullptr;

    return create_thread(function_index, inst, unk_60);
}

// sub_842E10
chuck::vm::vm_thread* script::create_thread(actor* owner, string_hash fullname) {
    i32                         function_index = -1;
    chuck::vm::script_instance* inst           = nullptr;

    if (owner && owner->unk_0b0 && (inst = owner->unk_0b0->instance)) {
        function_index = find_function(fullname, inst->parent);

        if (function_index < 0) {
            inst           = inst->parent->parent->get_global_script_object()->global_instance;
            function_index = find_function(fullname, inst->parent);
        }
    }

    if (function_index < 0) {
        inst = references::master_global_instance.read();

        if (inst)
            function_index = find_function(fullname, inst->parent);

        if (function_index < 0)
            return nullptr;
    }

    return create_thread(function_index, inst, owner ? (u32)owner->unk_0c0 : 0);
}

// sub_607A60
bool script::call_function(      actor*       owner,
                                 string_hash  fullname,
                           const script_arg  &arg0,
                           const script_arg  &arg1,
                           const script_arg  &arg2,
                           const script_arg  &arg3,
                           const script_arg  &arg4,
                           const script_arg  &arg5,
                           const script_arg  &arg6,
                           const script_arg  &arg7) {

    chuck::vm::vm_thread* thread = create_thread(owner, fullname);

    if (!thread)
        return false;

    const script_arg* inputs[] { &arg0, &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7 };

    container::fixed_vector<script_arg, 8> args;

    for (const script_arg* input : inputs) {
        if (!input->type)
            break;

        args.push_back(*input);
    }

    for (const script_arg &arg : args)
        thread->dstack.push(arg.value, (&references::script_arg_sizes.get())[arg.type]);

    return start_thread(thread, false);
}

// sub_842CB0
bool script::push_entity(chuck::vm::vm_thread* thread, entity* ent) {
    if (!thread)
        return false;

    arch_base_vhandle handle = ent ? ent->my_handle : arch_base_vhandle();

    thread->dstack.push(&handle, sizeof(handle));

    return true;
}

// sub_842D60
bool script::push_uint(chuck::vm::vm_thread* thread, u32 value) {
    if (!thread)
        return false;

    thread->dstack.push_uint(value);

    return true;
}

// sub_825480
chuck::vm::script_instance* script::add_instance(const mash::string                            &name,
                                                       chuck::vm::script_object*                object,
                                                       chuck::vm::vm_thread**                   constructor_thread,
                                                       u32                                      unk_60,
                                                       chuck::vm::e_script_instance_stack_size  stack_size) {

    *constructor_thread = nullptr;

    if (!object || !object->parent)
        return nullptr;

    chuck::vm::script_instance* inst = object->add_instance(mash::string(name.c_str()), nullptr, constructor_thread, stack_size);

    if (*constructor_thread)
        (*constructor_thread)->unk_60 = unk_60;

    return inst;
}

// sub_825520
chuck::vm::script_instance* script::add_instance(const mash::string                            &name,
                                                       chuck::vm::script_object*                object,
                                                       chuck::vm::vm_thread**                   constructor_thread,
                                                       actor*                                   owner,
                                                       chuck::vm::e_script_instance_stack_size  stack_size) {

    return add_instance(name, object, constructor_thread, owner ? (u32)owner->unk_0c0 : 0, stack_size);
}

// sub_824FB0
bool script::start_thread(chuck::vm::vm_thread* thread, bool run_now) {
    if (!thread)
        return false;

    if (run_now) {
        string_hash key_prefix;
        key_prefix.initialize(mash::ALLOCATED);

        thread->inst->run_single_thread(thread, true, key_prefix);
    }

    return true;
}

// sub_825090
bool script::run_thread(chuck::vm::vm_thread* thread) {
    if (!thread)
        return false;

    string_hash key_prefix;
    key_prefix.initialize(mash::ALLOCATED);

    thread->inst->run_single_thread(thread, true, key_prefix);

    return true;
}

// sub_825020
bool script::run_thread_until_finished(chuck::vm::vm_thread* thread) {
    if (!thread)
        return false;

    string_hash key_prefix;
    key_prefix.initialize(mash::ALLOCATED);

    thread->inst->run_single_thread_until_finished(thread, true, key_prefix);

    return true;
}
