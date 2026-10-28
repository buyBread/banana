#include "retail.hh"
#include "game/als/als.hh"
#include "game/als/als_jq.hh"
#include "game/als/animation_logic_system.hh"
#include "game/als/mocomp.hh"
#include "game/als/state_machine.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/time_interface.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_5E88D0
bool als::animation_logic_system_interface::force_update(f32 delta_t, bool unk) {
    if (!unk && !treyarch::references::unk_010fc53e.read())
        return false;

    {
        engine_lock_scope access(get_access_lock());
        engine_lock_scope forcing(&references::force_update_lock.get());

        frame_advance(delta_t, true);

        animation_logic_system::frame_advance_flush_queue(false);
    }

    force_updated = true;

    return true;
}

// sub_5ECB60
void als::animation_logic_system_interface::frame_advance_all_alses(f32 delta_t) {
    engine_lock_scope lock(&references::als_list_lock.get());

    frame_advance_als_list(&references::the_high_priority_als_list.get(), &references::the_high_priority_als_advance_list.get(), delta_t);
    frame_advance_als_list(&references::the_low_priority_als_list.get(),  &references::the_low_priority_als_advance_list.get(),  delta_t);
}

// sub_5EC900
void als::animation_logic_system_interface::frame_advance_als_list(als_list** the_list, als_list** the_advance_list, f32 delta_t) {
    if (!*the_list || !(*the_list)->size())
        return;

    // the list that has to advance now becomes the advance list; what advances goes back into the other one
    als_list* advancing = *the_list;
    *the_list           = *the_advance_list;
    *the_advance_list   = advancing;

    als_jq::start();
    als_jq::delta_t.write(delta_t);
    als_jq::lock_batch_pool();

    als_list::node* end = (*the_advance_list)->head();

    for (als_list::node* position = end->next; position != end; position = position->next) {
        animation_logic_system_interface* the_als = position->value;

        if (!the_als->als_is_suspended()) {
            actor* the_actor = the_als->get_actor();

            bool advances = ((the_actor->flags & 1) == 0 && the_actor->get_primary_region()) ||
                            (the_actor->get_ai_core() && !retail::sub_4C53F0((u32*)the_actor->get_ai_core()));

            if (advances) {
                bool unk = false;

                if (i32 basic_inode = retail::sub_4C77B0((i32)the_actor)) {
                    string_hash key;
                    key.initialize(mash::ALLOCATED, nullptr, 0x37C2EE03);

                    unk = retail::sub_4E5470((u32*)basic_inode, (u32*)&key, nullptr) != 0;
                }

                if (the_actor->is_a_conglomerate() && !unk) {
                    bool sync_anim = (the_actor->ext_flags & entity_ext_flag_sync_anim) != 0;
                    bool occluded  = the_actor->was_occluded_last_frame();

                    ((animation_logic_system*)the_als)->update_matrix_publishing(!occluded, sync_anim);
                }

                the_als->cache_actor_position();

                als_jq::static_batch.get().payload[0] = the_als;
                als_jq::add_batch(1);
            }
        }

        (*the_list)->push_back(the_als);
    }

    als_jq::unlock_batch_pool();
    als_jq::finish(false);

    animation_logic_system::frame_advance_flush_queue(true);

    (*the_advance_list)->clear();
}

// sub_5ECBF0
void als::animation_logic_system_interface::add_to_als_list(animation_logic_system_interface* the_als) {
    engine_lock_scope lock(&references::als_list_lock.get());

    als_list** the_list;
    als_list** the_advance_list;

    if (the_als->is_high_priority()) {
        the_list         = &references::the_high_priority_als_list.get();
        the_advance_list = &references::the_high_priority_als_advance_list.get();
    } else {
        the_list         = &references::the_low_priority_als_list.get();
        the_advance_list = &references::the_low_priority_als_advance_list.get();
    }

    // both lists of a pair exist while either has an als in it
    if (!*the_list) {
        als_list* list = (als_list*)mm::allocate(sizeof(als_list));

        if (list)
            new (list) als_list();

        *the_list = list;

        als_list* advance_list = (als_list*)mm::allocate(sizeof(als_list));

        if (advance_list)
            new (advance_list) als_list();

        *the_advance_list = advance_list;
    }

    (*the_list)->push_back(the_als);
}

