#include "game/wds/entity/interface/player_web_shooters.hh"
#include "game/resource_key.hh"
#include "game/script/script_access.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity_base.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// sub_995FB0
void player_web_shooters::destruct_mashed_class() {
    finalize(mash::FROM_MASH);
}

// sub_994B10
void player_web_shooters::finalize(mash::allocation_scope scope) {
    if (!web_shooter_script_inst)
        return;

    kill_all_webs(-1.0f);

    chuck::vm::script_object* so = web_shooter_script_inst->parent;

    script::destroy_instance(web_shooter_script_inst, true);

    auto* owner_ent = (entity_base*)owner.resolve();

    script::unload_script_exec(so, owner_ent->is_an_actor() ? (actor*)owner_ent : nullptr);

    frame_advance_func_id             = -1;
    set_default_web_kill_time_func_id = -1;
    kill_all_webs_func_id             = -1;
    kill_web_func_id                  = -1;
    fire_web_func_id                  = -1;
    fire_web_at_ground_func_id        = -1;
    fire_web_at_ceiling_func_id       = -1;
}

// sub_994B80
void player_web_shooters::init(entity_base* _owner, const char* script_obj, const char* script_exe) {
    if (!_owner)
        return;

    owner = _owner->my_handle;

    if (web_shooter_script_inst)
        return;

    resource_key script_exec_prefix {};

    if (!script::load_script_exec(script_exe, &script_exec_prefix, _owner->is_an_actor() ? (actor*)_owner : nullptr))
        return;

    string_hash obj_name;
    obj_name.initialize(mash::ALLOCATED, script_obj);

    chuck::vm::script_object* so = script::find_object(obj_name, mash::string(script_exe), &script_exec_prefix);

    if (!so) {
        script::unload_script_exec(script_exe, _owner->is_an_actor() ? (actor*)_owner : nullptr);
        return;
    }

    mash::string inst_name(mash::string::fmt, "0x%08x_%s_%d", _owner->id.source_hash_code, "Player_Web_Shooter", id);

    chuck::vm::vm_thread* constructor_thread = nullptr;

    web_shooter_script_inst = script::add_instance(mash::string(inst_name.c_str()),
                                                   so,
                                                   &constructor_thread,
                                                   _owner->is_an_actor() ? (actor*)_owner : nullptr,
                                                   chuck::vm::script_instance_stack_size_large);

    script::push_value(constructor_thread, owner);
    script::run_thread(constructor_thread);

    string_hash function_name;

    function_name.initialize(mash::ALLOCATED, "frame_advance(num)");
    frame_advance_func_id = script::find_function(function_name, so);

    function_name.initialize(mash::ALLOCATED, "set_default_web_kill_time(num)");
    set_default_web_kill_time_func_id = script::find_function(function_name, so);

    function_name.initialize(mash::ALLOCATED, "kill_webs(num)");
    kill_all_webs_func_id = script::find_function(function_name, so);

    function_name.initialize(mash::ALLOCATED, "kill_web(uint,num)");
    kill_web_func_id = script::find_function(function_name, so);

    function_name.initialize(mash::ALLOCATED, "fire_web_adv(num,entity,vector3d,num,num,num,num,num)");
    fire_web_func_id = script::find_function(function_name, so);

    function_name.initialize(mash::ALLOCATED, "fire_web_at_ground_adv(num,vector3d,num,num,num,num,num,num,num)");
    fire_web_at_ground_func_id = script::find_function(function_name, so);

    function_name.initialize(mash::ALLOCATED, "fire_web_at_ceiling_adv(num,vector3d,num,num,num,num,num,num,num)");
    fire_web_at_ceiling_func_id = script::find_function(function_name, so);
}

// sub_987AC0
void player_web_shooters::frame_advance(f32 delta_t) {
    if (frame_advance_func_id == -1 || !web_shooter_script_inst)
        return;

    auto* owner_ent = (entity_base*)owner.resolve();

    chuck::vm::vm_thread* thread = script::create_thread(frame_advance_func_id,
                                                         web_shooter_script_inst,
                                                         owner_ent && owner_ent->is_an_actor() ? (actor*)owner_ent : nullptr);

    script::push_value(thread, delta_t);
    script::run_thread(thread);
}

// sub_987B20
void player_web_shooters::kill_all_webs(f32 time) {
    if (kill_all_webs_func_id == -1 || !web_shooter_script_inst)
        return;

    auto* owner_ent = (entity_base*)owner.resolve();

    chuck::vm::vm_thread* thread = script::create_thread(kill_all_webs_func_id,
                                                         web_shooter_script_inst,
                                                         owner_ent && owner_ent->is_an_actor() ? (actor*)owner_ent : nullptr);

    script::push_value(thread, time);
    script::run_thread(thread);
}

// sub_987B80
void player_web_shooters::kill_web(u32 id, f32 time) {
    if (kill_web_func_id == -1 || !web_shooter_script_inst)
        return;

    auto* owner_ent = (entity_base*)owner.resolve();

    chuck::vm::vm_thread* thread = script::create_thread(kill_web_func_id,
                                                         web_shooter_script_inst,
                                                         owner_ent && owner_ent->is_an_actor() ? (actor*)owner_ent : nullptr);

    script::push_uint(thread, id);
    script::push_value(thread, time);
    script::run_thread(thread);
}

// sub_987BF0
u32 player_web_shooters::fire_web(      bool               right_hand,
                                        arch_base_vhandle  target,
                                  const vector3d          &target_pos,
                                        f32                attach_len,
                                        f32                speed,
                                        f32                acc,
                                        f32                curly_rad,
                                        bool               sound) {

    u32 web_id = 0;

    if (fire_web_func_id == -1 || !web_shooter_script_inst)
        return web_id;

    auto* owner_ent = (entity_base*)owner.resolve();

    chuck::vm::vm_thread* thread = script::create_thread(fire_web_func_id,
                                                         web_shooter_script_inst,
                                                         owner_ent && owner_ent->is_an_actor() ? (actor*)owner_ent : nullptr);

    script::push_value(thread, right_hand ? 1.0f : 0.0f);
    script::push_value(thread, target);
    script::push_value(thread, target_pos);
    script::push_value(thread, attach_len);
    script::push_value(thread, speed);
    script::push_value(thread, acc);
    script::push_value(thread, curly_rad);
    script::push_value(thread, sound ? 1.0f : 0.0f);
    script::run_thread_with_return(thread, &web_id);

    return web_id;
}
