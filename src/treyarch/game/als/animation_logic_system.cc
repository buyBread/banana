#include "retail.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/game/als/animation_logic_system.hh"
#include "treyarch/game/animation/animation_controller.hh"
#include "treyarch/game/wds/entity/actor.hh"

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

// sub_5E1070
void als::animation_logic_system::frame_advance_flush_queue(bool unk) {
    ((void (__cdecl*)(bool))retail::sub_5E1070)(unk);
}

// sub_5ED8C0
void als::animation_logic_system::frame_advance(f32 delta_t, bool force) {
    ((void (__thiscall*)(animation_logic_system*, f32, bool))retail::sub_5ED8C0)(this, delta_t, force);
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
