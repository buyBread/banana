#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "treyarch/chuck/vm/vm_thread.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_A1F660
void script_executable::register_callbacks(resolve_signal_callback_t               resolve_signal,
                                           resolve_extern_callback_t               resolve_extern,
                                           get_chuck_client_library_key_callback_t get_chuck_client_library_key,
                                           get_script_executable_folder_callback_t get_script_executable_folder) {

    references::resolve_signal_callback              .write(resolve_signal);
    references::resolve_extern_callback              .write(resolve_extern);
    references::get_chuck_client_library_key_callback.write(get_chuck_client_library_key);
    references::get_script_executable_folder_callback.write(get_script_executable_folder);
}

// sub_A1F920
void script_executable::first_run(f32 time_inc, bool ignore_suspended) {
    if ((flags & script_executable_flag_suspended_for_script_vars) || (flags & script_executable_flag_first_run_called))
        return;

    flags = (e_script_executable_flags)(flags | script_executable_flag_first_run_called);

    if (flags & script_executable_flag_from_mash) {
        script_instance* global_instance = global_script_object->global_instance;

        global_instance->data.set_to_zero();

        i32 count = (i32)object_instances.size;

        for (i32 index = 0; index < count; ++index) {
            const script_executable_object_instance_info* info = object_instances[index];

            u8* target = global_instance->data.buffer + info->offset;

            switch (info->id) {
                case -1:
                case -2:
                    break;

                case script_executable_object_instance_info_id_num:
                    *(f32*)target = info->num;
                    break;

                case script_executable_object_instance_info_id_made_from_num:
                    *(chuck_dynamic_array_t**)target = vm_dynamic_array_manager::inst()->create_new_dynamic_array(global_instance, (i32)info->num);
                    break;

                case script_executable_object_instance_info_id_uint:
                    *(u32*)target = info->uint;
                    break;

                default:
                    *(const char**)target = permanent_string_table[info->id]->c_str();
                    break;
            }
        }
    }

    i32 count = (i32)script_objects.size;

    for (i32 index = 0; index < count; ++index) {
        script_object* object = script_objects[index];

        if (object && (object->flags & script_object_flag_singleton)) {
            object->add_instance("__singleton", script_instance_stack_size_normal);
            object->run(ignore_suspended);
        }
    }
}

// sub_A1FD60
void script_executable::run(f32 time_inc, bool ignore_suspended) {
    if ((flags & script_executable_flag_suspended_for_script_vars) || suspend_count > 0)
        return;

    flags = (e_script_executable_flags)((flags & ~script_executable_flag_needs_run) | script_executable_flag_running);

    for (script_object* object : script_objects) {
        if (object && object->instances.head && (object->flags & script_object_flag_needs_run)) {
            flags = (e_script_executable_flags)(flags | script_executable_flag_needs_run);

            object->run(ignore_suspended);
        }
    }

    flags = (e_script_executable_flags)(flags & ~script_executable_flag_running);
}

// sub_A1FA30
bool script_executable::has_threads() const {
    i32 count = (i32)script_objects.size;

    for (i32 index = 0; index < count; ++index) {
        script_object* object = script_objects[index];

        if (object && object->has_threads())
            return true;
    }

    return false;
}

// sub_A1F790
script_object* script_executable::get_object(i32 index) const {
    return script_objects.data[index];
}

// sub_9ED9D0
script_object* script_executable::get_global_script_object() const {
    return global_script_object;
}

// sub_A1F6C0
script_object* script_executable::find_object(string_hash object_name, i32* index) const {
    i32 count = (i32)script_objects.size;

    if (!count)
        return nullptr;

    i32 low  = 0;
    i32 high = count - 1 < 0 ? 0 : count - 1;
    i32 mid  = count / 2;

    script_object* object = script_objects.data[mid];

    while (!(object->name == object_name)) {
        i32 previous = mid;

        if (object->name.source_hash_code >= object_name.source_hash_code) {
            high = mid - 1;

            if (high < 0)
                return nullptr;
        } else {
            low = mid + 1;

            if (low >= count)
                return nullptr;
        }

        if (low > high)
            return nullptr;

        mid = (high + low) / 2;

        if (previous == mid)
            return nullptr;

        object = script_objects.data[mid];
    }

    if (index)
        *index = mid;

    return object;
}

// sub_A1FDE0
vm_thread* script_executable::find_thread(u32 thread_id) const {
    i32 count = (i32)script_objects.size;

    for (i32 index = 0; index < count; ++index) {
        script_object* object = script_objects.data[index];

        if (!object)
            continue;

        for (script_instance* inst = object->instances.head; inst; inst = inst->vm_simple_list_next) {
            ref_lock_scope    instance_scope(inst->parent->instance_lock);
            engine_lock_scope thread_scope(&inst->thread_lock);

            for (vm_thread* thread = inst->threads.head; thread; thread = thread->vm_simple_list_next) {
                if (thread->thread_id == thread_id)
                    return thread;
            }
        }
    }

    return nullptr;
}
