#include "treyarch/game/night_sky_color.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_7D1A70
void night_sky_color::construct_mashed_class() {
    self = nullptr;

    dome_scale0              .construct_mashed_class();
    dome_scale1              .construct_mashed_class();
    dome_scale2              .construct_mashed_class();
    dome_scale3              .construct_mashed_class();
    dome_power0              .construct_mashed_class();
    dome_power1              .construct_mashed_class();
    dome_power2              .construct_mashed_class();
    dome_power3              .construct_mashed_class();
    dome_color0              .construct_mashed_class();
    dome_color0_intensity    .construct_mashed_class();
    dome_color1              .construct_mashed_class();
    dome_color1_intensity    .construct_mashed_class();
    dome_color2              .construct_mashed_class();
    dome_color2_intensity    .construct_mashed_class();
    dome_color3              .construct_mashed_class();
    dome_color3_intensity    .construct_mashed_class();
    dome_base_color          .construct_mashed_class();
    dome_color_base_intensity.construct_mashed_class();
}

// sub_7D1B30
void night_sky_color::destruct_mashed_class() {
    dome_scale0              .destruct_mashed_class();
    dome_scale1              .destruct_mashed_class();
    dome_scale2              .destruct_mashed_class();
    dome_scale3              .destruct_mashed_class();
    dome_power0              .destruct_mashed_class();
    dome_power1              .destruct_mashed_class();
    dome_power2              .destruct_mashed_class();
    dome_power3              .destruct_mashed_class();
    dome_color0              .destruct_mashed_class();
    dome_color0_intensity    .destruct_mashed_class();
    dome_color1              .destruct_mashed_class();
    dome_color1_intensity    .destruct_mashed_class();
    dome_color2              .destruct_mashed_class();
    dome_color2_intensity    .destruct_mashed_class();
    dome_color3              .destruct_mashed_class();
    dome_color3_intensity    .destruct_mashed_class();
    dome_base_color          .destruct_mashed_class();
    dome_color_base_intensity.destruct_mashed_class();
}

// sub_7DBD50
void night_sky_color::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    dome_scale0              .unmash(mash_info, this, buffer);
    dome_scale1              .unmash(mash_info, this, buffer);
    dome_scale2              .unmash(mash_info, this, buffer);
    dome_scale3              .unmash(mash_info, this, buffer);
    dome_power0              .unmash(mash_info, this, buffer);
    dome_power1              .unmash(mash_info, this, buffer);
    dome_power2              .unmash(mash_info, this, buffer);
    dome_power3              .unmash(mash_info, this, buffer);
    dome_color0              .unmash(mash_info, this, buffer);
    dome_color0_intensity    .unmash(mash_info, this, buffer);
    dome_color1              .unmash(mash_info, this, buffer);
    dome_color1_intensity    .unmash(mash_info, this, buffer);
    dome_color2              .unmash(mash_info, this, buffer);
    dome_color2_intensity    .unmash(mash_info, this, buffer);
    dome_color3              .unmash(mash_info, this, buffer);
    dome_color3_intensity    .unmash(mash_info, this, buffer);
    dome_base_color          .unmash(mash_info, this, buffer);
    dome_color_base_intensity.unmash(mash_info, this, buffer);
}
