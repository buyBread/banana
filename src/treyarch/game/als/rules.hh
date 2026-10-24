#pragma once

#include "treyarch/game/als/param.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace als {
    enum class e_comparison : u32 {
        equal = 0,
        not_equal,
        greater_than,
        less_than,
        greater_than_or_equal_to,
        less_than_or_equal_to
    };

    // what a mashed basic_rule_list slot holds until its rule is unmashed
    enum class e_rule_class : u32 {
        number_compare_equal = 0,
        vector_component_number_compare_equal,
        number_compare_not_equal,
        vector_component_number_compare_not_equal,
        number_compare_greater_than,
        vector_component_number_compare_greater_than,
        number_compare_less_than,
        vector_component_number_compare_less_than,
        number_compare_greater_than_or_equal_to,
        vector_component_number_compare_greater_than_or_equal_to,
        number_compare_less_than_or_equal_to,
        vector_component_number_compare_less_than_or_equal_to,
        param_compare,
        vector_component_param_compare,
        vector_param_compare,
        vector_component_vector_param_compare,
        hash_equal,
        hash_not_equal,
        hash_param_equal,
        hash_param_not_equal,
        bounded_number,
        inverse_bounded_number,
        vector_component_bounded_number,
        vector_component_inverse_bounded_number,
        upgrade_number_compare,
        upgrade_bounded_number,
        upgrade_inverse_bounded_number,
        als_state_number_compare,
        als_state_vector_component_number_compare,
        als_state_hash_equal,
        als_state_hash_not_equal,
        als_state_bounded_number,
        als_state_inverse_bounded_number,
        als_state_vector_component_bounded_number,
        als_state_vector_component_inverse_bounded_number,
        als_state_number_param_compare,
        als_state_number_param_vector_component_compare,
        ai_number_compare,
        ai_vector_component_number_compare,
        ai_hash_equal,
        ai_hash_not_equal,
        ai_bounded_number,
        ai_inverse_bounded_number,
        ai_vector_component_bounded_number,
        ai_vector_component_inverse_bounded_number,
        ai_number_param_compare,
        ai_number_param_vector_component_compare,
        target_ai_number_compare,
        target_ai_vector_component_number_compare,
        target_ai_hash_equal,
        target_ai_hash_not_equal,
        target_ai_bounded_number,
        target_ai_inverse_bounded_number,
        target_ai_vector_component_bounded_number,
        target_ai_vector_component_inverse_bounded_number,
        target_ai_number_param_compare,
        target_ai_number_param_vector_component_compare,
        ai_inode_number_compare,
        ai_inode_vector_component_number_compare,
        ai_inode_hash_equal,
        ai_inode_hash_not_equal,
        ai_inode_bounded_number,
        ai_inode_inverse_bounded_number,
        ai_inode_vector_component_bounded_number,
        ai_inode_vector_component_inverse_bounded_number,
        ai_inode_number_param_compare,
        ai_inode_number_param_vector_component_compare,
        ifc_number_compare,
        ifc_vector_component_number_compare,
        ifc_hash_equal,
        ifc_hash_not_equal,
        ifc_bounded_number,
        ifc_inverse_bounded_number,
        ifc_vector_component_bounded_number,
        ifc_vector_component_inverse_bounded_number,
        ifc_number_param_compare,
        ifc_number_param_vector_component_compare
    };

    // rules only do something when they're evaluated, so mashed ones get their retail vtables from the type registry
    class transition_rule_base : public mash::mash_virtual_base {

    public:
        param_id param;
    };

    // ==== constant compares ==== //

    class constant_number_compare_rule : public transition_rule_base {

    public:
        f32 value;
    };

    class vector_component_constant_number_compare_rule : public constant_number_compare_rule {

    public:
        u32 component; // 0-2 x/y/z, then values derived from the whole vector
    };

    class number_compare_equal_rule                                     : public constant_number_compare_rule {};
    class number_compare_not_equal_rule                                 : public constant_number_compare_rule {};
    class number_compare_greater_than_rule                              : public constant_number_compare_rule {};
    class number_compare_less_than_rule                                 : public constant_number_compare_rule {};
    class number_compare_greater_than_or_equal_to_rule                  : public constant_number_compare_rule {};
    class number_compare_less_than_or_equal_to_rule                     : public constant_number_compare_rule {};
    class vector_component_number_compare_equal_rule                    : public vector_component_constant_number_compare_rule {};
    class vector_component_number_compare_not_equal_rule                : public vector_component_constant_number_compare_rule {};
    class vector_component_number_compare_greater_than_rule             : public vector_component_constant_number_compare_rule {};
    class vector_component_number_compare_less_than_rule                : public vector_component_constant_number_compare_rule {};
    class vector_component_number_compare_greater_than_or_equal_to_rule : public vector_component_constant_number_compare_rule {};
    class vector_component_number_compare_less_than_or_equal_to_rule    : public vector_component_constant_number_compare_rule {};

    // ==== parameter compares ==== //

    class param_compare_rule : public transition_rule_base {

    public:
        param_id     other_param;
        e_comparison comparison;
    };

    class vector_component_param_compare_rule : public param_compare_rule {

    public:
        u32 component;
    };

    class vector_param_compare_rule : public param_compare_rule {

    public:
        u32 other_component;
    };

    class vector_component_vector_param_compare_rule : public vector_param_compare_rule {

    public:
        u32 component;
    };

    // ==== hashes ==== //

    class hash_equal_rule : public transition_rule_base {

    public:
        string_hash value;
    };

    class hash_not_equal_rule : public hash_equal_rule {};

    class hash_param_equal_rule : public transition_rule_base {

    public:
        param_id other_param;
    };

    class hash_param_not_equal_rule : public hash_param_equal_rule {};

    // ==== bounds ==== //

    class bounded_number_rule : public transition_rule_base {

    public:
        f32 lower_bound;
        f32 upper_bound;
    };

    class inverse_bounded_number_rule : public bounded_number_rule {};

    class vector_component_bounded_number_rule : public bounded_number_rule {

    public:
        u32 component;
    };

    class vector_component_inverse_bounded_number_rule : public vector_component_bounded_number_rule {};

    // ==== upgrades, other state machines, the ai and its target ==== //

    // param is the hash of an upgrade or parameter name rather than an id
    class upgrade_number_compare_rule : public constant_number_compare_rule {

    public:
        e_comparison comparison;
    };

    class upgrade_bounded_number_rule         : public bounded_number_rule {};
    class upgrade_inverse_bounded_number_rule : public bounded_number_rule {};

    class als_state_number_compare_rule : public constant_number_compare_rule {

    public:
        e_comparison comparison;
    };

    class als_state_vector_component_number_compare_rule : public als_state_number_compare_rule {

    public:
        u32 component;
    };

    class als_state_hash_equal_rule                              : public hash_equal_rule {};
    class als_state_hash_not_equal_rule                          : public hash_not_equal_rule {};
    class als_state_bounded_number_rule                          : public bounded_number_rule {};
    class als_state_inverse_bounded_number_rule                  : public bounded_number_rule {};
    class als_state_vector_component_bounded_number_rule         : public vector_component_bounded_number_rule {};
    class als_state_vector_component_inverse_bounded_number_rule : public vector_component_bounded_number_rule {};
    class als_state_number_param_compare_rule                    : public param_compare_rule {};
    class als_state_number_param_vector_component_compare_rule   : public vector_param_compare_rule {};

    class ai_number_compare_rule : public constant_number_compare_rule {

    public:
        e_comparison comparison;
    };

    class ai_vector_component_number_compare_rule : public ai_number_compare_rule {

    public:
        u32 component;
    };

    class ai_hash_equal_rule                              : public hash_equal_rule {};
    class ai_hash_not_equal_rule                          : public hash_not_equal_rule {};
    class ai_bounded_number_rule                          : public bounded_number_rule {};
    class ai_inverse_bounded_number_rule                  : public bounded_number_rule {};
    class ai_vector_component_bounded_number_rule         : public vector_component_bounded_number_rule {};
    class ai_vector_component_inverse_bounded_number_rule : public vector_component_bounded_number_rule {};
    class ai_number_param_compare_rule                    : public param_compare_rule {};
    class ai_number_param_vector_component_compare_rule   : public vector_param_compare_rule {};

    class target_ai_number_compare_rule : public constant_number_compare_rule {

    public:
        e_comparison comparison;
    };

    class target_ai_vector_component_number_compare_rule : public target_ai_number_compare_rule {

    public:
        u32 component;
    };

    class target_ai_hash_equal_rule                              : public hash_equal_rule {};
    class target_ai_hash_not_equal_rule                          : public hash_not_equal_rule {};
    class target_ai_bounded_number_rule                          : public bounded_number_rule {};
    class target_ai_inverse_bounded_number_rule                  : public bounded_number_rule {};
    class target_ai_vector_component_bounded_number_rule         : public vector_component_bounded_number_rule {};
    class target_ai_vector_component_inverse_bounded_number_rule : public vector_component_bounded_number_rule {};
    class target_ai_number_param_compare_rule                    : public param_compare_rule {};
    class target_ai_number_param_vector_component_compare_rule   : public vector_param_compare_rule {};

    // ==== info nodes and entity interfaces ==== //

    // class_id names the ai info node or entity interface the parameter is read from
    class class_constant_number_compare_rule : public transition_rule_base {

    public:
        string_hash  class_id;
        f32          value;
        e_comparison comparison;
    };

    class class_vector_component_constant_number_compare_rule : public class_constant_number_compare_rule {

    public:
        u32 component;
    };

    class class_hash_compare_rule : public transition_rule_base {

    public:
        string_hash class_id;
        string_hash value;
    };

    class class_bounded_number_rule : public transition_rule_base {

    public:
        f32         lower_bound;
        f32         upper_bound;
        string_hash class_id;
    };

    class class_vector_component_bounded_number_rule : public class_bounded_number_rule {

    public:
        u32 component;
    };

    class class_param_compare_rule : public transition_rule_base {

    public:
        string_hash  class_id;
        param_id     other_param;
        e_comparison comparison;
    };

    class class_vector_component_param_compare_rule : public class_param_compare_rule {

    public:
        u32 other_component;
    };

    class ai_inode_number_compare_rule                              : public class_constant_number_compare_rule {};
    class ai_inode_vector_component_number_compare_rule             : public class_vector_component_constant_number_compare_rule {};
    class ai_inode_hash_equal_rule                                  : public class_hash_compare_rule {};
    class ai_inode_hash_not_equal_rule                              : public class_hash_compare_rule {};
    class ai_inode_bounded_number_rule                              : public class_bounded_number_rule {};
    class ai_inode_inverse_bounded_number_rule                      : public class_bounded_number_rule {};
    class ai_inode_vector_component_bounded_number_rule             : public class_vector_component_bounded_number_rule {};
    class ai_inode_vector_component_inverse_bounded_number_rule     : public class_vector_component_bounded_number_rule {};
    class ai_inode_number_param_compare_rule                        : public class_param_compare_rule {};
    class ai_inode_number_param_vector_component_compare_rule       : public class_vector_component_param_compare_rule {};

    class ifc_number_compare_rule                                   : public class_constant_number_compare_rule {};
    class ifc_vector_component_number_compare_rule                  : public class_vector_component_constant_number_compare_rule {};
    class ifc_hash_equal_rule                                       : public class_hash_compare_rule {};
    class ifc_hash_not_equal_rule                                   : public class_hash_compare_rule {};
    class ifc_bounded_number_rule                                   : public class_bounded_number_rule {};
    class ifc_inverse_bounded_number_rule                           : public class_bounded_number_rule {};
    class ifc_vector_component_bounded_number_rule                  : public class_vector_component_bounded_number_rule {};
    class ifc_vector_component_inverse_bounded_number_rule          : public class_vector_component_bounded_number_rule {};
    class ifc_number_param_compare_rule                             : public class_param_compare_rule {};
    class ifc_number_param_vector_component_compare_rule            : public class_vector_component_param_compare_rule {};

    ASSERT_SIZEOF  (transition_rule_base,        0x08);
    ASSERT_OFFSETOF(transition_rule_base, param, 0x04);

    ASSERT_SIZEOF  (constant_number_compare_rule,        0x0C);
    ASSERT_OFFSETOF(constant_number_compare_rule, value, 0x08);

    ASSERT_SIZEOF  (vector_component_constant_number_compare_rule,            0x10);
    ASSERT_OFFSETOF(vector_component_constant_number_compare_rule, component, 0x0C);

    ASSERT_SIZEOF  (param_compare_rule,              0x10);
    ASSERT_OFFSETOF(param_compare_rule, other_param, 0x08);
    ASSERT_OFFSETOF(param_compare_rule, comparison,  0x0C);

    ASSERT_SIZEOF  (vector_component_param_compare_rule,                   0x14);
    ASSERT_OFFSETOF(vector_component_param_compare_rule, component,        0x10);
    ASSERT_SIZEOF  (vector_param_compare_rule,                             0x14);
    ASSERT_OFFSETOF(vector_param_compare_rule, other_component,            0x10);
    ASSERT_SIZEOF  (vector_component_vector_param_compare_rule,            0x18);
    ASSERT_OFFSETOF(vector_component_vector_param_compare_rule, component, 0x14);

    ASSERT_SIZEOF  (hash_equal_rule,                    0x0C);
    ASSERT_OFFSETOF(hash_equal_rule, value,             0x08);
    ASSERT_SIZEOF  (hash_param_equal_rule,              0x0C);
    ASSERT_OFFSETOF(hash_param_equal_rule, other_param, 0x08);

    ASSERT_SIZEOF  (bounded_number_rule,                             0x10);
    ASSERT_OFFSETOF(bounded_number_rule, lower_bound,                0x08);
    ASSERT_OFFSETOF(bounded_number_rule, upper_bound,                0x0C);
    ASSERT_SIZEOF  (vector_component_bounded_number_rule,            0x14);
    ASSERT_OFFSETOF(vector_component_bounded_number_rule, component, 0x10);

    ASSERT_SIZEOF  (upgrade_number_compare_rule,                               0x10);
    ASSERT_OFFSETOF(upgrade_number_compare_rule, comparison,                   0x0C);
    ASSERT_SIZEOF  (als_state_vector_component_number_compare_rule,            0x14);
    ASSERT_OFFSETOF(als_state_vector_component_number_compare_rule, component, 0x10);
    ASSERT_SIZEOF  (ai_vector_component_number_compare_rule,                   0x14);
    ASSERT_OFFSETOF(ai_vector_component_number_compare_rule, component,        0x10);
    ASSERT_SIZEOF  (target_ai_vector_component_number_compare_rule,            0x14);
    ASSERT_OFFSETOF(target_ai_vector_component_number_compare_rule, component, 0x10);

    ASSERT_SIZEOF  (class_constant_number_compare_rule,                             0x14);
    ASSERT_OFFSETOF(class_constant_number_compare_rule, class_id,                   0x08);
    ASSERT_OFFSETOF(class_constant_number_compare_rule, value,                      0x0C);
    ASSERT_OFFSETOF(class_constant_number_compare_rule, comparison,                 0x10);
    ASSERT_SIZEOF  (class_vector_component_constant_number_compare_rule,            0x18);
    ASSERT_OFFSETOF(class_vector_component_constant_number_compare_rule, component, 0x14);

    ASSERT_SIZEOF  (class_hash_compare_rule,           0x10);
    ASSERT_OFFSETOF(class_hash_compare_rule, class_id, 0x08);
    ASSERT_OFFSETOF(class_hash_compare_rule, value,    0x0C);

    ASSERT_SIZEOF  (class_bounded_number_rule,                             0x14);
    ASSERT_OFFSETOF(class_bounded_number_rule, lower_bound,                0x08);
    ASSERT_OFFSETOF(class_bounded_number_rule, upper_bound,                0x0C);
    ASSERT_OFFSETOF(class_bounded_number_rule, class_id,                   0x10);
    ASSERT_SIZEOF  (class_vector_component_bounded_number_rule,            0x18);
    ASSERT_OFFSETOF(class_vector_component_bounded_number_rule, component, 0x14);

    ASSERT_SIZEOF  (class_param_compare_rule,                                   0x14);
    ASSERT_OFFSETOF(class_param_compare_rule, class_id,                         0x08);
    ASSERT_OFFSETOF(class_param_compare_rule, other_param,                      0x0C);
    ASSERT_OFFSETOF(class_param_compare_rule, comparison,                       0x10);
    ASSERT_SIZEOF  (class_vector_component_param_compare_rule,                  0x18);
    ASSERT_OFFSETOF(class_vector_component_param_compare_rule, other_component, 0x14);
}} // treyarch::als
