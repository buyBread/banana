#include "retail.hh"
#include "game/wds/entity/grenade.hh"
#include "game/wds/entity/interface/thrown_weapon_interface.hh"
#include "game/wds/entity/interface/weapon_interface.hh"
#include "game/math/references.hh"

using namespace treyarch;

// sub_632FF0
void grenade::prepare_for_launch(thrown_weapon_interface* own) {
    const vector3d &zero = references::unk_00ffec30.get();

    thrown_weapon_owner = own;

    unk_0f0 = false;
    unk_0f2 = false;
    unk_0f7 = false;
    unk_0f5 = true;
    unk_0f3 = false;

    physical_interface* phys = physical_ifc();
    phys->set_gravity(!is_a_rocket());

    physical_ifc()->cancel_all_velocity(zero, true);
    physical_ifc()->enable();
    physical_ifc()->suspend(false);
    physical_ifc()->set_velocity(zero);
    physical_ifc()->set_angular_velocity(zero);
    physical_ifc()->velocity_increment = zero;

    unk_11c = 0.0f;

    // the executable loads into the thrower's pack, else the holder's
    amalga::resource_pack_slot* slot = nullptr;

    if (!thrown_weapon_owner->my_entity->is_an_actor() ||
        !(slot = ((actor*)thrown_weapon_owner->my_entity)->pack_slot))

        if (thrown_weapon_owner->weapon_ifc->holder.resolve())
            slot = ((actor*)thrown_weapon_owner->get_holder())->pack_slot;

    arch_base_vhandle owner_handle   = thrown_weapon_owner->my_entity->my_handle;
    arch_base_vhandle grenade_handle = my_handle;

    script_arg grenade_arg;
    grenade_arg.type  = 3;
    grenade_arg.value = &grenade_handle;

    script_arg owner_arg;
    owner_arg.type  = 3;
    owner_arg.value = &owner_handle;

    script_obj_instance.init(*(const mash::string*)thrown_weapon_owner->script_exec,
                             *(const mash::string*)thrown_weapon_owner->script_obj,
                             slot,
                             this,
                             nullptr,
                             true,
                             grenade_arg,
                             owner_arg,
                             script_arg(),
                             script_arg(),
                             script_arg(),
                             script_arg(),
                             script_arg(),
                             script_arg());

    script_obj_instance.register_function(mash::string("launch(vector3d,entity)"), 0);
    script_obj_instance.register_function(mash::string("frame_advance(num)"), 0);
    script_obj_instance.register_function(mash::string("detonate(num,vector3d,entity)"), 0);
    script_obj_instance.register_function(mash::string("allow_target(vector3d,entity)"), 0);
    script_obj_instance.register_function(mash::string("apply_damage(vector3d,entity,num)"), 0);
    script_obj_instance.register_function(mash::string("clear()"), 0);
    script_obj_instance.register_function(mash::string("bounce(vector3d,vector3d,entity,num)"), 0);

    change_list_status(false);
}

void grenade::change_list_status(bool unk) {
    retail::sub_61D4B0((i32)this, unk);
}
