#include <cmath>
#include <cstdio>

#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/animation/animation_controller.hh"
#include "treyarch/game/animation/scene_anim.hh"
#include "treyarch/game/cutscene/cut_scene_player.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/input/input_mgr.hh"
#include "treyarch/game/light/light_source.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/nal/nal.hh"
#include "treyarch/nal/transport_fk.hh"
#include "treyarch/shared/filespec.hh"
#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/math/po.hh"
#include "treyarch/shared/math/references.hh"
#include "treyarch/shared/math/vmath/matrix_rot.hh"
#include "treyarch/shared/math/vmath/quaternion.hh"

using namespace treyarch;

// sub_97A6D0
scene_anim::scene_anim() : scene_anim_inst(nullptr),
                           finished(false),
                           dead(false),
                           paused(false) {}

// sub_5F5F30
scene_anim::~scene_anim() {
    references::g_world_ptr.read()->remove_scene_anim(my_name);

    for (scene_anim_light &light : lights) {
        if (light.light)
            ((void (__thiscall*)(entity*, u32))light.light->vtable[0])(light.light, 1);
    }

    lights.clear();

    for (scene_anims_info &info : my_anim_info_list) {
        entity* ent = (entity*)info.ent.resolve();

        if (!ent)
            continue;

        if (ent->is_an_actor()) {
            ((void (__thiscall*)(entity*))ent->vtable[157])(ent);          // slot 157
            ((void (__thiscall*)(entity*, f32))ent->vtable[139])(ent, 1.0f); // slot 139
            ((void (__thiscall*)(entity*))ent->vtable[141])(ent);          // unbind_from_scene_anim

            ent->ext_flags &= ~0x40000;
            input_mgr::inst()->unk_80c = 0;

            if (ent->has_ifc(entity_ifc_physical)) {
                ent->physical_ifc()->cancel_all_velocity(references::unk_00fd4370.get());

                ((void (__thiscall*)(entity*, bool, bool))ent->vtable[69])(ent, info.status & 1, true); // slot 69

                retail::sub_6299F0((u32*)ent);
            }

            if (ent->has_ifc(entity_ifc_standing))
                retail::sub_659160((i32)ent->my_ifc_storage->get_ifc(entity_ifc_standing), 0);

            if (((actor*)ent)->get_ai_core() && !retail::sub_601C50((u32*)ent))
                retail::sub_4DF4B0((i32*)((actor*)ent)->get_ai_core(), 0); // ai_core::pop_ai_disable

            info.ent = arch_base_vhandle();

            terrain* the_terrain = references::g_world_ptr.read()->the_terrain;

            ((void (__thiscall*)(entity*, terrain*, i32, i32))ent->vtable[89])(ent, the_terrain, 0, 0); // slot 89
        }

        if (ent == references::g_world_ptr.read()->camera_mgr.marky_camera) {
            app::inst()->get_game()->enable_marky_cam(false, true, -1000.0f, 0.0f);

            references::g_world_ptr.read()->camera_mgr.marky_camera->flags &= ~0x400000;

            if (references::g_world_ptr.read()->chase_cam_ptr) {
                retail::sub_626CA0((u32*)references::g_world_ptr.read()->chase_cam_ptr,
                                   (u32*)&references::g_world_ptr.read()->camera_mgr.marky_camera->my_rel_po->get_position());

                camera* chase_cam = references::g_world_ptr.read()->chase_cam_ptr;

                ((void (__thiscall*)(camera*, i32))chase_cam->vtable[165])(chase_cam, 2); // slot 165
            }
        }

        ent->raise_event(scene_anim_finished);
        ent->raise_event(scene_anim_ending);
    }

    if (scene_anim_inst) {
        scene_anim_inst->destroy();

        delete scene_anim_inst;
    }

    scene_anim_inst = nullptr;
}

// sub_5ADB90
void scene_anim::load(const mash::string &filename) {
    my_name = filename;
}

// sub_5F7D80
void scene_anim::play(scene_anim_handle_t handle, u16 flags, f32 offset) {
    filespec spec(my_name);

    fixed_string scene_anim_name;

    scene_anim_name.hash = string_hash(hash::djb2(spec.name.c_str()));
    scene_anim_name.set_text(spec.name.c_str());

    the_scene_anim = nal::get_scene_anim(&scene_anim_name);

    if (scene_anim_name.text)
        memory::heap::free(scene_anim_name.text);

    // the assert that showed it is gone
    if (!the_scene_anim) {
        char message[256];

        std::snprintf(message, 0xFF, "Couldn't find scene animation: %s", spec.name.c_str());
    }

    my_anim_info_list.clear();

    the_handle      = handle;
    scene_anim_inst = the_scene_anim->create_instance();

    i32 index = 0;

    for (nal::anim_instance* participant = scene_anim_inst->instances; participant; participant = participant->next, index++)
        find_actor_and_create_controller_for_subanim(index, tl_fixed_string(participant->anim->name.get_text()), participant);

    my_flags = flags;

    frame_advance(offset);
}

