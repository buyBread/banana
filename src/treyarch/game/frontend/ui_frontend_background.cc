#include "treyarch/game/frontend/igo/igo_3d_camera_widget.hh"
#include "treyarch/game/frontend/igo/igo_color_channel.hh"
#include "treyarch/game/frontend/ui_frontend.hh"
#include "treyarch/game/post_process/post_process.hh"
#include "treyarch/ngl/display.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/shared/math/math.hh"

using namespace treyarch;

// sub_698F40
void ui_frontend::render_background_effects() {
    if (!background_effect_mode)
        return;

    ngl::list_begin_scene(ngl::scene_parameter_defaults);
    ngl::set_scene_name("UIFrontEnd::RenderBackgroundEffects");
    ngl::set_clear_flags(0);

    const vector4 rows[3] { vector4(1.0f, 0.0f, 0.0f, 0.0f),
                            vector4(0.0f, 1.0f, 0.0f, 0.0f),
                            vector4(0.0f, 0.0f, 1.0f, 0.0f) };

    matrix4x4 view;
    view.assign_rows(rows);
    ngl::set_world_to_view_matrix(&view);

    switch (background_effect_mode) {
        case 1:
            draw_background_overlay(1.0f, 0);
            break;

        case 2:
        case 3:
            draw_background_overlay(background_effect_strength, 0);
            break;

        case 4:
            references::unk_0102fe9c.write(1);
            break;

        case 7:
            draw_background_texture(1.0f);
            break;

        case 8:
        case 9:
            draw_background_texture(background_effect_strength);
            break;

        case 10:
            post_process::render_zoom_map_effect();
            break;

        case 11:
            draw_background_channels();
            break;

        default:
            break;
    }

    ngl::list_end_scene();
}

// sub_688F90
void ui_frontend::draw_background_overlay(f32 strength, u32) {
    ngl::set_z_test_enable(false);
    ngl::set_z_write_enable(false);

    ngl::quad overlay;
    ngl::init_quad(&overlay);

    f32 height = (f32)(i32)ngl::get_screen_height();
    f32 width  = (f32)(i32)ngl::get_screen_width();

    ngl::set_quad_rect(&overlay, 0.0f, 0.0f, width, height);
    ngl::set_quad_blend_mode(&overlay, 0x06C10000, 5);

    ngl::set_quad_color(&overlay, (u32)(i64)(strength * 128.0f) << 24);
    ngl::list_add_quad(&overlay);

    post_process::render_pause_menu_blur((f32)((f64)strength * 2.5 + 0.5));
}

// sub_689070
void ui_frontend::draw_background_texture(f32 strength) {
    if (!background_texture)
        return;

    f32 u = 0.0f;
    f32 v = 0.0f;

    if (camera_widget) {
        matrix4x4 camera = camera_widget->camera_matrix;

        u = (1.5707964f - math::arcsin(float_dot(references::background_texture_u_axis.get(), camera.x_row()))) / 6.2831855f;
        v = (1.5707964f - math::arcsin(float_dot(references::background_texture_v_axis.get(), camera.y_row()))) / 6.2831855f + 0.5f;
    }

    ngl::set_z_test_enable(false);
    ngl::set_z_write_enable(false);

    ngl::quad overlay;
    ngl::init_quad(&overlay);

    f32 height = (f32)(i32)ngl::get_screen_height();
    f32 width  = (f32)(i32)ngl::get_screen_width();

    ngl::set_quad_rect(&overlay, 0.0f, 0.0f, width, height);
    ngl::set_quad_map_flags(&overlay, 1);
    ngl::set_quad_blend_mode(&overlay, 0x06C10000, 5);
    ngl::set_quad_texture(&overlay, background_texture);
    ngl::set_quad_uv(&overlay, u, v, (f32)((f64)u + (f64)0.2f), (f32)((f64)v + (f64)0.2f));

    ngl::set_quad_color(&overlay, ((u32)(i64)(strength * 255.0f) << 24) | 0x00FFFFFF);
    ngl::list_add_quad(&overlay);

    post_process::render_pause_menu_blur((f32)((f64)strength * 2.5 + 0.5));
}
