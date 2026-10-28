#pragma once

#include <array>

#include "vc8/list.hh"
#include "game/wds/ai/param_block.hh"
#include "game/arch_base_vhandle.hh"
#include "game/hash/algo.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/vector_basic.hh"
#include "treyarch/mash/virtual_base.hh"
#include "game/math/po.hh"
#include "game/mutex.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;
    class ai_core;
    class line_info;
    class manip_obj_interface;

    // baked into each node by its .BAI
    enum e_info_node_type : u32 {
        info_node_type_ai_action_processor     = 0,
        info_node_type_aim_util                = 1,
        info_node_type_aim_mech                = 2,
        info_node_type_cpu_combat              = 3,
        info_node_type_ai_item_holder          = 4,
        info_node_type_ai_mutation             = 5,
        info_node_type_wall_sprint             = 6,
        info_node_type_ai_car                  = 7,
        info_node_type_ai_wander               = 8,
        info_node_type_std_puppet              = 9,
        info_node_type_als                     = 10,
        info_node_type_avoidance               = 11,
        info_node_type_biped_layer             = 12,
        info_node_type_unmoving_layer          = 13,
        info_node_type_quad_path               = 14,
        info_node_type_std_carry               = 15,
        info_node_type_ai_civilian_agressor    = 16,
        info_node_type_ai_civilian             = 17,
        info_node_type_ai_cover                = 18,
        info_node_type_crawl_layer             = 19,
        info_node_type_std_fear                = 20,
        info_node_type_flight_layer            = 21,
        info_node_type_jump_layer              = 22,
        info_node_type_loco                    = 23,
        info_node_type_path_graph              = 24,
        info_node_type_ai_rappel               = 25,
        info_node_type_search                  = 26,
        info_node_type_slave                   = 27,
        info_node_type_ai_switch               = 28,
        info_node_type_target_search           = 29,
        info_node_type_combat_target           = 30,
        info_node_type_voice_box               = 31,
        info_node_type_ai_weapon               = 32,
        info_node_type_hybrid_layer            = 33,
        info_node_type_ai_root                 = 34,
        info_node_type_ai_fd                   = 35,
        info_node_type_ai_basic                = 36,
        info_node_type_swift_surface_layer     = 37,
        info_node_type_manip_watch             = 38,
        info_node_type_swift_stationary_layer  = 39,
        info_node_type_swift_flight_layer      = 40,
        info_node_type_path_location           = 41,
        info_node_type_teleporting_layer       = 42,
        info_node_type_count
    };

    namespace references {
        // each type's name ("ai_action_processor", "puppet_inode", "carry" ...), hashed by the CRT initializers
        inline util::memory_reference<std::array<string_hash, info_node_type_count>> info_node_type_names { 0x00FC5500 };
    } // references

    // sub_4C3380
    inline e_info_node_type get_info_node_type(string_hash name) { // info_node_type_count when nothing has the name
        u32 type = 0;

        while (references::info_node_type_names.get()[type] != name) {
            if (++type >= info_node_type_count)
                return info_node_type_count;
        }

        return (e_info_node_type)type;
    }

    class info_node : public mash::mash_virtual_base {

    public:
        u32              unk_004;
        param_block      my_param_block;
        e_info_node_type my_type;
        ai_core*         my_ai_core;

        // sub_A79D60
        ai_core* get_core() const { return my_ai_core; }

        actor* get_actor() const;

        virtual bool unk_024(u32);
        virtual bool unk_028();
        virtual bool does_need_advance(bool post_pass) const;
        virtual bool unk_030(bool post_pass) const; // asked of a type's first node: advance every node of this type
        virtual void frame_advance(f32 delta_t);
        virtual void post_frame_advance(f32 delta_t);
        virtual bool unk_03c(u32);
        virtual void activate(ai_core* the_ai);
        virtual void deactivate();
        virtual void reset();
        virtual void enter_limbo();
        virtual void exit_limbo();
    };

    // SM3's als_inode
    class als_inode : public info_node {

    public:
        void* my_als; // als::animation_logic_system_interface
        u32   unk_034;
    };

    class loco_inode : public info_node {

    public:
        u8          reserved_030[0x54];
        string_hash loco_als_category;
        u8          reserved_088[0x04];
        bool        unk_08c;            // flight_mocomp: set hands the flight layer the whole move, clear only the run along the facing
        u8          reserved_08d[0x1F];
        u32         unk_0ac;            // the als's ai_locomotion_mode reads bit 1 on a swift surface layer

        // inlined @ sub_84EBB0
        void set_loco_als_category(string_hash cat) {
            loco_als_category = cat;
        }
    };

    // flight_mocomp flies the actor through it; swift_flight_layer_inode derives from it
    class flight_layer_inode : public loco_inode {

    public:
        u8       reserved_0b0[0x04];
        vector3d unk_0b4;            // with unk_115 set, flight_mocomp moves by it times dt
        u8       reserved_0c0[0x34];
        vector3d unk_0f4;            // otherwise toward it, at the inode's speeds
        u8       reserved_100[0x15];
        bool     unk_115;
        u8       reserved_116[0x02];
        f32      unk_118;            // the als's ai_flight_bank
        u8       reserved_11c[0x0C];
        f32      unk_128;            // the als's ai_flight_pitch
        u8       reserved_12c[0x24];
    };

    // named after the look-at natives that set them
    enum e_aim_util_flags : u32 {
        aim_util_flag_aim_head                       = 0x00020000,
        aim_util_flag_aim_head_direct                = 0x00040000,
        aim_util_flag_aim_head_and_eyes              = 0x00080000,
        aim_util_flag_aim_head_eyes_and_upper_body   = 0x00100000,
        aim_util_flag_aim_head_and_upper_body        = 0x00200000,
        aim_util_flag_aim_head_and_upper_body_direct = 0x00400000,
        aim_util_flag_aim_left_arm                   = 0x00800000,
        aim_util_flag_aim_right_arm                  = 0x01000000
    };

    class aim_util_inode : public info_node {

    public:
        u32      flags;
        u8       reserved_034[0x58C];
        vector3d target_to_look_at;
        u8       reserved_5cc[0x1C];

        // inlined @ sub_853120
        // inlined @ sub_8531B0
        // inlined @ sub_853240
        // inlined @ sub_8532D0
        // inlined @ sub_853360
        // inlined @ sub_8533F0
        // inlined @ sub_853480
        // inlined @ sub_853510
        void set_flag(e_aim_util_flags flag, bool enable) {
            if (enable)
                flags |= flag;
            else
                flags &= ~flag;
        }
    };

    class ai_car_inode : public info_node {

    public:
        u8 reserved_030[0x70];

        bool force_enter_car(arch_base_vhandle car, i32 seat, bool animate);
        bool force_exit_car (bool animate);
    };

    inline const string_hash goto_radius_key              = string_hash(hash::djb2("goto_radius"));
    inline const string_hash goto_speed_key               = string_hash(hash::djb2("goto_speed"));
    inline const string_hash goto_allow_facing_key        = string_hash(hash::djb2("goto_allow_facing"));
    inline const string_hash goto_allow_nonpathed_key     = string_hash(hash::djb2("goto_allow_nonpathed"));
    inline const string_hash turn_speed_deg_key           = string_hash(hash::djb2("turn_speed_deg"));
    inline const string_hash goto_force_non_pathed_key    = string_hash(hash::djb2("goto_force_non_pathed"));
    inline const string_hash goto_disable_avoidance_key   = string_hash(hash::djb2("goto_disable_avoidance"));
    inline const string_hash movement_info_updated_key    = string_hash(hash::djb2("movement_info_updated"));
    inline const string_hash interact_slave_key           = string_hash(hash::djb2("interact_slave"));
    inline const string_hash interact_master_key          = string_hash(hash::djb2("interact_master"));
    inline const string_hash ignore_same_state_switch_key = string_hash(hash::djb2("ignore_same_state_switch"));
    inline const string_hash switch_state_key             = string_hash(hash::djb2("switch_state"));
    inline const string_hash generic_goto_key             = string_hash(hash::djb2("generic_goto"));

    // the script callbacks a puppet raises when it stops
    inline const string_hash puppet_success_stop_key  = string_hash(hash::djb2("puppet_success_stop(entity)"));
    inline const string_hash puppet_failure_stop_key  = string_hash(hash::djb2("puppet_failure_stop(entity)"));
    inline const string_hash puppet_dead_stop_key     = string_hash(hash::djb2("puppet_dead_stop(entity)"));
    inline const string_hash puppet_damage_stop_key   = string_hash(hash::djb2("puppet_damage_stop(entity)"));
    inline const string_hash puppet_threat_stop_key   = string_hash(hash::djb2("puppet_threat_stop(entity)"));
    inline const string_hash puppet_stop_key          = string_hash(hash::djb2("puppet_stop(entity)"));
    inline const string_hash puppet_external_stop_key = string_hash(hash::djb2("puppet_external_stop(entity)"));

    class std_puppet_inode : public info_node {

    public:
        mash::vector_basic<arch_base_vhandle> slaves;
        mash::vector_basic<vector3d>          goto_destinations;
        engine_recursive_lock                 access_lock;

        // sub_4910A0
        void lock_access() {
            access_lock.acquire();
        }

        // sub_4910B0
        void unlock_access() {
            access_lock.release();
        }

        // sub_4B3300
        void set_puppet_goto_destination(vector3d destination) {
            goto_destinations.clear();
            goto_destinations.push_back(destination);
        }

        // sub_4B3340
        void set_puppet_goto_destinations(const mash::vector_basic<vector3d>* destinations) {
            goto_destinations = *destinations;
        }
    };

    inline const string_hash biped_layer_inode_key            = string_hash(hash::djb2("biped_layer_inode"));
    inline const string_hash crawl_layer_inode_key            = string_hash(hash::djb2("crawl_layer_inode"));
    inline const string_hash flight_layer_inode_key           = string_hash(hash::djb2("flight_layer_inode"));
    inline const string_hash swift_flight_layer_inode_key     = string_hash(hash::djb2("swift_flight_layer_inode"));
    inline const string_hash hybrid_layer_inode_key           = string_hash(hash::djb2("hybrid_layer_inode"));
    inline const string_hash swift_surface_layer_inode_key    = string_hash(hash::djb2("swift_surface_layer_inode"));
    inline const string_hash swift_stationary_layer_inode_key = string_hash(hash::djb2("swift_stationary_layer_inode"));
    inline const string_hash teleporting_layer_inode_key      = string_hash(hash::djb2("teleporting_layer_inode"));

    class std_carry_inode : public info_node {

    public:
        std_carry_inode*                 carry_master;
        vc8::list<std_carry_inode*>*     carry_slaves;
        vc8::list<manip_obj_interface*>* unk_038;            // the manip objs it holds
        std_carry_inode*                 desired_slave;
        u8                               reserved_040[0x28];

        // inlined
        bool is_being_carried() const {
            return carry_master != nullptr;
        }

        // sub_425020
        // inlined @ sub_86A3B0
        bool has_carried_objects() const { // slaves or manip objs
            return (carry_slaves && !carry_slaves->empty()) || (unk_038 && !unk_038->empty());
        }

        bool add_slave   (std_carry_inode* slave, bool engage_slave_mode = true);
        void remove_slave(std_carry_inode* slave, bool kill_slave_mode = true);
    };

    class std_fear_inode : public info_node {

    public:
        u8 reserved_030[0x38];

        void reset_morale();
        void break_morale_force_cower(f32 unk);
        void break_morale_force_flee (vector3d position, f32 unk);
        void break_morale_force_flee (arch_base_vhandle target, f32 unk);
    };

    inline const string_hash targeting_weight_key = string_hash(hash::djb2("targeting_weight"));

    class target_search_inode : public info_node {

    public:
        u8 reserved_030[0x38];
    };

    class combat_target_inode : public target_search_inode {

    public:
        u8 reserved_068[0x80];

        // sub_455B60
        void set_targeting_weight(i32 weight) {
            my_param_block.set_pb_int(targeting_weight_key, weight);
        }
    };

    class crawl_layer_inode : public loco_inode {

    public:
        u8 reserved_0b0[0xC8];

        void attach_to_surface   (const line_info &lif, const vector3d &desired_forward);
        void release_from_surface(const vector3d &desired_forward);
    };

    class swift_stationary_layer_inode : public loco_inode {

    public:
        u8 reserved_0b0[0x08];
    };

    inline const string_hash teleport_target_pos_key = string_hash(hash::djb2("teleport_target_pos"));

    class teleporting_layer_inode : public swift_stationary_layer_inode {

    public:
        arch_base_vhandle unk_0b8;           // als::combat_pole_swing_attach swings the ai round it
        u8                reserved_0bc[0x1C];

        vector3d get_teleport_target_pos() const;
    };

    // the hero's; wall_po is the wall's facing (z) and up (y) that wallsprint_mocomp runs along
    class wall_sprint_inode : public info_node {

    public:
        u8  reserved_030[0x28];
        i32 unk_058;            // above 1, wallsprint_mocomp moves the actor 1/n of the way onto wall_po per frame
        u8  reserved_05c[0x14];
        po  wall_po;
        u8  reserved_0b0[0xB0];

        // sub_54AE00
        po* get_wall_po() { return &wall_po; }
    };

    ASSERT_SIZEOF  (info_node,                 0x30);
    ASSERT_OFFSETOF(info_node, unk_004,        0x04);
    ASSERT_OFFSETOF(info_node, my_param_block, 0x08);
    ASSERT_OFFSETOF(info_node, my_type,        0x28);
    ASSERT_OFFSETOF(info_node, my_ai_core,     0x2C);

    ASSERT_SIZEOF  (als_inode,         0x38);
    ASSERT_OFFSETOF(als_inode, my_als, 0x30);

    ASSERT_SIZEOF  (loco_inode,                    0xB0);
    ASSERT_OFFSETOF(loco_inode, loco_als_category, 0x84);
    ASSERT_OFFSETOF(loco_inode, unk_08c,           0x8C);
    ASSERT_OFFSETOF(loco_inode, unk_0ac,           0xAC);

    ASSERT_SIZEOF  (flight_layer_inode,          0x150);
    ASSERT_OFFSETOF(flight_layer_inode, unk_0b4, 0xB4);
    ASSERT_OFFSETOF(flight_layer_inode, unk_0f4, 0xF4);
    ASSERT_OFFSETOF(flight_layer_inode, unk_115, 0x115);
    ASSERT_OFFSETOF(flight_layer_inode, unk_118, 0x118);
    ASSERT_OFFSETOF(flight_layer_inode, unk_128, 0x128);

    ASSERT_SIZEOF  (aim_util_inode,                    0x5E8);
    ASSERT_OFFSETOF(aim_util_inode, flags,             0x030);
    ASSERT_OFFSETOF(aim_util_inode, target_to_look_at, 0x5C0);

    ASSERT_SIZEOF(ai_car_inode, 0xA0);

    ASSERT_SIZEOF  (std_puppet_inode,                    0x60);
    ASSERT_OFFSETOF(std_puppet_inode, slaves,            0x30);
    ASSERT_OFFSETOF(std_puppet_inode, goto_destinations, 0x40);
    ASSERT_OFFSETOF(std_puppet_inode, access_lock,       0x50);

    ASSERT_SIZEOF(crawl_layer_inode, 0x178);

    ASSERT_SIZEOF  (std_carry_inode,                0x68);
    ASSERT_OFFSETOF(std_carry_inode, carry_master,  0x30);
    ASSERT_OFFSETOF(std_carry_inode, carry_slaves,  0x34);
    ASSERT_OFFSETOF(std_carry_inode, unk_038,       0x38);
    ASSERT_OFFSETOF(std_carry_inode, desired_slave, 0x3C);

    ASSERT_SIZEOF(std_fear_inode,      0x68);
    ASSERT_SIZEOF(target_search_inode, 0x68);
    ASSERT_SIZEOF(combat_target_inode, 0xE8);

    ASSERT_SIZEOF(swift_stationary_layer_inode, 0xB8);

    ASSERT_SIZEOF  (teleporting_layer_inode,          0xD8);
    ASSERT_OFFSETOF(teleporting_layer_inode, unk_0b8, 0xB8);

    ASSERT_SIZEOF  (wall_sprint_inode,          0x160);
    ASSERT_OFFSETOF(wall_sprint_inode, unk_058, 0x58);
    ASSERT_OFFSETOF(wall_sprint_inode, wall_po, 0x70);
} // treyarch
