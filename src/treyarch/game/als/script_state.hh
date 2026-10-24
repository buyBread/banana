#pragma once

#include "treyarch/game/als/als.hh"
#include "treyarch/game/als/rules.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace als {
    // passes when every rule passes
    class basic_rule_list {

    public:
        transition_rule_base** rules;
        u32                    num_rules;

        ~basic_rule_list();

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
            transition_category = 1
        };

        struct destination {
            string_hash name;   // a state, or a category for transition_category
            f32         weight; // the chance of being picked when there are several
        };

        destination* destinations;
        u16          num_destinations;
        e_action     the_action;

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
        u32 order;       // checked in ascending order together with the owner's implicit or explicit transitions
    };

    // takes the first action whose rules pass, once its own rules pass
    class trans_rule : public basic_rule_list {

    public:
        mash::vector
            <basic_rule_data> actions;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class implicit_trans : public trans_rule {

    public:
        u32 order;
    };

    // only considered when the requested category is this one
    class explicit_trans : public trans_rule {

    public:
        category_id category;
        u32         order;

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
    };

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
}} // treyarch::als
