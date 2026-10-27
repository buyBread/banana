#pragma once

#include "game/als/param.hh"
#include "game/wds/ai/info_node.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/virtual_base.hh"
#include "treyarch/mm/heap.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;
    class ai_core;
    class entity;
}

namespace treyarch { namespace als {
    class animation_logic_system;
    class state_machine;

    // what rules read their parameters from
    struct rule_context {
        animation_logic_system* the_als;
        state_machine*          the_sm;
    };

    enum class e_comparison : u32 {
        equal = 0,
        not_equal,
        greater_than,
        less_than,
        greater_than_or_equal_to,
        less_than_or_equal_to
    };

    // anything else fails
    bool compare_numbers(e_comparison comparison, f32 lhs, f32 rhs);

    // where the prefixed rules read their parameter, named by hash; missing or mistyped reads as zero
    f32         get_upgrade_float_param             (string_hash name);
    f32         get_als_state_float_param           (state_machine* the_sm, string_hash name);
    f32         get_als_state_vector_component_param(state_machine* the_sm, string_hash name, e_vector_component component);
    string_hash get_als_state_hash_param            (state_machine* the_sm, string_hash name);
    f32         get_ai_float_param                  (ai_core*       core,   string_hash name);
    f32         get_ai_vector_component_param       (ai_core*       core,   string_hash name, e_vector_component component);
    string_hash get_ai_hash_param                   (ai_core*       core,   string_hash name);

    // the class rules read an info node's block (class_id holds its type) or an entity interface's parameters (class_id names it)
    f32         get_ai_inode_float_param           (ai_core* core,  e_info_node_type type, string_hash name);
    f32         get_ai_inode_vector_component_param(ai_core* core,  e_info_node_type type, string_hash name, e_vector_component component);
    string_hash get_ai_inode_hash_param            (ai_core* core,  e_info_node_type type, string_hash name);
    f32         get_ifc_float_param                (entity*  owner, string_hash      ifc,  string_hash name);
    f32         get_ifc_vector_component_param     (entity*  owner, string_hash      ifc,  string_hash name, e_vector_component component);
    string_hash get_ifc_hash_param                 (entity*  owner, string_hash      ifc,  string_hash name);

    // the hero's target, else the ai's
    ai_core* get_target_ai_core(actor* the_actor);

    mash::string get_vector_component_string(e_vector_component component);

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

    class transition_rule_base : public mash::mash_virtual_base {

    public:
        param_id param;

        transition_rule_base() = default;

        // inlined @ sub_723DA0
        explicit transition_rule_base(mash::generating_vtable_version so_do_nothing) : mash_virtual_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        // retail passes this on the stack, so every override keeps the convention
        virtual bool __stdcall evaluate(const rule_context &context) const;

        // inlined into the deleting destructors
        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }

        static string_hash my_type_key;
    };

    // ==== constant compares ==== //

    class constant_number_compare_rule : public transition_rule_base {

    public:
        f32 value;

        constant_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit constant_number_compare_rule(mash::generating_vtable_version so_do_nothing) : transition_rule_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        static string_hash my_type_key;
    };

    class vector_component_constant_number_compare_rule : public constant_number_compare_rule {

    public:
        e_vector_component component;

        vector_component_constant_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit vector_component_constant_number_compare_rule(mash::generating_vtable_version so_do_nothing) : constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        static string_hash my_type_key;
    };

    class number_compare_equal_rule : public constant_number_compare_rule {

