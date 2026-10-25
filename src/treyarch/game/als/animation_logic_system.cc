#include <cstring>

#include "retail.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/amalga/resource_pack_slot.hh"
#include "treyarch/game/als/als_jq.hh"
#include "treyarch/game/als/animation_logic_system.hh"
#include "treyarch/game/animation/animation_controller.hh"
#include "treyarch/game/animation/animation_controller_jq.hh"
#include "treyarch/game/animation/animation_pose_jq.hh"
#include "treyarch/game/script/script_access.hh"
#include "treyarch/game/wds/ai/ai_core.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/game/wds/entity/capsule_alter_sys.hh"
#include "treyarch/game/wds/entity/interface/physical_interface.hh"
#include "treyarch/game/wds/entity/interface/standing_interface.hh"
#include "treyarch/jq/native_locks.hh"
#include "treyarch/shared/math/references.hh"

using namespace treyarch;

// sub_5ED580
als::animation_logic_system::animation_logic_system(actor* _the_actor) : als_shared(nullptr),
                                                                         the_actor(_the_actor),
                                                                         the_controller(nullptr),
                                                                         mocomp_layer_index(0),
                                                                         curr_mocomp(nullptr),
                                                                         unk_a6c(nullptr),
                                                                         is_suspended(false),
                                                                         needs_reinit_anim(false),
                                                                         unk_a72(true),
                                                                         unk_a73(false),
                                                                         unk_a74(false),
                                                                         unk_a75(true),
                                                                         coordinator(nullptr),
                                                                         coordinator_unk(0) {

    force_updated = false;

    access_lock = (engine_recursive_lock*)memory::heap::allocate(sizeof(engine_recursive_lock));

    if (access_lock)
        access_lock->reset();

    access_lock->reset();

    the_mocomp_data = (motion_compensator*)memory::heap::allocate(0x100);
    actor_position  = the_actor->my_abs_po->get_position();

    add_to_als_list(this);
}

// sub_5ED750
als::animation_logic_system::~animation_logic_system() {
    if (curr_mocomp) {
        curr_mocomp->deactivate();
        curr_mocomp->~motion_compensator();
        curr_mocomp = nullptr;
    }

    mocomp_layer_index = 0;

    if (coordinator) {
        coordinator->remove_coordinated(this);
        coordinator     = nullptr;
        coordinator_unk = 0;
    }

    while (coordinated.size())
        coordinated[0]->set_coordinator(nullptr, 0);

    memory::heap::free(the_mocomp_data);
    the_mocomp_data = nullptr;

    remove_from_als_list(this);

    unk_a6c = nullptr;

    if (access_lock)
        memory::heap::free(access_lock);
}

// sub_5D4C70
als::state_machine_interface* als::animation_logic_system::get_als_layer(layer_id the_layer) {
    return get_als_layer_internal(the_layer);
}

// sub_5D4C80
bool als::animation_logic_system::has_als_layer(layer_id the_layer) const {
    return has_als_layer_internal(the_layer);
}

// sub_5BB2F0
als::state_machine* als::animation_logic_system::get_als_layer_internal(layer_id the_layer) {
    if (the_layer == the_base_sm.get_layer_id())
        return &the_base_sm;

    for (u32 index = 0; index < the_layers.size(); ++index) {
        if (the_layer == the_layers[index]->get_layer_id())
            return the_layers[index];
    }

    return nullptr;
}

// sub_5BB370
bool als::animation_logic_system::has_als_layer_internal(layer_id the_layer) const {
    if (the_layer == the_base_sm.get_layer_id())
        return true;

    for (u32 index = 0; index < the_layers.size(); ++index) {
        if (the_layer == the_layers[index]->get_layer_id())
            return true;
    }

    return false;
}

// sub_5BB280
bool als::animation_logic_system::is_als_interruptable() const {
    // -1 stands for the base machine, but the unsigned compare ends the walk before it starts
    for (u32 index = (u32)-1; index < the_layers.size(); ++index) {
        const state_machine* machine = index == (u32)-1 ? (const state_machine*)&the_base_sm : the_layers[index];

        if (machine->is_active() && (machine->curr_state->flags & state::super_uninterruptable))
            return false;
    }

    return true;
}

// sub_5D45E0
string_hash als::animation_logic_system::get_internal_hash(state_machine* the_sm, param_id param) {
    return the_cache.get_internal_hash(this, the_sm, param);
}

// sub_5E2300
// inlined @ sub_5E8FE0
void als::animation_logic_system::end_layer(layer_id the_layer) {
    state_machine* machine = get_als_layer_internal(the_layer);

    if (machine && machine->is_active()) {
        machine->end_layer();

        retail::sub_5BA3B0((i32*)the_controller, the_layer); // animation_controller::kill_layer_anims
    }
}

