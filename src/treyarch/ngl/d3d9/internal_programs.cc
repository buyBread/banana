#include "treyarch/ngl/d3d9/internal_programs.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"

using namespace treyarch;
using namespace ngl::shaders;

static ngl::d3d9::vertex_program quad_pc_vertex_program;
static ngl::d3d9::vertex_program screen_pc_vertex_program;
static ngl::d3d9::vertex_program framebuffer_copy_vertex_program;
static ngl::d3d9::vertex_program screen_pcuv_vertex_program;
static ngl::d3d9::vertex_program screen_pcuv4_vertex_program;
static ngl::d3d9::vertex_program screen_puv_material_vertex_program;
static ngl::d3d9::vertex_program font_vertex_program;

static ngl::d3d9::pixel_program vertex_color_pixel_program;
static ngl::d3d9::pixel_program texture_modulate_pixel_program;
static ngl::d3d9::pixel_program framebuffer_copy_pixel_program;
static ngl::d3d9::pixel_program depth_copy_pixel_program;
static ngl::d3d9::pixel_program rawz_depth_copy_pixel_program;
static ngl::d3d9::pixel_program movie_yuv_pixel_program;
static ngl::d3d9::pixel_program white_pixel_program;
static ngl::d3d9::pixel_program restore_depth_pixel_program;
static ngl::d3d9::pixel_program four_texture_average_pixel_program;
static ngl::d3d9::pixel_program fog_pixel_program;

void ngl::d3d9::initialize_internal_programs() {
    internal_program_set &programs = references::internal_programs.get();

    quad_pc_vertex_program.create({ e_shader_program::internal_quad_pc_vertex });
    programs.quad_pc_vertex_program = quad_pc_vertex_program.get();

    vertex_color_pixel_program.create({ e_shader_program::internal_vertex_color_pixel });
    programs.quad_pc_pixel_program = vertex_color_pixel_program.get();

    screen_pc_vertex_program.create({ e_shader_program::internal_screen_pc_vertex });
    programs.screen_pc_vertex_program = screen_pc_vertex_program.get();

    framebuffer_copy_vertex_program.create({ e_shader_program::scene_color_copy_vertex });
    programs.framebuffer_copy_vertex_program = framebuffer_copy_vertex_program.get();

    screen_pcuv_vertex_program.create({ e_shader_program::internal_screen_pcuv_vertex });
    programs.screen_pcuv_vertex_program = screen_pcuv_vertex_program.get();

    screen_pcuv4_vertex_program.create({ e_shader_program::internal_screen_pcuv4_vertex });
    programs.screen_pcuv4_vertex_program = screen_pcuv4_vertex_program.get();

    screen_puv_material_vertex_program.create({ e_shader_program::internal_screen_puv_material_vertex });
    programs.screen_puv_material_vertex_program = screen_puv_material_vertex_program.get();

    font_vertex_program.create({ e_shader_program::internal_font_vertex });
    programs.font_vertex_program = font_vertex_program.get();

    texture_modulate_pixel_program.create({ e_shader_program::internal_texture_modulate_pixel });
    programs.texture_modulate_pixel_program = texture_modulate_pixel_program.get();

    framebuffer_copy_pixel_program.create({ e_shader_program::scene_color_copy_pixel });
    programs.framebuffer_copy_pixel_program = framebuffer_copy_pixel_program.get();

    depth_copy_pixel_program.create({ e_shader_program::internal_depth_copy_pixel });
    programs.depth_copy_pixel_program = depth_copy_pixel_program.get();

    rawz_depth_copy_pixel_program.create({ e_shader_program::internal_rawz_depth_copy_pixel });
    programs.rawz_depth_copy_pixel_program = rawz_depth_copy_pixel_program.get();

    movie_yuv_pixel_program.create({ e_shader_program::internal_movie_yuv_pixel });
    programs.movie_yuv_pixel_program = movie_yuv_pixel_program.get();

    programs.screen_pc_pixel_program = vertex_color_pixel_program.get();

    white_pixel_program.create({ e_shader_program::internal_white_pixel });
    programs.white_pixel_program = white_pixel_program.get();

    restore_depth_pixel_program.create({ e_shader_program::internal_restore_depth_pixel });
    programs.restore_depth_pixel_program = restore_depth_pixel_program.get();

    four_texture_average_pixel_program.create({ e_shader_program::internal_four_texture_average_pixel });
    programs.four_texture_average_pixel_program = four_texture_average_pixel_program.get();

    fog_pixel_program.create({ e_shader_program::internal_fog_pixel });
    programs.fog_pixel_program = fog_pixel_program.get();
}
