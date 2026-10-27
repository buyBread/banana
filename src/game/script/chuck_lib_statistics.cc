#include "retail.hh"
#include "game/script/chuck_lib_statistics.hh"

using namespace treyarch;

bool slf__award_achievement__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8768D0)(this, stack);
}

bool slf__online_record_results__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876920)(this, stack);
}

bool slf__online_open_leaderboard_session__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876950)(this, stack);
}

bool slf__online_close_leaderboard_session__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824ED0)(this, stack);
}
