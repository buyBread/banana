#pragma once

#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/virtual_base.hh"
#include "treyarch/mm/mm.hh"
#include "game/arch_base_vhandle.hh"
#include "game/global.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/mutex.hh"
#include "game/variance_variable.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // what a parameter holds; the typed lookups ask for one
    enum e_param_type : u32 {
        param_type_float          = 0,
        param_type_int            = 1,
        param_type_hash           = 2,
        param_type_vector3d       = 3,
        param_type_float_variance = 4,
        param_type_string         = 5,
        param_type_class          = 6,
        param_type_entity         = 7,
        param_type_pointer        = 8
    };

    // one named parameter; the value lives in the subclass, and every read and write goes through the vtable
    class param_base : public mash::mash_virtual_base {

    public:
        e_param_type my_type;
        string_hash  my_name;

        explicit param_base(const string_hash &name);

        // inlined @ sub_723DA0
        explicit param_base(mash::generating_vtable_version so_do_nothing) : mash_virtual_base(so_do_nothing),
                                                                             my_name(so_do_nothing) {}

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        // sub_5FD110
        string_hash get_name() const {
            return my_name;
        }

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        virtual void        copy_data(const param_base* other) = 0; // other is the same type
        virtual param_base* clone() const = 0;

        virtual f32                           get_data_float() const;
        virtual i32                           get_data_int() const;
        virtual string_hash                   get_data_hash() const;
        virtual const vector3d               &get_data_vector3d() const;
        virtual const variance_variable<f32> &get_data_float_variance() const;
        virtual const mash::string           &get_data_fixedstring() const;
        virtual mash::mash_virtual_base*      get_data_class() const;
        virtual const arch_base_vhandle      &get_data_entity() const;
        virtual void*                         get_data_pointer() const;

        virtual void set_data_float(f32 value);
        virtual void set_data_int(i32 value);
        virtual void set_data_hash(string_hash value);
        virtual void set_data_vector3d(const vector3d &value);
        virtual void set_data_float_variance(const variance_variable<f32> &value);
        virtual void set_data_fixedstring(const mash::string &value);

        // msvc lays overloads out last to first, so the key form is slot 26 and the copy slot 27
        virtual void set_data_class(const mash::mash_virtual_base* object);
        virtual void set_data_class(mash::virtual_types_key the_class_to_make);

        virtual void set_data_entity(const arch_base_vhandle &value);
        virtual void set_data_pointer(const void* value);

        virtual bool deep_compare(const param_base* other) const = 0; // a different type never matches

        static string_hash my_type_key;
    };

    class param_float : public param_base {

    public:
        f32 value;

        param_float(const string_hash &name, f32 value);

        // inlined @ sub_723DA0
        explicit param_float(mash::generating_vtable_version so_do_nothing) : param_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void        copy_data(const param_base* other) override;
        param_base* clone() const override;
        f32         get_data_float() const override;
        void        set_data_float(f32 data) override;
        bool        deep_compare(const param_base* other) const override;

        static string_hash my_type_key;
    };

    class param_int : public param_base {

    public:
        i32 value;

        param_int(const string_hash &name, i32 value);

        // inlined @ sub_723DA0
        explicit param_int(mash::generating_vtable_version so_do_nothing) : param_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void        copy_data(const param_base* other) override;
        param_base* clone() const override;
        i32         get_data_int() const override;
        void        set_data_int(i32 data) override;
        bool        deep_compare(const param_base* other) const override;

        static string_hash my_type_key;
    };

    class param_hash : public param_base {

    public:
        string_hash value;

        param_hash(const string_hash &name, string_hash value);

        // inlined @ sub_723DA0
        explicit param_hash(mash::generating_vtable_version so_do_nothing) : param_base(so_do_nothing),
                                                                             value(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void        copy_data(const param_base* other) override;
        param_base* clone() const override;
        string_hash get_data_hash() const override;
        void        set_data_hash(string_hash data) override;
        bool        deep_compare(const param_base* other) const override;

        static string_hash my_type_key;
    };

    class param_vector3d : public param_base {

    public:
        vector3d value;

        param_vector3d(const string_hash &name, const vector3d &value);

        // inlined @ sub_723DA0
        explicit param_vector3d(mash::generating_vtable_version so_do_nothing) : param_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void            copy_data(const param_base* other) override;
        param_base*     clone() const override;
        const vector3d &get_data_vector3d() const override;
        void            set_data_vector3d(const vector3d &data) override;
        bool            deep_compare(const param_base* other) const override;

        static string_hash my_type_key;
    };

    class param_float_variance : public param_base {

    public:
        variance_variable<f32> value;

        param_float_variance(const string_hash &name, const variance_variable<f32> &value);

        // inlined @ sub_723DA0
        explicit param_float_variance(mash::generating_vtable_version so_do_nothing) : param_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void                          copy_data(const param_base* other) override;
        param_base*                   clone() const override;
        const variance_variable<f32> &get_data_float_variance() const override;
        void                          set_data_float_variance(const variance_variable<f32> &data) override;
        bool                          deep_compare(const param_base* other) const override; // rolls both values, so rarely true

        static string_hash my_type_key;
    };

    class param_string : public param_base {

    public:
        mash::string value;

        param_string(const string_hash &name, const mash::string &value);

        // inlined @ sub_723DA0
        explicit param_string(mash::generating_vtable_version so_do_nothing) : param_base(so_do_nothing),
                                                                               value(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void                copy_data(const param_base* other) override;
        param_base*         clone() const override;
        const mash::string &get_data_fixedstring() const override;
        void                set_data_fixedstring(const mash::string &data) override;
        bool                deep_compare(const param_base* other) const override;

        static string_hash my_type_key;
    };

    // what a param_class makes its objects with
    class param_block_creatable_class : public mash::mash_virtual_base {

    public:
        static param_block_creatable_class* create_subclass(mash::virtual_types_key the_class_to_make);
    };

    // owns its object: a copy of whatever it is given
    class param_class : public param_base {

    public:
        mash::mash_virtual_base* value;

        param_class(const string_hash &name, const mash::mash_virtual_base* object);

        // inlined @ sub_723DA0
        explicit param_class(mash::generating_vtable_version so_do_nothing) : param_base(so_do_nothing) {}

        ~param_class() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void                     copy_data(const param_base* other) override;
        param_base*              clone() const override;
        mash::mash_virtual_base* get_data_class() const override;
        void                     set_data_class(const mash::mash_virtual_base* object) override;
        void                     set_data_class(mash::virtual_types_key the_class_to_make) override;
        bool                     deep_compare(const param_base* other) const override;

        static string_hash my_type_key;
    };

    class param_entity : public param_base {

    public:
        arch_base_vhandle value;

        param_entity(const string_hash &name, arch_base_vhandle value);

        // inlined @ sub_723DA0
        explicit param_entity(mash::generating_vtable_version so_do_nothing) : param_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void                     copy_data(const param_base* other) override;
        param_base*              clone() const override;
        const arch_base_vhandle &get_data_entity() const override;
        void                     set_data_entity(const arch_base_vhandle &data) override;
        bool                     deep_compare(const param_base* other) const override;

        static string_hash my_type_key;
    };

    class param_pointer : public param_base {

    public:
        void* value;

        param_pointer(const string_hash &name, void* value);

        // inlined @ sub_723DA0
        explicit param_pointer(mash::generating_vtable_version so_do_nothing) : param_base(so_do_nothing) {}

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void        copy_data(const param_base* other) override;
        param_base* clone() const override;
        void*       get_data_pointer() const override;
        void        set_data_pointer(const void* data) override;
        bool        deep_compare(const param_base* other) const override;

        static string_hash my_type_key;
    };

    // a list of parameters sorted by name; every read and write takes the lock
    class alignas(8) param_block {

    public:
                mash::vector
                    <param_base>*     param_array;
                u32                   reserved_004;
        mutable engine_recursive_lock lock;
        mutable param_base*           cached_param;           // the last hit, checked before searching
                bool                  param_array_is_dynamic; // the list was allocated on the heap rather than unmashed
                u8                    pad_01d[0x03];

        ~param_block();

        void set_pb_float         (string_hash parameter, f32 value);
        void set_pb_int           (string_hash parameter, i32 value);
        void set_pb_hash          (string_hash parameter, const string_hash &value);
        void set_pb_fixedstring   (string_hash parameter, const char* value);
        void set_pb_vector3d      (string_hash parameter, const vector3d &value);
        void set_pb_float_variance(string_hash parameter, const variance_variable<f32> &value);
        void set_pb_entity        (string_hash parameter, const arch_base_vhandle &value);

        // a missing parameter reads as zero
        f32                      get_pb_float         (string_hash parameter) const;
        i32                      get_pb_int           (string_hash parameter) const;
        string_hash              get_pb_hash          (string_hash parameter) const;
        const char*              get_pb_fixedstring   (string_hash parameter) const;
        vector3d                 get_pb_vector3d      (string_hash parameter) const;
        variance_variable<f32>   get_pb_float_variance(string_hash parameter) const;
        mash::mash_virtual_base* get_pb_class         (string_hash parameter) const;
        arch_base_vhandle        get_pb_entity        (string_hash parameter) const;

        f32                      get_optional_pb_float         (string_hash parameter, f32 default_value, bool* exists) const;
        i32                      get_optional_pb_int           (string_hash parameter, i32 default_value, bool* exists) const;
        string_hash              get_optional_pb_hash          (string_hash parameter, string_hash default_value, bool* exists) const;
        const char*              get_optional_pb_fixedstring   (string_hash parameter, const char* default_value, bool* exists) const;
        vector3d                 get_optional_pb_vector3d      (string_hash parameter, const vector3d &default_value, bool* exists) const;
        variance_variable<f32>   get_optional_pb_float_variance(string_hash parameter, const variance_variable<f32> &default_value, bool* exists) const;
        mash::mash_virtual_base* get_optional_pb_class         (string_hash parameter, mash::mash_virtual_base* default_value, bool* exists) const;
        arch_base_vhandle        get_optional_pb_entity        (string_hash parameter, arch_base_vhandle default_value, bool* exists) const;

        bool         does_parameter_exist   (string_hash parameter) const;
        e_param_type get_parameter_data_type(string_hash parameter) const; // the parameter must exist

        void copy_from_pb_override(const param_block &old_data);
        void reserve(u32 max_params);

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);

    protected:
        // an existing parameter takes the value; otherwise a clone goes in by name, or last when you know what you're doing
        void add_param(const param_base*                   param,
                       only_if_you_know_what_you_are_doing ignore_param_search_and_sort);

    private:
        param_base* common_find_data(string_hash parameter) const;

        void allocate_param_array();
    };

    ASSERT_SIZEOF  (param_base,          0x0C);
    ASSERT_OFFSETOF(param_base, my_type, 0x04);
    ASSERT_OFFSETOF(param_base, my_name, 0x08);

    ASSERT_SIZEOF  (param_float,                 0x10);
    ASSERT_SIZEOF  (param_int,                   0x10);
    ASSERT_SIZEOF  (param_hash,                  0x10);
    ASSERT_SIZEOF  (param_vector3d,              0x18);
    ASSERT_SIZEOF  (param_float_variance,        0x14);
    ASSERT_SIZEOF  (param_string,                0x18);
    ASSERT_SIZEOF  (param_class,                 0x10);
    ASSERT_SIZEOF  (param_entity,                0x10);
    ASSERT_SIZEOF  (param_pointer,               0x10);
    ASSERT_OFFSETOF(param_float,          value, 0x0C);
    ASSERT_OFFSETOF(param_vector3d,       value, 0x0C);
    ASSERT_OFFSETOF(param_float_variance, value, 0x0C);
    ASSERT_OFFSETOF(param_string,         value, 0x0C);

    ASSERT_SIZEOF  (param_block,                         0x20);
    ASSERT_OFFSETOF(param_block, param_array,            0x00);
    ASSERT_OFFSETOF(param_block, lock,                   0x08);
    ASSERT_OFFSETOF(param_block, cached_param,           0x18);
    ASSERT_OFFSETOF(param_block, param_array_is_dynamic, 0x1C);
} // treyarch
