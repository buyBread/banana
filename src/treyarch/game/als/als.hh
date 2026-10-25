#pragma once

#include "treyarch/game/als/param.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/math/algebra/vector3d.hh"
#include "treyarch/shared/mutex.hh"
#include "treyarch/shared/tl_fixed_string.hh"
#include "treyarch/shared/variance_variable.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"
#include "vc8/list.hh"
#include "vc8/vector.hh"

namespace treyarch {
    namespace mash {
        class mash_virtual_base;
        class string;
    } // mash

    class actor;
    class animation_controller;
    class entity;
    struct anim_event_param;

    using nal_anim_id = tl_fixed_string;

    enum only_if_you_know_what_you_are_doing : u32 {
        i_know_what_i_am_doing = 0xDEADBEEF,
        i_am_not_sure          = 0xCBADF00D
    };

    namespace als {
        // WoS has more layers than SM3's ten, so only the base layer keeps its name
        enum layer_types : u32 {
            base_layer = 0
        };

        using layer_id    = layer_types;
        using category_id = string_hash;
        using state_id    = string_hash;

        struct state_unk_44;

        class animation_logic_system_shared;
        class animation_logic_system_interface;

        // a layer as the rest of the game sees it
        class state_machine_interface {

        public:
            virtual ~state_machine_interface() {}

            virtual category_id get_category_id() const = 0;
            virtual state_id    get_state_id() const = 0;

            // msvc lays these out in reverse, as retail has them
            virtual void set_desired_param(external_float_parameter_types  id, f32             data) = 0;
            virtual void set_desired_param(external_vector_parameter_types id, const vector3d &data) = 0;
            virtual void set_desired_param(external_hash_parameter_types   id, string_hash     data) = 0;

            virtual void request_category_transition(category_id                       requested_category,
                                                     animation_logic_system_interface* my_als,
                                                     bool                              unk) = 0;

            virtual bool is_interruptable() const = 0;
            virtual bool did_transition_succeed() const = 0;
            virtual bool did_transition_succeed_from_request() const = 0;
            virtual bool is_request_satisfied() const = 0;
            virtual bool is_active() const = 0;
            virtual void force_als_state(state_id destination_state, only_if_you_know_what_you_are_doing well_do_ya_punk) = 0;
            virtual void set_speed(f32 speed) = 0;
            virtual void set_secondary_speed(f32 speed) = 0;
            virtual f32  get_speed() const = 0;
            virtual f32  get_secondary_speed() const = 0;
            virtual void kill_layer() = 0;
            virtual bool does_category_exist(category_id the_cat_id) const = 0;

            virtual f32                      get_pb_float(string_hash parameter) const = 0;
            virtual i32                      get_pb_int(string_hash parameter) const = 0;
            virtual string_hash              get_pb_hash(string_hash parameter) const = 0;
            virtual const char*              get_pb_fixedstring(string_hash parameter) const = 0;
            virtual vector3d                 get_pb_vector3d(string_hash parameter) const = 0;
            virtual variance_variable<f32>   get_pb_float_variance(string_hash parameter) const = 0;
            virtual const char*              get_pb_mstring(string_hash parameter) const = 0;
            virtual mash::mash_virtual_base* get_pb_class(string_hash parameter) const = 0;

            virtual f32 get_optional_pb_float(      string_hash  parameter,
                                              const f32         &default_value,
                                                    bool*        exists) const = 0;

            virtual i32 get_optional_pb_int(      string_hash  parameter,
                                            const i32         &default_value,
                                                  bool*        exists) const = 0;

            virtual string_hash get_optional_pb_hash(string_hash parameter, string_hash default_value, bool* exists) const = 0;
            virtual const char* get_optional_pb_fixedstring(string_hash parameter, const char* default_value, bool* exists) const = 0;

            virtual vector3d get_optional_pb_vector3d(      string_hash  parameter,
                                                      const vector3d    &default_value,
                                                            bool*        exists) const = 0;

            virtual variance_variable<f32> get_optional_pb_float_variance(      string_hash             parameter,
                                                                          const variance_variable<f32> &default_value,
                                                                                bool*                   exists) const = 0;

            virtual const char*              get_optional_pb_mstring(string_hash parameter, const char* default_value, bool* exists) const = 0;
            virtual mash::mash_virtual_base* get_optional_pb_class(string_hash               parameter,
                                                                   mash::mash_virtual_base*  default_value,
                                                                   bool*                     exists) const = 0;

