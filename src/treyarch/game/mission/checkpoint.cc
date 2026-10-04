#include "retail.hh"
#include "treyarch/game/mission/mission_manager.hh"

using namespace treyarch;

// sub_97EA50
void mission_manager::save_player_restart(const vector3 &position, vector3 xz_facing) {
    xz_facing.y = 0.0f;

    if (xz_facing.length2() > 0.0001f) {
        f32 direction[4] = { xz_facing.x, 0.0f, xz_facing.z, 0.0f };
        f32 normalized[4];

        retail::sub_401AA0((u64*)normalized, direction);

        xz_facing = vector3(normalized[0], normalized[1], normalized[2]);
    } else {
        xz_facing.x = 1.0f;
        xz_facing.z = 0.0f;
    }

    using insert_vector3_key = void (__thiscall*)(void*       registry,
                                                  string_hash entity,
                                                  string_hash key,
                                                  vector3     default_value,
                                                  vector3*    value,
                                                  bool        set_value);

    vector3 value;

    // script_registry_insert_vector3d_key
    ((insert_vector3_key)retail::sub_803800)(working_checkpoint_registry,
                                             references::registry_mission_manager.read(),
                                             references::registry_player_restart_position.read(),
                                             position,
                                             &value,
                                             true);

    ((insert_vector3_key)retail::sub_803800)(working_checkpoint_registry,
                                             references::registry_mission_manager.read(),
                                             references::registry_player_restart_xzfacing.read(),
                                             xz_facing,
                                             &value,
                                             true);

    // copies the working registry into the saved one
    retail::sub_804DF0((i32)saved_checkpoint_registry, (i32)working_checkpoint_registry);
}
