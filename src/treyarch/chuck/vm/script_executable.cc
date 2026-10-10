#include "treyarch/chuck/script_library/slc_manager.hh"
#include "treyarch/chuck/vm/opcodes.hh"
#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_manager.hh"
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

// sub_A1F650
u16* script_executable::lookup_sx_code_segment(u32 offset) const {
    return exe_image + (offset >> 1);
}

// sub_A1FA70
void script_executable::link() {
    if ((flags & script_executable_flag_suspended_for_script_vars) || (flags & script_executable_flag_linked))
        return;

    u16* operands = exe_image;
    u16* end      = exe_image + (exe_image_size >> 1);

    flags = (e_script_executable_flags)(flags | script_executable_flag_linked);

    while (operands < end) {
        e_opcode_arg selector = (e_opcode_arg)((*operands++ >> 2) & 0x3F);

        u32 linked;

        // the selector alone decides how many words follow
        switch (selector) {
            case e_opcode_arg::NUM:
            case e_opcode_arg::SDR:
            case e_opcode_arg::PSIG:
            case e_opcode_arg::UINT:
            case e_opcode_arg::WORD_PAIR:
                operands += 2;

                continue;

            case e_opcode_arg::WORD:
            case e_opcode_arg::PCR:
            case e_opcode_arg::SPR:
            case e_opcode_arg::POPO:
                operands += 1;

                continue;

            case e_opcode_arg::JT:
                operands += operands[0] + 2;

                continue;

            case e_opcode_arg::NL:
                operands += 2 * operands[0] + 1;

                continue;

            case e_opcode_arg::STR:
                linked = (u32)permanent_string_table.data[operands[0]]->c_str();

                break;

            case e_opcode_arg::SFR:
            case e_opcode_arg::GSIG_SFR:
            case e_opcode_arg::ISIG_SFR:
            case e_opcode_arg::LSIG_SFR:
                linked = (u32)script_objects.data[operands[0]]->get_function_ptr(operands[1]);

                break;

            // a missing function only reaches a stripped "your scripts are out-of-sync with this executable" print (nullsub_1)
            case e_opcode_arg::LFR:
                linked = (u32)script_library::slc_manager::inst()->get_class(operands[0])->get_function(operands[1]);

                break;

            case e_opcode_arg::CLV: {
                script_library::script_library_class* library_class = script_library::slc_manager::inst()->get_class(operands[0]);

                mash::string instance_name(permanent_string_table.data[operands[1]]->c_str());

                linked = library_class->find_instance(instance_name);

                break;
            }

            case e_opcode_arg::GV:
                linked = operands[1] == 1 ?
                    (u32)script_manager::inst()->get_game_var_address(operands[0]) :
                    (u32)script_manager::inst()->get_shared_var_address(operands[0]);

                break;

            case e_opcode_arg::EI:
                linked = references::resolve_extern_callback.read()(permanent_string_table.data[operands[0]]->c_str(),
                                                                    permanent_string_table.data[operands[1]]->c_str());

                break;

            case e_opcode_arg::OBJ:
            case e_opcode_arg::OBJ_REF:
            case e_opcode_arg::UNK_34:
            case e_opcode_arg::UNK_35:
                linked = (u32)script_objects.data[operands[0]];
                
                break;

            default:
                continue;
        }

        // high word first, like every serialized u32 in the image
        operands[0] = (u16)(linked >> 16);
        operands[1] = (u16)linked;

        operands += 2;
    }
}

// sub_A1FED0
void script_executable::post_un_mash_fixup() {
    i32 count = (i32)script_objects.size;

    // an image that was already fixed up once only needs its auto instances back
    if (flags & script_executable_flag_un_mashed) {
        flags = (e_script_executable_flags)(flags | script_executable_flag_needs_run);

        for (i32 index = 0; index < count; ++index)
            script_objects.data[index]->quick_post_un_mash_fixup();

        return;
    }

    global_script_object = script_objects.data[(u32)global_script_object];

    for (i32 index = 0; index < count; ++index)
        script_objects.data[index]->post_un_mash_fixup(this);

    i32 instance_count = (i32)object_instances.size;

    for (i32 index = 0; index < instance_count; ++index) {
        script_function* parms = object_instances.data[index]->parms;

        if (parms)
            parms->post_un_mash_fixup(global_script_object);
    }
}

// sub_A1F820
void script_executable::un_load(bool call_all_destructors) {
    script_manager::inst()->run_notification_callback(script_manager_callback_reason_about_to_unload, this, nullptr);

    flags = (e_script_executable_flags)(flags | script_executable_flag_unloading);

    script_object_function_cache_element* cache = &references::function_cache.get();

    for (i32 slot = 0; slot < 20; ++slot) {
        script_object_function_cache_element &element = cache[slot];

        if (element.parent && element.parent->parent == this) {
            element.parent         = nullptr;
            element.function_index = -1;
            element.usage          = 0;
        }
    }

    i32 count = (i32)script_objects.size;

    for (i32 index = 0; index < count; ++index) {
        script_object* object = script_objects.data[index];

        if (object)
            object->destruct_instances(call_all_destructors);
    }

    // keep running until no destructor thread is left
    if (call_all_destructors && suspend_count <= 0) {
        bool ran;

        do {
            ran = false;

            if (count <= 0)
                break;

            for (i32 index = 0; index < count; ++index) {
                script_object* object = script_objects.data[index];

                if (object && object->has_threads()) {
                    object->run(true);

                    ran = true;
                }
            }
        } while (ran);
    }

    for (i32 index = 0; index < count; ++index) {
        script_object* object = script_objects.data[index];

        if (object)
            object->delete_all_instances();
    }

    flags = (e_script_executable_flags)(flags & ~(script_executable_flag_unloading | script_executable_flag_published));

    script_manager::inst()->run_notification_callback(script_manager_callback_reason_just_unloaded, this, nullptr);
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
