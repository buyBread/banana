#include "retail.hh"
#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/script/script_access.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/resource_key.hh"

using namespace treyarch;

// sub_980000
void mission_manager::setup_global_data(const char* level_name, amalga::resource_pack_slot* slot) {
    {
        mash::string level(level_name);
        mash::string underscore("_");
        mash::string name = underscore + level;

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name.c_str());

        gen_global_exec_name = hash;
    }

    string_hash megacity;
    megacity.initialize(mash::ALLOCATED, "_megacity");

    unk_23c = gen_global_exec_name == megacity;

    if (unk_23c)
        retail::sub_804C30((u32*)icon_entity_pool,
                           (void*)"itm_markgen_clone",
                           88,
                           4,
                           (i32)on_icon_entity_acquired,
                           (i32)reset_icon_entity,
                           (i32)reset_icon_entity,
                           0);

    resource_key key;
    key.hash = string_hash();
    key.set(gen_global_exec_name, 9);

    if (!retail::sub_7629D0((u32*)&key, nullptr, nullptr))
        return;

    if (chuck::vm::script_manager::inst()->is_loaded(gen_global_exec_name, string_hash()))
        chuck::vm::script_manager::inst()->un_load(gen_global_exec_name, false, string_hash());

    chuck::vm::script_manager::inst()->load(gen_global_exec_name, 0, slot, string_hash());

    gen_global_exec = chuck::vm::script_manager::inst()->find_executable(gen_global_exec_name, string_hash());

    while (gen_global_exec->has_threads())
        chuck::vm::script_manager::inst()->run_single_exec(gen_global_exec_name, string_hash(), 0.0f, false);

    i32 passes = 3;

    chuck::vm::script_instance* header = gen_global_exec->find_instance(references::internal_generated_script_header.read(),
                                                                        references::internal_generated_script_header.read());

    // the header's first two nums are its version and how many passes to run
    i32 version = 0;

    if (header) {
        const f32* header_data = (const f32*)header->data.buffer;

        version = (i32)header_data[0];

        if (version >= 2)
            passes = (i32)header_data[1];
    }

    if (version >= 5) {
        while (gen_global_exec->has_threads())
            chuck::vm::script_manager::inst()->run_single_exec(gen_global_exec_name, string_hash(), 0.0f, false);

        mash::string init_all_data = mash::string(level_name) + mash::string("_init_all_data()");

        string_hash init_all_data_hash;
        init_all_data_hash.initialize(mash::ALLOCATED, init_all_data.c_str());

        i32 function_index = script::find_function(init_all_data_hash, gen_global_exec->get_global_script_object());

        script::create_thread(function_index, gen_global_exec->get_global_script_object()->global_instance, 0);

        for (; passes > 0; --passes)
            chuck::vm::script_manager::inst()->run_single_exec(gen_global_exec_name, string_hash(), 0.0f, false);

        while (gen_global_exec->has_threads())
            chuck::vm::script_manager::inst()->run_single_exec(gen_global_exec_name, string_hash(), 0.0f, false);
    } else {
        for (; passes > 0; --passes)
            chuck::vm::script_manager::inst()->run_single_exec(gen_global_exec_name, string_hash(), 0.0f, false);
    }

    fixup_mission_headers();
}

// sub_97F6A0
void mission_manager::fixup_mission_headers() {
    if (!gen_global_exec)
        return;

    {
        fixup_mission_header_children_visitor visitor(gen_global_exec);
        gen_global_exec->walk_instances(&visitor);
    }

    {
        fixup_mission_header_visitor visitor(gen_global_exec);
        gen_global_exec->walk_instances(&visitor);
    }
}

// sub_97FDB0
// sub_97FBE0
void mission_manager::on_icon_entity_acquired(arch_base_vhandle handle) {
    arch_base* object = handle.resolve();

    if (!object)
        return;

    ((entity*)object)->set_visible(true, false);

    if (!object->is_an_actor())
        return;

    actor* owner = (actor*)object;

    if (owner->unk_0b0->instance)
        return;

    string_hash object_name;
    retail::sub_5FD120((u32*)(owner->unk_0b0->unk_010 + 4), (u32*)&object_name);

    string_hash filename;

    chuck::vm::script_object* script_object =
        chuck::vm::script_manager::inst()->find_object(*(string_hash*)retail::sub_5FD120((u32*)owner->unk_0b0->unk_014, (u32*)&filename),
                                                       object_name,
                                                       owner->unk_0b0->unk_018);

    chuck::vm::vm_thread* thread = nullptr;

    {
        mash::string enx("__enx");

        owner->unk_0b0->instance = script::add_instance(enx,
                                                        script_object,
                                                        &thread,
                                                        owner,
                                                        chuck::vm::script_instance_stack_size_normal);
    }

    script::push_entity(thread, owner);
    script::start_thread(thread, true);
}

// sub_97FDC0
// sub_97FD20
void mission_manager::reset_icon_entity(arch_base_vhandle handle) {
    arch_base* object = handle.resolve();

    if (!object)
        return;

    if (object->is_an_actor() && ((actor*)object)->unk_0b0) {
        if (chuck::vm::script_instance* inst = ((actor*)object)->unk_0b0->instance) {
            inst->parent->remove_instance(inst, true);

            ((actor*)object)->unk_0b0->instance = nullptr;
        }
    }

    vector3 far_away(-999.0f);
    retail::sub_626CA0((u32*)object, (u32*)&far_away);

    ((entity*)object)->set_visible(false, false);
}

// sub_97ECC0
bool fixup_mission_header_children_visitor::allow_object(const chuck::vm::script_object* object) {
    string_hash name;

    return *(string_hash*)retail::sub_5FD120((u32*)object, (u32*)&name) == references::mission_header_instance_base_name.read();
}

// sub_97F5E0
chuck::vm::e_visit_result fixup_mission_header_children_visitor::visit(const chuck::vm::script_instance* inst) {
    mission_header_instance_base* header = (mission_header_instance_base*)inst->data.buffer;

    const char* child_name = (const char*)header->child_inst;

    if (!*child_name) {
        header->child_inst = nullptr;

        return chuck::vm::visit_result_continue;
    }

    string_hash child_hash;
    child_hash.initialize(mash::ALLOCATED, child_name);

    chuck::vm::script_instance* child = exec->find_instance(references::mission_header_instance_base_name.read(), child_hash);

    header->child_inst = child;

    const f32 &mission_var = mission_manager::inst()->mission_vars[(i64)header->unk_01c];

    if (child) {
        mission_header_instance_base* child_header = (mission_header_instance_base*)child->data.buffer;

        child_header->flags |= mission_header_flag_eligible_required;

        if (mission_var >= 0.0f)
            child_header->flags |= mission_header_flag_eligible;
    }

    return chuck::vm::visit_result_continue;
}

// sub_97ECC0
bool fixup_mission_header_visitor::allow_object(const chuck::vm::script_object* object) {
    string_hash name;

    return *(string_hash*)retail::sub_5FD120((u32*)object, (u32*)&name) == references::mission_header_instance_base_name.read();
}

// sub_983DD0
chuck::vm::e_visit_result fixup_mission_header_visitor::visit(const chuck::vm::script_instance* inst) {
    u32 header_flags = ((mission_header_instance_base*)inst->data.buffer)->flags;

    if ((header_flags & mission_header_flag_unk_00008000) &&
        !(header_flags & mission_header_flag_eligible_required) &&
        !(header_flags & mission_header_flag_uses_trigger))

        mission_manager::inst()->add_mission_icon(inst);

    return chuck::vm::visit_result_continue;
}
