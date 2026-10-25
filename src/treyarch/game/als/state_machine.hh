#pragma once

#include "treyarch/game/als/als.hh"
#include "treyarch/game/als/animation_blend.hh"
#include "treyarch/game/als/animation_event.hh"
#include "treyarch/game/als/internal.hh"
#include "treyarch/game/als/param.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/shared/resource_key.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace als {
    class animation_logic_system;

    // a layer's live state; the states and categories it moves between are shared
    class state_machine : public state_machine_interface {

    public:
        struct smi_variables {
            bool            is_set_kill;
            bool            is_request_or_force;
            bool            is_force_state;
            bool            unk_0b; // request_category_transition's third argument
            string_hash     request_id;
            parameter_list* requested_params;
        };

        struct smi_return_vars {
            bool        did_do_trans;
            bool        trans_from_force_or_request;
            bool        is_still_fulfilling;
            bool        is_interruptable;
            bool        is_active;
            bool        unk_19;
            u8          pad_1a[0x02];
            category_id prev_cat;
            u32         unk_20;
        };

        struct pend_req {
            parameter_list* pending_params;
            category_id     pending_cat;
        };

        state_machine_shared*          shared_portion;
        smi_variables                  our_smi;
        smi_return_vars                our_ret;
        request_data                   curr_req_data;
        const state*                   curr_state;
        const category*                curr_category;
        pend_req                       pending_requests;
        parameter_list                 parameter_buffers[2]; // the request and pending pointers swap between these
        const state*                   prev_state;
        const category*                prev_category;
        mash::vector<animation_event>* curr_events;
        mash::vector<animation_event>* curr_hit_pause_events;
        animation_blends_list*         curr_blends;
        resource_key                   unk_794;
        u32                            unk_79c;
        i32                            unk_7a0[4];
        mutable const state*           search_cache_state;
        mutable const category*        search_cache_category;
        animation_controller*          the_controller;
        bool                           unk_7bc;
        u8                             pad_7bd[0x03];

        state_machine();
        ~state_machine() override;

        void* operator new(std::size_t size) noexcept {
            return memory::heap::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }

        category_id get_category_id() const override;
        state_id    get_state_id() const override;

        void set_desired_param(external_float_parameter_types  id, f32             data) override;
        void set_desired_param(external_vector_parameter_types id, const vector3d &data) override;
        void set_desired_param(external_hash_parameter_types   id, string_hash     data) override;

        void request_category_transition(category_id                       requested_category,
                                         animation_logic_system_interface* my_als,
                                         bool                              unk) override;

        bool is_interruptable() const override;
        bool did_transition_succeed() const override;
        bool did_transition_succeed_from_request() const override;
        bool is_request_satisfied() const override;
        bool is_active() const override;
        void force_als_state(state_id destination_state, only_if_you_know_what_you_are_doing well_do_ya_punk) override;
        void set_speed(f32 speed) override;
        void set_secondary_speed(f32 speed) override;
        f32  get_speed() const override;
        f32  get_secondary_speed() const override;
        void kill_layer() override;
        bool does_category_exist(category_id the_cat_id) const override;

        f32                      get_pb_float(string_hash parameter) const override;
        i32                      get_pb_int(string_hash parameter) const override;
        string_hash              get_pb_hash(string_hash parameter) const override;
        const char*              get_pb_fixedstring(string_hash parameter) const override;
        vector3d                 get_pb_vector3d(string_hash parameter) const override;
        variance_variable<f32>   get_pb_float_variance(string_hash parameter) const override;
        const char*              get_pb_mstring(string_hash parameter) const override;
        mash::mash_virtual_base* get_pb_class(string_hash parameter) const override;

        f32 get_optional_pb_float(      string_hash  parameter,
                                  const f32         &default_value,
                                        bool*        exists) const override;

        i32 get_optional_pb_int(      string_hash  parameter,
                                const i32         &default_value,
                                      bool*        exists) const override;

        string_hash get_optional_pb_hash(string_hash parameter, string_hash default_value, bool* exists) const override;
        const char* get_optional_pb_fixedstring(string_hash parameter, const char* default_value, bool* exists) const override;

        vector3d get_optional_pb_vector3d(      string_hash  parameter,
                                          const vector3d    &default_value,
                                                bool*        exists) const override;

        variance_variable<f32> get_optional_pb_float_variance(      string_hash             parameter,
                                                              const variance_variable<f32> &default_value,
                                                                    bool*                   exists) const override;

        const char*              get_optional_pb_mstring(string_hash parameter, const char* default_value, bool* exists) const override;
        mash::mash_virtual_base* get_optional_pb_class(string_hash               parameter,
                                                       mash::mash_virtual_base*  default_value,
                                                       bool*                     exists) const override;

        bool does_parameter_exist(string_hash parameter) const override;
        u32  get_parameter_data_type(string_hash parameter) const override;

        f32 get_time_to_end_of_anim(bool allow_looping, bool unk) const override;
        f32 get_time_to_signal(string_hash signal_name, bool* signal_exists, bool unk, u32* unk2) const override;
        f32 unk_039(string_hash signal_name, bool* signal_exists, bool unk, u32* unk2) const override;
        f32 get_duration() const override;
        f32 get_current_normalized_time() const override;
        f32 get_current_anim_time() const override;

        bool is_cat_our_prev_cat(category_id test_category) const override;
        bool is_requesting_category(category_id requested_category) const override;

        bool          unk_045() const override;
        u32           get_num_unk_44() const override;
        state_unk_44* get_unk_44(u32 index) const override;
        void*         unk_048(bool unk, entity* the_entity) const override;

        virtual mash::string unk_049(string_hash signal_name, bool in_frames) const;
        virtual mash::string unk_050() const;
        virtual bool         unk_051() const;
        virtual layer_id     get_layer_id() const = 0;
        virtual bool         determine_if_request_satisfied(animation_logic_system* the_als) const = 0;
        virtual f32          get_playback_speed() const;
        virtual f32          get_start_time() const;

        const category* find_category(category_id the_id, bool error_on_not_found) const;
        void            kill_als_state_script(animation_logic_system* the_als, bool unk);

    protected:
        void reset(state_machine_shared* shared_machine);
    };

    class base_state_machine : public state_machine {

    public:
        layer_id get_layer_id() const override;
        bool     determine_if_request_satisfied(animation_logic_system* the_als) const override;

        void init(animation_logic_system* the_als, state_machine_shared* shared_machine);
    };

    class layer_state_machine : public state_machine {

    public:
        layer_id get_layer_id() const override;
        bool     determine_if_request_satisfied(animation_logic_system* the_als) const override;

        virtual void set_layer_id(layer_id new_id);

        void init(animation_logic_system* the_als, layer_state_machine_shared* shared_machine);
    };

    ASSERT_SIZEOF  (state_machine::smi_variables,                   0x0C);
    ASSERT_OFFSETOF(state_machine::smi_variables, request_id,       0x04);
    ASSERT_OFFSETOF(state_machine::smi_variables, requested_params, 0x08);

    ASSERT_SIZEOF  (state_machine::smi_return_vars,           0x10);
    ASSERT_OFFSETOF(state_machine::smi_return_vars, prev_cat, 0x08);
    ASSERT_OFFSETOF(state_machine::smi_return_vars, unk_20,   0x0C);

    ASSERT_SIZEOF  (state_machine,                        0x7C0);
    ASSERT_OFFSETOF(state_machine, shared_portion,        0x004);
    ASSERT_OFFSETOF(state_machine, our_smi,               0x008);
    ASSERT_OFFSETOF(state_machine, our_ret,               0x014);
    ASSERT_OFFSETOF(state_machine, curr_req_data,         0x024);
    ASSERT_OFFSETOF(state_machine, curr_state,            0x030);
    ASSERT_OFFSETOF(state_machine, curr_category,         0x034);
    ASSERT_OFFSETOF(state_machine, pending_requests,      0x038);
    ASSERT_OFFSETOF(state_machine, parameter_buffers,     0x040);
    ASSERT_OFFSETOF(state_machine, prev_state,            0x780);
    ASSERT_OFFSETOF(state_machine, prev_category,         0x784);
    ASSERT_OFFSETOF(state_machine, curr_events,           0x788);
    ASSERT_OFFSETOF(state_machine, curr_hit_pause_events, 0x78C);
    ASSERT_OFFSETOF(state_machine, curr_blends,           0x790);
    ASSERT_OFFSETOF(state_machine, unk_794,               0x794);
    ASSERT_OFFSETOF(state_machine, unk_79c,               0x79C);
    ASSERT_OFFSETOF(state_machine, unk_7a0,               0x7A0);
    ASSERT_OFFSETOF(state_machine, search_cache_state,    0x7B0);
    ASSERT_OFFSETOF(state_machine, search_cache_category, 0x7B4);
    ASSERT_OFFSETOF(state_machine, the_controller,        0x7B8);
    ASSERT_OFFSETOF(state_machine, unk_7bc,               0x7BC);

    ASSERT_SIZEOF(base_state_machine,  0x7C0);
    ASSERT_SIZEOF(layer_state_machine, 0x7C0);
}} // treyarch::als
