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

    if (owner && owner->adv_ptrs && (inst = owner->adv_ptrs->my_script)) {
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

bool script::load_script_exec(const char* filename, resource_key* key, amalga::resource_pack_slot* slot) {
    return ((bool (__cdecl*)(const char*, resource_key*, amalga::resource_pack_slot*))retail::sub_842830)(filename, key, slot);
}

bool script::unload_script_exec(const char* filename, amalga::resource_pack_slot* slot) {
    return ((bool (__cdecl*)(const char*, amalga::resource_pack_slot*))retail::sub_842A10)(filename, slot);
}

bool script::unload_script_exec(chuck::vm::script_object* object, amalga::resource_pack_slot* slot) {
    return ((bool (__cdecl*)(chuck::vm::script_object*, amalga::resource_pack_slot*))retail::sub_842A60)(object, slot);
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

// inlined @ sub_84ABE0
// inlined @ sub_84AD80
// inlined @ sub_856FB0
// inlined @ sub_85F770
// inlined @ sub_863EE0
// inlined @ sub_878B90
native_resource_context::native_resource_context(const chuck::vm::vm_stack &stack) : m_context((amalga::resource_pack_slot*)stack.thread->resource_context) {
    if (stack.thread->entry || !m_context)
        return;

    amalga::resource_manager::references::unk_010300a8.get().acquire();
    amalga::resource_manager::references::resource_context_stack_mutex.read()->acquire();

    amalga::resource_manager::push_resource_context(m_context);
}

// inlined @ sub_84ABE0
// inlined @ sub_84AD80
// inlined @ sub_856FB0
// inlined @ sub_85F770
// inlined @ sub_863EE0
// inlined @ sub_878B90
native_resource_context::~native_resource_context() {
    if (!m_context)
        return;

    amalga::resource_manager::pop_resource_context();

    amalga::resource_manager::references::resource_context_stack_mutex.read()->release();
    amalga::resource_manager::references::unk_010300a8.get().release();
}

bool script_object_function::set(const mash::string &func_declaration, chuck::vm::script_object* so, u32 unk) {
    using set_t = bool (__thiscall*)(script_object_function*, const mash::string*, chuck::vm::script_object*, u32);

    return ((set_t)retail::sub_842ED0)(this, &func_declaration, so, unk);
}

void script_object_function::parse_arguments(const mash::string &func_declaration, u32* argument_types, u32 unk) {
    retail::sub_8256A0((u32*)&func_declaration, argument_types, unk);
}

void script_object_function::push_arguments(      chuck::vm::vm_thread*                   thread,
                                                  u32                                     argument_types,
                                            const container::fixed_vector<script_arg, 8> &args) {

    using push_arguments_t = void (__cdecl*)(chuck::vm::vm_thread*, u32, const container::fixed_vector<script_arg, 8>*);

    ((push_arguments_t)retail::sub_842F50)(thread, argument_types, &args);
}

// sub_8801F0
script_object_instance::script_object_instance() : script_exec_prefix() {
    lock.reset();

    pack_slot          = nullptr;
    script_exec_prefix = resource_key {};
    inst_handle        = arch_base_vhandle();

    functions.clear();

    lock.reset();

    cached_function_index = -1;
}

// sub_883570
script_object_instance::~script_object_instance() {
    destroy();
}

// sub_880250
void script_object_instance::destroy() {
    engine_lock_scope lock_scope(&lock);

    chuck::vm::script_instance* inst = script::get_instance(inst_handle);

    if (inst) {
        chuck::vm::script_object* so = inst->parent;

        script::destroy_instance(inst, true);
        script::unload_script_exec(so, pack_slot);
    }

    pack_slot          = nullptr;
    script_exec_prefix = resource_key {};
    inst_handle        = arch_base_vhandle();

    functions.clear();

    cached_function_index = -1;
}

// sub_880300
bool script_object_instance::init(const mash::string                           &script_exec,
                                  const mash::string                           &script_obj,
                                        amalga::resource_pack_slot*             slot,
                                        actor*                                  owner,
                                  const char*                                   inst_name,
                                        bool                                    reuse,
                                  const container::fixed_vector<script_arg, 8> &args) {

    engine_lock_scope lock_scope(&lock);

    destroy();

    if (script_exec.empty() || script_obj.empty())
        return false;

    pack_slot = slot ? slot : owner ? owner->pack_slot : nullptr;

    if (!script::load_script_exec(script_exec.c_str(), &script_exec_prefix, pack_slot)) {
        pack_slot = nullptr;

        return false;
    }

    // the object is named by the constructor's declaration up to its parameters
    mash::string parsed_script_obj = script_obj.slice(0, script_obj.find(mash::string::pos_t { 0 }, '('));

    string_hash obj_name;
    obj_name.initialize(mash::ALLOCATED, parsed_script_obj.c_str());

    chuck::vm::script_object* so = script::find_object(obj_name, mash::string(script_exec.c_str()), &script_exec_prefix);

    if (!so) {
        script::unload_script_exec(script_exec.c_str(), pack_slot);
        pack_slot = nullptr;

        return false;
    }

    mash::string name;

    if (inst_name && std::strlen(inst_name))
        name = inst_name;
    else {
        u32 id = owner ? owner->id.source_hash_code : ++references::ownerless_instance_count.get();

        name = mash::string(mash::string::fmt, "0x%08x_%s", id, parsed_script_obj.c_str());
    }

    string_hash name_hash;
    name_hash.initialize(mash::ALLOCATED, name.c_str());

    chuck::vm::vm_thread*       constructor_thread = nullptr;
    chuck::vm::script_instance* inst               = script::find_instance(name_hash, so);

    // an instance already running under the name is rebuilt only when reusing; otherwise it's left alone and nothing constructs it
    if (inst) {
        if (reuse)
            script::reconstruct_instance(inst, &constructor_thread);
    } else
        inst = script::add_instance(mash::string(name.c_str()),
                                    so,
                                    &constructor_thread,
                                    (u32)pack_slot,
                                    chuck::vm::script_instance_stack_size_large);

    inst_handle = script::get_instance_handle(inst);

    u32 argument_types;

    script_object_function::parse_arguments(script_obj, &argument_types, 0);
    script_object_function::push_arguments(constructor_thread, argument_types, args);

    script::run_thread(constructor_thread);

    return true;
}

// sub_8805C0
bool script_object_instance::init(const mash::string                &script_exec,
                                  const mash::string                &script_obj,
                                        amalga::resource_pack_slot*  slot,
                                        actor*                       owner,
                                  const char*                        inst_name,
                                        bool                         reuse,
                                  const script_arg                  &arg0,
                                  const script_arg                  &arg1,
                                  const script_arg                  &arg2,
                                  const script_arg                  &arg3,
                                  const script_arg                  &arg4,
                                  const script_arg                  &arg5,
                                  const script_arg                  &arg6,
                                  const script_arg                  &arg7) {

    const script_arg* inputs[] { &arg0, &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7 };

    container::fixed_vector<script_arg, 8> args;

    for (const script_arg* input : inputs) {
        if (!input->type)
            break;

        args.push_back(*input);
    }

    return init(script_exec, script_obj, slot, owner, inst_name, reuse, args);
}

// sub_8835D0
bool script_object_instance::register_function(const mash::string &function_name, u32 unk) {
    // std::lower_bound over the functions by function_name
    using lower_bound_t = script_object_function** (__cdecl*)(script_object_function**,
                                                              script_object_function*,
                                                              script_object_function*,
                                                              const script_object_function*,
                                                              bool (__cdecl*)(u32*, u32*),
                                                              void*);

    // vector::insert
    using insert_t = script_object_function** (__thiscall*)(vc8::vector<script_object_function>*,
                                                            script_object_function**,
                                                            script_object_function*,
                                                            const script_object_function*);

    engine_lock_scope lock_scope(&lock);

    chuck::vm::script_instance* inst = script::get_instance(inst_handle);

    if (!inst || !inst->parent)
        return false;

    script_object_function func;
    func.function_name  = string_hash();
    func.function_index = -1;
    func.argument_types = 0;

    if (!func.set(function_name, inst->parent, unk))
        return false;

    cached_function_index = -1;

    script_object_function* where;

    ((lower_bound_t)retail::sub_879750)(&where, functions.begin(), functions.end(), &func, retail::sub_825690, nullptr);
    ((insert_t)retail::sub_880700)(&functions, &where, where, &func);

    return true;
}
