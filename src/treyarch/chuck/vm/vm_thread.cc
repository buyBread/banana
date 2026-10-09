#include <cstring>
#include <new>
#include <windows.h>

#include "treyarch/chuck/script_library/script_library_function.hh"
#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_function.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "treyarch/chuck/vm/vm_string.hh"
#include "treyarch/chuck/vm/vm_thread.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_A21B60
void vm_thread::register_callbacks(raise_global_signal_callback_t             raise_global_signal,
                                   raise_instance_signal_callback_t           raise_instance_signal,
                                   raise_library_signal_callback_t            raise_library_signal,
                                   clear_global_callback_by_name_callback_t   clear_global_callback_by_name,
                                   clear_global_callback_by_id_callback_t     clear_global_callback_by_id,
                                   clear_instance_callback_by_name_callback_t clear_instance_callback_by_name,
                                   clear_instance_callback_by_id_callback_t   clear_instance_callback_by_id,
                                   add_global_callback_callback_t             add_global_callback,
                                   add_instance_callback_callback_t           add_instance_callback,
                                   add_library_callback_callback_t            add_library_callback) {

    references::raise_global_signal_callback            .write(raise_global_signal);
    references::raise_instance_signal_callback          .write(raise_instance_signal);
    references::raise_library_signal_callback           .write(raise_library_signal);
    references::clear_global_callback_by_name_callback  .write(clear_global_callback_by_name);
    references::clear_global_callback_by_id_callback    .write(clear_global_callback_by_id);
    references::clear_instance_callback_by_name_callback.write(clear_instance_callback_by_name);
    references::clear_instance_callback_by_id_callback  .write(clear_instance_callback_by_id);
    references::add_global_callback_callback            .write(add_global_callback);
    references::add_instance_callback_callback          .write(add_instance_callback);
    references::add_library_callback_callback           .write(add_library_callback);
}

// sub_A22140
vm_thread::vm_thread(      script_instance* instance,
                     const script_function* function,
                           void*            requested_user_data,
                           u32              stack_size) : inst(instance),
                                                          ex(function),
                                                          creator(nullptr),
                                                          dstack(this, stack_size),
                                                          pc(function->buffer),
                                                          flow_stack(nullptr),
                                                          current_instance(instance),
                                                          entry(0),
                                                          user_data(requested_user_data),
                                                          unk_60(0),
                                                          camera_priority(0.0f),
                                                          local_references { nullptr, nullptr, 0 },
                                                          list(nullptr),
                                                          vm_simple_list_previous(nullptr),
                                                          vm_simple_list_next(nullptr) {

    thread_id = _InterlockedIncrement((volatile LONG*)&references::id_counter.get());
}

// sub_A22AC0
vm_thread::~vm_thread() {
    if (inst)
        inst->run_callbacks(script_instance_callback_reason_thread_about_to_die, this);

    release_local_references();

    while (flow_stack) {
        vm_thread_flow_stack_element* element = flow_stack;

        flow_stack = element->previous;

        if (element)
            references::flow_stack_element_pool.get().release(element);
    }
}

// sub_A22A00
void vm_thread::release_local_references() {
    vm_thread_local_reference* reference = local_references.head;

    if (!reference)
        return;

    do {
        if (local_references.head)
            local_references.erase(local_references.head);

        if (reference->mode & 1) {
            vm_dynamic_array_manager* manager = vm_dynamic_array_manager::inst();
            auto*                     array   = (chuck_dynamic_array_t*)reference->allocation;

            if (reference->mode & 4)
                manager->remove_string_array_reference(array);
            else if (reference->mode & 8)
                manager->remove_instance_array_reference((script_instance*)this, array); // the context is never read
            else
                manager->remove_reference(array);
        }

        references::local_reference_pool.get().release(reference);

        reference = local_references.head;
    } while (local_references.head);
}

// sub_A21FE0
void vm_thread::add_local_reference(void* allocation, u32 mode) {
    if (!allocation)
        return;

    void* memory = references::local_reference_pool.get().allocate();

    vm_thread_local_reference* reference = memory ?
        new (memory) vm_thread_local_reference(allocation, mode | 1) : nullptr;

    reference->list                    = &local_references;
    reference->vm_simple_list_previous = local_references.last;
    reference->vm_simple_list_next     = nullptr;

    if (local_references.last)
        local_references.last->vm_simple_list_next = reference;

    bool was_empty = !local_references.head;

    local_references.last = reference;

    if (was_empty)
        local_references.head = reference;

    ++local_references.size;
}

// sub_A22970
void vm_thread::remove_local_reference(void* allocation) {
    if (!allocation)
        return;

    vm_thread_local_reference* reference = local_references.head;

    while (reference) {
        if (!(reference->mode & 1) || reference->allocation != allocation) {
            reference = reference->vm_simple_list_next;

            continue;
        }

        vm_thread_local_reference* removed = reference;

        reference = local_references.erase(reference);

        references::local_reference_pool.get().release(removed);
    }
}

