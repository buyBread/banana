#pragma once

#include "game/arch_base.hh"
#include "game/math/algebra/vector3d.hh"
#include "treyarch/mash/string.hh"
#include "vc8/map.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class mission_objective_data;

    enum e_mission_progression_flags : u32 {
        mission_progression_flag_failed        = 0x01,
        mission_progression_flag_required      = 0x02,
        mission_progression_flag_activated     = 0x04,
        mission_progression_flag_completed     = 0x08,
        mission_progression_flag_displayable   = 0x10,
        mission_progression_flag_autocompleted = 0x20,
        mission_progression_flag_autoactivated = 0x40
    };

    // what the mission progression registry files by id: a stage or an objective
    class mission_progression_data : public arch_base {

    public:
        using children_t = vc8::map<i32, mission_objective_data*>;

        i32          id;
        u32          flags;
        mash::string description;
        children_t   children;    // keyed by the child count when each was made

        // slot 53; clears failed, activated and completed, then tells the registry
        void restart() {
            ((void (__thiscall*)(mission_progression_data*))vtable[53])(this);
        }

        // slot 54; a script text id, only objectives keep it
        void set_description(i32 text_id) {
            ((void (__thiscall*)(mission_progression_data*, i32))vtable[54])(this, text_id);
        }

        bool is_flagged(e_mission_progression_flags flag) const {
            return flags & flag;
        }

        void set_required     (bool on);
        void set_displayable  (bool on);
        void set_autoactivated(bool on);
        void set_autocompleted(bool on);
    };

    class mission_stage_data : public mission_progression_data {

    public:
        vector3d facing_direction;
        vector3d restart_position;

        void set_restart_data(const vector3d* position, const vector3d* facing);
    };

    class mission_objective_data : public mission_progression_data {

    public:
        f32          points;            // what the objective text shows, unless bit 0 is set
        f32          initial_score;
        f32          actual_score;
        f32          target_score;
        f32          unk_038;
        bool         unk_03c;
        u8           pad_03d[0x03];
        mash::string short_description;

        // slot 55
        void set_short_description(i32 text_id) {
            ((void (__thiscall*)(mission_objective_data*, i32))vtable[55])(this, text_id);
        }

        void set_actual_score(f32 score);
        void init_tracking_scores(f32 start, f32 target);
        void set_awarded_points(f32 awarded, f32 unk_b, bool unk_c);
    };

    ASSERT_SIZEOF  (mission_progression_data,              0x28);
    ASSERT_OFFSETOF(mission_progression_data, id,          0x08);
    ASSERT_OFFSETOF(mission_progression_data, description, 0x10);
    ASSERT_OFFSETOF(mission_progression_data, children,    0x1C);

    ASSERT_SIZEOF  (mission_stage_data,                   0x40);
    ASSERT_OFFSETOF(mission_stage_data, facing_direction, 0x28);
    ASSERT_OFFSETOF(mission_stage_data, restart_position, 0x34);

    ASSERT_SIZEOF  (mission_objective_data,                    0x4C);
    ASSERT_OFFSETOF(mission_objective_data, actual_score,      0x30);
    ASSERT_OFFSETOF(mission_objective_data, unk_03c,           0x3C);
    ASSERT_OFFSETOF(mission_objective_data, short_description, 0x40);
} // treyarch
