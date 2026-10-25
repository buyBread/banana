#pragma once

#include "treyarch/game/als/als.hh"
#include "treyarch/game/event/generic_event_callback.hh"
#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "treyarch/shared/math/po.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class actor;
    class flight_layer_inode;

namespace als {
    class animation_logic_system;
    class state_machine;

    // what a layer's state can say about the anim's offset
    inline const string_hash mocomp_ignore_positional_offset_id  = string_hash(hash::djb2("mocomp_ignore_positional_offset"));
    inline const string_hash mocomp_allow_knockback_scale_id     = string_hash(hash::djb2("mocomp_allow_knockback_scale"));
    inline const string_hash mocomp_allow_knockup_scale_id       = string_hash(hash::djb2("mocomp_allow_knockup_scale"));
    inline const string_hash mocomp_allow_knockdown_scale_id     = string_hash(hash::djb2("mocomp_allow_knockdown_scale"));
    inline const string_hash mocomp_allow_air_knockback_scale_id = string_hash(hash::djb2("mocomp_allow_air_knockback_scale"));
    inline const string_hash mocomp_allow_air_knockup_scale_id   = string_hash(hash::djb2("mocomp_allow_air_knockup_scale"));
    inline const string_hash mocomp_allow_air_knockdown_scale_id = string_hash(hash::djb2("mocomp_allow_air_knockdown_scale"));

    // and about turning
    inline const string_hash use_movement_window_id        = string_hash(hash::djb2("use_movement_window"));
    inline const string_hash use_accel_decel_mode_id       = string_hash(hash::djb2("use_accel_decel_mode"));
    inline const string_hash use_instant_facing_id         = string_hash(hash::djb2("use_instant_facing"));
    inline const string_hash inverse_facing_id             = string_hash(hash::djb2("inverse_facing"));
    inline const string_hash use_x_facing_id               = string_hash(hash::djb2("use_x_facing"));
    inline const string_hash use_destination_facing_id     = string_hash(hash::djb2("use_destination_facing"));
    inline const string_hash turn_rate_id                  = string_hash(hash::djb2("turn_rate"));
    inline const string_hash turn_accel_time_id            = string_hash(hash::djb2("turn_accel_time"));
    inline const string_hash turn_direction_id             = string_hash(hash::djb2("turn_direction"));
    inline const string_hash reorient_to_vertical_speed_id = string_hash(hash::djb2("reorient_to_vertical_speed"));
    inline const string_hash movement_speed_id             = string_hash(hash::djb2("movement_speed"));
    inline const string_hash use_fake_foot_ik_id           = string_hash(hash::djb2("use_fake_foot_ik"));
    inline const string_hash use_fake_leg_ik_id            = string_hash(hash::djb2("use_fake_leg_ik"));
    inline const string_hash return_rot_speed_id           = string_hash(hash::djb2("return_rot_speed"));
    inline const string_hash lock_foot_id                  = string_hash(hash::djb2("lock_foot"));
    inline const string_hash r_foot_id                     = string_hash(hash::djb2("r_foot_id"));
    inline const string_hash l_foot_id                     = string_hash(hash::djb2("l_foot_id"));
    inline const string_hash inverse_velocity_id           = string_hash(hash::djb2("inverse_velocity"));
    inline const string_hash move_dir_from_anim_id         = string_hash(hash::djb2("move_dir_from_anim"));
    inline const string_hash facing_input_dir_blend_id     = string_hash(hash::djb2("facing_input_dir_blend"));
    inline const string_hash allow_speed_override_id       = string_hash(hash::djb2("allow_speed_override"));
    inline const string_hash max_playback_speed_id         = string_hash(hash::djb2("max_playback_speed"));
    inline const string_hash min_playback_speed_id         = string_hash(hash::djb2("min_playback_speed"));

    namespace references {
        // static event-type ids, hashed from their names by the CRT initializers
        inline util::memory_reference<string_hash> move_begin   { 0x0102C614 };
        inline util::memory_reference<string_hash> move_end     { 0x0102C72C };
        inline util::memory_reference<string_hash> rotate_begin { 0x0102C46C };
        inline util::memory_reference<string_hash> rotate_end   { 0x0102C62C };

