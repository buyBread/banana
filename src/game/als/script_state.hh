#pragma once

#include <type_traits>

#include "game/als/als.hh"
#include "game/als/internal.hh"
#include "game/als/rules.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/vector_basic.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace als {
    // what a trans group is asked to check
    enum e_trans_type : u32 {
        trans_implicit = 0,
        trans_explicit = 1,
        trans_layer    = 2  // unsupported, like any other type
    };

    // passes when every rule passes
    class basic_rule_list {

    public:
        transition_rule_base** rules;
        u32                    num_rules;

        ~basic_rule_list();

        // an empty list passes
        bool check(const rule_context &context) const;

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
        void custom_unmash(mash::mash_info_struct* mash_info,
                           void*                   containing_class_ptr,
                           mash::buffer_type       buffer);
    };

    // an action and the rules that allow it
    class basic_rule_data : public basic_rule_list {

    public:
        enum e_action : u16 {
            transition          = 0,
            transition_category = 1,
            defer               = 2, // no transition, the category gets its turn
            fail                = 3,
            end_layer           = 4
        };

        struct destination {
            string_hash name;   // a state, or a category for transition_category
            f32         weight; // the chance of being picked when there are several
        };

        destination* destinations;
        u16          num_destinations;
        e_action     the_action;

        state_id get_dest() const;
        void     process_action(request_data &result) const;

        // weighted by the positive weights from first on; -1 when there are none
        static i32 pick_destination(const destination* destinations, i32 num_destinations, i32 first);

        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
        void custom_unmash(mash::mash_info_struct* mash_info,
                           void*                   containing_class_ptr,
                           mash::buffer_type       buffer);
    };

    // hands the transition over to a shared transition group when its rules pass
    class trans_group_ref : public basic_rule_list {

    public:
        u32 trans_group; // index into the state machine's trans_group_list
        i32 order;       // checked in ascending order together with the owner's implicit or explicit transitions

        bool check_transition(request_data &result, e_trans_type type, rule_context context, category_id desired_category) const;
    };

    // takes the first action whose rules pass, once its own rules pass
    class trans_rule : public basic_rule_list {

    public:
        mash::vector
            <basic_rule_data> actions;

        const basic_rule_data* get_passing_action(const rule_context &context) const;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class implicit_trans : public trans_rule {

    public:
        i32 order;
    };

    // only considered when the requested category is this one
    class explicit_trans : public trans_rule {

    public:
        category_id category;
        i32         order;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // only considered when the state being left has this name, or this category when from_is_category
    class incoming_trans : public trans_rule {

    public:
        u32  from;
        bool from_is_category;
        u8   pad_021[0x03];

        // the state being left must match before the rules run
        bool check_incoming(const rule_context &context) const;
    };

    class scripted_state : public state {

    public:
        // scales the playback speed while the animation time is in [start, end)
        struct speed_range {
            f32 speed;
            f32 start;
            f32 end;
        };

        nal_anim_id               anim_name;
        mash::vector
            <trans_group_ref>     trans_group_ref_list;
        mash::vector
            <implicit_trans>      implicit_trans_list;
        mash::vector
            <explicit_trans>      explicit_trans_list;
        mash::vector_basic
            <speed_range>         speed_range_list;
        f32                       attack_start; // negative when the state has no attack region
        f32                       attack_end;
        u8                        pad_0f4[0x04];

        scripted_state() = default;

        // sub_723B40
        // inlined @ sub_723B80
        explicit scripted_state(mash::generating_vtable_version so_do_nothing) : state(so_do_nothing),
                                                                                 trans_group_ref_list(so_do_nothing),
                                                                                 implicit_trans_list(so_do_nothing),
                                                                                 explicit_trans_list(so_do_nothing),
                                                                                 speed_range_list(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        mash::virtual_types_key get_mocomp_type() const override;
        request_data            do_implicit_trans(animation_logic_system* the_als, state_machine* the_sm) const override;
        request_data            do_explicit_trans(animation_logic_system* the_als,
                                                  state_machine*          the_sm,
                                                  category_id             desired_category) const override;
        nal_anim_id             get_nal_anim_name() const override;
        f32                     get_secondary_playback_speed(f32 anim_time, f32 anim_duration) const override;

        static string_hash my_type_key;
    };

    class base_layer_scripted_state : public scripted_state {

    public:
        mash::virtual_types_key mocomp_type;
        u8                      pad_0fc[0x04];

        base_layer_scripted_state() = default;

        // sub_723B80
        explicit base_layer_scripted_state(mash::generating_vtable_version so_do_nothing) : scripted_state(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        mash::virtual_types_key get_mocomp_type() const override;

        static string_hash my_type_key;
    };

    class scripted_category : public category {

    public:
        mash::vector
            <trans_group_ref> trans_group_ref_list;
        mash::vector
            <implicit_trans>  implicit_trans_list;
        mash::vector
            <explicit_trans>  explicit_trans_list;
        mash::vector
            <incoming_trans>  incoming_trans_list;

        scripted_category() = default;

        // inlined @ sub_723DA0
        explicit scripted_category(mash::generating_vtable_version so_do_nothing) : category(so_do_nothing),
                                                                                    trans_group_ref_list(so_do_nothing),
                                                                                    implicit_trans_list(so_do_nothing),
                                                                                    explicit_trans_list(so_do_nothing),
                                                                                    incoming_trans_list(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        request_data do_implicit_trans(animation_logic_system* the_als, state_machine* the_sm) const override;
        request_data do_explicit_trans(animation_logic_system* the_als,
                                       state_machine*          the_sm,
                                       category_id             desired_category) const override;
        request_data do_incoming_trans(animation_logic_system* the_als,
                                       state_machine*          the_sm,
                                       bool                    unk) const override;

        static string_hash my_type_key;
    };

    class scripted_trans_group : public transition_group_base {

    public:
        mash::vector
            <trans_group_ref> trans_group_ref_list;
        mash::vector
            <implicit_trans>  implicit_trans_list;
        mash::vector
            <explicit_trans>  explicit_trans_list;
        string_hash           name;

        scripted_trans_group() = default;

        // inlined @ sub_723DA0
        explicit scripted_trans_group(mash::generating_vtable_version so_do_nothing) : transition_group_base(so_do_nothing),
                                                                                       trans_group_ref_list(so_do_nothing),
                                                                                       implicit_trans_list(so_do_nothing),
                                                                                       explicit_trans_list(so_do_nothing),
                                                                                       name(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void placeholder_function() override;

        bool check_transition(request_data &result, e_trans_type type, rule_context context, category_id desired_category) const;

        static string_hash my_type_key;
    };

    // the owner's transitions and trans group refs, merged by ascending order; the first to pass makes the request
    // inlined @ sub_5BD030
    // inlined @ sub_5D6E90
    // inlined @ sub_5D7090
    // inlined @ sub_5D75D0
    // inlined @ sub_5D7780
    template<typename T>
    bool check_transitions(const mash::vector<T>               &transitions,
                           const mash::vector<trans_group_ref> &refs,
                                 request_data                  &result,
                                 e_trans_type                   type,
                           const rule_context                  &context,
                                 category_id                    desired_category) {

        u32 transition_index = 0;
        u32 ref_index        = 0;

        for (u32 checked = 0; checked < transitions.size + refs.size; ++checked) {
            const T*               transition = transition_index < transitions.size ? transitions[transition_index] : nullptr;
            const trans_group_ref* ref        = ref_index        < refs.size        ? refs[ref_index]               : nullptr;

            if (transition && (!ref || transition->order < ref->order)) {
                ++transition_index;

                if constexpr (std::is_same_v<T, explicit_trans>) {
                    if (transition->category != desired_category)
                        continue;
                }

                if (!transition->check(context))
                    continue;

                if (const basic_rule_data* action = transition->get_passing_action(context)) {
                    action->process_action(result);
                    return true;
                }
            } else {
                ++ref_index;

                if (ref->check_transition(result, type, context, desired_category))
                    return true;
            }
        }

        return false;
    }

    ASSERT_SIZEOF  (basic_rule_list,            0x08);
    ASSERT_OFFSETOF(basic_rule_list, rules,     0x00);
    ASSERT_OFFSETOF(basic_rule_list, num_rules, 0x04);

    ASSERT_SIZEOF  (basic_rule_data,                   0x10);
    ASSERT_SIZEOF  (basic_rule_data::destination,      0x08);
    ASSERT_OFFSETOF(basic_rule_data, destinations,     0x08);
    ASSERT_OFFSETOF(basic_rule_data, num_destinations, 0x0C);
    ASSERT_OFFSETOF(basic_rule_data, the_action,       0x0E);

    ASSERT_SIZEOF  (trans_group_ref,              0x10);
    ASSERT_OFFSETOF(trans_group_ref, trans_group, 0x08);
    ASSERT_OFFSETOF(trans_group_ref, order,       0x0C);

    ASSERT_SIZEOF  (trans_rule,          0x1C);
    ASSERT_OFFSETOF(trans_rule, actions, 0x08);

    ASSERT_SIZEOF  (implicit_trans,        0x20);
    ASSERT_OFFSETOF(implicit_trans, order, 0x1C);

    ASSERT_SIZEOF  (explicit_trans,           0x24);
    ASSERT_OFFSETOF(explicit_trans, category, 0x1C);
    ASSERT_OFFSETOF(explicit_trans, order,    0x20);

    ASSERT_SIZEOF  (incoming_trans,                   0x24);
    ASSERT_OFFSETOF(incoming_trans, from,             0x1C);
    ASSERT_OFFSETOF(incoming_trans, from_is_category, 0x20);

    ASSERT_SIZEOF  (scripted_state::speed_range,          0x0C);
    ASSERT_SIZEOF  (scripted_state,                       0xF8);
    ASSERT_OFFSETOF(scripted_state, anim_name,            0x80);
    ASSERT_OFFSETOF(scripted_state, trans_group_ref_list, 0xA0);
    ASSERT_OFFSETOF(scripted_state, implicit_trans_list,  0xB4);
    ASSERT_OFFSETOF(scripted_state, explicit_trans_list,  0xC8);
    ASSERT_OFFSETOF(scripted_state, speed_range_list,     0xDC);
    ASSERT_OFFSETOF(scripted_state, attack_start,         0xEC);
    ASSERT_OFFSETOF(scripted_state, attack_end,           0xF0);

    ASSERT_SIZEOF  (base_layer_scripted_state,              0x100);
    ASSERT_OFFSETOF(base_layer_scripted_state, mocomp_type, 0x0F8);

    ASSERT_SIZEOF  (scripted_category,                       0x5C);
    ASSERT_OFFSETOF(scripted_category, trans_group_ref_list, 0x0C);
    ASSERT_OFFSETOF(scripted_category, implicit_trans_list,  0x20);
    ASSERT_OFFSETOF(scripted_category, explicit_trans_list,  0x34);
    ASSERT_OFFSETOF(scripted_category, incoming_trans_list,  0x48);

    ASSERT_SIZEOF  (scripted_trans_group,                       0x44);
    ASSERT_OFFSETOF(scripted_trans_group, trans_group_ref_list, 0x04);
    ASSERT_OFFSETOF(scripted_trans_group, implicit_trans_list,  0x18);
    ASSERT_OFFSETOF(scripted_trans_group, explicit_trans_list,  0x2C);
    ASSERT_OFFSETOF(scripted_trans_group, name,                 0x40);
}} // treyarch::als