    public:
        number_compare_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit number_compare_equal_rule(mash::generating_vtable_version so_do_nothing) : constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    class number_compare_not_equal_rule : public constant_number_compare_rule {

    public:
        number_compare_not_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit number_compare_not_equal_rule(mash::generating_vtable_version so_do_nothing) : constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    class number_compare_greater_than_rule : public constant_number_compare_rule {

    public:
        number_compare_greater_than_rule() = default;

        // inlined @ sub_723DA0
        explicit number_compare_greater_than_rule(mash::generating_vtable_version so_do_nothing) : constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    class number_compare_less_than_rule : public constant_number_compare_rule {

    public:
        number_compare_less_than_rule() = default;

        // inlined @ sub_723DA0
        explicit number_compare_less_than_rule(mash::generating_vtable_version so_do_nothing) : constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    class number_compare_greater_than_or_equal_to_rule : public constant_number_compare_rule {

    public:
        number_compare_greater_than_or_equal_to_rule() = default;

        // inlined @ sub_723DA0
        explicit number_compare_greater_than_or_equal_to_rule(mash::generating_vtable_version so_do_nothing) : constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    class number_compare_less_than_or_equal_to_rule : public constant_number_compare_rule {

    public:
        number_compare_less_than_or_equal_to_rule() = default;

        // inlined @ sub_723DA0
        explicit number_compare_less_than_or_equal_to_rule(mash::generating_vtable_version so_do_nothing) : constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    class vector_component_number_compare_equal_rule : public vector_component_constant_number_compare_rule {

    public:
        vector_component_number_compare_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit vector_component_number_compare_equal_rule(mash::generating_vtable_version so_do_nothing) : vector_component_constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    class vector_component_number_compare_not_equal_rule : public vector_component_constant_number_compare_rule {

    public:
        vector_component_number_compare_not_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit vector_component_number_compare_not_equal_rule(mash::generating_vtable_version so_do_nothing) : vector_component_constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    class vector_component_number_compare_greater_than_rule : public vector_component_constant_number_compare_rule {

    public:
        vector_component_number_compare_greater_than_rule() = default;

        // inlined @ sub_723DA0
        explicit vector_component_number_compare_greater_than_rule(mash::generating_vtable_version so_do_nothing) : vector_component_constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    class vector_component_number_compare_less_than_rule : public vector_component_constant_number_compare_rule {

    public:
        vector_component_number_compare_less_than_rule() = default;

        // inlined @ sub_723DA0
        explicit vector_component_number_compare_less_than_rule(mash::generating_vtable_version so_do_nothing) : vector_component_constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    class vector_component_number_compare_greater_than_or_equal_to_rule : public vector_component_constant_number_compare_rule {

    public:
        vector_component_number_compare_greater_than_or_equal_to_rule() = default;

        // inlined @ sub_723DA0
        explicit vector_component_number_compare_greater_than_or_equal_to_rule(mash::generating_vtable_version so_do_nothing) : vector_component_constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    class vector_component_number_compare_less_than_or_equal_to_rule : public vector_component_constant_number_compare_rule {

    public:
        vector_component_number_compare_less_than_or_equal_to_rule() = default;

        // inlined @ sub_723DA0
        explicit vector_component_number_compare_less_than_or_equal_to_rule(mash::generating_vtable_version so_do_nothing) : vector_component_constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    // ==== parameter compares ==== //

    class param_compare_rule : public transition_rule_base {

    public:
        param_id     other_param;
        e_comparison comparison;

        param_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit param_compare_rule(mash::generating_vtable_version so_do_nothing) : transition_rule_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class vector_component_param_compare_rule : public param_compare_rule {

    public:
        e_vector_component component;

        vector_component_param_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit vector_component_param_compare_rule(mash::generating_vtable_version so_do_nothing) : param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class vector_param_compare_rule : public param_compare_rule {

    public:
        e_vector_component other_component;

        vector_param_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit vector_param_compare_rule(mash::generating_vtable_version so_do_nothing) : param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class vector_component_vector_param_compare_rule : public vector_param_compare_rule {

    public:
        e_vector_component component;

        vector_component_vector_param_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit vector_component_vector_param_compare_rule(mash::generating_vtable_version so_do_nothing) : vector_param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    // ==== hashes ==== //

    class hash_equal_rule : public transition_rule_base {

    public:
        string_hash value;

        hash_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit hash_equal_rule(mash::generating_vtable_version so_do_nothing) : transition_rule_base(so_do_nothing),
                                                                                  value(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class hash_not_equal_rule : public hash_equal_rule {

    public:
        hash_not_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit hash_not_equal_rule(mash::generating_vtable_version so_do_nothing) : hash_equal_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    class hash_param_equal_rule : public transition_rule_base {

    public:
        param_id other_param;

        hash_param_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit hash_param_equal_rule(mash::generating_vtable_version so_do_nothing) : transition_rule_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class hash_param_not_equal_rule : public hash_param_equal_rule {

    public:
        hash_param_not_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit hash_param_not_equal_rule(mash::generating_vtable_version so_do_nothing) : hash_param_equal_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_operator_string() const;

        static string_hash my_type_key;
    };

    // ==== bounds ==== //

    class bounded_number_rule : public transition_rule_base {

    public:
        f32 lower_bound;
        f32 upper_bound;

        bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit bounded_number_rule(mash::generating_vtable_version so_do_nothing) : transition_rule_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class inverse_bounded_number_rule : public bounded_number_rule {

    public:
        inverse_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit inverse_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class vector_component_bounded_number_rule : public bounded_number_rule {

    public:
        e_vector_component component;

        vector_component_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit vector_component_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class vector_component_inverse_bounded_number_rule : public vector_component_bounded_number_rule {

    public:
        vector_component_inverse_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit vector_component_inverse_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : vector_component_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    // ==== upgrades, other state machines, the ai and its target ==== //

    // param is the hash of an upgrade or parameter name rather than an id

    class upgrade_number_compare_rule : public constant_number_compare_rule {

    public:
        e_comparison comparison;

        upgrade_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit upgrade_number_compare_rule(mash::generating_vtable_version so_do_nothing) : constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class upgrade_bounded_number_rule : public bounded_number_rule {

    public:
        upgrade_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit upgrade_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class upgrade_inverse_bounded_number_rule : public bounded_number_rule {

    public:
        upgrade_inverse_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit upgrade_inverse_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class als_state_number_compare_rule : public constant_number_compare_rule {

    public:
        e_comparison comparison;

        als_state_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit als_state_number_compare_rule(mash::generating_vtable_version so_do_nothing) : constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class als_state_vector_component_number_compare_rule : public als_state_number_compare_rule {

    public:
        e_vector_component component;

        als_state_vector_component_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit als_state_vector_component_number_compare_rule(mash::generating_vtable_version so_do_nothing) : als_state_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_component_string() const;

        static string_hash my_type_key;
    };

    class als_state_hash_equal_rule : public hash_equal_rule {

    public:
        als_state_hash_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit als_state_hash_equal_rule(mash::generating_vtable_version so_do_nothing) : hash_equal_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class als_state_hash_not_equal_rule : public hash_not_equal_rule {

    public:
        als_state_hash_not_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit als_state_hash_not_equal_rule(mash::generating_vtable_version so_do_nothing) : hash_not_equal_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class als_state_bounded_number_rule : public bounded_number_rule {

    public:
        als_state_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit als_state_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class als_state_inverse_bounded_number_rule : public bounded_number_rule {

    public:
        als_state_inverse_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit als_state_inverse_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class als_state_vector_component_bounded_number_rule : public vector_component_bounded_number_rule {

    public:
        als_state_vector_component_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit als_state_vector_component_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : vector_component_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class als_state_vector_component_inverse_bounded_number_rule : public vector_component_bounded_number_rule {

    public:
        als_state_vector_component_inverse_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit als_state_vector_component_inverse_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : vector_component_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class als_state_number_param_compare_rule : public param_compare_rule {

    public:
        als_state_number_param_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit als_state_number_param_compare_rule(mash::generating_vtable_version so_do_nothing) : param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class als_state_number_param_vector_component_compare_rule : public vector_param_compare_rule {

    public:
        als_state_number_param_vector_component_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit als_state_number_param_vector_component_compare_rule(mash::generating_vtable_version so_do_nothing) : vector_param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class ai_number_compare_rule : public constant_number_compare_rule {

    public:
        e_comparison comparison;

        ai_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_number_compare_rule(mash::generating_vtable_version so_do_nothing) : constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ai_vector_component_number_compare_rule : public ai_number_compare_rule {

    public:
        e_vector_component component;

        ai_vector_component_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_vector_component_number_compare_rule(mash::generating_vtable_version so_do_nothing) : ai_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_component_string() const;

        static string_hash my_type_key;
    };

    class ai_hash_equal_rule : public hash_equal_rule {

    public:
        ai_hash_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_hash_equal_rule(mash::generating_vtable_version so_do_nothing) : hash_equal_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class ai_hash_not_equal_rule : public hash_not_equal_rule {

    public:
        ai_hash_not_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_hash_not_equal_rule(mash::generating_vtable_version so_do_nothing) : hash_not_equal_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class ai_bounded_number_rule : public bounded_number_rule {

    public:
        ai_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class ai_inverse_bounded_number_rule : public bounded_number_rule {

    public:
        ai_inverse_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_inverse_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ai_vector_component_bounded_number_rule : public vector_component_bounded_number_rule {

    public:
        ai_vector_component_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_vector_component_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : vector_component_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class ai_vector_component_inverse_bounded_number_rule : public vector_component_bounded_number_rule {

    public:
        ai_vector_component_inverse_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_vector_component_inverse_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : vector_component_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ai_number_param_compare_rule : public param_compare_rule {

    public:
        ai_number_param_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_number_param_compare_rule(mash::generating_vtable_version so_do_nothing) : param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class ai_number_param_vector_component_compare_rule : public vector_param_compare_rule {

    public:
        ai_number_param_vector_component_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_number_param_vector_component_compare_rule(mash::generating_vtable_version so_do_nothing) : vector_param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class target_ai_number_compare_rule : public constant_number_compare_rule {

    public:
        e_comparison comparison;

        target_ai_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit target_ai_number_compare_rule(mash::generating_vtable_version so_do_nothing) : constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class target_ai_vector_component_number_compare_rule : public target_ai_number_compare_rule {

    public:
        e_vector_component component;

        target_ai_vector_component_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit target_ai_vector_component_number_compare_rule(mash::generating_vtable_version so_do_nothing) : target_ai_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_component_string() const;

        static string_hash my_type_key;
    };

    class target_ai_hash_equal_rule : public hash_equal_rule {

    public:
        target_ai_hash_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit target_ai_hash_equal_rule(mash::generating_vtable_version so_do_nothing) : hash_equal_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class target_ai_hash_not_equal_rule : public hash_not_equal_rule {

    public:
        target_ai_hash_not_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit target_ai_hash_not_equal_rule(mash::generating_vtable_version so_do_nothing) : hash_not_equal_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class target_ai_bounded_number_rule : public bounded_number_rule {

    public:
        target_ai_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit target_ai_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class target_ai_inverse_bounded_number_rule : public bounded_number_rule {

    public:
        target_ai_inverse_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit target_ai_inverse_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class target_ai_vector_component_bounded_number_rule : public vector_component_bounded_number_rule {

    public:
        target_ai_vector_component_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit target_ai_vector_component_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : vector_component_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class target_ai_vector_component_inverse_bounded_number_rule : public vector_component_bounded_number_rule {

    public:
        target_ai_vector_component_inverse_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit target_ai_vector_component_inverse_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : vector_component_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class target_ai_number_param_compare_rule : public param_compare_rule {

    public:
        target_ai_number_param_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit target_ai_number_param_compare_rule(mash::generating_vtable_version so_do_nothing) : param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    class target_ai_number_param_vector_component_compare_rule : public vector_param_compare_rule {

    public:
        target_ai_number_param_vector_component_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit target_ai_number_param_vector_component_compare_rule(mash::generating_vtable_version so_do_nothing) : vector_param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        virtual mash::string get_param_string() const;
        virtual mash::string get_prefix_string() const;

        static string_hash my_type_key;
    };

    // ==== info nodes and entity interfaces ==== //

    class class_constant_number_compare_rule : public transition_rule_base {

    public:
        string_hash  class_id;
        f32          value;
        e_comparison comparison;

        class_constant_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit class_constant_number_compare_rule(mash::generating_vtable_version so_do_nothing) : transition_rule_base(so_do_nothing),
                                                                                                     class_id(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        static string_hash my_type_key;
    };

    class class_vector_component_constant_number_compare_rule : public class_constant_number_compare_rule {

    public:
        e_vector_component component;

        class_vector_component_constant_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit class_vector_component_constant_number_compare_rule(mash::generating_vtable_version so_do_nothing) : class_constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        virtual mash::string get_component_string() const;

        static string_hash my_type_key;
    };

    class class_hash_compare_rule : public transition_rule_base {

    public:
        string_hash class_id;
        string_hash value;

        class_hash_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit class_hash_compare_rule(mash::generating_vtable_version so_do_nothing) : transition_rule_base(so_do_nothing),
                                                                                          class_id(so_do_nothing),
                                                                                          value(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        static string_hash my_type_key;
    };

    class class_bounded_number_rule : public transition_rule_base {

    public:
        f32         lower_bound;
        f32         upper_bound;
        string_hash class_id;

        class_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit class_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : transition_rule_base(so_do_nothing),
                                                                                            class_id(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        static string_hash my_type_key;
    };

    class class_vector_component_bounded_number_rule : public class_bounded_number_rule {

    public:
        e_vector_component component;

        class_vector_component_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit class_vector_component_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : class_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        virtual mash::string get_component_string() const;

        static string_hash my_type_key;
    };

    class class_param_compare_rule : public transition_rule_base {

    public:
        string_hash  class_id;
        param_id     other_param;
        e_comparison comparison;

        class_param_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit class_param_compare_rule(mash::generating_vtable_version so_do_nothing) : transition_rule_base(so_do_nothing),
                                                                                           class_id(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        static string_hash my_type_key;
    };

    class class_vector_component_param_compare_rule : public class_param_compare_rule {

    public:
        e_vector_component other_component;

        class_vector_component_param_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit class_vector_component_param_compare_rule(mash::generating_vtable_version so_do_nothing) : class_param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        virtual mash::string get_component_string() const;

        static string_hash my_type_key;
    };

    class ai_inode_number_compare_rule : public class_constant_number_compare_rule {

    public:
        ai_inode_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_inode_number_compare_rule(mash::generating_vtable_version so_do_nothing) : class_constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ai_inode_vector_component_number_compare_rule : public class_vector_component_constant_number_compare_rule {

    public:
        ai_inode_vector_component_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_inode_vector_component_number_compare_rule(mash::generating_vtable_version so_do_nothing) : class_vector_component_constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ai_inode_hash_equal_rule : public class_hash_compare_rule {

    public:
        ai_inode_hash_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_inode_hash_equal_rule(mash::generating_vtable_version so_do_nothing) : class_hash_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ai_inode_hash_not_equal_rule : public class_hash_compare_rule {

    public:
        ai_inode_hash_not_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_inode_hash_not_equal_rule(mash::generating_vtable_version so_do_nothing) : class_hash_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ai_inode_bounded_number_rule : public class_bounded_number_rule {

    public:
        ai_inode_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_inode_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : class_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ai_inode_inverse_bounded_number_rule : public class_bounded_number_rule {

    public:
        ai_inode_inverse_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_inode_inverse_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : class_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ai_inode_vector_component_bounded_number_rule : public class_vector_component_bounded_number_rule {

    public:
        ai_inode_vector_component_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_inode_vector_component_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : class_vector_component_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ai_inode_vector_component_inverse_bounded_number_rule : public class_vector_component_bounded_number_rule {

    public:
        ai_inode_vector_component_inverse_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_inode_vector_component_inverse_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : class_vector_component_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ai_inode_number_param_compare_rule : public class_param_compare_rule {

    public:
        ai_inode_number_param_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_inode_number_param_compare_rule(mash::generating_vtable_version so_do_nothing) : class_param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ai_inode_number_param_vector_component_compare_rule : public class_vector_component_param_compare_rule {

    public:
        ai_inode_number_param_vector_component_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit ai_inode_number_param_vector_component_compare_rule(mash::generating_vtable_version so_do_nothing) : class_vector_component_param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ifc_number_compare_rule : public class_constant_number_compare_rule {

    public:
        ifc_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit ifc_number_compare_rule(mash::generating_vtable_version so_do_nothing) : class_constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ifc_vector_component_number_compare_rule : public class_vector_component_constant_number_compare_rule {

    public:
        ifc_vector_component_number_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit ifc_vector_component_number_compare_rule(mash::generating_vtable_version so_do_nothing) : class_vector_component_constant_number_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ifc_hash_equal_rule : public class_hash_compare_rule {

    public:
        ifc_hash_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit ifc_hash_equal_rule(mash::generating_vtable_version so_do_nothing) : class_hash_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ifc_hash_not_equal_rule : public class_hash_compare_rule {

    public:
        ifc_hash_not_equal_rule() = default;

        // inlined @ sub_723DA0
        explicit ifc_hash_not_equal_rule(mash::generating_vtable_version so_do_nothing) : class_hash_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ifc_bounded_number_rule : public class_bounded_number_rule {

    public:
        ifc_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit ifc_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : class_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ifc_inverse_bounded_number_rule : public class_bounded_number_rule {

    public:
        ifc_inverse_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit ifc_inverse_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : class_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ifc_vector_component_bounded_number_rule : public class_vector_component_bounded_number_rule {

    public:
        ifc_vector_component_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit ifc_vector_component_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : class_vector_component_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ifc_vector_component_inverse_bounded_number_rule : public class_vector_component_bounded_number_rule {

    public:
        ifc_vector_component_inverse_bounded_number_rule() = default;

        // inlined @ sub_723DA0
        explicit ifc_vector_component_inverse_bounded_number_rule(mash::generating_vtable_version so_do_nothing) : class_vector_component_bounded_number_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ifc_number_param_compare_rule : public class_param_compare_rule {

    public:
        ifc_number_param_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit ifc_number_param_compare_rule(mash::generating_vtable_version so_do_nothing) : class_param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

    class ifc_number_param_vector_component_compare_rule : public class_vector_component_param_compare_rule {

    public:
        ifc_number_param_vector_component_compare_rule() = default;

        // inlined @ sub_723DA0
        explicit ifc_number_param_vector_component_compare_rule(mash::generating_vtable_version so_do_nothing) : class_vector_component_param_compare_rule(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool __stdcall evaluate(const rule_context &context) const override;

        static string_hash my_type_key;
    };

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
