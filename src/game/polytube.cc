#include "retail.hh"
#include "game/math/references.hh"
#include "game/polytube.hh"

using namespace treyarch;

// sub_491C90
ai_tentacle_info* ai_tentacle_info::find_tentacle(const string_hash &search_id, ai_tentacle_info** array, i32 size) {
    for (i32 i = 0; i < size; ++i) {
        if (array[i]->id == search_id)
            return array[i];
    }

    return nullptr;
}

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
        retail::sub_64B5C0((i32)this); // destroy_tentacle
        retail::sub_63A590((i32)this); // create_tentacle
    }
}