// sub_A21C30
void vm_thread::fill_argument() {
    switch (current_opcode_arg) {
        case e_opcode_arg::NUM:
        case e_opcode_arg::STR:
        case e_opcode_arg::SFR:
        case e_opcode_arg::LFR:
        case e_opcode_arg::CLV:
        case e_opcode_arg::PSIG:
        case e_opcode_arg::GV:
        case e_opcode_arg::GSIG_SFR:
        case e_opcode_arg::ISIG_SFR:
        case e_opcode_arg::LSIG_SFR:
        case e_opcode_arg::OBJ:
        case e_opcode_arg::UINT:
        case e_opcode_arg::OBJ_REF:
        case e_opcode_arg::UNK_34:
        case e_opcode_arg::UNK_35:
            // high word first
            current_arg.uint  = (u32)*pc++ << 16;
            current_arg.uint += *pc++;

            break;

        case e_opcode_arg::WORD:
        case e_opcode_arg::PCR:
        case e_opcode_arg::SPR:
        case e_opcode_arg::POPO:
            current_arg.word = (i16)*pc++;

            break;

        // a member of the object's global instance
        case e_opcode_arg::SDR: {
            u16            object_index = *pc++;
            script_object* object       = inst->parent->parent->get_object(object_index);
            u16            offset       = *pc++;

            current_arg.pointer = object->global_instance->data.buffer + offset;

            break;
        }

        case e_opcode_arg::JT: {
            u16 count = *pc;

            current_arg.jump_table.displacements = pc + 1;
            pc                                   = pc + 1 + count;
            current_arg.jump_table.count         = count;

            current_arg.jump_table.default_displacement = *pc++;

            break;
        }

        case e_opcode_arg::WORD_PAIR:
            current_arg.word_pair.first  = (i16)*pc++;
            current_arg.word_pair.second = (i16)*pc++;
            
            break;

        case e_opcode_arg::NL: {
            u16 count = *pc++;

            current_arg.number_list.values = pc;
            current_arg.number_list.count  = count;
            pc                            += 2 * count;
            
            break;
        }

        default:
            break;
    }
}

// sub_A21D60
bool vm_thread::call_script_library_function(const argument_t &arg, u16* old_pc) {
    i32 used = (i32)(dstack.sp - dstack.buffer);

    script_library::script_library_function* function = arg.library_function;

    if (function->vtable->invoke(function, dstack)) {
        entry = 0;

        return true;
    }

    // run the call again next time, from the same stack depth
    pc        = old_pc;
    dstack.sp = dstack.buffer + used;
    entry     = 1;

    return false;
}

// sub_A221C0
void vm_thread::spawn_sub_thread(const argument_t &arg) {
    u32 size = arg.function->parms_stacksize;

    vm_thread* thread = inst->add_thread(arg.function, dstack.sp - size, size, nullptr, 0);

    thread->creator         = this;
    thread->camera_priority = camera_priority;

    dstack.pop(size);
    dstack.push_uint(thread->thread_id);
}

// sub_A22230
void vm_thread::spawn_parallel_thread(const argument_t &arg) {
    script_instance*       target   = (script_instance*)dstack.pop_uint();
    const script_function* function = arg.function;

    // from another executable, run the target's function of the same signature
    if (target->parent->parent != function->parent->parent) {
        i32 index = target->parent->find_func(function->fullname);

        if (index >= 0)
            function = target->parent->get_function_ptr(index);
    }

    u32 size = function->parms_stacksize;

    vm_thread* thread = target->add_thread(function, dstack.sp - size, size, nullptr, 0);

    thread->creator = this;

    if (arg.function != function)
        thread->creator = nullptr;

    thread->camera_priority = camera_priority;

    dstack.pop(size);
    dstack.push_uint(thread->thread_id);
}

// sub_A22300
void vm_thread::create_event_callback(      e_opcode_arg  argtype,
                                      const argument_t   &arg,
                                            bool          one_shot,
                                            bool          remap) {
 
    auto* target = (script_instance*)dstack.pop_uint();

    const script_function*   function = arg.function;
          script_executable* exec     = function->parent->parent;

    if (target->parent->parent != exec) {
        i32 index = target->parent->find_func(function->fullname);

        if (index >= 0)
            function = target->parent->get_function_ptr(index);
    }

    if (remap)
        function = target->parent->get_function_ptr(target->parent->find_func(function->fullname));

    dstack.pop(function->parms_stacksize);

    const void* parms  = dstack.sp;
    string_hash signal = dstack.pop_signal();
    u32         id     = 0;

    {
        engine_lock_scope scope(references::callback_lock.read());

        switch (argtype) {
            case e_opcode_arg::GSIG_SFR:
                id = references::add_global_callback_callback.read()(this,
                                                                     signal,
                                                                     target,
                                                                     (script_function*)function,
                                                                     parms,
                                                                     one_shot);
                
                break;

            case e_opcode_arg::ISIG_SFR: {
                script_instance* recipient = (script_instance*)dstack.pop_uint();

                id = references::add_instance_callback_callback.read()(this,
                                                                       signal,
                                                                       recipient,
                                                                       target,
                                                                       (script_function*)function,
                                                                       parms,
                                                                       one_shot);
                
                break;
            }

            case e_opcode_arg::LSIG_SFR: {
                arch_base_vhandle signaller(dstack.pop_uint());

                id = references::add_library_callback_callback.read()(this,
                                                                      signal,
                                                                      signaller,
                                                                      target,
                                                                      (script_function*)function,
                                                                      parms,
                                                                      one_shot);
                
                break;
            }

            default:
                break;
        }
    }

    dstack.push_uint(id);
}

// sub_A224B0
void vm_thread::create_static_event_callback(      e_opcode_arg  argtype,
                                             const argument_t   &arg,
                                                   bool          one_shot) {

    auto* function = (script_function*)arg.function;

    dstack.pop(function->parms_stacksize);

    const void* parms  = dstack.sp;
    string_hash signal = dstack.pop_signal();
    u32         id     = 0;

    {
        engine_lock_scope scope(references::callback_lock.read());

        switch (argtype) {
            case e_opcode_arg::GSIG_SFR:
                id = references::add_global_callback_callback.read()(this, signal, inst, function, parms, one_shot);
                
                break;

            case e_opcode_arg::ISIG_SFR: {
                script_instance* recipient = (script_instance*)dstack.pop_uint();

                id = references::add_instance_callback_callback.read()(this, signal, recipient, inst, function, parms, one_shot);
                
                break;
            }

            case e_opcode_arg::LSIG_SFR: {
                arch_base_vhandle signaller(dstack.pop_uint());

                id = references::add_library_callback_callback.read()(this,
                                                                      signal,
                                                                      signaller,
                                                                      inst,
                                                                      function,
                                                                      parms,
                                                                      one_shot);
                
                break;
            }

            default:
                break;
        }
    }

    dstack.push_uint(id);
}