// sub_5ECDA0
void als::animation_logic_system_interface::remove_from_als_list(animation_logic_system_interface* the_als) {
    engine_lock_scope lock(&references::als_list_lock.get());

    als_list** pairs[2][2] = { { &references::the_low_priority_als_list.get(),  &references::the_low_priority_als_advance_list.get()  },
                               { &references::the_high_priority_als_list.get(), &references::the_high_priority_als_advance_list.get() } };

    for (auto &pair : pairs) {
        als_list** the_list         = pair[0];
        als_list** the_advance_list = pair[1];

        if (!*the_list)
            continue;

        als_list::node* end = (*the_list)->head();

        for (als_list::node* position = end->next; position != end; position = position->next) {
            if (position->value == the_als) {
                (*the_list)->erase(position);

                break;
            }
        }

        if (!(*the_list)->size()) {
            (*the_list)->~als_list();
            mm::free(*the_list);

            *the_list = nullptr;

            if (*the_advance_list) {
                (*the_advance_list)->~als_list();
                mm::free(*the_advance_list);
            }

            *the_advance_list = nullptr;
        }
    }
}

// sub_5ECEE0
void als::animation_logic_system_interface::set_high_priority(animation_logic_system_interface* the_als) {
    engine_lock_scope lock(&references::als_list_lock.get());

    remove_from_als_list(the_als);
    add_to_als_list(the_als);
}

// sub_5ECF50
void als::animation_logic_system_interface::set_low_priority(animation_logic_system_interface* the_als) {
    engine_lock_scope lock(&references::als_list_lock.get());

    remove_from_als_list(the_als);
    add_to_als_list(the_als);
}

// sub_5D4600
void als::als_jq_callback(jq::batch* batch) {
    auto the_als = (animation_logic_system_interface*)batch->payload[0];

    actor*             the_actor   = the_als->get_actor();
    interface_storage* ifc_storage = the_actor->my_ifc_storage;

    f32 dilation;

    // biped physics runs on world time
    if ((ifc_storage && ifc_storage->has_ifc(entity_ifc_physical) &&
         ((physical_interface*)ifc_storage->get_ifc(entity_ifc_physical))->is_flagged(physical_interface_flag_biped_running)) ||

        !ifc_storage || !ifc_storage->has_ifc(entity_ifc_time))

        dilation = treyarch::references::g_world_ptr.read()->time_mgr.get_time_dilation_factor();
    else
        dilation = ((time_interface*)ifc_storage->get_ifc(entity_ifc_time))->calc_time_dilation();

    the_als->frame_advance((f32)((f64)dilation * (f64)als_jq::delta_t.read()), false);
}

// sub_5D4C90
void als::als_mocomp_jq_callback(jq::batch* batch) {
    auto index = (u32)batch->payload[0];

    references::queued_mocomps.get()[index]->post_anim_action(references::queued_deltas.get()[index]);
}

// sub_5B4A70
void als::multi_als_mocomp_jq_callback(jq::batch*) {}

// sub_5E0DE0
void als::animation_event_signal_jq_callback(jq::batch* batch) {
    raise_animation_event_signals((state_machine*)batch->payload[0]);
}

// sub_5E0E50
void als::multi_animation_event_signal_jq_callback(jq::batch* batch) {
    auto machines = (state_machine**)batch->payload[0];

    for (i32 index = 0; index < batch->count; ++index)
        raise_animation_event_signals(machines[index]);
}

// sub_5E0F70
void als::pre_animation_event_signal_jq_callback(jq::batch* batch) {
    raise_pre_animation_event_signals((state_machine*)batch->payload[0]);
}

// sub_5E0FE0
void als::multi_pre_animation_event_signal_jq_callback(jq::batch* batch) {
    auto machines = (state_machine**)batch->payload[0];

    for (i32 index = 0; index < batch->count; ++index)
        raise_pre_animation_event_signals(machines[index]);
}