// sub_5BAFB0
bool als::animation_logic_system::did_transition_succeed() const {
    if (the_base_sm.did_transition_succeed())
        return true;

    for (u32 index = 0; index < the_layers.size(); ++index) {
        if (the_layers[index] &&
            the_layers[index]->is_active() &&
            the_layers[index]->did_transition_succeed())

            return true;
    }

    return false;
}

// sub_5BB030
bool als::animation_logic_system::did_transition_succeed_from_request() const {
    if (the_base_sm.did_transition_succeed_from_request())
        return true;

    for (u32 index = 0; index < the_layers.size(); ++index) {
        if (the_layers[index] &&
            the_layers[index]->is_active() &&
            the_layers[index]->did_transition_succeed_from_request())

            return true;
    }

    return false;
}

// sub_5AF2F0
void als::animation_logic_system::suspend_logic_system(bool suspend) {
    if (is_suspended && !suspend)
        needs_reinit_anim = true;

    is_suspended = suspend;

    if (the_controller && the_controller->state)
        the_controller->state->is_suspended = is_suspended;
}

// sub_5ED700
bool als::animation_logic_system::als_is_suspended() const {
    return is_suspended;
}

// sub_5F6270
void als::animation_logic_system::create_instance_data(animation_logic_system_shared* shared_portion) {
    als_shared = shared_portion;

    if (!the_controller) {
        if (!the_actor->anim_ctrl)
            the_actor->allocate_anim_controller();

        the_controller = the_actor->anim_ctrl;
    }

    the_base_sm.init(this, als_shared->the_base_sm_shared);

    if (the_base_sm.curr_state && (the_base_sm.curr_state->flags & state::initially_suspended))
        suspend_logic_system(true);

    for (layer_state_machine_shared* shared_layer : als_shared->the_layers_shared) {
        layer_state_machine* layer = new layer_state_machine();
        layer->init(this, shared_layer);

        the_layers.push_back(layer);
    }

    change_mocomp(the_base_sm.curr_state, the_base_sm.get_layer_id());

    mocomp_layer_index = 0;
}

// sub_5EEE80
void als::animation_logic_system::delete_instance_data() {
    the_base_sm.kill_als_state_script(this, true);
    the_base_sm.shared_portion = nullptr;

    for (layer_state_machine* layer : the_layers) {
        layer->kill_als_state_script(this, true);

        if (layer)
            delete layer;
    }

    the_layers.clear();

    als_shared = nullptr;
}

// sub_5D5020
void als::animation_logic_system::reset_animation_player() {
    ((void (__thiscall*)(animation_controller*))retail::sub_5D44E0)(the_controller);
}

// sub_5ED720
engine_recursive_lock* als::animation_logic_system::get_access_lock() const {
    return access_lock;
}

// sub_5BB420
bool als::animation_logic_system::is_high_priority() const {
    if (!the_controller)
        return false;

    if (the_controller->state->unk_12c.size())
        return true;

    if (is_coordinator())
        return true;

    return false;
}

// sub_5ED710
actor* als::animation_logic_system::get_actor() const {
    return the_actor;
}

// sub_5AF2C0
void als::animation_logic_system::cache_actor_position() {
    actor_position = the_actor->my_abs_po->get_position();
}

// sub_5AFA40
void als::animation_logic_system::set_coordinator(animation_logic_system_interface* new_coordinator, i32 unk) {
    if (coordinator == new_coordinator)
        return;

    if (coordinator)
        coordinator->remove_coordinated(this);

    coordinator     = new_coordinator;
    coordinator_unk = unk;

    if (new_coordinator)
        new_coordinator->add_coordinated(this);
}

// sub_5AFA30
bool als::animation_logic_system::has_coordinator() const {
    return coordinator != nullptr;
}

// sub_5AFA90
als::animation_logic_system_interface* als::animation_logic_system::get_coordinator() const {
    return coordinator;
}

// sub_5AFAA0
i32 als::animation_logic_system::get_coordinator_unk() const {
    return has_coordinator() ? coordinator_unk : 0;
}

// sub_5BB3F0
bool als::animation_logic_system::is_coordinator() const {
    return coordinated.size() != 0;
}

// sub_5AFA20
vc8::vector<als::animation_logic_system_interface*>* als::animation_logic_system::get_coordinated() {
    return &coordinated;
}

// sub_5F6410
void als::animation_logic_system::add_coordinated(animation_logic_system_interface* the_als) {
    animation_logic_system_interface** position = coordinated.begin();

    while (position != coordinated.end() && *position != the_als)
        ++position;

    if (position == coordinated.end())
        coordinated.push_back(the_als);

    if (is_high_priority())
        set_high_priority(this);
}

