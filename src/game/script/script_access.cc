#include "retail.hh"
#include "game/amalga/resource_manager.hh"
#include "game/amalga/resource_types.hh"
#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "game/script/script_access.hh"
#include "game/wds/entity/actor.hh"
#include "game/container/fixed_vector.hh"

using namespace treyarch;

// sub_825600
void script::update_script_instance_shadow(chuck::vm::script_instance* inst) {
    if (!inst || inst->client_space)
        return;

    script_instance_shadow* inst_shadow = new script_instance_shadow();
    inst_shadow->set_instance(inst);

    inst->client_space = inst_shadow;
}

// sub_825670
chuck::vm::script_instance* script::get_instance(arch_base_vhandle handle) {
    script_instance_shadow* inst_shadow = (script_instance_shadow*)handle.resolve();

    return inst_shadow ? inst_shadow->get_volatile_instance() : nullptr;
}

// sub_842DA0
arch_base_vhandle script::get_instance_handle(chuck::vm::script_instance* inst) {
    update_script_instance_shadow(inst);

    script_instance_shadow* inst_shadow = get_script_instance_shadow(inst);

    return inst_shadow ? inst_shadow->my_handle : arch_base_vhandle();
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
chuck::vm::vm_thread* script::create_thread(i32 function_index, chuck::vm::script_instance* inst, u32 resource_context) {
    if (!inst || function_index < 0)
        return nullptr;

    chuck::vm::vm_thread* thread = inst->add_thread(inst->parent->get_function_ptr(function_index), nullptr, 0);

    if (thread)
        thread->resource_context = resource_context;

    // global functions take no instance
    if (inst != references::master_global_instance.read())
        thread->dstack.push(&inst, sizeof(inst));

    return thread;
}

// sub_842DD0
chuck::vm::vm_thread* script::create_thread(string_hash fullname, chuck::vm::script_instance* inst, u32 resource_context) {
    i32 function_index = find_function(fullname, inst->parent);

    if (function_index < 0)
        return nullptr;

    return create_thread(function_index, inst, resource_context);
}

// sub_842C80
chuck::vm::vm_thread* script::create_thread(i32 function_index, chuck::vm::script_instance* inst, actor* owner) {
    return create_thread(function_index, inst, owner ? (u32)owner->pack_slot : 0);
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

    return create_thread(function_index, inst, owner ? (u32)owner->pack_slot : 0);
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
                                                       u32                                      resource_context,
                                                       chuck::vm::e_script_instance_stack_size  stack_size) {

    *constructor_thread = nullptr;

    if (!object || !object->parent)
        return nullptr;

    chuck::vm::script_instance* inst = object->add_instance(mash::string(name.c_str()), nullptr, constructor_thread, stack_size);

    if (*constructor_thread)
        (*constructor_thread)->resource_context = resource_context;

    return inst;
}

// sub_825520
chuck::vm::script_instance* script::add_instance(const mash::string                            &name,
                                                       chuck::vm::script_object*                object,
                                                       chuck::vm::vm_thread**                   constructor_thread,
                                                       actor*                                   owner,
                                                       chuck::vm::e_script_instance_stack_size  stack_size) {

    return add_instance(name, object, constructor_thread, owner ? (u32)owner->pack_slot : 0, stack_size);
}

// sub_842880
bool script::load_script_exec(const char* filename, resource_key* key, actor* owner) {
    using load_script_exec_t = bool (__cdecl*)(const resource_key*, resource_key*, amalga::resource_pack_slot*);

    string_hash name;
    name.initialize(mash::ALLOCATED, filename);

    resource_key exec_key;
    exec_key.set(name, (u32)amalga::e_resource_type::script);

    return ((load_script_exec_t)retail::sub_842650)(&exec_key, key, owner ? owner->pack_slot : nullptr);
}

// sub_842B20
bool script::unload_script_exec(const char* filename, actor* owner) {
    using unload_script_exec_t = bool (__cdecl*)(const resource_key*, amalga::resource_pack_slot*);

    string_hash name;
    name.initialize(mash::ALLOCATED, filename);

    resource_key exec_key;
    exec_key.set(name, (u32)amalga::e_resource_type::script);

    return ((unload_script_exec_t)retail::sub_8428F0)(&exec_key, owner ? owner->pack_slot : nullptr);
}

// sub_842B90
bool script::unload_script_exec(chuck::vm::script_object* object, actor* owner) {
    using unload_script_exec_t = bool (__cdecl*)(const resource_key*, amalga::resource_pack_slot*);

    string_hash name;
    name.initialize(mash::ALLOCATED, object->parent->name.c_str());

    resource_key exec_key;
    exec_key.set(name, (u32)amalga::e_resource_type::script);

    return ((unload_script_exec_t)retail::sub_8428F0)(&exec_key, owner ? owner->pack_slot : nullptr);
}

// sub_8253E0
chuck::vm::script_object* script::find_object(string_hash obj_name, const mash::string &filename, const resource_key* key) {
    string_hash exec_name;
    exec_name.initialize(mash::ALLOCATED, filename.c_str());

    return chuck::vm::script_manager::inst()->find_object(exec_name, obj_name, key->hash);
}

// sub_825460
chuck::vm::script_instance* script::find_instance(string_hash inst_name, chuck::vm::script_object* object) {
    if (!object)
        return nullptr;

    return object->find_instance(inst_name);
}

// sub_825550
void script::reconstruct_instance(chuck::vm::script_instance* inst, chuck::vm::vm_thread** constructor_thread) {
    *constructor_thread = nullptr;

    inst->parent->reconstruct_instance(inst, nullptr, constructor_thread);
}

// sub_8255A0
void script::destroy_instance(chuck::vm::script_instance* inst, bool auto_destruct) {
    if (auto_destruct) {
        inst->enable_auto_destruct();
        return;
    }

    if (inst->parent)
        inst->parent->remove_instance(inst, true);
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

// inlined @ sub_856FB0
// inlined @ sub_863EE0
// inlined @ sub_878B90
native_resource_context::native_resource_context(const chuck::vm::vm_stack &stack) : m_context((amalga::resource_pack_slot*)stack.thread->resource_context) {
    if (stack.thread->entry || !m_context)
        return;

    amalga::resource_manager::references::unk_010300a8.get().acquire();
    amalga::resource_manager::references::resource_context_stack_mutex.read()->acquire();

    amalga::resource_manager::push_resource_context(m_context);
}

// inlined @ sub_856FB0
// inlined @ sub_863EE0
// inlined @ sub_878B90
native_resource_context::~native_resource_context() {
    if (!m_context)
        return;

    amalga::resource_manager::pop_resource_context();

    amalga::resource_manager::references::resource_context_stack_mutex.read()->release();
    amalga::resource_manager::references::unk_010300a8.get().release();
}
