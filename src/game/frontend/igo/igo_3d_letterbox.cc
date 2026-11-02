#include "retail.hh"
#include "game/frontend/igo/igo_3d_letterbox.hh"
#include "treyarch/ngl/quad/quad.hh"

using namespace treyarch;

// sub_694940
void igo_3d_letterbox::letterbox(f32 time, f32 height) {
    total_time     = time;
    desired_height = height;
    elapsed        = 0.0f;

    if ((f64)height > (f64)0.0001f)
        set_shown(true);

    if ((f64)time < (f64)0.0001f) {
        if ((f64)desired_height < (f64)0.0001f) {
            alpha      = 0.0f;
            cur_height = 0.0f;

            set_shown(false);
        } else {
            alpha      = 1.0f;
            cur_height = desired_height;
        }
    }
}

// slot 1
void igo_3d_letterbox::init() {
    ((void (__thiscall*)(igo_3d_letterbox*))retail::sub_694920)(this);
}

// sub_6AFA80
void igo_3d_letterbox::update(f32 time_inc) {
    if (!is_shown() || !(total_time > 0.0f))
        return;

    elapsed = (f32)((f64)elapsed + (f64)time_inc);

    if (total_time > elapsed) {
        f64 ratio = (f64)elapsed / (f64)total_time;

        if ((f64)desired_height > (f64)0.0001f) {
            f32 progress = (f32)ratio;
            f32 height   = (f32)((f64)desired_height * (f64)progress);

            height     = desired_height > height ? height : desired_height;
            cur_height = height > 0.0f ? height : 0.0f;

            f32 shade = 1.0f > progress ? progress : 1.0f;

            alpha = shade > 0.0f ? shade : 0.0f;
        } else {
            f32 left   = (f32)(1.0 - ratio);
            f32 height = (f32)((f64)cur_height * (f64)left);

            cur_height = height > 0.0f ? height : 0.0f;

            f32 shade = 1.0f > left ? left : 1.0f;

            alpha = shade > 0.0f ? shade : 0.0f;
        }

        return;
    }

    elapsed    = 0.0f;
    total_time = 0.0f;

    if ((f64)0.0001f > (f64)desired_height) {
        alpha      = 0.0f;
        cur_height = 0.0f;

        set_shown(false);
    } else {
        alpha      = 1.0f;
        cur_height = desired_height;
    }
}

// sub_6949F0
void igo_3d_letterbox::render() {
    if (!is_shown())
        return;

    ngl::quad bar;

    ngl::init_quad(&bar);
    ngl::set_quad_rect(&bar, 0.0f, 0.0f, 640.0f, cur_height);
    ngl::set_quad_color(&bar, (u32)(i64)(f32)((f64)alpha * 255.0) << 24);
    ngl::set_quad_z(&bar, 201.0f);
    ngl::list_add_quad(&bar);

    ngl::init_quad(&bar);
    ngl::set_quad_rect(&bar, 0.0f, (f32)(480.0 - (f64)cur_height), 640.0f, 480.0f);
    ngl::set_quad_color(&bar, (u32)(i64)(f32)((f64)alpha * 255.0) << 24);
    ngl::set_quad_z(&bar, 201.0f);
    ngl::list_add_quad(&bar);
}
