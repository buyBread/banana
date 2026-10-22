#include <cfloat>

#include "treyarch/game/gen_block.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// inlined @ sub_7DDD40
void gb_road::construct_mashed_class() {
    traffic_lanes.construct_mashed_class();

    unpacked          = nullptr;
    last_cam_distance = 0.0f;
    unk_33            = 0;
}

// inlined @ sub_7DDDC0
void gb_road::destruct_mashed_class() {
    traffic_lanes.destruct_mashed_class();
}

// inlined @ sub_7DDFE0
void gb_road::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    traffic_lanes.unmash(mash_info, this, buffer);
}

// inlined @ sub_7DCA80
void gb_block::construct_mashed_class() {
    vert.construct_mashed_class();
    road.construct_mashed_class();
}

// sub_7C2EB0
void gb_block::destruct_mashed_class() {
    vert.destruct_mashed_class();
    road.destruct_mashed_class();
}

// inlined @ sub_7DAB60
void gb_block::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    vert.unmash(mash_info, this, buffer);
    road.unmash(mash_info, this, buffer);
}

// inlined @ sub_7D3B60
void gb_block::finalize(mash::allocation_scope scope) {
    if (scope == mash::ALLOCATED) {
        vert.clear();
        road.clear();
    }
}

// inlined @ sub_7DEAC0
void gb_junction::construct_mashed_class() {
    roads.construct_mashed_class();
}

// inlined @ sub_7DA730
void gb_junction::destruct_mashed_class() {
    roads.destruct_mashed_class();
}

// inlined @ sub_7DC490
void gb_junction::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    roads.unmash(mash_info, this, buffer);
}

// sub_7DEAC0
void gb_map::construct_mashed_class() {
    self = nullptr;

    vertices        .construct_mashed_class();
    roads           .construct_mashed_class();
    sidewalk_corners.construct_mashed_class();
    blocks          .construct_mashed_class();
    junctions       .construct_mashed_class();
    road_defs       .construct_mashed_class();
    unk_8c          .construct_mashed_class();

    last_cam_pos = vector3d(FLT_MAX, FLT_MAX, FLT_MAX); // @branch: F32_MAX
}

// sub_7DEB90
void gb_map::destruct_mashed_class() {
    vertices        .destruct_mashed_class();
    roads           .destruct_mashed_class();
    sidewalk_corners.destruct_mashed_class();
    blocks          .destruct_mashed_class();
    junctions       .destruct_mashed_class();
    road_defs       .destruct_mashed_class();
    unk_8c          .destruct_mashed_class();
}

// sub_7DEE40
void gb_map::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    vertices        .unmash(mash_info, this, buffer);
    roads           .unmash(mash_info, this, buffer);
    sidewalk_corners.unmash(mash_info, this, buffer);
    blocks          .unmash(mash_info, this, buffer);
    junctions       .unmash(mash_info, this, buffer);
    road_defs       .unmash(mash_info, this, buffer);
    unk_8c          .unmash(mash_info, this, buffer);
}