        // "BIP01 R FOOT" and "BIP01 L FOOT", hashed by the CRT initializers
        inline util::memory_reference<string_hash> bip01_r_foot { 0x00FFECFC };
        inline util::memory_reference<string_hash> bip01_l_foot { 0x00FFED34 };
    } // references

    // where the accel/decel mode's turn rate is done ramping up and starts ramping down, through the movement window; nothing writes them
    inline f32 turn_ramp_up_end     = 0.25f;
    inline f32 turn_ramp_down_start = 0.5f;

    // input_orientation's switches for the speed param and for matching the playback speed to it; nothing writes them
    inline i32  speed_override_enabled = 1;
    inline bool match_playback_speed   = false;

    // the crawl ik's switch; ikcrawl_mocomp is a relative_orientation without it. nothing writes it
    inline bool ikcrawl_enabled = true;

    // moves and turns the actor the way its animation asks; built in the als's own storage
    class motion_compensator : public mash::mash_virtual_base {

    public:
        animation_logic_system* the_als;
        state_machine*          the_base;
        actor*                  the_actor;

        motion_compensator() = default;

        // inlined @ sub_723DA0
        explicit motion_compensator(mash::generating_vtable_version so_do_nothing) : mash_virtual_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        virtual void     activate(animation_logic_system* the_als_core, layer_id layer);
        virtual void     deactivate();
        virtual void     transition();
        virtual void     pre_anim_action(f32 delta_t);
        virtual void     post_anim_action(f32 delta_t);
        virtual bool     is_request_fulfilled();
        virtual bool     unk_015() const;
        virtual bool     force_reactivate() const; // a new state of the same type builds it again
        virtual layer_id get_layer_id() const;
        virtual void     get_po_curr_offset(po &curr_offset); // the anim's trajectory this frame, scaled as the state allows
        virtual void     set_po_curr_offset(actor* target, po &curr_offset);

        // builds the class in the buffer; null for a key it doesn't know
        static motion_compensator* create_subclass_in_place(mash::virtual_types_key the_class_to_make, void* memory_buffer);

        static string_hash my_type_key;
        static i32         last_created_size; // never read
    };

    // applies the anim's offset as it is
    class use_anim_only : public motion_compensator {

    public:
        use_anim_only() = default;

        // inlined @ sub_723DA0
        explicit use_anim_only(mash::generating_vtable_version so_do_nothing) : motion_compensator(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void post_anim_action(f32 delta_t) override;

        static string_hash my_type_key;
    };

    // turns the actor toward the state's direction and up at a turn rate, applying the anim's offset as it goes
    class simple_orientation : public motion_compensator {

    public:
        f32                    turn_rate;
        bool                   currently_rotating;
        bool                   use_movement_window;
        bool                   inside_movement_window;
        bool                   use_accel_decel_mode;
        bool                   use_instant_facing;
        u8                     pad_019[0x03];
        generic_event_callback move_begin_callback;
        generic_event_callback move_end_callback;
        generic_event_callback rotate_begin_callback;
        generic_event_callback rotate_end_callback;
        f32                    move_begin_signal_time;
        f32                    move_end_signal_time;

        simple_orientation();

        // inlined @ sub_723DA0
        // inlined @ sub_7191C0
        // inlined @ sub_7191F0
        // inlined @ sub_7197D0
        // inlined @ sub_719220
        explicit simple_orientation(mash::generating_vtable_version so_do_nothing) : motion_compensator(so_do_nothing),
                                                                                     currently_rotating(false),
                                                                                     use_movement_window(false),
                                                                                     inside_movement_window(false),
                                                                                     use_accel_decel_mode(false),
                                                                                     use_instant_facing(false),
                                                                                     move_begin_callback(so_do_nothing),
                                                                                     move_end_callback(so_do_nothing),
                                                                                     rotate_begin_callback(so_do_nothing),
                                                                                     rotate_end_callback(so_do_nothing) {}

        ~simple_orientation() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void activate(animation_logic_system* the_als_core, layer_id layer) override;
        void deactivate() override;
        void transition() override;
        void post_anim_action(f32 delta_t) override;

        virtual void get_directions(animation_logic_system* als_core,
                                    state_machine*          base,
                                    vector3d*               desired_dir,
                                    vector3d*               desired_up) const;

