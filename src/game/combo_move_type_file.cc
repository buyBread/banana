#include <cstdlib>

#include "game/combo_move_type_file.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_90AAC0
i32 combo_move_track_info::compare_move_id(const void* left, const void* right) {
    i32 left_id  = (*(combo_move_track_info**)left) ->move_id;
    i32 right_id = (*(combo_move_track_info**)right)->move_id;

    if (left_id > right_id)
        return 1;

    return left_id < right_id ? -1 : 0;
}

// sub_938150
void combo_move_type_file::construct_mashed_class() {
    self = nullptr;

    combo_move_info_array.construct_mashed_class();
    trick_move_info_array.construct_mashed_class();

    if (combo_move_info_array.size)
        std::qsort(combo_move_info_array.data,
                   combo_move_info_array.size,
                   sizeof(combat_move_track_info*),
                   combo_move_track_info::compare_move_id);

    if (trick_move_info_array.size)
        std::qsort(trick_move_info_array.data,
                   trick_move_info_array.size,
                   sizeof(combo_move_track_info*),
                   combo_move_track_info::compare_move_id);
}

// sub_933C90
void combo_move_type_file::destruct_mashed_class() {
    combo_move_info_array.destruct_mashed_class();
    trick_move_info_array.destruct_mashed_class();
}

// sub_93D0E0
void combo_move_type_file::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    combo_move_info_array.unmash(mash_info, this, buffer);
    trick_move_info_array.unmash(mash_info, this, buffer);
}