// sub_5F74C0
void scene_anim::find_actor_and_create_controller_for_subanim(      i32                 index,
                                                              const tl_fixed_string    &name,
                                                                    nal::anim_instance* instance) {

    nal::transport_list* skeleton = instance->anim->transport_list;

    auto fk = (nal::transport_fk*)skeleton->get_transport(0);

    nal::pose pose(skeleton->pose_layout, nullptr);

    instance->get_pose(0.0f, 0.0f, &pose, fk);

    if (pose.is_field_present(nal::pose_field_omnilight_data, 0)) {
        f32* data  = pose.get_field_data_ptr(nal::pose_field_omnilight_data, 0);
        f64  scale = (f64)(1.0f / 255.0f);

        light_properties properties(light_flavor_point,
                                    color { (f32)(data[0] * scale), (f32)(data[1] * scale), (f32)(data[2] * scale), 1.0f },
                                    color { 1.0f, 1.0f, 1.0f, 1.0f },
                                    0.0f,
                                    data[3]);

        mash::string light_name(name.text);

        light_name = "SCENE_ANIM_" + light_name;

        string_hash id;
        id.initialize(mash::ALLOCATED, light_name.c_str());

        light_source* light = new light_source(properties, nullptr, id);

        lights.push_back({ (entity*)light, instance, index });
    } else if (pose.is_field_present(nal::pose_field_spotlight_data, 0)) {
        f32* data  = pose.get_field_data_ptr(nal::pose_field_spotlight_data, 0);
        f64  scale = (f64)(1.0f / 255.0f);

        light_properties properties(light_flavor_spot,
                                    color { (f32)(data[0] * scale), (f32)(data[1] * scale), (f32)(data[2] * scale), 1.0f },
                                    color { 1.0f, 1.0f, 1.0f, 1.0f },
                                    0.0f,
                                    data[3]);

        properties.inner_angle = data[7];
        properties.outer_angle = data[8];

        mash::string light_name(name.text);

        light_name = "SCENE_ANIM_" + light_name;

        string_hash id;
        id.initialize(mash::ALLOCATED, light_name.c_str());

        light_source* light = new light_source(properties, nullptr, id);

        po       light_po  = references::po_identity_matrix.get();
        vector4d direction(data[4], data[5], data[6], 0.0f);

        retail::sub_44DE20((f32*)&light_po, (f32*)&direction);
        retail::sub_626BE0((i32)light, (u64*)&light_po); // entity_base::set_rel_po

        lights.push_back({ (entity*)light, instance, index });
    } else {
        actor* ent;

        if (name == tl_fixed_string("CAMERA")) {
            ent = references::g_world_ptr.read()->camera_mgr.marky_camera;

            app::inst()->get_game()->enable_marky_cam(true, true, -1000.0f, 0.0f);

            ent->flags |= 0x400000;
        } else {
            string_hash id;
            id.initialize(mash::ALLOCATED, name.text);

            ent = (actor*)retail::sub_7DF6E0((u32*)&id);

            if (!ent || !ent->is_an_actor())
                return;
        }

        if (!ent)
            return;

        instance->skeleton_map = ent->skeleton_map;

        bool was_hidden = (ent->flags & 0x4000) != 0;

        if (ent->get_ai_core() && !retail::sub_601C50((u32*)ent)) {
            retail::sub_4C7A40((i32)ent->get_ai_core());
            retail::sub_4DF470((i32*)ent->get_ai_core(), 0); // ai_core::push_ai_disable
        }

        if (ent->has_ifc(entity_ifc_physical)) {
            ent->physical_ifc()->cancel_all_velocity(references::unk_00fd4370.get());

            ((void (__thiscall*)(entity*, bool, bool))ent->vtable[69])(ent, false, true); // slot 69
        }

        if (ent->has_ifc(entity_ifc_standing))
            retail::sub_659160((i32)ent->my_ifc_storage->get_ifc(entity_ifc_standing), 0);

        ((void (__thiscall*)(entity*))ent->vtable[157])(ent); // slot 157

        scene_anims_info info;

        info.status = was_hidden;

        bool driven = true;

        if (references::cut_scene_player.read()->hero_unaffected()) {
            driven = false;

            ent->ext_flags &= ~0x40000;
        } else
            ent->ext_flags |= 0x40000;

        input_mgr::inst()->unk_80c = driven;

        // an entry whose entity was released is reused
        scene_anims_info* slot = my_anim_info_list.begin();

        while (slot != my_anim_info_list.end() && !slot->ent.null())
            ++slot;

        info.ent = ent->my_handle;

        // inlined normalize; a zero vector falls back to a constant
        const vector3d &up = ent->my_abs_po->get_y_facing();

        f32 length2 = (f32)((f64)up.x * up.x + (f64)up.y * up.y + (f64)up.z * up.z);

        vector4d unit_up;

        if (length2 > 0.0f) {
            f32 inverse = (f32)(1.0 / (f64)(f32)std::sqrt((f64)length2));

            unit_up = vector4d((f32)((f64)inverse * up.x),
                               (f32)((f64)inverse * up.y),
                               (f32)((f64)inverse * up.z),
                               (f32)((f64)inverse * 0.0));
        } else
            unit_up = references::unk_00fd4340.get();

        info.entity_up_vec = vector3d(unit_up.x, unit_up.y, unit_up.z);
        info.handle        = the_handle;
        info.name          = name.text;
        info.instance      = instance;

        ((void (__thiscall*)(entity*, nal::scene_anim_instance*, i32))ent->vtable[140])(ent, scene_anim_inst, index); // bind_to_scene_anim

        if (slot == my_anim_info_list.end())
            my_anim_info_list.push_back(info);
        else
            *slot = info;
    }
}

