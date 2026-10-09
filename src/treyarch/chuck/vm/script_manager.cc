#include "treyarch/chuck/vm/script_manager.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_A1A2C0
void script_manager::register_callbacks(notification_callback_t                      notification,
                                        get_script_executable_resource_callback_t    get_script_executable_resource,
                                        get_script_var_container_resource_callback_t get_script_var_container_resource,
                                        unk_predicate_callback_t                     unk_predicate_0,
                                        unk_predicate_callback_t                     unk_predicate_1,
                                        unk_predicate_callback_t                     unk_predicate_2,
                                        unk_predicate_callback_t                     unk_predicate_3,
                                        get_platform_callback_t                      get_platform) {

    notification_callback                      = notification;
    get_script_executable_resource_callback    = get_script_executable_resource;
    get_script_var_container_resource_callback = get_script_var_container_resource;
    unk_predicate_callbacks[0]                 = unk_predicate_0;
    unk_predicate_callbacks[1]                 = unk_predicate_1;
    unk_predicate_callbacks[2]                 = unk_predicate_2;
    unk_predicate_callbacks[3]                 = unk_predicate_3;
    get_platform_callback                      = get_platform;
}