        virtual f32  get_turn_rate(f32 delta_t, animation_logic_system* als_core, state_machine* base);
        virtual void other_po_changes(f32 delta_t, po* curr_offset);

        virtual void apply_rotation(      po*       curr_offset,
                                          f32       delta_t,
                                          f32       heading_change,
                                    const vector3d &curr_up,
                                    const vector3d &curr_dir,
                                    const vector3d &desired_up,
                                    const vector3d &desired_dir);

        virtual void face_desired_direction();  // what use_instant_facing turns with
        virtual f32  get_dir_tolerance() const; // dots past these count as facing already
        virtual f32  get_up_tolerance() const;

        // opens and closes the movement window on the move and rotate signals
        void update_movement_window();

        // one step toward the desired direction and up from the current (unit) axes
        void turn_toward_desired(po &curr_offset, f32 delta_t, vector3d curr_dir, vector3d curr_up);

        static string_hash my_type_key;
    };

    // turns like simple_orientation, but moves along the strafe direction instead of the anim's ground offset
    class strafe_mocomp : public simple_orientation {

    public:
        strafe_mocomp() = default;

        // inlined @ sub_723DA0
        explicit strafe_mocomp(mash::generating_vtable_version so_do_nothing) : simple_orientation(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void other_po_changes(f32 delta_t, po* curr_offset) override;

        static string_hash my_type_key;
    };

    // keeps the actor's own direction and up unless the state sets them
    class relative_orientation : public simple_orientation {

    public:
        relative_orientation() = default;

        // inlined @ sub_723DA0
        explicit relative_orientation(mash::generating_vtable_version so_do_nothing) : simple_orientation(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void get_directions(animation_logic_system* als_core,
                            state_machine*          base,
                            vector3d*               desired_dir,
                            vector3d*               desired_up) const override;

        static string_hash my_type_key;
    };

    // strafes with relative_orientation's directions
    class relative_strafe_mocomp : public strafe_mocomp {

    public:
        relative_strafe_mocomp() = default;

        // inlined @ sub_723DA0
        explicit relative_strafe_mocomp(mash::generating_vtable_version so_do_nothing) : strafe_mocomp(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void get_directions(animation_logic_system* als_core,
                            state_machine*          base,
                            vector3d*               desired_dir,
                            vector3d*               desired_up) const override;

        static string_hash my_type_key;
    };

    // pivots the turn about the lower foot (or the state's lock_foot) when the conglomerate has both
    class simple_orientation_on_foot : public simple_orientation {

    public:
        i16      foot_index[2];        // right and left; -2 without one
        vector3d foot_world_facing[2];
        vector3d foot_world_pos[2];
        f32      return_rot_speed;
        i32      lock_foot;            // -1 lets the lower foot be the pivot
        i32      rot_foot;             // the last pivot
        bool     use_fake_foot_ik;
        bool     use_fake_leg_ik;
        u8       pad_086[0x02];

        simple_orientation_on_foot();
        explicit simple_orientation_on_foot(mash::generating_vtable_version so_do_nothing);

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void activate(animation_logic_system* the_als_core, layer_id layer) override;
        void transition() override;
        void post_anim_action(f32 delta_t) override;

        void apply_rotation(      po*       curr_offset,
                                  f32       delta_t,
                                  f32       heading_change,
                            const vector3d &curr_up,
                            const vector3d &curr_dir,
                            const vector3d &desired_up,
                            const vector3d &desired_dir) override;

        // false when there's no pivot or the feet are level and none is locked
        static bool rotate_about_foot(      po*       curr_offset,
                                            actor*    target,
                                            f32       delta_t,
                                            f32       heading_change,
                                      const vector3d &curr_up,
                                      const vector3d &curr_dir,
                                      const vector3d &desired_up,
                                      const vector3d &desired_dir,
                                            i16       left_foot,
                                            i16       right_foot,
                                            i32       lock,
                                            i32*      pivot_foot);

        static void rotate_about_point(      po*       curr_offset,
                                             actor*    target,
                                       const vector3d &up,
                                       const vector3d &dir,
                                       const vector3d &pivot,
                                       const vector3d &axis,
                                             f32       angle);

        static string_hash my_type_key;
    };

    class strafe_mocomp_on_foot : public simple_orientation_on_foot {