// sub_5EEE10
void als::animation_logic_system::remove_coordinated(animation_logic_system_interface* the_als) {
    animation_logic_system_interface** position = coordinated.begin();

    while (position != coordinated.end() && *position != the_als)
        ++position;

    if (position != coordinated.end())
        coordinated.erase(position);

    if (!is_high_priority())
        set_low_priority(this);
}

// sub_5ED730
void als::animation_logic_system::set_unk_a75() {
    unk_a75 = true;
}

// sub_5ED740
animation_controller* als::animation_logic_system::get_controller() const {
    return the_controller;
}

// inlined @ sub_5E1070
bool als::animation_logic_system::prepare_controller_job(animation_controller* controller, f32 delta_t) {
    animation_controller_state* state = controller->state;

    if (!state || state->unk_128 || state->is_suspended) {
        state->job.controller = nullptr;
        return false;
    }

    state->job.controller       = controller;
    state->job.skip_matrices    = state->job.unk_b0;
    state->job.delta_t          = delta_t;
    state->job.num_pose_records = 0;

    return true;
}

// inlined @ sub_5E1070
void als::animation_logic_system::prepare_pre_event_job(state_machine* machine, f32 delta_t) {
    animation_controller_state* state = machine->the_controller->state;

    if (state && !state->unk_128 && !state->is_suspended)
        state->job.delta_t = delta_t;
    else
        state->job.controller = nullptr;
}