// sub_A21F00
void vm_thread::execute_push_with_modifier(const u8* source, u16 dsize) {
    i16 width = next_push_pop_modifier_arg.word;

    switch (next_push_pop_modifier) {
        case next_push_pop_modifier_none:
            dstack.push(source, dsize);
            
            break;

        case next_push_pop_modifier_inline_array:
            dstack.push(source + width * (i32)next_push_pop_modifier_subscript, dsize);
            
            break;

        case next_push_pop_modifier_indirect_array:
            dstack.push(*(const u8**)source + width * (i32)next_push_pop_modifier_subscript, dsize);
            
            break;

        case next_push_pop_modifier_address:
            dstack.push_uint((u32)source);

            next_push_pop_modifier = next_push_pop_modifier_none;
            
            return;

        case next_push_pop_modifier_dynamic_array: {
            const chuck_dynamic_array_t* array = *(const chuck_dynamic_array_t**)source;

            dstack.push((const u8*)array->begin() + 12 * (i32)(i64)next_push_pop_modifier_subscript, dsize);
            
            break;
        }

        default:
            break;
    }

    next_push_pop_modifier = next_push_pop_modifier_none;
}

// sub_A225F0
void vm_thread::execute_pop_with_modifier(u8* dest, u16 dsize, i32 limit) { // limit is never read?
    i16 width = next_push_pop_modifier_arg.word;

    switch (next_push_pop_modifier) {
        case next_push_pop_modifier_none:
            std::memcpy(dest, dstack.sp - dsize, dsize);
            dstack.pop(dsize);
            
            break;

        // the staged element width is what gets copied and popped
        case next_push_pop_modifier_inline_array:
            std::memcpy(dest + width * (i32)next_push_pop_modifier_subscript, dstack.sp - width, width);
            dstack.pop(width);
            
            break;

        case next_push_pop_modifier_indirect_array:
            std::memcpy(*(u8**)dest + width * (i32)next_push_pop_modifier_subscript, dstack.sp - width, width);
            dstack.pop(width);
            
            break;

        case next_push_pop_modifier_dynamic_array: {
            chuck_dynamic_array_t* array = *(chuck_dynamic_array_t**)dest;

            std::memcpy((u8*)array->begin() + 12 * (i32)(i64)next_push_pop_modifier_subscript, dstack.sp - width, width);
            dstack.pop(width);
            
            break;
        }

        default:
            break;
    }

    next_push_pop_modifier = next_push_pop_modifier_none;
}

// sub_A21DB0
void vm_thread::push_flow_stack(const script_function* current_exec) {
    void* memory = references::flow_stack_element_pool.get().allocate();

    vm_thread_flow_stack_element* element = memory ?
        new (memory) vm_thread_flow_stack_element {} : nullptr;

    element->pc       = pc;
    element->instance = current_instance;

    // a member function's first argument is its instance
    if (current_exec->parent->flags & script_object_flag_global_object)
        current_instance = nullptr;
    else
        current_instance = *(script_instance**)(dstack.sp - current_exec->parms_stacksize);

    element->previous = flow_stack;
    flow_stack        = element;
}

// sub_A21E40
void vm_thread::pop_flow_stack() {
    vm_thread_flow_stack_element* element = flow_stack;

    // the root function returned
    if (!element) {
        pc = nullptr;

        return;
    }

    flow_stack       = element->previous;
    pc               = element->pc;
    current_instance = element->instance;

    references::flow_stack_element_pool.get().release(element);
}

// sub_A22740
void vm_thread::create_new_instance(script_object* so, const char* inst_name) {
    const script_function* constructor = so->funcs.data[so->constructor_index];

    script_instance* created;

    {
        mash::string name(inst_name);
        vm_thread*   constructor_thread;

        // the constructor arguments sit under the 4-byte instance slot the compiler reserved
        created = so->add_instance( name,
                                    dstack.sp - constructor->parms_stacksize + 4,
                                   &constructor_thread,
                                    script_instance_stack_size_normal);
    }

    created->run(false);

    dstack.move_sp(4 - constructor->parms_stacksize);
    dstack.push_uint((u32)created);
}

// sub_A22820
bool vm_thread::kill_selected_thread() {
    bool finished = false;

    switch (current_opcode_arg) {
        case e_opcode_arg::NULL_ARG:
            return true;

        case e_opcode_arg::SFR: {
            const vm_thread* ignore_thread = nullptr;

            if (ex == current_arg.function) {
                finished      = true;
                ignore_thread = this;
            }

            inst->kill_thread(current_arg.function, ignore_thread);

            break;
        }

        case e_opcode_arg::UINT: {
            u32 victim_id = dstack.pop_uint();

            if (victim_id == thread_id)
                return true;

            vm_thread* victim = script_manager::inst()->find_thread(victim_id);

            if (victim) {
                victim->inst->kill_thread(victim);

                return false;
            }

            break;
        }

        default:
            break;
    }

    return finished;
}

// sub_A22880
bool vm_thread::kill_threads_in_target() {
    script_instance* target   = (script_instance*)dstack.pop_uint();
    bool             finished = false;

    if (!target)
        target = inst->parent->parent->get_global_script_object()->global_instance;

    const vm_thread* ignore_thread = nullptr;

    if (inst == target && ex == current_arg.function) {
        finished      = true;
        ignore_thread = this;
    }

    target->kill_thread(current_arg.function, ignore_thread);

    return finished;
}