    public:
        strafe_mocomp_on_foot() = default;
        explicit strafe_mocomp_on_foot(mash::generating_vtable_version so_do_nothing);

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void other_po_changes(f32 delta_t, po* curr_offset) override;

        static string_hash my_type_key;
    };

    class relative_orientation_on_foot : public simple_orientation_on_foot {

    public:
        relative_orientation_on_foot() = default;
        explicit relative_orientation_on_foot(mash::generating_vtable_version so_do_nothing);

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void get_directions(animation_logic_system* als_core,
                            state_machine*          base,
                            vector3d*               desired_dir,
                            vector3d*               desired_up) const override;

        static string_hash my_type_key;
    };

    class relative_strafe_mocomp_on_foot : public strafe_mocomp_on_foot {

    public:
        relative_strafe_mocomp_on_foot() = default;
        explicit relative_strafe_mocomp_on_foot(mash::generating_vtable_version so_do_nothing);

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void get_directions(animation_logic_system* als_core,
                            state_machine*          base,
                            vector3d*               desired_dir,
                            vector3d*               desired_up) const override;

        static string_hash my_type_key;
    };

    // faces the way the actor moves along the ground, upright
    class velocity_orientation : public simple_orientation {

    public:
        velocity_orientation() = default;

        // inlined @ sub_723DA0
        explicit velocity_orientation(mash::generating_vtable_version so_do_nothing) : simple_orientation(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void get_directions(animation_logic_system* als_core,
                            state_machine*          base,
                            vector3d*               desired_dir,
                            vector3d*               desired_up) const override;

        static string_hash my_type_key;
    };

    // faces up, keeping the actor's up unless the state sets one
    class vertical_orientation : public relative_orientation {

    public:
        vertical_orientation() = default;

        // inlined @ sub_723DA0
        explicit vertical_orientation(mash::generating_vtable_version so_do_nothing) : relative_orientation(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void get_directions(animation_logic_system* als_core,
                            state_machine*          base,
                            vector3d*               desired_dir,
                            vector3d*               desired_up) const override;

        static string_hash my_type_key;
    };

    // simple_orientation with the actor's standing and physics bits off while it runs
    class momentum_grapple_unsynched : public simple_orientation {

    public:
        momentum_grapple_unsynched() = default;

        // inlined @ sub_723DA0
        explicit momentum_grapple_unsynched(mash::generating_vtable_version so_do_nothing) : simple_orientation(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void activate(animation_logic_system* the_als_core, layer_id layer) override;
        void deactivate() override;

        static string_hash my_type_key;
    };

    // faces the way the actor moved when it started, upright
    class initial_velocity_orientation : public simple_orientation {

    public:
        vector3d initial_dir;

        initial_velocity_orientation() = default;

        // inlined @ sub_723DA0
        explicit initial_velocity_orientation(mash::generating_vtable_version so_do_nothing) : simple_orientation(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void activate(animation_logic_system* the_als_core, layer_id layer) override;
        bool force_reactivate() const override;

        void get_directions(animation_logic_system* als_core,
                            state_machine*          base,
                            vector3d*               desired_dir,
                            vector3d*               desired_up) const override;

        static string_hash my_type_key;
    };

    // faces and leans the way the actor moved when it started
    class initial3D_velocity_orientation : public simple_orientation {

    public:
        vector3d initial_dir;
        vector3d initial_up;

        initial3D_velocity_orientation() = default;

        // inlined @ sub_723DA0
        explicit initial3D_velocity_orientation(mash::generating_vtable_version so_do_nothing) : simple_orientation(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void activate(animation_logic_system* the_als_core, layer_id layer) override;
        bool force_reactivate() const override;

        void get_directions(animation_logic_system* als_core,
                            state_machine*          base,
                            vector3d*               desired_dir,
                            vector3d*               desired_up) const override;

        static string_hash my_type_key;
    };

    // turns only inside the rotate signals, and moves (along the strafe direction) only inside the move ones
    class strict_simple_orientation : public simple_orientation {

    public:
        bool inside_rotate_window;
        u8   pad_045[0x03];

        strict_simple_orientation();