// sub_5E1070
void als::animation_logic_system::frame_advance_flush_queue(bool threaded) {
    auto &controllers        = references::queued_controllers.get();
    auto &deltas             = references::queued_deltas.get();
    auto &signal_records     = references::queued_signal_records.get();
    auto &event_machines     = references::queued_event_machines.get();
    auto &pre_event_machines = references::queued_pre_event_machines.get();
    auto &pre_event_deltas   = references::queued_pre_event_deltas.get();

    // the state events before the animation advances
    u32 num_pre_events = references::num_queued_pre_event_machines.read();

    if (threaded && num_pre_events > 20) {
        multi_pre_animation_event_signal_jq::start();
        multi_pre_animation_event_signal_jq::lock_batch_pool();

        for (u32 index = 0; index < num_pre_events; ++index)
            prepare_pre_event_job(pre_event_machines[index], pre_event_deltas[index]);

        // retail hands the event machines here, not the pre-event ones
        jq::batch &batch     = multi_pre_animation_event_signal_jq::static_batch.get();
        u32        first     = 0;
        u32        remaining = num_pre_events;

        for (; remaining > 10; first += 10, remaining -= 10) {
            batch.payload[0] = &event_machines[first];
            batch.count      = 10;

            multi_pre_animation_event_signal_jq::add_batch(1);
        }

        if (remaining) {
            batch.payload[0] = &event_machines[first];
            batch.count      = remaining;

            multi_pre_animation_event_signal_jq::add_batch(1);
        }

        multi_pre_animation_event_signal_jq::unlock_batch_pool();
        multi_pre_animation_event_signal_jq::finish(false);
    } else {
        pre_animation_event_signal_jq::start(threaded);
        pre_animation_event_signal_jq::lock_batch_pool();

        for (u32 index = 0; index < num_pre_events; ++index) {
            prepare_pre_event_job(pre_event_machines[index], pre_event_deltas[index]);

            pre_animation_event_signal_jq::static_batch.get().payload[0] = pre_event_machines[index];
            pre_animation_event_signal_jq::add_batch(1);
        }

        pre_animation_event_signal_jq::unlock_batch_pool();
        pre_animation_event_signal_jq::finish(false);
    }

    // controller playback
    u32 num_controllers = references::num_queued_controllers.read();

    if (threaded && num_controllers > 20) {
        multi_animation_controller_jq::start();
        multi_animation_controller_jq::lock_batch_pool();

        jq::batch &batch    = multi_animation_controller_jq::static_batch.get();
        u32        first    = 0;
        u32        in_batch = 0;

        for (u32 index = 0; index < num_controllers; ++index) {
            if (in_batch >= 10) {
                batch.payload[0] = &controllers[first];
                batch.count      = in_batch;

                multi_animation_controller_jq::add_batch(1);

                first    = index;
                in_batch = 0;
            }

            prepare_controller_job(controllers[index], deltas[index]);

            ++in_batch;
        }

        if (in_batch) {
            batch.payload[0] = &controllers[first];
            batch.count      = in_batch;

            multi_animation_controller_jq::add_batch(1);
        }

        multi_animation_controller_jq::unlock_batch_pool();
        multi_animation_controller_jq::finish(false);
    } else {
        animation_controller_jq::start(threaded);
        animation_controller_jq::lock_batch_pool();

        for (u32 index = 0; index < num_controllers; ++index) {
            if (prepare_controller_job(controllers[index], deltas[index])) {
                animation_controller_jq::static_batch.get().payload[0] = &controllers[index]->state->job;
                animation_controller_jq::add_batch(1);
            }
        }

        animation_controller_jq::unlock_batch_pool();
        animation_controller_jq::finish(false);
    }

    // transport signals
    u32 num_signals = references::num_queued_signal_records.read();

    if (threaded && num_signals > 20) {
        multi_animation_signal_jq::start();
        multi_animation_signal_jq::lock_batch_pool();

        jq::batch &batch     = multi_animation_signal_jq::static_batch.get();
        u32        first     = 0;
        u32        remaining = num_signals;

        for (; remaining > 10; first += 10, remaining -= 10) {
            batch.payload[0] = &signal_records[first];
            batch.count      = 10;

            multi_animation_signal_jq::add_batch(1);
        }

        if (remaining) {
            batch.payload[0] = &signal_records[first];
            batch.count      = remaining;

            multi_animation_signal_jq::add_batch(1);
        }

        multi_animation_signal_jq::unlock_batch_pool();
        multi_animation_signal_jq::finish(false);
    } else {
        animation_signal_jq::start(threaded);
        animation_signal_jq::lock_batch_pool();

        for (u32 index = 0; index < num_signals; ++index) {
            animation_signal_jq::static_batch.get().payload[0] = signal_records[index];
            animation_signal_jq::add_batch(1);
        }

        animation_signal_jq::unlock_batch_pool();
        animation_signal_jq::finish(false);
    }

    // the state events after it
    u32 num_events = references::num_queued_event_machines.read();

    if (threaded && num_events > 20) {
        multi_animation_event_signal_jq::start();
        multi_animation_event_signal_jq::lock_batch_pool();

        jq::batch &batch     = multi_animation_event_signal_jq::static_batch.get();
        u32        first     = 0;
        u32        remaining = num_events;

        for (; remaining > 10; first += 10, remaining -= 10) {
            batch.payload[0] = &event_machines[first];
            batch.count      = 10;

            multi_animation_event_signal_jq::add_batch(1);
        }

        if (remaining) {
            batch.payload[0] = &event_machines[first];
            batch.count      = remaining;

            multi_animation_event_signal_jq::add_batch(1);
        }

        multi_animation_event_signal_jq::unlock_batch_pool();
        multi_animation_event_signal_jq::finish(false);
    } else {
        animation_event_signal_jq::start(threaded);
        animation_event_signal_jq::lock_batch_pool();

        for (u32 index = 0; index < num_events; ++index) {
            animation_event_signal_jq::static_batch.get().payload[0] = event_machines[index];
            animation_event_signal_jq::add_batch(1);
        }

        animation_event_signal_jq::unlock_batch_pool();
        animation_event_signal_jq::finish(false);
    }

    process_nal_queue(threaded);

    // the mocomps' post-animation actions, by queue index
    als_mocomp_jq::start(threaded);
    als_mocomp_jq::lock_batch_pool();

    for (u32 index = 0, num_mocomps = references::num_queued_mocomps.read(); index < num_mocomps; ++index) {
        als_mocomp_jq::static_batch.get().payload[0] = (void*)index;
        als_mocomp_jq::add_batch(1);
    }

    als_mocomp_jq::unlock_batch_pool();
    als_mocomp_jq::finish(false);

    retail::sub_5DD170(); // the unnamed lite queue

    references::num_queued_controllers       .write(0);
    references::num_queued_mocomps           .write(0);
    references::num_queued_deltas            .write(0);
    references::num_queued_signal_records    .write(0);
    references::num_queued_event_machines    .write(0);
    references::num_queued_pre_event_machines.write(0);
    references::num_queued_pre_event_deltas  .write(0);
}

// sub_5D4D70
void als::animation_logic_system::process_nal_queue(bool threaded) {
    auto &controllers = references::queued_controllers.get();

    scratch_arena arena;
    arena.base      = &references::pose_scratch.get();
    arena.size      = 0x20000;
    arena.alignment = 16;

    u32 next            = 0;
    u32 num_controllers = references::num_queued_controllers.read();

    // as many controllers per round as the scratch holds the matrices of
    do {
        animation_pose_jq::start(threaded);
        animation_pose_jq::lock_batch_pool();

        arena.cursor = arena.base;

        u32 first = next;

        while (next < num_controllers && controllers[next]->add_pose_jq_batch(&arena, false))
            ++next;

        animation_pose_jq::unlock_batch_pool();
        animation_pose_jq::finish(false);

        for (; first < next; ++first)
            controllers[first]->get_matrix_data_from_pose(false);
    } while (next < num_controllers);
}

