#pragma once

#include "game/container/simple_list.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/mutex.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"
#include "vc8/vector.hh"

namespace treyarch {
    class ai_tentacle_info;
    class dangler;

    // SM3's eTentacleEngineType, the values the engines answer
    enum e_tentacle_engine_type : i32 {
        tentacle_engine_strike       = 1,
        tentacle_engine_swirl_strike = 2,
        tentacle_engine_dangle       = 5, // ai_tentacle_dangle and ai_tentacle_dangle_old
        tentacle_engine_follow       = 6,
        tentacle_engine_slack        = 7
    };

    namespace references {
        inline util::memory_reference<void*> ai_tentacle_engine_vtable       { 0x00B8F0AC };
        inline util::memory_reference<void*> ai_tentacle_dangle_vtable       { 0x00B8F0EC };
        inline util::memory_reference<void*> ai_tentacle_follow_vtable       { 0x00B8F110 };
        inline util::memory_reference<void*> ai_tentacle_strike_vtable       { 0x00B8F170 };
        inline util::memory_reference<void*> ai_tentacle_swirl_strike_vtable { 0x00B8F18C };
        inline util::memory_reference<void*> ai_tentacle_slack_vtable        { 0x00B8F1A8 };

        inline util::memory_reference<u32> ai_tentacle_engine_id_counter { 0x00FC49D4 };

        inline util::memory_reference<engine_recursive_lock> ai_tentacle_engine_id_lock { 0x00FC4B68 };
    } // references

    // one way a tentacle moves; the tentacle info runs the first on its list
    class ai_tentacle_engine {

    public:
        void**                    vtable;
        container::simple_list_variables
            <ai_tentacle_engine*> simple_list_vars;
        u32                       id;
        ai_tentacle_info*         tentacle;
        ai_tentacle_engine*       modifier;

        explicit ai_tentacle_engine(ai_tentacle_info* t);

        // slot 0
        void destroy() {
            ((void (__thiscall*)(ai_tentacle_engine*, u32))vtable[0])(this, 1);
        }

        // slot 1
        e_tentacle_engine_type get_type() {
            return ((e_tentacle_engine_type (__thiscall*)(ai_tentacle_engine*))vtable[1])(this);
        }

        // slot 3
        bool frame_advance(f32 delta_t, bool merging) { // true once the engine is done
            return ((bool (__thiscall*)(ai_tentacle_engine*, f32, bool))vtable[3])(this, delta_t, merging);
        }

        // slot 4
        void translate_engine(const vector3d &delta) {
            ((void (__thiscall*)(ai_tentacle_engine*, const vector3d*))vtable[4])(this, &delta);
        }

        // slot 6; dangles, follows and slacks
        void set_length(f32 length) {
            ((void (__thiscall*)(ai_tentacle_engine*, f32))vtable[6])(this, length);
        }

        // slot 7; dangles, follows and slacks
        f32 get_length() {
            return ((f32 (__thiscall*)(ai_tentacle_engine*))vtable[7])(this);
        }

        // slot 9; follows and slacks
        f32 get_slack_percent() {
            return ((f32 (__thiscall*)(ai_tentacle_engine*))vtable[9])(this);
        }

        // slot 10; follows and slacks
        void set_slack_percent(f32 percent) {
            ((void (__thiscall*)(ai_tentacle_engine*, f32))vtable[10])(this, percent);
        }
    };

    // pulls the tentacle's end at a point
    class ai_tentacle_strike : public ai_tentacle_engine {

    public:
        vector3d target;
        f32      speed;
        i32      max_pull;

        explicit ai_tentacle_strike(ai_tentacle_info* t);

        void setup(const vector3d &_target, f32 _speed, i32 _max_pull);
    };

    // swings the tentacle's end at a point along a curve
    class ai_tentacle_swirl_strike : public ai_tentacle_engine {

    public:
        vector3d target;
        f32      speed;
        f32      curve_rad;
        f32      line_dist;
        f32      accel;

        explicit ai_tentacle_swirl_strike(ai_tentacle_info* t);

        void setup(const vector3d &_target, f32 _speed, f32 _accel, f32 _curve_rad);
    };

    // SM3's ai_tentacle_dangle; follows and slacks build on it
    class ai_tentacle_dangle_old : public ai_tentacle_engine {

    public:
        dangler* tentacle_dangler;
        bool     update_end;
        bool     update_base;
        bool     copy_back;
        u8       pad_023;

        void     set_gravity(const vector3d &grav);
        vector3d get_gravity() const;

        void set_air_resistance(f32 coefficient);
        void set_elasticity(f32 coefficient);

    protected:
        // inlined @ sub_4B9F70
        // inlined @ sub_4B9FA0
        explicit ai_tentacle_dangle_old(ai_tentacle_info* t) : ai_tentacle_engine(t) {
            tentacle_dangler = nullptr;
            update_base      = false;
            update_end       = true;
            copy_back        = true;
        }
    };