        // inlined @ sub_723DA0
        explicit strict_simple_orientation(mash::generating_vtable_version so_do_nothing) : simple_orientation(so_do_nothing),
                                                                                            inside_rotate_window(false) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void activate(animation_logic_system* the_als_core, layer_id layer) override;
        void post_anim_action(f32 delta_t) override;
        void other_po_changes(f32 delta_t, po* curr_offset) override;

        static string_hash my_type_key;
    };

    // moves and faces as the params say, every frame
    class direct_mocomp : public simple_orientation {

    public:
        vector3d des_move_dir;
        vector3d des_face_dir;
        vector3d des_up_dir;
        f32      des_speed;

        direct_mocomp() = default;

        // inlined @ sub_723DA0
        explicit direct_mocomp(mash::generating_vtable_version so_do_nothing) : simple_orientation(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void activate(animation_logic_system* the_als_core, layer_id layer) override;
        void post_anim_action(f32 delta_t) override;
        bool force_reactivate() const override;

        void get_directions(animation_logic_system* als_core,
                            state_machine*          base,
                            vector3d*               desired_dir,
                            vector3d*               desired_up) const override;

        void other_po_changes(f32 delta_t, po* curr_offset) override;

        virtual void update_params(f32 delta_t);

        static string_hash my_type_key;
    };

    // moves along the input direction at the anim's own speed, keeping the playback speed in the state's bounds
    class input_orientation : public relative_orientation {

    public:
        input_orientation() = default;

        // inlined @ sub_723DA0
        explicit input_orientation(mash::generating_vtable_version so_do_nothing) : relative_orientation(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void deactivate() override;
        void post_anim_action(f32 delta_t) override;
        void other_po_changes(f32 delta_t, po* curr_offset) override;

        static string_hash my_type_key;
    };

    // faces whatever tows the actor and keeps its hitch point on the tow point
    class towed_orientation : public simple_orientation {

    public:
        towed_orientation() = default;

        // inlined @ sub_723DA0
        explicit towed_orientation(mash::generating_vtable_version so_do_nothing) : simple_orientation(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void post_anim_action(f32 delta_t) override;

        // in the world; the actor's own position when nothing tows it
        vector3d get_tow_point() const;

        static string_hash my_type_key;
    };

    // hands the actor to the crawl ik, which places it on the geometry around it
    class ikcrawl_mocomp : public relative_orientation {

    public:
        ikcrawl_mocomp() = default;

        // inlined @ sub_723DA0
        explicit ikcrawl_mocomp(mash::generating_vtable_version so_do_nothing) : relative_orientation(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void activate(animation_logic_system* the_als_core, layer_id layer) override;
        void deactivate() override;
        void transition() override;
        void post_anim_action(f32 delta_t) override;

        void get_directions(animation_logic_system* als_core,
                            state_machine*          base,
                            vector3d*               desired_dir,
                            vector3d*               desired_up) const override;

        static string_hash my_type_key;
    };

    // turns within the wall sprint inode's wall_po and keeps the actor on it, the anim's offset following the wall
    class wallsprint_mocomp : public relative_orientation {

    public:
        wallsprint_mocomp() = default;

        // inlined @ sub_723DA0
        explicit wallsprint_mocomp(mash::generating_vtable_version so_do_nothing) : relative_orientation(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void post_anim_action(f32 delta_t) override;
        void other_po_changes(f32 delta_t, po* curr_offset) override;

        void apply_rotation(      po*       curr_offset,
                                  f32       delta_t,
                                  f32       heading_change,
                            const vector3d &curr_up,
                            const vector3d &curr_dir,
                            const vector3d &desired_up,
                            const vector3d &desired_dir) override;

        void face_desired_direction() override;

        static string_hash my_type_key;
    };

    inline const string_hash biped_physics_mocomp_key = string_hash(hash::djb2("als::biped_physics_mocomp"));

    ASSERT_SIZEOF  (motion_compensator,            0x10);
    ASSERT_OFFSETOF(motion_compensator, the_als,   0x04);
    ASSERT_OFFSETOF(motion_compensator, the_base,  0x08);
    ASSERT_OFFSETOF(motion_compensator, the_actor, 0x0C);

    ASSERT_SIZEOF(use_anim_only, 0x10);

