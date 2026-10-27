#include "game/day_sky_color.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_7D1150
void day_sky_color::construct_mashed_class() {
    self = nullptr;

    ray_scaler                 .construct_mashed_class();
    mie_scaler                 .construct_mashed_class();
    inscatter_scaler           .construct_mashed_class();
    g                          .construct_mashed_class();
    sun_intensity              .construct_mashed_class();
    turbidity                  .construct_mashed_class();
    skydome_zh_power           .construct_mashed_class();
    skydome_zenith             .construct_mashed_class();
    skydome_horizon            .construct_mashed_class();
    atmospheric_wave_length_r  .construct_mashed_class();
    atmospheric_wave_length_g  .construct_mashed_class();
    atmospheric_wave_length_b  .construct_mashed_class();
    sun_radius_inner0          .construct_mashed_class();
    sun_radius_outer0          .construct_mashed_class();
    sun_color0                 .construct_mashed_class();
    sun_color_intensity0       .construct_mashed_class();
    sun_radius_inner1          .construct_mashed_class();
    sun_radius_outer1          .construct_mashed_class();
    sun_color1                 .construct_mashed_class();
    sun_color_intensity1       .construct_mashed_class();
    sun_radius_inner2          .construct_mashed_class();
    sun_radius_outer2          .construct_mashed_class();
    sun_color2                 .construct_mashed_class();
    sun_color_intensity2       .construct_mashed_class();
    sun_radius_inner3          .construct_mashed_class();
    sun_radius_outer3          .construct_mashed_class();
    sun_color3                 .construct_mashed_class();
    sun_color_intensity3       .construct_mashed_class();
    god_ray_look_at_inner_angle.construct_mashed_class();
    god_ray_look_at_outer_angle.construct_mashed_class();
    god_ray_screen_area_x_min  .construct_mashed_class();
    god_ray_screen_area_x_max  .construct_mashed_class();
    god_ray_screen_area_y_min  .construct_mashed_class();
    god_ray_screen_area_y_max  .construct_mashed_class();
    god_ray_color              .construct_mashed_class();
    god_ray_intensity          .construct_mashed_class();
    sun_ring_scale_inner       .construct_mashed_class();
    sun_ring_scale_outer       .construct_mashed_class();
}

// sub_7D12F0
void day_sky_color::destruct_mashed_class() {
    ray_scaler                 .destruct_mashed_class();
    mie_scaler                 .destruct_mashed_class();
    inscatter_scaler           .destruct_mashed_class();
    g                          .destruct_mashed_class();
    sun_intensity              .destruct_mashed_class();
    turbidity                  .destruct_mashed_class();
    skydome_zh_power           .destruct_mashed_class();
    skydome_zenith             .destruct_mashed_class();
    skydome_horizon            .destruct_mashed_class();
    atmospheric_wave_length_r  .destruct_mashed_class();
    atmospheric_wave_length_g  .destruct_mashed_class();
    atmospheric_wave_length_b  .destruct_mashed_class();
    sun_radius_inner0          .destruct_mashed_class();
    sun_radius_outer0          .destruct_mashed_class();
    sun_color0                 .destruct_mashed_class();
    sun_color_intensity0       .destruct_mashed_class();
    sun_radius_inner1          .destruct_mashed_class();
    sun_radius_outer1          .destruct_mashed_class();
    sun_color1                 .destruct_mashed_class();
    sun_color_intensity1       .destruct_mashed_class();
    sun_radius_inner2          .destruct_mashed_class();
    sun_radius_outer2          .destruct_mashed_class();
    sun_color2                 .destruct_mashed_class();
    sun_color_intensity2       .destruct_mashed_class();
    sun_radius_inner3          .destruct_mashed_class();
    sun_radius_outer3          .destruct_mashed_class();
    sun_color3                 .destruct_mashed_class();
    sun_color_intensity3       .destruct_mashed_class();
    god_ray_look_at_inner_angle.destruct_mashed_class();
    god_ray_look_at_outer_angle.destruct_mashed_class();
    god_ray_screen_area_x_min  .destruct_mashed_class();
    god_ray_screen_area_x_max  .destruct_mashed_class();
    god_ray_screen_area_y_min  .destruct_mashed_class();
    god_ray_screen_area_y_max  .destruct_mashed_class();
    god_ray_color              .destruct_mashed_class();
    god_ray_intensity          .destruct_mashed_class();
    sun_ring_scale_inner       .destruct_mashed_class();
    sun_ring_scale_outer       .destruct_mashed_class();
}

// sub_7DB960
void day_sky_color::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    ray_scaler                 .unmash(mash_info, this, buffer);
    mie_scaler                 .unmash(mash_info, this, buffer);
    inscatter_scaler           .unmash(mash_info, this, buffer);
    g                          .unmash(mash_info, this, buffer);
    sun_intensity              .unmash(mash_info, this, buffer);
    turbidity                  .unmash(mash_info, this, buffer);
    skydome_zh_power           .unmash(mash_info, this, buffer);
    skydome_zenith             .unmash(mash_info, this, buffer);
    skydome_horizon            .unmash(mash_info, this, buffer);
    atmospheric_wave_length_r  .unmash(mash_info, this, buffer);
    atmospheric_wave_length_g  .unmash(mash_info, this, buffer);
    atmospheric_wave_length_b  .unmash(mash_info, this, buffer);
    sun_radius_inner0          .unmash(mash_info, this, buffer);
    sun_radius_outer0          .unmash(mash_info, this, buffer);
    sun_color0                 .unmash(mash_info, this, buffer);
    sun_color_intensity0       .unmash(mash_info, this, buffer);
    sun_radius_inner1          .unmash(mash_info, this, buffer);
    sun_radius_outer1          .unmash(mash_info, this, buffer);
    sun_color1                 .unmash(mash_info, this, buffer);
    sun_color_intensity1       .unmash(mash_info, this, buffer);
    sun_radius_inner2          .unmash(mash_info, this, buffer);
    sun_radius_outer2          .unmash(mash_info, this, buffer);
    sun_color2                 .unmash(mash_info, this, buffer);
    sun_color_intensity2       .unmash(mash_info, this, buffer);
    sun_radius_inner3          .unmash(mash_info, this, buffer);
    sun_radius_outer3          .unmash(mash_info, this, buffer);
    sun_color3                 .unmash(mash_info, this, buffer);
    sun_color_intensity3       .unmash(mash_info, this, buffer);
    god_ray_look_at_inner_angle.unmash(mash_info, this, buffer);
    god_ray_look_at_outer_angle.unmash(mash_info, this, buffer);
    god_ray_screen_area_x_min  .unmash(mash_info, this, buffer);
    god_ray_screen_area_x_max  .unmash(mash_info, this, buffer);
    god_ray_screen_area_y_min  .unmash(mash_info, this, buffer);
    god_ray_screen_area_y_max  .unmash(mash_info, this, buffer);
    god_ray_color              .unmash(mash_info, this, buffer);
    god_ray_intensity          .unmash(mash_info, this, buffer);
    sun_ring_scale_inner       .unmash(mash_info, this, buffer);
    sun_ring_scale_outer       .unmash(mash_info, this, buffer);
}
