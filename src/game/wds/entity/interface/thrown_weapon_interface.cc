#include <cstdlib>

#include "retail.hh"
#include "game/wds/entity/interface/thrown_weapon_interface.hh"
#include "game/wds/entity/interface/weapon_interface.hh"
#include "game/wds/entity/grenade.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "game/math/references.hh"
#include "game/global.hh"
#include "treyarch/jq/native_locks.hh"

using namespace treyarch;

string_hash thrown_weapon_interface::my_type_key { 0x599EFF7B }; // "thrown_weapon_interface"

// slot 0
thrown_weapon_interface::~thrown_weapon_interface() {
    retail::sub_65D190((u32*)this);
}

// slot 1
void thrown_weapon_interface::construct_mashed_class() {
    ((void (__thiscall*)(thrown_weapon_interface*))retail::sub_65D280)(this);
}

// slot 2
void thrown_weapon_interface::destruct_mashed_class() {
    ((void (__thiscall*)(thrown_weapon_interface*))retail::sub_65D340)(this);
}

// slot 3
void thrown_weapon_interface::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(thrown_weapon_interface*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_641770)(this,
                                                                                                                          mash_info,
                                                                                                                          containing_class_ptr,
                                                                                                                          buffer);
}

// slot 4
mash::virtual_types_key thrown_weapon_interface::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const thrown_weapon_interface*))retail::sub_65D130)(this);
}

// slot 6
bool thrown_weapon_interface::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const thrown_weapon_interface*, mash::virtual_types_key))retail::sub_65D140)(this, parent_class);
}

// slot 8
i32 thrown_weapon_interface::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const thrown_weapon_interface*))retail::sub_65D180)(this);
}

// slot 16
e_entity_ifc thrown_weapon_interface::get_ifc_id() const {
    return ((e_entity_ifc (__thiscall*)(const thrown_weapon_interface*))retail::sub_A70E70)(this);
}

// slot 18
void thrown_weapon_interface::set_owner(entity* owner) {
    ((void (__thiscall*)(thrown_weapon_interface*, entity*))retail::sub_672E90)(this, owner);
}

// slot 19
i32 thrown_weapon_interface::unk_019(void* unk) {
    return ((i32 (__thiscall*)(thrown_weapon_interface*, void*))retail::sub_604CB0)(this, unk);
}

// slot 20
i32 thrown_weapon_interface::unk_020() {
    return ((i32 (__thiscall*)(thrown_weapon_interface*))retail::sub_65D0B0)(this);
}

// slot 21
i32 thrown_weapon_interface::unk_021() {
    return ((i32 (__thiscall*)(thrown_weapon_interface*))retail::sub_65D0C0)(this);
}

// slot 22
i32 thrown_weapon_interface::unk_022() {
    return ((i32 (__thiscall*)(thrown_weapon_interface*))retail::sub_65D0D0)(this);
}

// slot 23
i32 thrown_weapon_interface::unk_023() {
    return ((i32 (__thiscall*)(thrown_weapon_interface*))retail::sub_65D0E0)(this);
}

// slot 24
i32 thrown_weapon_interface::unk_024() {
    return ((i32 (__thiscall*)(thrown_weapon_interface*))retail::sub_65D0F0)(this);
}

// slot 25
i32 thrown_weapon_interface::unk_025() {
    return ((i32 (__thiscall*)(thrown_weapon_interface*))retail::sub_65D100)(this);
}

// slot 26
i32 thrown_weapon_interface::unk_026(i32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(thrown_weapon_interface*, i32, i32, i32))retail::sub_61B510)(this, unk, unk_2, unk_3);
}

// slot 27
i32 thrown_weapon_interface::unk_027(i32 unk) {
    return ((i32 (__thiscall*)(thrown_weapon_interface*, i32))retail::sub_65D110)(this, unk);
}


