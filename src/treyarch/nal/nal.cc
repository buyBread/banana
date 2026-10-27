#include "retail.hh"
#include "treyarch/nal/blend.hh"
#include "treyarch/nal/nal.hh"
#include "treyarch/nal/transport_fk.hh"
#include "treyarch/math/matrix_diag.hh"
#include "treyarch/base/tl_system.hh"
#include "treyarch/base/tl_defs.hh"

using namespace treyarch;

// sub_9D1800
void nal::init() {
    tl_stack_range_init();

    auto null_callback = (apk::file_callback)retail::sub_5B4A70;

    apk::register_file_type(transport_list_type,
                            transport_list_version,
                            null_callback,
                            null_callback);

    apk::register_file_type(anim_type,
                            anim_version,
                            load_anim,
                            remove_anim);

    apk::register_file_type(scene_anim_type,
                            scene_anim_version,
                            load_scene_anim,
                            null_callback);
}

// sub_9CD550
nal::transport_list* nal::get_skeleton(fixed_string* name) {
    return (transport_list*)apk::resolve_resource(name, transport_list_type);
}

// sub_9CD570
nal::anim* nal::get_anim(fixed_string* name) {
    return (anim*)apk::resolve_resource(name, anim_type);
}

// sub_9CD590
nal::scene_anim* nal::get_scene_anim(fixed_string* name) {
    return (scene_anim*)apk::resolve_resource(name, scene_anim_type);
}

// sub_9D1790
void nal::load_anim(apk::file*       owner,
                    apk::file_entry* entry,
                    void**           mapped_sections,
                    void*            user_data) {

    (void)owner;
    (void)entry;
    (void)user_data;

    if (references::skip_linking.read())
        return;

    for (anim* value = (anim*)mapped_sections[0]; value; value = value->next) {
        value->flags     |= anim_flag_unk_04;
        value->ref_count  = 1;

        value->link_transport_list();
    }
}

// sub_9CFAF0
void nal::remove_anim(apk::file*       owner,
                      apk::file_entry* entry,
                      void**           mapped_sections,
                      void*            user_data) {

    (void)owner;
    (void)entry;
    (void)user_data;

    // each anim's release check is gone, only the walk is left
    for (anim* value = (anim*)mapped_sections[0]; value; value = value->next) {}
}

// sub_9D1770
void nal::load_scene_anim(apk::file*       owner,
                          apk::file_entry* entry,
                          void**           mapped_sections,
                          void*            user_data) {

    (void)owner;
    (void)entry;
    (void)user_data;

    if (references::skip_linking.read())
        return;

    ((scene_anim*)mapped_sections[0])->link_transport_lists();
}

// sub_9D2030
void nal::nal_jq_callback(jq::batch* batch) {
    auto input = (nal_jq_batch_input_t*)batch->payload[0];

    input->combined_pose->present.set_all_zeroes();

    if (input->pose_count) {
        if (input->instances[0])
            input->instances[0]->get_pose(input->t[0], input->t_prev[0], input->poses[0], (transport_fk*)input->transport_list->get_transport(0));

        input->combined_pose->copy(*input->poses[0]);

        u8 stack_memory_a[0x2000];

        // each further layer blends over the ones before it
        for (u32 i = 1; i < input->pose_count; i++) {
            if (input->instances[i])
                input->instances[i]->get_pose(input->t[i], input->t_prev[i], input->poses[i], (transport_fk*)input->transport_list->get_transport(0));

            nal::pose combined(input->combined_pose->pose_layout, tl_align(stack_memory_a, 16));

            combined.copy(*input->combined_pose);

            nal::blend(input->combined_pose, input->alphas[i], &combined, input->poses[i]);
        }
    } else
        input->combined_pose->copy(input->combined_pose->pose_layout->default_pose);

    if (input->pose_count && input->combined_pose->is_field_present(pose_field_fk_trajectory, 0))
        *input->trajectory = *(po*)input->combined_pose->get_field_data_ptr(pose_field_fk_trajectory, 0);
    else
        *input->trajectory = *(const po*)&identity_po;

    if (!input->skip_matrices) {
        auto fk = (transport_fk*)input->transport_list->get_transport(0);

        for (u32 i = 0; i < fk->bone_count; i++)
            input->result_matrices[i] = math::identity_mat33();

        if (input->pose_count)
            fk->get_bone_matrices(input->combined_pose, input->result_matrices);
    }
}
