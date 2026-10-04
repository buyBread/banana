#include "retail.hh"
#include "treyarch/game/wds/ai/ai_core.hh"
#include "treyarch/game/wds/ai/core_ai_resource.hh"
#include "treyarch/game/wds/ai/fight_director.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/references.hh"

using namespace treyarch;

// inlined @ sub_4E1780
void ai_core_list::remove(ai_core_list_node* node) {
    if (node->previous)
        node->previous->next = node->next;
    else
        node->list->head = node->next;

    if (node->next)
        node->next->previous = node->previous;
    else
        node->list->tail = node->previous;

    --node->list->count;

    node->next     = nullptr;
    node->previous = nullptr;
    node->list     = nullptr;
}

// inlined @ sub_4E1780
void ai_core_list::push_back(ai_core_list_node* node) {
    if (tail) {
        tail->next     = node;
        node->previous = tail;
        node->next     = nullptr;
        tail           = node;
    } else {
        node->next     = head;
        node->previous = nullptr;

        if (head)
            head->previous = node;

        head = node;

        if (!node->next)
            tail = node;
    }

    node->list = this;
    ++count;
}

// sub_4CE100
void ai_core::team_lists::clear() {
    for (dinkumware::vector<ai_core*> &team : teams)
        team.clear();
}

// sub_4DB9A0
void ai_core::team_lists::rebuild() {
    clear();

    for (ai_core_list_node* node = references::ai_core_list_high.get().head; node; node = node->next) {
        ai_core* core = node->core;

        if (core->my_team < ai_team::num_teams && !(core->flags & ai_core_flag_in_limbo))
            teams[core->my_team].push_back(core);
    }

    for (ai_core_list_node* node = references::ai_core_list_low.get().head; node; node = node->next) {
        ai_core* core = node->core;

        if (core->my_team < ai_team::num_teams && !(core->flags & ai_core_flag_in_limbo))
            teams[core->my_team].push_back(core);
    }

    valid = true;
}

// inlined @ sub_981D50
info_node* ai_core::get_info_node(e_info_node_type type) const {
    u32 word = type >> 5;
    u32 bit  = 1u << (type & 0x1F);

    if (!my_info_node_list || !(info_node_mask[word] & bit))
        return nullptr;

    // nodes are packed in type order, so the high word's nodes sit after every low-word node
    u32 position = retail::sub_401F50(info_node_mask[word] & (bit - 1));

    if (word)
        position += retail::sub_401F50(info_node_mask[0]);

    return (*my_info_node_list)[position];
}

// sub_4CE030
void ai_core::enter_limbo() {
    if (my_info_node_list) {
        for (u32 index = 0; index < my_info_node_list->size; ++index)
            (*my_info_node_list)[index]->enter_limbo();
    }

    flags |= ai_core_flag_in_limbo;

    retail::sub_4CD5B0((u32*)this);
}

// sub_4C6500
void ai_core::exit_limbo() {
    if (my_info_node_list) {
        for (u32 index = 0; index < my_info_node_list->size; ++index)
            (*my_info_node_list)[index]->exit_limbo();
    }

    flags &= ~ai_core_flag_in_limbo;
}

// inlined @ sub_4E1780
bool ai_core::should_be_in_limbo() const {
    entity* owner = (entity*)my_actor;

    if (!(flags & ai_core_flag_ignore_limbo)) {
        if (owner->unk_014 & 1)
            return true;

        if (!retail::sub_612D30((u32*)owner)) // entity::get_primary_region
            return true;
    }

    return ((bool (__thiscall*)(entity*))owner->vtable[0x198 / 4])(owner);
}

// inlined @ sub_4E1780
bool ai_core::wants_low_priority() const {
    if (flags & ai_core_flag_forcing_high_priority)
        return false;

    if (retail::sub_601C50((u32*)my_actor))
        return false;

    return my_resource->low_priority_advance ||
           my_team == ai_team::team_true_neutral ||
           ((flags & ai_core_flag_unk_00000008) && my_team == ai_team::team_civilian);
}

// inlined @ sub_4E1780
void ai_core::refresh_team() {
    string_hash no_team;
    no_team.initialize(mash::ALLOCATED);

    string_hash team;

    retail::sub_435400((u32*)&my_param_block, // param_block::get_optional_pb_hash
                       &team.source_hash_code,
                       references::team_hash.read().source_hash_code,
                       (i32)no_team.source_hash_code,
                       nullptr);

    my_team = ai_team::get_enum_by_hash(team);
}

// inlined @ sub_4E1780
void ai_core::refresh_alive() {
    entity* owner    = (entity*)my_actor;
    bool    is_alive = true;

    if (owner->has_ifc(entity_ifc_damage)) {
        u8* damage = (u8*)owner->my_ifc_storage->get_ifc(entity_ifc_damage);

        is_alive = *(i32*)(damage + 0xB4) > 0;
    }

    alive = is_alive;
}

// sub_4E1780
void ai_core::frame_advance_all_core_ais(f32 delta_t) {
    engine_lock_scope list_lock(&references::ai_core_list_lock.get());

    ai_core_list &high_list = references::ai_core_list_high.get();
    ai_core_list &low_list  = references::ai_core_list_low.get();
    team_lists   &all_teams = references::team_lists.get();

    for (ai_core_list_node* node = high_list.head; node;) {
        ai_core* core = node->core;

        if (core->should_be_in_limbo()) {
            if (!(core->flags & ai_core_flag_in_limbo))
                core->enter_limbo();
        } else if (core->flags & ai_core_flag_in_limbo) {
            core->exit_limbo();
        }

        core->refresh_team();
        core->refresh_alive();

        ai_core_list_node* next = node->next;

        if (core->wants_low_priority()) {
            ai_core_list::remove(node);
            low_list.push_back(core->list_node);

            all_teams.valid = false;
        }

        node = next;
    }

    for (ai_core_list_node* node = low_list.head; node;) {
        ai_core* core = node->core;

        if (core->should_be_in_limbo()) {
            if (!(core->flags & ai_core_flag_in_limbo))
                core->enter_limbo();
        } else if (core->flags & ai_core_flag_in_limbo) {
            core->exit_limbo();
        }

        core->refresh_team();
        core->refresh_alive();

        ai_core_list_node* next = node->next;

        if (!core->wants_low_priority()) {
            ai_core_list::remove(node);
            high_list.push_back(core->list_node);

            all_teams.valid = false;
        }

        node = next;
    }

    all_teams.rebuild();

    retail::sub_4F5C80(delta_t);
    retail::sub_436250(delta_t);

    ai_core* hero_core = (ai_core*)retail::sub_602830((u32*)references::g_world_ptr.read()->hero_ptr); // actor::get_ai_core

    if (hero_core) {
        if (info_node* hero_targets = hero_core->get_info_node(info_node_type_combat_target))
            retail::sub_472FF0((u32*)hero_targets);
    }

    retail::sub_4CBD30();
    retail::sub_4CE750((u32*)references::fight_director.read(), delta_t); // fight director frame
    retail::sub_99CB30(delta_t);
    retail::sub_4E0F90(delta_t, false); // info node pass
    retail::sub_4DCA30();
    retail::sub_4CA920(delta_t, false); // pre_ai_core_jq

    if (hero_core) {
        if (info_node* hero_targets = hero_core->get_info_node(info_node_type_combat_target))
            retail::sub_48BA50((u32**)hero_targets);
    }

    retail::sub_4DD0E0();
    retail::sub_4CA790(delta_t, false); // ai_core::advance_all_cores
    retail::sub_4E0F90(delta_t, true);  // info node post pass
}