// sub_5BB180
f32 als::get_default_blend_time(u32 flags, f32 duration) {
    if (flags & (state::cloned_anim | state::no_blend))
        return 0.0f;

    if (flags & state::quick_blend)
        return 0.13333f;

    if (flags & state::medium_blend)
        return 0.26666f;

    if (!(flags & state::long_blend))
        return 0.0f;

    f32 blend_time = (f32)((f64)duration * (f64)0.2f);

    if (!(0.5f > blend_time))
        blend_time = 0.5f;

    return (f64)blend_time > (f64)0.26666f ? 0.26666f : blend_time;
}

// sub_5AFAC0
const als::als_meta_anim_table_shared* als::animation_logic_system::get_meta_anim_table() const {
    return als_shared ? als_shared->meta_anim_table : nullptr;
}

// sub_5E1EC0
void als::animation_logic_system::enter_biped_physics(state_machine** machines, i32 num_machines, bool unk) {
    using enter_biped_physics_t = void (__thiscall*)(animation_logic_system*, state_machine**, i32, bool);

    ((enter_biped_physics_t)retail::sub_5E1EC0)(this, machines, num_machines, unk);
}

// sub_5BB200
void als::animation_logic_system::exit_biped_physics() {
    engine_lock_scope lock(&jq::references::unk_01003428.get());

    the_actor->physical_ifc()->stop_biped_physics();
}

// sub_5AF9A0
void als::animation_logic_system::update_seated_in_car(state_machine* the_sm) {
    if (!the_actor->get_ai_core() || the_sm->get_layer_id() != base_layer)
        return;

    i32 not_seated = 0;
    i32 seated     = the_sm->get_optional_pb_int(seated_in_car_id, not_seated, nullptr);

    if (seated)
        the_actor->get_ai_core()->flags |= ai_core_flag_seated_in_car;
    else
        the_actor->get_ai_core()->flags &= ~ai_core_flag_seated_in_car;
}