            virtual bool does_parameter_exist(string_hash parameter) const = 0;
            virtual u32  get_parameter_data_type(string_hash parameter) const = 0;

            virtual f32 get_time_to_end_of_anim(bool allow_looping, bool scale_by_speed) const = 0;
            virtual f32 get_time_to_signal   (string_hash signal_name, bool* signal_exists, bool scale_by_speed, anim_event_param* param) const = 0;
            virtual f32 get_time_since_signal(string_hash signal_name, bool* signal_exists, bool scale_by_speed, anim_event_param* param) const = 0;
            virtual f32 get_duration() const = 0;
            virtual f32 get_current_normalized_time() const = 0;
            virtual f32 get_current_anim_time() const = 0;

            virtual bool is_cat_our_prev_cat(category_id test_category) const = 0;
            virtual bool is_requesting_category(category_id requested_category) const = 0;

            virtual bool          unk_045() const = 0;
            virtual u32           get_num_unk_44() const = 0;
            virtual state_unk_44* get_unk_44(u32 index) const = 0;
            virtual void*         unk_048(bool unk, entity* the_entity) const = 0;
        };

        class animation_logic_system_interface {

        public:
            using als_list = vc8::list<animation_logic_system_interface*>;

            bool force_updated; // set by force_update, cleared by the next frame
            u8   pad_005[0x03];

            virtual ~animation_logic_system_interface() {}

            virtual state_machine_interface* get_als_layer(layer_id the_layer) = 0;
            virtual bool                     has_als_layer(layer_id the_layer) const = 0;

            virtual bool did_transition_succeed() const = 0;
            virtual bool did_transition_succeed_from_request() const = 0;
            virtual void suspend_logic_system(bool suspend) = 0;
            virtual bool als_is_suspended() const = 0;
            virtual void create_instance_data(animation_logic_system_shared* shared_portion) = 0;
            virtual void delete_instance_data() = 0;
            virtual void   reset_animation_player() = 0;
            virtual engine_recursive_lock* get_access_lock() const = 0;
            virtual bool   is_high_priority() const = 0;
            virtual actor* get_actor() const = 0;
            virtual void   cache_actor_position() = 0;

            virtual void                              set_coordinator(animation_logic_system_interface* new_coordinator, i32 unk) = 0;
            virtual bool                              has_coordinator() const = 0;
            virtual animation_logic_system_interface* get_coordinator() const = 0;
            virtual i32                               get_coordinator_unk() const = 0;
            virtual bool                              is_coordinator() const = 0;
            virtual vc8::vector
                <animation_logic_system_interface*>*  get_coordinated() = 0;
            virtual void                              add_coordinated(animation_logic_system_interface* the_als) = 0;
            virtual void                              remove_coordinated(animation_logic_system_interface* the_als) = 0;

            virtual void                  force_matrix_update() = 0;
            virtual animation_controller* get_controller() const = 0;
            virtual void                  frame_advance(f32 delta_t, bool force) = 0;

            bool force_update(f32 delta_t, bool unk);

            static void frame_advance_all_alses(f32 delta_t);
            static void frame_advance_als_list(als_list** the_list, als_list** the_advance_list, f32 delta_t);
            static void add_to_als_list(animation_logic_system_interface* the_als);
            static void remove_from_als_list(animation_logic_system_interface* the_als);
            static void set_high_priority(animation_logic_system_interface* the_als);
            static void set_low_priority(animation_logic_system_interface* the_als);
        };

        namespace references {
            // the high pair advances first; an als moves between a list and its advance list every frame
            inline util::memory_reference<animation_logic_system_interface::als_list*> the_high_priority_als_list         { 0x00FD4310 };
            inline util::memory_reference<animation_logic_system_interface::als_list*> the_high_priority_als_advance_list { 0x00FD4314 };
            inline util::memory_reference<animation_logic_system_interface::als_list*> the_low_priority_als_list          { 0x00FD4308 };
            inline util::memory_reference<animation_logic_system_interface::als_list*> the_low_priority_als_advance_list  { 0x00FD430C };

            inline util::memory_reference<engine_recursive_lock> als_list_lock     { 0x00FD6C20 };
            inline util::memory_reference<engine_recursive_lock> force_update_lock { 0x00FD43C0 };
        } // references
    } // als
} // treyarch