// sub_66CE80
bool thrown_weapon_interface::spawn_grenade(      arch_base_vhandle  target,
                                            const vector3d          &target_pos,
                                            const vector3d          &dir,
                                                  f32                force,
                                                  bool               use_pos,
                                            const vector3d          &pos) {

    // SM3's random() and random(2.0f) as retail inlines them
    auto random_unit = [] { return (f32)((f64)(f32)std::rand() * (f64)(1.0f / 32767.0f)); };
    auto random_two  = [] { return (f32)((f64)(f32)std::rand() * (f64)(1.0f / 32767.0f) * 2.0); };

    grenade*            last_grenade_spawned = grenade_pool->get_grenade(this);
    physical_interface* grenade_phys_ifc     = last_grenade_spawned->physical_ifc();

    if (weapon_ifc->flags & 0x00800000) {
        po the_po = references::unk_010038a0.get();
        the_po.set_facing(math::vector4(dir.x, dir.y, dir.z, 0.0f));

        retail::sub_626BE0((i32)last_grenade_spawned, (u64*)&the_po); // entity_base::set_rel_po

        const f32 x = random_two();
        const f32 y = random_two();
        const f32 z = random_two();

        grenade_phys_ifc->set_angular_velocity(vector3d((f32)(((f64)z - 1.0) * (f64)3.14159265f),
                                                        (f32)(((f64)y - 1.0) * (f64)3.14159265f),
                                                        (f32)(((f64)x - 1.0) * (f64)3.14159265f)));
        grenade_phys_ifc->set_gravity(false);
    } else if (weapon_ifc->flags & 0x00010000) {
        if (!grenade_phys_ifc->has_guidance_sys())
            grenade_phys_ifc->create_guidance_sys(guidance_system::guidance_rocket);

        auto guide = (rocket_guidance_sys*)grenade_phys_ifc->guidance_sys();

        if (weapon_ifc->flags & 0x00020000) {
            guide->set_guided(unk_234 >= random_unit());
            guide->set_point_guided(false);
        } else if (weapon_ifc->flags & 0x80000000) {
            guide->set_point_guided(unk_234 >= (f32)retail::sub_408AC0());
            guide->set_guided(false);
        }

        guide->set_target(target);
        guide->set_target_pos(target_pos);

        guide->set_guided_accuracy(guided_accuracy);
        guide->set_accel_delay(accel_delay);
        guide->set_guidance_delay(guidance_delay);

        if (guidance_delay <= 0.0f)
            guide->guidance_delay = 0.0f;

        guide->set_wobble_timer(wobble_timer);
        guide->set_accel_delay_var(accel_delay_var);
        guide->set_guidance_delay_var(guidance_delay_var);
        guide->set_wobble_timer_var(wobble_timer_var);
        guide->set_accel_factor(accel_factor);
        guide->set_turn_factor(turn_factor);
        guide->set_thrust_timer(thrust_timer);

        po the_po = references::unk_010038a0.get();
        the_po.set_facing(math::vector4(dir.x, dir.y, dir.z, 0.0f));

        retail::sub_626BE0((i32)last_grenade_spawned, (u64*)&the_po); // entity_base::set_rel_po
    } else {
        const f32 x = random_two();
        const f32 y = random_two();
        const f32 z = random_two();

        grenade_phys_ifc->set_angular_velocity(vector3d((f32)(((f64)z - 1.0) * (f64)3.14159265f),
                                                        (f32)(((f64)y - 1.0) * (f64)3.14159265f),
                                                        (f32)(((f64)x - 1.0) * (f64)3.14159265f)));
        grenade_phys_ifc->set_gravity(true);
    }

    grenade_phys_ifc->set_sticky((weapon_ifc->flags & 0x00080000) != 0);
    grenade_phys_ifc->set_flag(physical_interface_flag_stuck, false);
    grenade_phys_ifc->set_bouncy(true);
    grenade_phys_ifc->sticky_offset = sticky_offset;
    grenade_phys_ifc->set_bounce_factor(bounce_factor);
    grenade_phys_ifc->set_slide_factor(slide_factor);

    if (use_pos) {
        last_grenade_spawned->unk_0f5 = false;

        retail::sub_626CA0((u32*)last_grenade_spawned, (u32*)&pos); // entity_base::set_rel_position
    } else
        retail::sub_626CA0((u32*)last_grenade_spawned, (u32*)&my_entity->my_abs_po->get_position()); // entity_base::set_rel_position

    last_grenade_spawned->invalidate_frame_delta();

    last_grenade_spawned->unk_194 = last_grenade_spawned->my_abs_po->get_position();

    if (entity* visual = last_grenade_spawned->get_visual_entity()) {
        retail::sub_626BE0((i32)visual, (u64*)last_grenade_spawned->my_abs_po); // entity_base::set_rel_po

        visual->set_active(true);
        visual->set_visible(false, false);
        visual->set_visible(true, false);

        f32 fade_distance = unk_268;

        if (fade_distance > 0.0f) {
            i32 preset = retail::sub_77F090(fade_distance); // the fader preset closest to the distance

            visual->ext_flags = preset | (visual->ext_flags & ~0xFu);
            retail::sub_6123B0((u32*)visual, preset, (i32)i_am_not_sure);
        }

        visual->flags |= entity_flag_moving;

        if (visual->has_ifc(entity_ifc_physical))
            visual->physical_ifc()->enable(false);

        visual->set_collisions_active(false, true);
        visual->compute_sector(references::g_world_ptr.read()->the_terrain, false, nullptr);

        if (!unk_2d9)
            visual->flags |= entity_flag_detectable;
        else
            visual->flags &= ~entity_flag_detectable;
    } else if (!unk_2d9)
        last_grenade_spawned->flags |= entity_flag_detectable;
    else
        last_grenade_spawned->flags &= ~entity_flag_detectable;

    // the grenade takes the regions of the thrower, else of the holder
    if (!my_entity->is_in_limbo()) {
        last_grenade_spawned->force_regions(my_entity);
        last_grenade_spawned->unforce_regions();
    } else if (weapon_ifc->holder.resolve()) {
        entity* holder = get_holder();

        if (!holder->is_in_limbo()) {
            last_grenade_spawned->force_regions(get_holder());
            last_grenade_spawned->unforce_regions();
        }
    }

    last_grenade_spawned->set_collisions_active(false, true);
    last_grenade_spawned->flags |= entity_flag_moving;
    last_grenade_spawned->set_active(true);
    last_grenade_spawned->set_visible(true, false);

    last_grenade_spawned->fuse_timer = (f32)(((f64)random_two() - 1.0) * (f64)fuse_time_var + (f64)fuse_time);
    last_grenade_spawned->arm_timer  = arm_time;

    if (unk_2da) {
        // each component at least 0.0001 away from zero
        auto component = [&random_unit] {
            if (random_unit() < 0.5f)
                return (f64)-0.0001f - (f64)random_unit();

            return (f64)random_unit() + (f64)0.0001f;
        };

        const f64 x = component();
        const f64 y = component();
        const f64 z = component();

        last_grenade_spawned->spin_axis = vector3d((f32)z, (f32)y, (f32)x);
    } else
        last_grenade_spawned->spin_axis = spin_axis;

    last_grenade_spawned->spin_axis  = math::unitize(last_grenade_spawned->spin_axis);
    last_grenade_spawned->spin_speed = (f32)(((f64)random_two() - 1.0) * (f64)spin_speed_var + (f64)spin_speed);

    if (!(weapon_ifc->flags & 0x00010000) && last_grenade_spawned->arm_timer > 0.0f)
        last_grenade_spawned->unk_0f4 = true;

    grenade_phys_ifc->suspend(false);
    grenade_phys_ifc->enable();
    grenade_phys_ifc->cancel_all_velocity(references::unk_00ffec30.get(), true);

    retail::sub_640D20((u8*)last_grenade_spawned);

    last_grenade_spawned->compute_sector(references::g_world_ptr.read()->the_terrain, false, nullptr);

    if (grenade_phys_ifc->has_guidance_sys())
        grenade_phys_ifc->guidance_sys()->launch(dir, force);
    else
        grenade_phys_ifc->add_velocity_increment(vector3d((f32)((f64)dir.x * force),
                                                          (f32)((f64)dir.y * force),
                                                          (f32)((f64)dir.z * force)),
                                                 true);

    if (last_grenade_spawned->has_ifc(entity_ifc_sound)) {
        string_hash event;

        event.initialize(mash::ALLOCATED, "stop_grenade_loop");
        retail::sub_65FA00((i32)last_grenade_spawned->my_ifc_storage->get_ifc(entity_ifc_sound), (i32)event.source_hash_code, false); // sound_interface: post event

        event.initialize(mash::ALLOCATED, "start_grenade_loop");
        retail::sub_65FA00((i32)last_grenade_spawned->my_ifc_storage->get_ifc(entity_ifc_sound), (i32)event.source_hash_code, false); // sound_interface: post event

        event.initialize(mash::ALLOCATED, "grenade_throw");
        retail::sub_65FA00((i32)last_grenade_spawned->my_ifc_storage->get_ifc(entity_ifc_sound), (i32)event.source_hash_code, false); // sound_interface: post event
    }

    if (weapon_ifc->flags & 0x00200000)
        last_grenade_spawned->unk_148 = my_entity->add_callback(references::radio_detonate.read(),
                                                                (code_event_callback_function)retail::sub_604C90,
                                                                last_grenade_spawned,
                                                                true);

    {
        engine_lock_scope lock_scope(&jq::references::unk_01003260.get());

        live_grenades->push_back(last_grenade_spawned);
        references::unk_01003320.get().push_back(last_grenade_spawned);
    }

    string_hash null_hash;

    if (weapon_ifc->flags & 0x00080000) {
        null_hash.initialize(mash::ALLOCATED);

        if (unk_220 != null_hash)
            last_grenade_spawned->unk_1b8 = last_grenade_spawned->spawn_looping_effect(unk_220, 0);
    }

    null_hash.initialize(mash::ALLOCATED);

    if (unk_21c != null_hash)
        last_grenade_spawned->unk_1bc = last_grenade_spawned->spawn_looping_effect(unk_21c, 0);

    null_hash.initialize(mash::ALLOCATED);

    if (unk_224 != null_hash)
        my_entity->spawn_effect(unk_224, 0);

    entity* script_ent = last_grenade_spawned->get_visual_entity() ?
        last_grenade_spawned->get_visual_entity() : last_grenade_spawned;

    retail::sub_668BF0((i32)unk_0e8, // cached_special_effect::spawn
                       false,
                       (i32*)&last_grenade_spawned->my_abs_po->get_position(),
                       (i32)&last_grenade_spawned->my_abs_po->get_z_facing(),
                       (i32*)my_entity,
                       (i32)script_ent,
                       (f32**)my_entity,
                       (i32*)&unk_1f4,
                       true,
                       !(weapon_ifc->flags & 0x00000002),
                       (i32)my_entity,
                       -1.0f);

    arch_base_vhandle script_ent_handle = (last_grenade_spawned->get_visual_entity() ?
        last_grenade_spawned->get_visual_entity() : last_grenade_spawned)->my_handle;

    vector3d launch_velocity((f32)((f64)dir.x * force),
                             (f32)((f64)dir.y * force),
                             (f32)((f64)dir.z * force));

    script_arg velocity_arg;
    velocity_arg.type  = 2;
    velocity_arg.value = &launch_velocity;

    script_arg ent_arg;
    ent_arg.type  = 3;
    ent_arg.value = &script_ent_handle;

    script_arg none[6];

    return retail::sub_87CFC0(&last_grenade_spawned->script_obj_instance, // script_object_instance: call a registered function
                              references::unk_010039b8.read().source_hash_code,
                              (u32*)&velocity_arg,
                              (u32*)&ent_arg,
                              (u32*)&none[0],
                              (u32*)&none[1],
                              (u32*)&none[2],
                              (u32*)&none[3],
                              (u32*)&none[4],
                              (u32*)&none[5]);
}

// sub_66CDA0
grenade* grenade_cache::get_grenade(thrown_weapon_interface* thrown) {
    engine_lock_scope lock_scope(&lock);

    grenade* gren = nullptr;

    for (arch_base_vhandle* it = grenades.begin(); it != grenades.end();) {
        auto candidate = (grenade*)it->resolve();

        if (!candidate) {
            grenades.erase(it);

            continue;
        }

        if (candidate->unk_0f0) {
            gren = candidate;

            break;
        }

        ++it;
    }

    if (!gren)
        gren = (grenade*)retail::sub_66CA00((u32*)this, (i32*)thrown); // grenade_cache::push_new_grenade

    // slot 160; every grenade carries the one grenade vtable
    gren->prepare_for_launch(thrown);

    return gren;
}