    // what a tentacle dangles with; its own setup stays native
    class ai_tentacle_dangle : public ai_tentacle_engine {

    public:
        dangler* tentacle_dangler;
        f32      unk_020;
        f32      unk_024; // a random angle
        bool     unk_028; // a random bit
        bool     update_end;
        bool     update_base;
        bool     copy_back;

        explicit ai_tentacle_dangle(ai_tentacle_info* t);
    };

    // hangs the tentacle with some slack, its end optionally riding an entity
    class ai_tentacle_follow : public ai_tentacle_dangle_old {

    public:
        f32               drag;
        f32               slack_percent;
        arch_base_vhandle attach_ent;
        vector3d          local_attach_end; // in the attached entity's frame

        // sub_4B9F70
        // inlined @ sub_4B9FA0
        explicit ai_tentacle_follow(ai_tentacle_info* t) : ai_tentacle_dangle_old(t) {
            vtable     = (void**)&references::ai_tentacle_follow_vtable.get();
            attach_ent = arch_base_vhandle();
        }

        void setup(f32                _slack_percent,
                   f32                _drag,
                   const vector3d    &grav,
                   arch_base_vhandle  _attach_ent,
                   bool               allow_coll,
                   bool               _update_end);

        void set_attach_ent(arch_base_vhandle _attach_ent);
        void set_attach_ent(arch_base_vhandle _attach_ent, const vector3d &local_end);
        void set_drag(f32 a);
    };

    // a follow held to a fixed rope length
    class ai_tentacle_slack : public ai_tentacle_follow {

    public:
        f32 rope_length;

        explicit ai_tentacle_slack(ai_tentacle_info* t);

        void setup(f32                _rope_len,
                   f32                _drag,
                   const vector3d    &gravity,
                   arch_base_vhandle  _attach_ent,
                   bool               allow_coll,
                   bool               _update_end);
    };

    // curls a web's line as it flies; its setup and frame stay native
    class ai_tentacle_web_curly : public ai_tentacle_dangle_old {

    public:
        arch_base_vhandle     spiderman;
        vc8::vector<vector3d> curl_pos;
        vector3d              curl_dir;
        f32                   dangle_blend;
        f32                   num_curls;
        i32                   curl_type;

        void reset_curl();
    };

    ASSERT_SIZEOF  (ai_tentacle_engine,                   0x1C);
    ASSERT_OFFSETOF(ai_tentacle_engine, simple_list_vars, 0x04);
    ASSERT_OFFSETOF(ai_tentacle_engine, id,               0x10);
    ASSERT_OFFSETOF(ai_tentacle_engine, modifier,         0x18);

    ASSERT_SIZEOF  (ai_tentacle_strike,           0x30);
    ASSERT_OFFSETOF(ai_tentacle_strike, target,   0x1C);
    ASSERT_OFFSETOF(ai_tentacle_strike, max_pull, 0x2C);

    ASSERT_SIZEOF  (ai_tentacle_swirl_strike,            0x38);
    ASSERT_OFFSETOF(ai_tentacle_swirl_strike, curve_rad, 0x2C);
    ASSERT_OFFSETOF(ai_tentacle_swirl_strike, accel,     0x34);

    ASSERT_SIZEOF  (ai_tentacle_dangle_old,                   0x24);
    ASSERT_OFFSETOF(ai_tentacle_dangle_old, tentacle_dangler, 0x1C);
    ASSERT_OFFSETOF(ai_tentacle_dangle_old, copy_back,        0x22);

    ASSERT_SIZEOF  (ai_tentacle_dangle,             0x2C);
    ASSERT_OFFSETOF(ai_tentacle_dangle, unk_024,    0x24);
    ASSERT_OFFSETOF(ai_tentacle_dangle, update_end, 0x29);

    ASSERT_SIZEOF  (ai_tentacle_follow,                   0x3C);
    ASSERT_OFFSETOF(ai_tentacle_follow, drag,             0x24);
    ASSERT_OFFSETOF(ai_tentacle_follow, attach_ent,       0x2C);
    ASSERT_OFFSETOF(ai_tentacle_follow, local_attach_end, 0x30);

    ASSERT_SIZEOF  (ai_tentacle_slack,              0x40);
    ASSERT_OFFSETOF(ai_tentacle_slack, rope_length, 0x3C);

    ASSERT_SIZEOF  (ai_tentacle_web_curly,            0x50);
    ASSERT_OFFSETOF(ai_tentacle_web_curly, spiderman, 0x24);
    ASSERT_OFFSETOF(ai_tentacle_web_curly, curl_pos,  0x28);
    ASSERT_OFFSETOF(ai_tentacle_web_curly, curl_dir,  0x38);
    ASSERT_OFFSETOF(ai_tentacle_web_curly, curl_type, 0x4C);
} // treyarch
