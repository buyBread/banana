#include "retail.hh"
#include "game/wds/time/wds_time_manager.hh"

using namespace treyarch;

wds_time_manager::wds_time_manager() {
    retail::sub_95CA90((u32*)this);
}
