#include "retail.hh"
#include "game/mission/mission_manager.hh"

using namespace treyarch;

// sub_97EA50
void mission_manager::save_player_restart(const vector3d &position, vector3d xz_facing) {
    xz_facing.y = 0.0f;

    if (xz_facing.length2() > 0.0001f) {
        f32 direction[4] = { xz_facing.x, 0.0f, xz_facing.z, 0.0f };
        f32 normalized[4];

        retail::sub_401AA0((u64*)normalized, direction);

        xz_facing = vector3d(normalized[0], normalized[1], normalized[2]);
    } else {
        xz_facing.x = 1.0f;
        xz_facing.z = 0.0f;
    }

    vector3d value;

    working_checkpoint_registry->insert_vector3d_key(references::registry_mission_manager.read(),
                                                     references::registry_player_restart_position.read(),
                                                     position,
                                                     &value,
                                                     true);

    working_checkpoint_registry->insert_vector3d_key(references::registry_mission_manager.read(),
                                                     references::registry_player_restart_xzfacing.read(),
                                                     xz_facing,
                                                     &value,
                                                     true);

    *saved_checkpoint_registry = *working_checkpoint_registry;
}

// sub_97EBC0
void mission_manager::save_checkpoint(const vector3d &position, vector3d xz_facing, f32 new_checkpoint_value) {
    save_player_restart(position, xz_facing);

    current_mission.unk_03c = 0;
    checkpoint              = (u32)(i64)new_checkpoint_value;
}

// sub_97E9C0
void mission_manager::script_registry_insert_rational_key(string_hash the_entity,
                                                          string_hash key,
                                                          f32         default_value,
                                                          f32*        value,
                                                          bool        set_value) {

    working_checkpoint_registry->insert_rational_key(the_entity, key, default_value, value, set_value);
}

// sub_97EA00
void mission_manager::script_registry_insert_vector3d_key(string_hash the_entity,
                                                          string_hash key,
                                                          vector3d    default_value,
                                                          vector3d*   value,
                                                          bool        set_value) {

    working_checkpoint_registry->insert_vector3d_key(the_entity, key, default_value, value, set_value);
}
