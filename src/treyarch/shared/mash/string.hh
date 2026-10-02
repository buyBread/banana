#pragma once

#include <cassert>
#include <cstdlib>
#include <cstring>

#include "treyarch/shared/mash/types.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace mash {
    class mash_info_struct;

    class string {

    public:
        enum fmtd { fmt };

        struct pos_t {
            i32 value;

            constexpr operator i32() const noexcept {
                return value;
            }
        };

        static constexpr i32    npos       = -1;
        static constexpr size_t max_length = 0xffff;

        i32   m_size;
        char* m_data;
        void* m_source_slab;

        string();

        explicit string(from_mash_in_place_constructor*) noexcept {
            // don't initialize the three serialized fields; they're already sitting in the image
            ++live_count();
        }

        string(const string &other);
        string(const string &other, i32 start, i32 count);
        string(const char* value);
        string(fmtd, const char* format, ...);

        explicit string(i32 value);
        explicit string(f32 value);

        ~string();

        void finalize(allocation_scope scope);

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash_info_struct* mash_info,
                    void*             containing_class_ptr,
                    buffer_type       buffer);

        i32 get_mash_sizeof() const {
            return sizeof(*this);
        }

        string &operator=(const string &other);
        string &operator=(const char* value);

        bool empty() const noexcept {
            return m_size == 0;
        }

        i32 size() const noexcept {
            return m_size;
        }

        i32 length() const noexcept {
            return m_size;
        }

        const char* c_str() const noexcept {
            return m_data;
        }

        char* data() noexcept {
            return m_data;
        }

        const char* data() const noexcept {
            return m_data;
        }

        char at(i32 index) const {
            return (*this)[index];
        }

        char operator[](i32 index) const {
            assert(index >= 0);
            assert(index <= m_size);

            return m_data[index];
        }

        // sub_A6CAB0
        void copy(const char* source, i32 source_length = npos) {
            update_guts(source, source_length);
        }

        void copy(const string &source) {
            update_guts(source.m_data, source.m_size);
        }

        void update_guts(const char* source, i32 source_length = npos);

        void append(const char* source, i32 source_length = npos);
        void append(char value);

        void append(const string &source) {
            append(source.m_data);
        }

        string &operator+=(const string &source);
        string &operator+=(const char* source);

        string &operator+=(char value) {
            append(value);

            return *this;
        }

        void clear();

        string truncate(i32 requested_size) {
            i32 new_size = requested_size;

            if (new_size < 0)
                new_size = 0;

            if (new_size > m_size)
                new_size = m_size;

            m_data[new_size] = '\0';
            m_size = new_size;

            return *this;
        }

        i32 compare(const char* other) const;

        bool is_equal(const char* other) const {
            return strncmp(m_data, other, max_length) == 0;
        }

        i32 find(const char* substring, i32 start = 0) const;
        i32 find(pos_t start, char value) const;
        i32 rfind(char value, i32 start = npos) const;

        i32 rfind(const char* substring) const {
            assert(substring != nullptr);

            i32 substring_length =
                (i32)strlen(substring);

            if (substring_length > m_size)
                return npos;

            for (i32 index = m_size - substring_length;index >= 0; --index) {
                if (memcmp(m_data + index, substring, substring_length) == 0)
                    return index;
            }

            return npos;
        }

        string &to_upper();
        string &to_lower();

        string substr(i32 start = 0, i32 count = npos) const;

        // sub_4288E0
        string slice(i32 start, i32 end) const {
            if (start < 0)
                start += m_size;

            if (end < 0)
                end += m_size;

            return substr(start, end - start);
        }

        string &remove_leading(const char* characters);
        string &remove_trailing(const char* characters);

        string &remove_surrounding_whitespace() {
            remove_leading(" \n\t\r");
            remove_trailing(" \n\t\r");

            return *this;
        }

        i32 to_int() const {
            return atoi(m_data);
        }

        f32 to_float() const {
            return strtof(m_data, nullptr);
            // return atof(m_data);
        }

        static string from_int(i32 value) {
            return string(value);
        }

        static string from_float( f32 value) {
            return string(value);
        }

        static i32 &live_count();

        friend bool operator==(const string &left, const string &right) {
            return left.is_equal(right.c_str());
        }

        friend bool operator!=(const string &left, const string &right) {
            return !(left == right);
        }

        friend bool operator==(const string &left, const char* right) {
            return left.is_equal(right);
        }

        friend bool operator==(const char* left, const string &right) {
            return right.is_equal(left);
        }

        friend bool operator!=(const string &left, const char* right) {
            return !left.is_equal(right);
        }

        friend bool operator!=(const char* left, const string &right) {
            return !(left == right);
        }

        /*
            compare() has reversed strcmp-style sign:
            +1 means *this < argument;
            -1 means *this > argument
        */

        friend bool operator<(const string &left, const string &right) {
            return left.compare(right.c_str()) == 1;
        }

        friend bool operator>(const string &left, const string &right) {
            return left.compare(right.c_str()) == -1;
        }

        friend bool operator<=(const string &left, const string &right) {
            return !(left > right);
        }

        friend bool operator>=(const string &left, const string &right) {
            return !(left < right);
        }

        friend string operator+(const string &left, const string &right);
        friend string operator+(const string &left, const char*   right);
        friend string operator+(const char*   left, const string &right);

    private:
        void initialize();
        void destroy_guts();
    };

    string operator+(const string &left, const string &right);
    string operator+(const string &left, const char*   right);
    string operator+(const char*   left, const string &right);

    ASSERT_SIZEOF  (string,                0x0C);
    ASSERT_OFFSETOF(string, m_size,        0x00);
    ASSERT_OFFSETOF(string, m_data,        0x04);
    ASSERT_OFFSETOF(string, m_source_slab, 0x08);

    namespace references {
        // every empty string points at this one shared null byte
        inline util::memory_reference<char> null_string_guts { 0x01126D24 };

        inline util::memory_reference<i32> string_count { 0x01126D20 };

        // strings that borrow their bytes from a mash image carry this in m_source_slab, those never get freed
        inline util::memory_reference<void*> string_source_slab { 0x00FBF24C };

        // 256 entries, zero for anything that has no upper case counterpart
        inline util::memory_reference<u8> upper_case_table { 0x00FBF250 };
    } // references
}} // treyarch::mash