// sub_5E88B0
void scene_anim::finish() {
    frame_advance(scene_anim_inst->instances->anim->duration);

    finished = true;
}

// sub_5E00B0
void scene_anim::frame_advance(f32 delta_t) {
    if (paused || !scene_anim_inst)
        return;

    f32 t_prev = scene_anim_inst->t;

    scene_anim_inst->t_prev = t_prev;
    scene_anim_inst->t      = (f32)((f64)delta_t / (f64)scene_anim_inst->instances->anim->duration + (f64)t_prev);

    f32 t = scene_anim_inst->t;

    for (scene_anim_light &light : lights) {
        entity*              light_entity = light.light;
        nal::transport_list* skeleton     = light.instance->anim->transport_list;

        auto fk = (nal::transport_fk*)skeleton->get_transport(0);

        nal::pose pose(skeleton->pose_layout, nullptr);

        light.instance->get_pose(scene_anim_inst->t, scene_anim_inst->t_prev, &pose, fk);

        po light_po;

        retail::sub_4015B0((u64*)&light_po);

        math::mat43 matrices[nal::max_bones];

        for (u32 i = 0; i < fk->bone_count; i++)
            matrices[i] = math::identity_mat33();

        fk->get_bone_matrices(&pose, matrices);

        const nal::po &origin = the_scene_anim->origins[light.index];

        math::quaternion rotation = *(const math::quaternion*)&origin.get_m33().get_quaternion();
        vector3d         position = *(const vector3d*)&origin.get_w();

        math::rot_mat33 scratch;

        auto rows = (math::rot_mat33*)retail::sub_48D800((u32*)&scratch, (f32*)&rotation);

        matrix4x4 basis;

        retail::sub_408B10((u64*)&basis, (i32)rows);

        light_po.get_matrix().x = basis.x;
        light_po.get_matrix().y = basis.y;
        light_po.get_matrix().z = basis.z;
        light_po.get_matrix().w = vector4d(position.x, position.y, position.z, 1.0f);

        retail::sub_44E230((u64*)&light_po, (f32*)&matrices[fk->trajectory_bone], (f32*)&light_po);
        retail::sub_626BE0((i32)light_entity, (u64*)&light_po); // entity_base::set_rel_po
    }

    for (u32 i = 0; i < my_anim_info_list.size(); i++) {
        scene_anims_info &info = my_anim_info_list[i];

        actor* ent = (actor*)info.ent.resolve();

        if (!ent)
            continue;

        animation_controller* ctrl          = ent->anim_ctrl;
        bool                  looping       = info.instance->force_looping || (info.instance->anim->flags & nal::anim_flag_looping);
        bool                  include_start = false;

        if (ctrl->state->layer_anims[0].size())
            include_start = ctrl->state->layer_anims[0].back()->just_started;

        raise_animation_signals_for_time_span(info.instance->anim, t, t_prev, ent, include_start, looping);

        nal::transport_list* skeleton = ent->m_skeleton ? ent->m_skeleton : info.instance->anim->transport_list;

        auto fk = (nal::transport_fk*)skeleton->get_transport(0);

        nal::pose pose(skeleton->pose_layout, nullptr);

        info.instance->get_pose(scene_anim_inst->t, scene_anim_inst->t_prev, &pose, fk);

        math::mat43 matrices[nal::max_bones];

        for (u32 j = 0; j < fk->bone_count; j++)
            matrices[j] = math::identity_mat33();

        fk->get_bone_matrices(&pose, matrices);

        // only for the call below; the pointer is left behind when this returns
        ctrl->state->pose_input.result_matrices = matrices;
        ctrl->state->combined_pose.copy(pose);

        retail::sub_796C70((i32)info.ent.value);

        ctrl->get_matrix_data_from_pose(true);
    }
}