    ASSERT_SIZEOF  (simple_orientation,                         0x44);
    ASSERT_OFFSETOF(simple_orientation, turn_rate,              0x10);
    ASSERT_OFFSETOF(simple_orientation, use_instant_facing,     0x18);
    ASSERT_OFFSETOF(simple_orientation, move_begin_callback,    0x1C);
    ASSERT_OFFSETOF(simple_orientation, rotate_end_callback,    0x34);
    ASSERT_OFFSETOF(simple_orientation, move_begin_signal_time, 0x3C);
    ASSERT_OFFSETOF(simple_orientation, move_end_signal_time,   0x40);

    ASSERT_SIZEOF(strafe_mocomp,          0x44);
    ASSERT_SIZEOF(relative_orientation,   0x44);
    ASSERT_SIZEOF(relative_strafe_mocomp, 0x44);

    ASSERT_SIZEOF  (simple_orientation_on_foot,                    0x88);
    ASSERT_OFFSETOF(simple_orientation_on_foot, foot_index,        0x44);
    ASSERT_OFFSETOF(simple_orientation_on_foot, foot_world_facing, 0x48);
    ASSERT_OFFSETOF(simple_orientation_on_foot, foot_world_pos,    0x60);
    ASSERT_OFFSETOF(simple_orientation_on_foot, return_rot_speed,  0x78);
    ASSERT_OFFSETOF(simple_orientation_on_foot, lock_foot,         0x7C);
    ASSERT_OFFSETOF(simple_orientation_on_foot, rot_foot,          0x80);
    ASSERT_OFFSETOF(simple_orientation_on_foot, use_fake_foot_ik,  0x84);

    ASSERT_SIZEOF(strafe_mocomp_on_foot,          0x88);
    ASSERT_SIZEOF(relative_orientation_on_foot,   0x88);
    ASSERT_SIZEOF(relative_strafe_mocomp_on_foot, 0x88);

    ASSERT_SIZEOF(velocity_orientation,       0x44);
    ASSERT_SIZEOF(vertical_orientation,       0x44);
    ASSERT_SIZEOF(momentum_grapple_unsynched, 0x44);

    ASSERT_SIZEOF  (initial_velocity_orientation,              0x50);
    ASSERT_OFFSETOF(initial_velocity_orientation, initial_dir, 0x44);

    ASSERT_SIZEOF  (initial3D_velocity_orientation,              0x5C);
    ASSERT_OFFSETOF(initial3D_velocity_orientation, initial_dir, 0x44);
    ASSERT_OFFSETOF(initial3D_velocity_orientation, initial_up,  0x50);

    ASSERT_SIZEOF  (strict_simple_orientation,                       0x48);
    ASSERT_OFFSETOF(strict_simple_orientation, inside_rotate_window, 0x44);

    ASSERT_SIZEOF  (direct_mocomp,               0x6C);
    ASSERT_OFFSETOF(direct_mocomp, des_move_dir, 0x44);
    ASSERT_OFFSETOF(direct_mocomp, des_face_dir, 0x50);
    ASSERT_OFFSETOF(direct_mocomp, des_up_dir,   0x5C);
    ASSERT_OFFSETOF(direct_mocomp, des_speed,    0x68);

    ASSERT_SIZEOF(input_orientation, 0x44);
    ASSERT_SIZEOF(towed_orientation, 0x44);

    ASSERT_SIZEOF(ikcrawl_mocomp,    0x44);
    ASSERT_SIZEOF(wallsprint_mocomp, 0x44);
} // als

    inline const string_hash shaba_simple_helicopter_id = string_hash(hash::djb2("shaba_simple_helicopter"));
    inline const string_hash reverse_mode_id            = string_hash(hash::djb2("reverse_mode"));

    // flies the actor through its flight layer inode; outside the als in retail
    class flight_mocomp : public als::simple_orientation {

    public:
        flight_layer_inode* flight_inode; // found by the first post-action

        flight_mocomp() = default;

        // inlined @ sub_723DA0
        explicit flight_mocomp(mash::generating_vtable_version so_do_nothing) : simple_orientation(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void activate(als::animation_logic_system* the_als_core, als::layer_id layer) override;
        void post_anim_action(f32 delta_t) override;

        static string_hash my_type_key;
    };

    ASSERT_SIZEOF  (flight_mocomp,               0x48);
    ASSERT_OFFSETOF(flight_mocomp, flight_inode, 0x44);
} // treyarch