// sub_5ED8C0
void als::animation_logic_system::frame_advance(f32 delta_t, bool force) {
    if (is_suspended)
        return;

    if ((the_actor->ext_flags & 0x40000) && !force && !unk_a72)
        return;

    // without a region the actor only animates while its ai runs
    if ((the_actor->flags & 1) || !the_actor->get_primary_region()) {
        if (!the_actor->get_ai_core())
            return;

        if (retail::sub_4C53F0((u32*)the_actor->get_ai_core()))
            return;
    }

    unk_a72       = false;
    force_updated = false;

    std::memset(&the_cache, 0, sizeof(the_cache));

    state_machine* machines[16]   = { &the_base_sm };
    state_machine* active[16]     = {};
    f32            anim_times[16] = {};

    i32 num_active   = 0;
    i32 num_machines = 1;

    for (u32 index = 0; index < the_layers.size(); ++index) {
        if (the_layers[index])
            machines[num_machines++] = the_layers[index];
    }

    machines[0]->process_requests(this);

    for (i32 index = 1; index < num_machines; ++index) {
        the_cache.invalidate_per_layer();

        machines[index]->process_requests(this);
    }

    for (i32 index = 0; index < num_machines; ++index) {
        state_machine* machine = machines[index];

        if (!machine->is_active())
            continue;

        active[num_active]     = machine;
        anim_times[num_active] = machine->get_current_anim_time();

        // a state script that asked to be run every frame
        if (machine->frame_advance_func != -1) {
            if (chuck::vm::script_instance* inst = script::get_instance(machine->als_state_script_inst)) {
                chuck::vm::vm_thread* thread = script::create_thread(machine->frame_advance_func, inst, get_actor());

                script::push_value(thread, delta_t);
                script::run_thread(thread);
            } else
                machine->frame_advance_func = -1;
        }

        ++num_active;
    }

    if (the_base_sm.our_ret.did_do_trans) {
        interface_storage* ifc_storage = the_actor->my_ifc_storage;

        if (ifc_storage && ifc_storage->has_ifc(entity_ifc_physical)) {
            const state* base_state = the_base_sm.curr_state;

            if ((!base_state || !(base_state->flags & state::biped_physics_state)) &&
                the_actor->physical_ifc()->is_flagged(physical_interface_flag_biped_running))

                exit_biped_physics();
            else if (the_base_sm.curr_state &&
                     (the_base_sm.curr_state->flags & state::biped_physics_state) &&
                     !the_actor->physical_ifc()->is_flagged(physical_interface_flag_biped_running))

                enter_biped_physics(active, num_active, false);

            capsule_alter_sys* capsule = the_actor->capsule_alter;

            if (capsule && (capsule->mode == 2 || capsule->mode == 4)) {
                bool     exists = false;
                vector3d custom_base;
                vector3d custom_end;
                f32      custom_radius = 0.0f;

                custom_base = the_base_sm.get_optional_pb_vector3d(capsule_base_id, treyarch::references::unk_00fd4370.get(), &exists);

                if (exists)
                    custom_end = the_base_sm.get_optional_pb_vector3d(capsule_end_id, treyarch::references::unk_00fd4370.get(), &exists);

                if (exists)
                    custom_radius = the_base_sm.get_optional_pb_float(capsule_radius_id, custom_radius, &exists);

                // the base state can reshape the capsule; leaving such a state puts the previous shape back
                if (exists) {
                    capsule->set_custom_capsule(custom_base, custom_end, custom_radius);

                    if (capsule->mode != 2)
                        capsule->set_mode(2);
                    else
                        capsule->update(true);
                } else if (capsule->mode == 2)
                    capsule->set_mode(capsule->prev_mode);
            }

            if (the_base_sm.curr_state && (the_base_sm.curr_state->flags & state::cancel_all_velocity))
                the_actor->physical_ifc()->cancel_all_velocity(treyarch::references::unk_00fd4370.get(), true);
        }
    }

    bool pokes_standing = false;

    for (i32 index = 0; index < num_active; ++index) {
        state_machine*     machine = active[index];
        animation_records &records = the_controller->state->unk_02c[machine->get_layer_id()];

        if (records.size())
            records.back()->unk_06c = false;

        // nothing new to play: a wrapped loop takes the speed again
        if (!machine->our_ret.did_do_trans && (index != 0 || machine->unk_7bc) && !needs_reinit_anim) {
            animation_records &playing = the_controller->state->unk_02c[machine->get_layer_id()];

            i32 loops      = (i32)(playing.size() ? playing.back()->normalized_time : 0.0f);
            i32 prev_loops = (i32)(playing.size() ? playing.back()->prev_normalized_time : 0.0f);

            if (prev_loops != loops) {
                f32 speed = machine->get_playback_speed();

                animation_records &speed_records = the_controller->state->unk_02c[machine->get_layer_id()];

                if (speed_records.size()) {
                    speed_records.back()->speed = speed;

                    if (0.0f > speed)
                        speed_records.back()->speed = 0.0f;
                }
            }

            continue;
        }

        const state* new_state  = machine->curr_state;
        const state* prev_state = machine->prev_state;

        machine->unk_7bc = true;

        the_actor->raise_event(als_state_change);

        update_seated_in_car(machine);

        if (!(new_state->flags & state::unk_08000000)) {
            if ((new_state->flags & state::non_animated) || !the_actor->unk_0c0 || !the_actor->unk_0c0->is_data_ready()) {
                // nothing to play; the controller rests unless something else needs it
                if (index == 0 && the_controller && !the_controller->state->unk_12c.size() && the_controller->state)
                    the_controller->state->is_suspended = true;

                needs_reinit_anim = false;

                continue;
            }

            if (the_controller && the_controller->state)
                the_controller->state->is_suspended = false;

            nal_anim_id anim_name = new_state->get_nal_anim_name();

            bool differs = true;

            if ((new_state->flags & state::use_prev_anim_start_time) && prev_state)
                differs = !(prev_state->get_nal_anim_name() == anim_name);

            if (differs) {
                u32 blend_flags = new_state->flags;

                // the previous state's blend out decides how this one blends in
                if (prev_state) {
                    u32 blend_in = 0;

                    switch (prev_state->flags & (state::no_blend_out | state::quick_blend_out | state::medium_blend_out | state::long_blend_out)) {
                        case state::no_blend_out:     blend_in = state::no_blend;     break;
                        case state::quick_blend_out:  blend_in = state::quick_blend;  break;
                        case state::medium_blend_out: blend_in = state::medium_blend; break;
                        case state::long_blend_out:   blend_in = state::long_blend;   break;
                    }

                    if (blend_in)
                        blend_flags = (blend_flags & ~(state::no_blend | state::quick_blend | state::medium_blend | state::long_blend)) | blend_in;
                }

                {
                    engine_lock_scope                          starting(&references::anim_start_lock.get());
                    amalga::push_resource_context_stack_object context(the_actor->unk_0c0);

                    bool                found;
                    nal_als_anim_bridge bridge = get_anim_bridge(get_meta_anim_table(),
                                                                 anim_name,
                                                                 the_controller->state ? (actor*)the_controller->state->unk_0e4 : nullptr,
                                                                 &found);

                    if (!bridge.nal_anim && !bridge.als_anim) {
                        if (the_controller && the_controller->state)
                            the_controller->state->is_suspended = true;
                    } else {
                        animation_blends_list* blends = machine->curr_blends;
                        animation_blend*       blend  = nullptr;

                        if (blends && prev_state)
                            blend = blends->find_blend(prev_state->get_nal_anim_name());

                        auto duration = (f32)retail::sub_5BA290((i32*)the_controller, machine->get_layer_id()); // animation_controller

                        f32 blend_time = 0.0f;

                        if (!machine->our_ret.unk_19) {
                            if (machine->find_param_block_with_param(blend_time_override_id, param_type_float))
                                blend_time = machine->get_pb_float(blend_time_override_id);
                            else if (blend && blend->unk_28 >= 0.0f)
                                blend_time = blend->unk_28;
                            else if (blends && blends->unk_04 >= 0.0f)
                                blend_time = blends->unk_04;
                            else
                                blend_time = machine->get_optional_pb_float(custom_blend_time_id, get_default_blend_time(blend_flags, duration), nullptr);
                        }

                        if ((f64)blend_time > (f64)duration)
                            blend_time = (f32)((f64)duration - (f64)0.03334f);

                        f32 start_time;

                        if (new_state->flags & state::use_prev_anim_start_time)
                            start_time = anim_times[index];
                        else {
                            f32 base_start_time = machine->get_start_time();
                            f32 offset;

                            if (blend)
                                offset = machine->get_optional_pb_float(start_time_offset_override_id, blend->unk_2c, nullptr);
                            else if (blends)
                                offset = machine->get_optional_pb_float(start_time_offset_override_id, blends->unk_08, nullptr);
                            else
                                offset = machine->get_optional_pb_float(start_time_offset_override_id, 0.0f, nullptr);

                            start_time = offset + base_start_time;

                            // the blend mustn't outlast the animation's own blend in
                            bool exists        = false;
                            f32  blend_in_time = machine->get_time_to_state_event(blend_in_complete_time, start_time, &exists, nullptr);

                            if (exists && blend_in_time >= 0.0f && (f64)blend_time > (f64)blend_in_time)
                                start_time = (f32)((f64)start_time - ((f64)blend_time - (f64)blend_in_time));
                        }

                        i32  not_mirrored = 0;
                        bool mirrored     = machine->get_optional_pb_int(mirrored_animation_id, not_mirrored, nullptr) != 0;
                        bool looping      = machine->is_looping();
                        auto layer        = index ? machine->get_layer_id() : base_layer;

                        using play_t = void (__thiscall*)(animation_controller*,
                                                          nal_als_anim_bridge*,
                                                          f32,
                                                          f32,
                                                          bool,
                                                          bool,
                                                          layer_id,
                                                          animation_logic_system*,
                                                          state_machine*,
                                                          bool);

                        ((play_t)retail::sub_5DFE20)(the_controller, &bridge, start_time, blend_time, mirrored, true, layer, this, machine, looping); // animation_controller
                    }
                }

                if (new_state->flags & state::unk_04000000)
                    pokes_standing = true;
            }
        }

        f32 speed = machine->get_playback_speed();

        animation_records &speed_records = the_controller->state->unk_02c[machine->get_layer_id()];

        if (speed_records.size()) {
            speed_records.back()->speed = speed;

            if (0.0f > speed)
                speed_records.back()->speed = 0.0f;
        }

        needs_reinit_anim = false;
    }

    for (i32 index = 0; index < num_active; ++index)
        active[index]->process_post_requests(this);

    bool all_suspended = true;

    for (i32 index = 0; index < num_active; ++index) {
        state_machine* machine       = active[index];
        const state*   machine_state = machine->curr_state;

        if (!machine_state)
            continue;

        if (!(machine_state->flags & state::suspended))
            all_suspended = false;

        if (!(machine_state->flags & state::non_animated)) {
            f32 duration  = machine->get_duration();
            f32 anim_time = machine->get_current_anim_time();

            machine->set_secondary_speed(machine_state->get_secondary_playback_speed(anim_time, duration));
        }

        if (index != 0)
            continue;

        if (machine_state->my_param_block) {
            using get_class_t = void* (__thiscall*)(param_block*, string_hash, i32, bool*);

            // param_block
            if (void* expression = ((get_class_t)retail::sub_435730)(machine_state->my_param_block, expression_animation_data_id, 0, nullptr)) {
                f32 duration  = machine->get_duration();
                f32 anim_time = machine->get_current_anim_time();

                using apply_t = void (__thiscall*)(void*, animation_logic_system*, actor*, f32, f32);

                ((apply_t)retail::sub_5D4B40)(expression, this, the_actor, anim_time, duration);

                continue;
            }
        }

        unk_025(nullptr);
    }

    // the topmost machine that asks for its own mocomp gets it; the base one by default
    for (i32 index = num_active - 1; index >= 0; --index) {
        state_machine* machine       = active[index];
        const state*   machine_state = machine->curr_state;

        if (!machine_state)
            continue;

        bool takes_mocomp = machine->our_ret.did_do_trans ?
            index == 0 || (machine_state->flags & state::use_this_layers_mocomp) :
            index == 0 && mocomp_layer_index != 0;

        if (takes_mocomp) {
            the_cache.invalidate_per_layer();

            mocomp_layer_index = index;

            if (curr_mocomp && !curr_mocomp->unk_016() &&
                curr_mocomp->get_virtual_type_key() == machine_state->get_mocomp_type() &&
                !(machine_state->flags & state::force_mo_comp_init))

                curr_mocomp->transition();
            else
                change_mocomp(machine_state, active[index]->get_layer_id());

            if (index != 0) {
                interface_storage* ifc_storage = the_actor->my_ifc_storage;

                if (ifc_storage && ifc_storage->has_ifc(entity_ifc_physical) &&
                    the_actor->physical_ifc()->is_flagged(physical_interface_flag_biped_running))

                    exit_biped_physics();

                break;
            }

            // what's left of a stripped assert
            curr_mocomp->is_or_is_subclass_of(biped_physics_mocomp_key.source_hash_code);

            const state* base_state = the_base_sm.curr_state;

            if (base_state && (base_state->flags & state::biped_physics_state) &&
                curr_mocomp->is_or_is_subclass_of(biped_physics_mocomp_key.source_hash_code)) {

                interface_storage* ifc_storage = the_actor->my_ifc_storage;

                if (ifc_storage && ifc_storage->has_ifc(entity_ifc_physical) &&
                    !the_actor->physical_ifc()->is_flagged(physical_interface_flag_biped_running))

                    enter_biped_physics(active, num_active, true);
            }

            break;
        }

        // the layer whose mocomp is in use keeps it
        if (mocomp_layer_index && mocomp_layer_index == index && (machine_state->flags & state::use_this_layers_mocomp))
            break;
    }

    if (pokes_standing) {
        if (interface_storage* ifc_storage = the_actor->my_ifc_storage) {
            auto standing = (standing_interface*)ifc_storage->find_ifc(entity_ifc_standing);

            if (standing && (standing->unk_044 & 1))
                ((void (__thiscall*)(standing_interface*, f32, bool, f32))retail::sub_676ED0)(standing, 0.01f, true, 0.5f); // standing_interface
        }
    }

    the_cache.invalidate_per_layer();

    if (all_suspended) {
        suspend_logic_system(true);

        return;
    }

    const state* base_state = the_base_sm.curr_state;

    if (base_state && (base_state->flags & state::biped_physics_state) && !(base_state->flags & state::biped_physics_blend_anim)) {
        retail::sub_5D3370((i32*)the_controller, delta_t); // animation_controller, biped physics

        return;
    }

    curr_mocomp->pre_anim_action(delta_t);

    engine_lock_scope queuing(&references::queue_lock.get());

    references::queued_controllers  .get()[references::num_queued_controllers  .get()++] = the_controller;
    references::queued_mocomps      .get()[references::num_queued_mocomps      .get()++] = curr_mocomp;
    references::queued_deltas.get()[references::num_queued_deltas.get()++] = delta_t;

    for (i32 index = 0; index < num_active; ++index) {
        state_machine* machine = active[index];

        if ((machine->curr_events           && machine->curr_events->size) ||
            (machine->curr_hit_pause_events && machine->curr_hit_pause_events->size)) {

            if (machine->curr_events && machine->curr_events->size)
                references::queued_event_machines.get()[references::num_queued_event_machines.get()++] = machine;

            if (machine->curr_hit_pause_events && machine->curr_hit_pause_events->size) {
                references::queued_pre_event_machines.get()[references::num_queued_pre_event_machines.get()++] = machine;
                references::queued_pre_event_deltas  .get()[references::num_queued_pre_event_deltas  .get()++] = delta_t;
            }
        } else if (the_controller->state->unk_02c[machine->get_layer_id()].size()) {
            animation_controller_state* controller_state = the_controller->state;
            layer_id                    layer            = machine->get_layer_id();

            animation_records* signal_records = controller_state && controller_state->unk_02c[layer].size() ?
                &controller_state->unk_02c[layer] :
                nullptr;

            references::queued_signal_records.get()[references::num_queued_signal_records.get()++] = signal_records;
        }
    }
}

// sub_5D4F80
void als::animation_logic_system::unk_025(void* unk) {
    ((void (__thiscall*)(animation_logic_system*, void*))retail::sub_5D4F80)(this, unk);
}

// sub_5D4F10
void als::animation_logic_system::change_mocomp(const state* curr_state, layer_id layer) {
    ((void (__thiscall*)(animation_logic_system*, const state*, layer_id))retail::sub_5D4F10)(this, curr_state, layer);
}

// sub_5EEF70
void als::als_res_data::post_unmash_fixup(actor* owner, u32) {
    string_hash none;
    none.initialize(mash::ALLOCATED);

    if (key.hash == none) {
        the_als = nullptr;

        return;
    }

    auto shared = (animation_logic_system_shared*)amalga::resource_manager::get_resource(key, nullptr, nullptr);

    if (!shared) {
        the_als = nullptr;

        return;
    }

    the_als = new animation_logic_system(owner);
    the_als->create_instance_data(shared);
}
