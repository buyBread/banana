#include "treyarch/game/sin_container.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// inlined @ sub_762520
void sin_district_container::construct_mashed_class() {
    district_name.construct_mashed_class();
    info         .construct_mashed_class();
}

// inlined @ sub_755830
void sin_district_container::destruct_mashed_class() {
    district_name.destruct_mashed_class();
    info         .destruct_mashed_class();
}

// inlined @ sub_75CE20
void sin_district_container::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    district_name.unmash(mash_info, this, buffer);
    info         .unmash(mash_info, this, buffer);
}

// inlined @ sub_76B750
void sin_container::construct_mashed_class() {
    self = nullptr;

    master_script_name .construct_mashed_class();
    district_graph_name.construct_mashed_class();
    districts          .construct_mashed_class();
}

// inlined @ sub_762560
void sin_container::destruct_mashed_class() {
    master_script_name .destruct_mashed_class();
    district_graph_name.destruct_mashed_class();
    districts          .destruct_mashed_class();
}

// inlined @ sub_767680
void sin_container::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    master_script_name .unmash(mash_info, this, buffer);
    district_graph_name.unmash(mash_info, this, buffer);
    districts          .unmash(mash_info, this, buffer);
}
