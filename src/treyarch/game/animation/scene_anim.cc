#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/animation/animation_controller.hh"
#include "treyarch/game/animation/scene_anim.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/input/input_mgr.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/nal/transport_fk.hh"
#include "treyarch/shared/math/po.hh"
#include "treyarch/shared/math/references.hh"
#include "treyarch/shared/math/vmath/matrix_rot.hh"
#include "treyarch/shared/math/vmath/quaternion.hh"

using namespace treyarch;

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

                ((void (__thiscall*)(entity*, bool, bool))ent->vtable[69])(ent, info.was_hidden, true); // slot 69

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

        if (ctrl->state->unk_02c[0].size())
            include_start = ctrl->state->unk_02c[0].back()->unk_06c;

        retail::sub_5B9BF0((i32)info.instance->anim, t, t_prev, (i32)ent, include_start, looping); // raise_animation_signals_for_time_span

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
