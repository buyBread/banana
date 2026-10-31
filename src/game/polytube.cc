#include "retail.hh"
#include "game/math/references.hh"
#include "game/polytube.hh"
#include "game/wds/ai/ai_tentacle_info.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_6030E0
void polytube::remove_from_list() {
    engine_lock_scope lock(&references::polytube_lock.get());

    if (prev_polytube)
        prev_polytube->next_polytube = next_polytube;
    else if (references::inactive_polytubes.read() == this)
        references::inactive_polytubes.write(next_polytube);
    else if (references::active_polytubes.read() == this)
        references::active_polytubes.write(next_polytube);

    if (next_polytube)
        next_polytube->prev_polytube = prev_polytube;

    next_polytube = nullptr;
    prev_polytube = nullptr;
}

// sub_62A600
bool polytube::has_active_anim() const {
    if (!pt_anims.begin() || !pt_anims.size())
        return false;

    for (const polytube_pt_anim* anim = pt_anims.begin(); anim != pt_anims.end(); ++anim) {
        if (anim->flags & 1)
            return true;
    }

    return false;
}

// sub_639D20
void polytube::v_animate() { // SM3's change_list_status: anything still moving goes on the active list
    engine_lock_scope lock(&references::polytube_lock.get());

    remove_from_list();

    bool active = tentacle_info != nullptr;

    // a NaN v_anim counts as moving
    if (!active && is_visible())
        active = v_anim != 0.0f || has_active_anim();

    if (!active)
        active = !tracked_control_pts.empty();

    if (!active)
        active = widths_active;

    if (active) {
        next_polytube = references::active_polytubes.read();
        references::active_polytubes.write(this);
    } else {
        next_polytube = references::inactive_polytubes.read();
        references::inactive_polytubes.write(this);
    }

    if (next_polytube)
        next_polytube->prev_polytube = this;
}

// sub_661B20
void polytube::add_control_pt(const vector3d &point) { // in the polytube's own frame
    the_spline.add_control_pt(point);

    const i32 anims = (i32)pt_anims.size();

    // animated tubes get an idle animation for every new point
    if (anims) {
        const i32 points = (i32)the_spline.control_pts.size();

        if (anims < points) {
            for (i32 left = points - anims; left; --left) {
                polytube_pt_anim idle;

                idle.flags    = 0;
                idle.base     = references::unk_00ffec30.get();
                idle.delta    = references::unk_00ffec30.get();
                idle.dist     = 0.0f;
                idle.duration = 0.0f;
                idle.time     = 0.0f;
                idle.speed    = 0.0f;

                pt_anims.push_back(idle);
            }
        }
    }

    if (tentacle_info && !external_tentacle_info) {
        destroy_tentacle_info();
        create_tentacle_info();
    }
}

// sub_63A590
void polytube::create_tentacle_info() {
    if (external_tentacle_info || tentacle_info)
        return;

    void* memory = mm::allocate(sizeof(ai_tentacle_info));

    tentacle_info = memory ? (ai_tentacle_info*)retail::sub_4BE500((i32)memory) : nullptr; // ai_tentacle_info::ai_tentacle_info

    tentacle_info->create_tentacle(this);

    v_animate();
}

// sub_64B5C0
// inlined @ sub_64B600
void polytube::destroy_tentacle_info() {
    if (external_tentacle_info || !tentacle_info)
        return;

    delete tentacle_info;
    tentacle_info = nullptr;

    v_animate();
}

// sub_64B600
void polytube::set_external_tentacle_info(ai_tentacle_info* inf) { // the tube's own tentacle goes first
    if (inf) {
        destroy_tentacle_info();

        tentacle_info          = inf;
        external_tentacle_info = true;
    } else {
        tentacle_info          = nullptr;
        external_tentacle_info = false;
    }
}