// sub_A228D0
bool vm_thread::massacre_selected_threads() {
    switch (current_opcode_arg) {
        case e_opcode_arg::NULL_ARG:
            inst->massacre_threads_by_function(nullptr, this);

            return true;

        case e_opcode_arg::SFR:
            if (inst->massacre_threads_by_function(current_arg.function, this))
                return true;

            break;

        case e_opcode_arg::UINT: {
            u32 victim_id = dstack.pop_uint();

            if (victim_id == thread_id) {
                inst->massacre_threads_by_function(nullptr, this);

                return true;
            }

            vm_thread* victim = script_manager::inst()->find_thread(victim_id);

            return victim && victim->inst->massacre_threads_by_thread(victim, this);
        }

        default:
            break;
    }

    return false;
}

// sub_A22940
bool vm_thread::massacre_threads_in_target() {
    script_instance* target   = (script_instance*)dstack.pop_uint();
    bool             finished = false;

    if (inst == target)
        finished = ex == current_arg.function;

    if (target->massacre_threads_by_function(current_arg.function, this))
        return true;

    return finished;
}

// sub_A22B70
bool vm_thread::run() {
    next_push_pop_modifier           = next_push_pop_modifier_none;
    next_push_pop_modifier_arg.uint  = 0;
    next_push_pop_modifier_subscript = 0.0f;
    current_arg.uint                 = 0;

    // a native call that isn't done yet stops the run
    bool keep_running = true;

    do {
        u16* instruction = pc;

        current_opcode     = (e_opcode)(((u8*)instruction)[1] & 0x7F);
        current_opcode_arg = (e_opcode_arg)((((u8*)instruction)[0] >> 2) & 0x3F);
        current_dsize      = 4 * (((u8*)instruction)[0] & 3);
        pc                 = instruction + 1;

        fill_argument();

        bool int_operands = current_opcode_arg != e_opcode_arg::NULL_ARG;
        i16  local        = current_arg.word;

        switch (current_opcode) {
            case e_opcode::ADD:
                if (!int_operands) {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = dstack.top_num() + rhs;
                } else {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() += rhs;
                }

                break;

            case e_opcode::AND:
                if (int_operands) {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() &= rhs;
                } else {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = (f32)((i32)rhs & (i32)dstack.top_num());
                }

                break;

            case e_opcode::BF:
                if (dstack.pop_num() == 0.0f)
                    pc = (u16*)((u8*)pc + current_arg.word);

                break;

            case e_opcode::BF2:
                if (dstack.top_num() == 0.0f)
                    pc = (u16*)((u8*)pc + current_arg.word);

                break;

            case e_opcode::BRA:
                pc = (u16*)((u8*)pc + current_arg.word);

                break;

            case e_opcode::BSL:
                keep_running = call_script_library_function(current_arg, instruction);

                break;

            case e_opcode::BSR:
                push_flow_stack(current_arg.function);

                pc = current_arg.function->buffer;

                break;

            case e_opcode::BST:
                spawn_sub_thread(current_arg);

                break;

            case e_opcode::BTH:
                spawn_parallel_thread(current_arg);

                break;

            case e_opcode::DEC:
                if (!int_operands)
                    dstack.top_num() = dstack.top_num() - 1.0f;
                else if (current_opcode_arg == e_opcode_arg::UINT_NULL)
                    --dstack.top_uint();

                break;

            case e_opcode::DIV:
                if (!int_operands) {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = dstack.top_num() / rhs;
                } else if (current_opcode_arg == e_opcode_arg::UINT_NULL) {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() /= rhs;
                }

                break;

            case e_opcode::DUP:
                switch (current_opcode_arg) {
                    case e_opcode_arg::NULL_ARG:
                        dstack.push(dstack.sp - current_dsize, current_dsize);

                        break;

                    case e_opcode_arg::SPR:
                        std::memcpy(dstack.sp + local, dstack.sp - current_dsize, current_dsize);

                        break;

                    case e_opcode_arg::POPO: {
                        script_instance* target = (script_instance*)dstack.pop_uint();

                        std::memcpy(target->data.buffer + local, dstack.sp - current_dsize, current_dsize);

                        break;
                    }

                    case e_opcode_arg::SDR:
                        std::memcpy(current_arg.pointer, dstack.sp - current_dsize, current_dsize);

                        break;

                    default:
                        break;
                }

                break;

            case e_opcode::EQ:
                switch (current_opcode_arg) {
                    case e_opcode_arg::NULL_ARG: {
                        f32 rhs = dstack.pop_num();

                        dstack.top_num() = dstack.top_num() != rhs ? 0.0f : 1.0f;

                        break;
                    }

                    case e_opcode_arg::NUM:
                        dstack.top_num() = dstack.top_num() != current_arg.number ? 0.0f : 1.0f;

                        break;

                    // the popped object is ignored; the one under it is tested for null
                    case e_opcode_arg::OBJ:
                        dstack.sp -= 4;
                        dstack.top_num() = (f32)(dstack.top_uint() == 0);

                        break;

                    case e_opcode_arg::UINT_NULL: {
                        u32 rhs = dstack.pop_uint();

                        dstack.top_uint() = dstack.top_uint() == rhs;

                        break;
                    }

                    default:
                        break;
                }
                break;

            case e_opcode::GE:
                if (int_operands) {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() = dstack.top_uint() >= rhs;
                } else {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = dstack.top_num() < rhs ? 0.0f : 1.0f;
                }

                break;

            case e_opcode::GT:
                if (int_operands) {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() = rhs < dstack.top_uint();
                } else {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = dstack.top_num() <= rhs ? 0.0f : 1.0f;
                }

                break;

            case e_opcode::INC:
                if (!int_operands)
                    dstack.top_num() = dstack.top_num() + 1.0f;
                else if (current_opcode_arg == e_opcode_arg::UINT_NULL)
                    ++dstack.top_uint();

                break;

            case e_opcode::KIL:
                if (kill_selected_thread())
                    return true;

                break;

            case e_opcode::LE:
                if (int_operands) {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() = rhs >= dstack.top_uint();
                } else {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = rhs < dstack.top_num() ? 0.0f : 1.0f;
                }

                break;

            case e_opcode::LNT:
                if (int_operands)
                    dstack.top_uint() = dstack.top_uint() == 0;
                else
                    dstack.top_num() = dstack.top_num() == 0.0f ? 1.0f : 0.0f;

                break;

            case e_opcode::LT:
                if (int_operands) {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() = dstack.top_uint() < rhs;
                } else {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = rhs <= dstack.top_num() ? 0.0f : 1.0f;
                }

                break;

            case e_opcode::MOD:
                if (int_operands) {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() %= rhs;
                } else {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = (f32)((i32)dstack.top_num() % (i32)rhs);
                }

                break;

            case e_opcode::MUL:
                if (int_operands) {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() *= rhs;
                } else {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = rhs * dstack.top_num();
                }

                break;

            case e_opcode::NE:
                switch (current_opcode_arg) {
                    case e_opcode_arg::NULL_ARG: {
                        f32 rhs = dstack.pop_num();

                        dstack.top_num() = dstack.top_num() == rhs ? 0.0f : 1.0f;

                        break;
                    }

                    case e_opcode_arg::OBJ:
                        dstack.sp -= 4;
                        dstack.top_num() = (f32)(dstack.top_uint() != 0);

                        break;

                    case e_opcode_arg::UINT_NULL: {
                        u32 rhs = dstack.pop_uint();

                        dstack.top_uint() = dstack.top_uint() != rhs;

                        break;
                    }

                    default:
                        break;
                }

                break;

            case e_opcode::NEG:
                if (!int_operands)
                    dstack.top_num() = -0.0f - dstack.top_num();

                break;

            case e_opcode::NOT:
                if (!int_operands)
                    dstack.top_num() = (f32)~(i32)dstack.top_num();
                else if (current_opcode_arg == e_opcode_arg::UINT_NULL)
                    dstack.top_uint() = ~dstack.top_uint();

                break;

            case e_opcode::OR:
                if (int_operands) {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() |= rhs;
                } else {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = (f32)((i32)dstack.top_num() | (i32)rhs);
                }

                break;

            case e_opcode::POP:
                switch (current_opcode_arg) {
                    case e_opcode_arg::NULL_ARG:
                    case e_opcode_arg::NUM:
                    case e_opcode_arg::STR:
                    case e_opcode_arg::UINT:
                        dstack.pop(current_dsize);

                        break;

                    case e_opcode_arg::SPR:
                        execute_pop_with_modifier(dstack.sp + local, (u16)current_dsize, -1);

                        break;

                    case e_opcode_arg::POPO: {
                        script_instance* target = (script_instance*)dstack.pop_uint();

                        execute_pop_with_modifier(target->data.buffer + local, (u16)current_dsize, -1);

                        break;
                    }

                    case e_opcode_arg::SDR: {
                        script_instance* global_instance = inst->parent->parent->get_global_script_object()->global_instance;

                        i32 limit = (i32)(global_instance->data.buffer + global_instance->data.blocksize - (u8*)current_arg.pointer);

                        execute_pop_with_modifier((u8*)current_arg.pointer, (u16)current_dsize, limit);

                        break;
                    }

                    case e_opcode_arg::GV:
                        references::global_variable_lock.read()->acquire();

                        execute_pop_with_modifier((u8*)current_arg.pointer, (u16)current_dsize, -1);

                        references::global_variable_lock.read()->release();

                        break;

                    // destroys the popped instance
                    case e_opcode_arg::OBJ: {
                        script_instance* doomed = (script_instance*)dstack.pop_uint();

                        if (doomed) {
                            script_object* object = doomed->parent;

                            if (!(object->parent->flags & script_executable_flag_unloading))
                                object->remove_instance(doomed, true);
                        }

                        break;
                    }

                    default:
                        break;
                }

                break;

            case e_opcode::PSH:
                switch (current_opcode_arg) {
                    case e_opcode_arg::NUM:
                        dstack.push_num(current_arg.number);

                        break;

                    case e_opcode_arg::STR:
                    case e_opcode_arg::CLV:
                    case e_opcode_arg::PSIG:
                    case e_opcode_arg::UINT:
                        dstack.push_uint(current_arg.uint);

                        break;

                    case e_opcode_arg::SPR:
                        execute_push_with_modifier(dstack.sp + local, (u16)current_dsize);

                        break;

                    case e_opcode_arg::POPO: {
                        script_instance* source = (script_instance*)dstack.pop_uint();

                        if (source)
                            execute_push_with_modifier(source->data.buffer + local, (u16)current_dsize);

                        break;
                    }

                    case e_opcode_arg::SDR:
                        execute_push_with_modifier((const u8*)current_arg.pointer, (u16)current_dsize);

                        break;

                    case e_opcode_arg::GV:
                        references::global_variable_lock.read()->acquire();

                        execute_push_with_modifier((const u8*)current_arg.pointer, (u16)current_dsize);

                        references::global_variable_lock.read()->release();

                        break;

                    case e_opcode_arg::OBJ:
                        create_new_instance(current_arg.object, "__new");

                        break;

                    // the object of the same name in the popped instance's executable
                    case e_opcode_arg::OBJ_REF: {
                        script_instance*   source = (script_instance*)dstack.pop_uint();
                        script_executable* exec   = source->parent->parent;

                        create_new_instance(exec->find_object(current_arg.object->name, nullptr), "__new");

                        break;
                    }

                    case e_opcode_arg::UNK_34:
                        dstack.push_uint((u32)current_arg.object->first_instance());

                        break;

                    case e_opcode_arg::UNK_35:
                        create_new_instance(current_arg.object, (const char*)dstack.pop_uint());

                        break;

                    default:
                        break;
                }
                break;

            case e_opcode::RET:
                pop_flow_stack();

                if (!pc)
                    return true;

                break;

            case e_opcode::SHL:
                if (int_operands) {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() <<= rhs;
                } else {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = (f32)((i32)dstack.top_num() << (i32)rhs);
                }

                break;

            case e_opcode::SHR:
                if (int_operands) {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() >>= rhs;
                } else {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = (f32)((i32)dstack.top_num() >> (i32)rhs);
                }

                break;

            case e_opcode::SPA:
                dstack.move_sp(current_arg.word);

                break;

            case e_opcode::SPA0:
                dstack.move_sp_and_zero(current_arg.word);

                break;

            case e_opcode::SUB:
                if (int_operands) {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() -= rhs;
                } else {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = dstack.top_num() - rhs;
                }
                
                break;

            case e_opcode::XOR:
                if (int_operands) {
                    u32 rhs = dstack.pop_uint();

                    dstack.top_uint() ^= rhs;
                } else {
                    f32 rhs = dstack.pop_num();

                    dstack.top_num() = (f32)((i32)rhs ^ (i32)dstack.top_num());
                }

                break;

            case e_opcode::ECB:
                create_event_callback(current_opcode_arg, current_arg, false, false);

                break;

            case e_opcode::SCB:
                create_static_event_callback(current_opcode_arg, current_arg, false);

                break;

            case e_opcode::ECO:
                create_event_callback(current_opcode_arg, current_arg, true, false);

                break;

            case e_opcode::SCO:
                create_static_event_callback(current_opcode_arg, current_arg, true);

                break;

            case e_opcode::REGISTER_CALLBACK_TARGET_MODE1_REMAP:
                create_event_callback(current_opcode_arg, current_arg, true, true);

                break;

            case e_opcode::REGISTER_CALLBACK_TARGET_MODE0_REMAP:
                create_event_callback(current_opcode_arg, current_arg, false, true);

                break;

            case e_opcode::KL2:
                if (kill_threads_in_target())
                    return true;

                break;

            case e_opcode::MAS:
                if (massacre_selected_threads())
                    return true;

                break;

            case e_opcode::MS2:
                if (massacre_threads_in_target())
                    return true;

                break;

            case e_opcode::WAITFRAME:
                return false;

            case e_opcode::PAE:
                next_push_pop_modifier           = next_push_pop_modifier_inline_array;
                next_push_pop_modifier_arg.word  = current_arg.word;
                next_push_pop_modifier_subscript = dstack.pop_num();

                break;

            case e_opcode::CASTISTR:
            case e_opcode::CASTFSTR: {
                f32   value  = dstack.pop_num();
                char* buffer = &references::cast_string_buffer.get();

                if (current_opcode == e_opcode::CASTISTR)
                    int_to_string((i32)value, buffer);
                else
                    float_to_string(value, 3, buffer, 1024);

                char* string = vm_dynamic_array_manager::inst()->create_new_string(nullptr, 10, buffer, "");

                // the result is also kept in a local; the offset counts the popped operand
                *(char**)(dstack.sp + local + 4) = string;

                dstack.push_uint((u32)string);

                break;
            }

            case e_opcode::STR_ADD: {
                const char* second = (const char*)dstack.pop_uint();
                const char* first  = (const char*)dstack.pop_uint();

                char* string = vm_dynamic_array_manager::inst()->create_new_string(nullptr, 10, first, second);

                *(char**)(dstack.sp + local + 8) = string;

                dstack.push_uint((u32)string);

                break;
            }

            case e_opcode::STR_EQ:
                if (current_opcode_arg == e_opcode_arg::NULL_ARG) {
                    const char* rhs = (const char*)dstack.pop_uint();

                    dstack.top_num() = (f32)(std::strcmp((const char*)dstack.top_uint(), rhs) == 0);
                } else if (current_opcode_arg == e_opcode_arg::STR)
                    dstack.top_num() = (f32)(std::strcmp((const char*)dstack.top_uint(), current_arg.string) == 0);

                break;

            case e_opcode::STR_NE:
                if (current_opcode_arg == e_opcode_arg::NULL_ARG) {
                    const char* rhs = (const char*)dstack.pop_uint();

                    dstack.top_num() = (f32)(std::strcmp((const char*)dstack.top_uint(), rhs) != 0);
                } else if (current_opcode_arg == e_opcode_arg::STR)
                    dstack.top_num() = (f32)(std::strcmp((const char*)dstack.top_uint(), current_arg.string) != 0);

                break;

            case e_opcode::BRA_JT: {
                i32 index = (i32)dstack.pop_num();

                if (index < 0 || index >= current_arg.jump_table.count)
                    pc = (u16*)((u8*)pc + current_arg.jump_table.default_displacement);
                else
                    pc = (u16*)((u8*)pc + current_arg.jump_table.displacements[index]);

                break;
            }

            case e_opcode::RETS:
                dstack.move_sp(current_arg.word);

                pop_flow_stack();

                if (!pc)
                    return true;

                break;

            case e_opcode::PARE:
                next_push_pop_modifier_arg       = current_arg;
                next_push_pop_modifier           = next_push_pop_modifier_indirect_array;
                next_push_pop_modifier_subscript = dstack.pop_num();

                break;

            case e_opcode::ADDR:
                next_push_pop_modifier = next_push_pop_modifier_address;

                break;

            case e_opcode::DC: {
                chuck_dynamic_array_t* array = vm_dynamic_array_manager::inst()->create_new_dynamic_array(nullptr, current_arg.word);

                dstack.push_uint((u32)array);
                add_local_reference(array, 0);

                break;
            }

            case e_opcode::DD:
                dstack.push_uint((u32)vm_dynamic_array_manager::inst()->create_new_dynamic_array(inst, current_arg.word));

                break;

            case e_opcode::RELEASE_TRACKED_PLAIN_ARRAY: {
                auto* array = (chuck_dynamic_array_t*)dstack.pop_uint();

                remove_local_reference(array);
                vm_dynamic_array_manager::inst()->remove_reference(array);

                break;
            }

            case e_opcode::DPS: {
                dstack.pop(current_dsize);

                u8* payload = dstack.sp;

                auto* array = (chuck_dynamic_array_t*)dstack.pop_uint();

                // appended as whatever was on the stack, then overwritten
                chuck_dynamic_array_element_t element;

                array->push_back(element);

                std::memcpy((u8*)array->end() - 12, payload, current_dsize);

                break;
            }

            case e_opcode::DPO: {
                auto* array = (chuck_dynamic_array_t*)dstack.pop_uint();

                dstack.push((u8*)array->end() - 12, current_dsize);

                if (array->size())
                    array->pop_back();

                break;
            }

            case e_opcode::DSZ: {
                auto* array = (chuck_dynamic_array_t*)dstack.pop_uint();

                dstack.push_num((f32)array->size());

                break;
            }

            case e_opcode::DPA:
                next_push_pop_modifier_arg       = current_arg;
                next_push_pop_modifier           = next_push_pop_modifier_dynamic_array;
                next_push_pop_modifier_subscript = dstack.pop_num();

                break;

            case e_opcode::DCL: {
                auto* array = (chuck_dynamic_array_t*)dstack.pop_uint();

                array->erase(array->begin(), array->end());

                break;
            }

            case e_opcode::RAS:
            case e_opcode::RASA: {
                void* raise_args      = nullptr;
                u32   args_stack_size = 0;

                if (current_opcode == e_opcode::RASA) {
                    args_stack_size = dstack.pop_uint();

                    dstack.pop(args_stack_size);

                    raise_args = dstack.sp;
                }

                string_hash signal = dstack.pop_signal();

                switch (current_opcode_arg) {
                    case e_opcode_arg::GSIG:
                        references::raise_global_signal_callback.read()(this, signal, raise_args, args_stack_size);

                        break;

                    case e_opcode_arg::ISIG: {
                        script_instance* target = (script_instance*)dstack.pop_uint();

                        references::raise_instance_signal_callback.read()(this, signal, target, raise_args, args_stack_size);

                        break;
                    }

                    case e_opcode_arg::LSIG: {
                        arch_base_vhandle signaller(dstack.pop_uint());

                        references::raise_library_signal_callback.read()(this, signal, signaller, raise_args, args_stack_size);

                        break;
                    }

                    default:
                        break;
                }

                break;
            }

            case e_opcode::AUTODEST: {
                script_instance* target = (script_instance*)dstack.pop_uint();

                if (target)
                    target->enable_auto_destruct();

                break;
            }

            case e_opcode::CASTUINT:
                dstack.top_uint() = (u32)(i64)dstack.top_num();

                break;

            case e_opcode::CASTNUM:
                dstack.top_num() = (f32)dstack.top_uint();

                break;

            case e_opcode::CGC:
                references::callback_lock.read()->acquire();

                if (current_opcode_arg == e_opcode_arg::STR) {
                    string_hash name;

                    name.initialize(mash::ALLOCATED, current_arg.string);

                    references::clear_global_callback_by_name_callback.read()(this, name);
                } else if (current_opcode_arg == e_opcode_arg::UINT_NULL)
                    references::clear_global_callback_by_id_callback.read()(this, dstack.pop_uint());

                references::callback_lock.read()->release();

                break;

            case e_opcode::CIC: {
                script_instance* target = (script_instance*)dstack.pop_uint();

                references::callback_lock.read()->acquire();

                if (current_opcode_arg == e_opcode_arg::STR) {
                    string_hash name;

                    name.initialize(mash::ALLOCATED, current_arg.string);

                    references::clear_instance_callback_by_name_callback.read()(this, name, target);
                } else if (current_opcode_arg == e_opcode_arg::UINT_NULL)
                    references::clear_instance_callback_by_id_callback.read()(this, dstack.pop_uint(), target);

                references::callback_lock.read()->release();

                break;
            }

            // the receiver is the first argument; its object's function of the same signature runs
            case e_opcode::BVSR: {
                const script_function* prototype = current_arg.function;

                script_object*   receiver = (*(script_instance**)(dstack.sp - prototype->parms_stacksize))->parent;
                script_function* function = receiver->get_function_ptr(receiver->find_func(prototype->fullname));

                push_flow_stack(function);

                pc = function->buffer;

                break;
            }

            case e_opcode::INC_LOCAL:
                dstack.local_num(local) = dstack.local_num(local) + 1.0f;

                break;

            case e_opcode::DEC_LOCAL:
                dstack.local_num(local) = dstack.local_num(local) - 1.0f;

                break;

            case e_opcode::LT_LOCAL:
                dstack.top_num() = dstack.top_num() <= dstack.local_num(local) ? 0.0f : 1.0f;

                break;

            case e_opcode::DPA_LOCAL:
                next_push_pop_modifier           = next_push_pop_modifier_dynamic_array;
                next_push_pop_modifier_arg.word  = 4;
                next_push_pop_modifier_subscript = dstack.local_num(local);

                break;

            case e_opcode::PAE_LOCAL:
                next_push_pop_modifier           = next_push_pop_modifier_inline_array;
                next_push_pop_modifier_arg.word  = 4;
                next_push_pop_modifier_subscript = dstack.local_num(local);

                break;

            case e_opcode::ADD_LOCAL:
                dstack.top_num() = dstack.local_num(local) + dstack.top_num();

                break;

            case e_opcode::SUB_LOCAL:
                dstack.top_num() = dstack.top_num() - dstack.local_num(local);

                break;

            case e_opcode::MUL_LOCAL:
                dstack.top_num() = dstack.local_num(local) * dstack.top_num();

                break;

            // the local holds an instance; the second word is the member offset
            case e_opcode::PSH_LM: {
                auto* source = (script_instance*)dstack.local_uint(current_arg.word_pair.first);

                dstack.push(source->data.buffer + current_arg.word_pair.second, current_dsize);

                break;
            }

            case e_opcode::POP_LM: {
                auto* target = (script_instance*)dstack.local_uint(current_arg.word_pair.first);

                std::memcpy(target->data.buffer + current_arg.word_pair.second, dstack.sp - (u16)current_dsize, current_dsize);
                dstack.pop(current_dsize);

                break;
            }

            case e_opcode::PSH_NUM:
                dstack.push_num(current_arg.number);

                break;

            case e_opcode::PSH_NL:
                dstack.push(current_arg.number_list.values, 4 * current_arg.number_list.count);

                break;

            case e_opcode::PSH_STR:
            case e_opcode::PUSH_IMMEDIATE_WORD:
                dstack.push_uint(current_arg.uint);

                break;

            case e_opcode::PAE_LIT:
                next_push_pop_modifier           = next_push_pop_modifier_inline_array;
                next_push_pop_modifier_subscript = current_arg.number;
                next_push_pop_modifier_arg.word  = 4;

                break;

            case e_opcode::PAE_LIT12:
                next_push_pop_modifier           = next_push_pop_modifier_inline_array;
                next_push_pop_modifier_subscript = current_arg.number;
                next_push_pop_modifier_arg.word  = 12;

                break;

            case e_opcode::DPA_LIT:
                next_push_pop_modifier_subscript = current_arg.number;
                next_push_pop_modifier           = next_push_pop_modifier_dynamic_array;
                next_push_pop_modifier_arg.word  = 4;

                break;

            case e_opcode::DPA_LIT12:
                next_push_pop_modifier           = next_push_pop_modifier_dynamic_array;
                next_push_pop_modifier_subscript = current_arg.number;
                next_push_pop_modifier_arg.word  = 12;

                break;

            case e_opcode::GT_LIT:
                dstack.top_num() = dstack.top_num() <= current_arg.number ? 0.0f : 1.0f;

                break;

            case e_opcode::EQ_LIT:
                dstack.top_num() = dstack.top_num() != current_arg.number ? 0.0f : 1.0f;

                break;

            case e_opcode::LT_LIT:
                dstack.top_num() = current_arg.number <= dstack.top_num() ? 0.0f : 1.0f;

                break;

            case e_opcode::LE_LIT:
                dstack.top_num() = current_arg.number < dstack.top_num() ? 0.0f : 1.0f;

                break;

            case e_opcode::LNT_LOCAL:
                dstack.push_num(dstack.local_num(local) == 0.0f ? 1.0f : 0.0f);

                break;

            // whether the popped pointer is a live instance slot whose run has been called
            case e_opcode::CST: {
                auto* candidate = (script_instance*)dstack.pop_uint();

                bool live = candidate &&
                            references::instance_pool.get().contains(candidate) &&
                            (candidate->flags & script_instance_flag_run_called);

                dstack.push_num(live ? 1.0f : 0.0f);

                break;
            }

            case e_opcode::COPY_TOP_WORD_TO_LOCAL:
                dstack.local_uint(local) = dstack.top_uint();

                break;

            case e_opcode::RELEASE_LOCAL_PLAIN_ARRAY_REF:
                vm_dynamic_array_manager::inst()->remove_string_reference((const void*)dstack.local_uint(local));

                break;

            case e_opcode::RETAIN_PLAIN_ARRAY_REF:
                if (current_opcode_arg == e_opcode_arg::NULL_ARG)
                    vm_dynamic_array_manager::inst()->add_string_reference((const void*)dstack.top_uint());
                else if (current_opcode_arg == e_opcode_arg::SPR)
                    vm_dynamic_array_manager::inst()->add_string_reference((const void*)dstack.local_uint(local));

                break;

            case e_opcode::RELEASE_TOP_PLAIN_ARRAY_REF:
                if (current_opcode_arg == e_opcode_arg::NULL_ARG)
                    vm_dynamic_array_manager::inst()->remove_string_reference((const void*)dstack.top_uint());

                break;

            // each element is handed over by its address
            case e_opcode::RETAIN_NESTED_ARRAY_ELEMENTS:
            case e_opcode::RELEASE_NESTED_ARRAY_ELEMENTS: {
                auto* array = (chuck_dynamic_array_t*)dstack.pop_uint();

                if (!array)
                    break;

                vm_dynamic_array_manager* manager = vm_dynamic_array_manager::inst();

                for (u32 index = 0; index < array->size(); ++index) {
                    const void* element = (const u8*)array->begin() + 12 * index;

                    if (current_opcode == e_opcode::RETAIN_NESTED_ARRAY_ELEMENTS)
                        manager->add_string_reference(element);
                    else
                        manager->remove_string_reference(element);
                }

                break;
            }

            case e_opcode::CLEAR_NESTED_ARRAY:
                vm_dynamic_array_manager::inst()->release_string_elements((chuck_dynamic_array_t*)dstack.pop_uint());

                break;

            case e_opcode::DESTROY_TRACKED_NESTED_ARRAY: {
                auto* array = (chuck_dynamic_array_t*)dstack.pop_uint();

                remove_local_reference(array);
                vm_dynamic_array_manager::inst()->remove_string_array_reference(array);

                break;
            }

            case e_opcode::CREATE_TRACKED_NESTED_ARRAY: {
                chuck_dynamic_array_t* array = vm_dynamic_array_manager::inst()->create_new_dynamic_array(nullptr, current_arg.word);

                dstack.push_uint((u32)array);
                add_local_reference(array, 4);

                break;
            }

            default: // NOP, INT, BOUND, INSTCHECK
                break;
        }
    } while (keep_running);

    return false;
}
