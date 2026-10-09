#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"

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
