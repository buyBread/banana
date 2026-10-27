#include "treyarch/nal/anim.hh"
#include "treyarch/nal/anim_instance.hh"
#include "treyarch/nal/nal.hh"
#include "treyarch/base/memory.hh"

using namespace treyarch;

// inlined @ sub_9D1790
void nal::anim::link_transport_list() {
    if (transport_list)
        return;

    transport_list = get_skeleton(&transport_list_name);

    if (!transport_list)
        memory::report("NAL2 error: TransportList not found in the directory. Please report to animation programmer.");
}

// sub_9CD4A0
nal::transport_data* nal::anim::get_transport_data(u32 id) {
    for (nal::transport_data* value = transport_data; value; value = value->next) {
        if (value->id == id)
            return value;
    }

    return nullptr;
}

// sub_9CD4C0
bool nal::anim::has_transport_data(u32 id) {
    for (nal::transport_data* value = transport_data; value; value = value->next) {
        if (value->id == id)
            return true;
    }

    return false;
}

// sub_9CD2C0
void nal::time_to_frame_pair(nal::anim* anim,
                             f32        t,
                             f32*       frame,
                             i32*       frame_0,
                             i32*       frame_1,
                             bool       looping) {

    // a looping anim wraps to its first frame, the others stop at their last
    if ((anim->flags & anim_flag_looping) || looping) {
        *frame   = (f32)(i32)anim->frame_count * t;
        *frame_0 = (i32)*frame;

        if (*frame_0 >= (i32)anim->frame_count)
            *frame_0 = 0;

        *frame_1 = *frame_0 + 1;

        if (*frame_1 >= (i32)anim->frame_count)
            *frame_1 = 0;
    } else {
        *frame   = (f32)(i32)(anim->frame_count - 1) * t;
        *frame_0 = (i32)*frame;

        if (*frame_0 >= (i32)anim->frame_count)
            *frame_0 = anim->frame_count - 1;

        *frame_1 = *frame_0 + 1;

        if (*frame_1 >= (i32)anim->frame_count)
            *frame_1 = anim->frame_count - 1;
    }
}

// sub_9CEBB0
void nal::time_to_frames(nal::anim* anim,
                         f32        t_prev,
                         f32        t,
                         f32*       frame,
                         i32*       frame_0,
                         i32*       frame_1,
                         f32*       fraction,
                         i32*       wraps,
                         bool       looping) {

    *wraps = 0;

    if ((anim->flags & anim_flag_looping) || looping) {
        i32 whole;

        // only the fraction is kept; wraps counts the passes since t_prev
        if (t >= 0.0f) {
            whole = (i32)t;
            t     = t - (f32)whole;
        } else {
            whole = 1 - (i32)t;
            t     = (f32)whole + t;
        }

        i32 whole_prev = t_prev >= 0.0f ? (i32)t_prev : 1 - (i32)t_prev;

        *wraps = whole - whole_prev;
    } else {
        f32 clamped = 1.0f;

        if (t < 1.0f)
            clamped = t;

        if (clamped < 0.0f)
            clamped = 0.0f;

        t = clamped;
    }

    time_to_frame_pair(anim, t, frame, frame_0, frame_1, looping);

    *fraction = *frame - (f32)*frame_0;
}

// sub_5AB1C0
nal::anim_instance* nal::anim::create_instance(u32 instance_flags, nal::skeleton_map* skel_map, nal::transport_list* transports) {
    if (!transport_list)
        transport_list = transports;

    return new nal::anim_instance(this, instance_flags, skel_map);
}
