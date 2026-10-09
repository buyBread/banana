#include "treyarch/game/script/script_access.hh"

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
