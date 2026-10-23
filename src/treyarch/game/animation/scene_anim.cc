#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/animation/scene_anim.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/input/input_mgr.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/shared/math/references.hh"

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
