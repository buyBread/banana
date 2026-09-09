#pragma once

#include <cstdarg>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "treyarch/shared/mash/types.hh"
#include "util/gimmie/fn.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace mash {
    class mash_info_struct;

    class string {

public:
    struct fmtd { i32 value; };

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

    // the game points every empty string at one shared null byte
    inline static char* const null = (char*)0x01126D24;

    string() noexcept: m_size(0), m_data(null), m_source_slab(nullptr) {
        util::gimmie::fn<void(__thiscall*)(string*)>(0x00A6CC70)(this);
    }

    explicit string(from_mash_in_place_constructor*) noexcept {
        // don't initialize the three serialized fields; they're already sitting in the image
        ++live_count();
    }

    string(const string &other) : m_size(0), m_data(null), m_source_slab(nullptr) {
        util::gimmie::fn<string*(__thiscall*)(string*, const string*)>(0x00A6CE30)(this, &other);
    }

    string(const char* value) : m_size(0), m_data(null), m_source_slab(nullptr) {
        util::gimmie::fn<string*(__thiscall*)(string*, const char*)>(0x00A6CE00)(this, value);
    }

    explicit string(i32 value) : m_size(0), m_data(null), m_source_slab(nullptr) {
        util::gimmie::fn<string*(__thiscall*)(string*, i32)>(0x00A6CD50)(this, value);
    }

    explicit string(f32 value) : m_size(0), m_data(null), m_source_slab(nullptr) {
        util::gimmie::fn<string*(__thiscall*)(string*, f32)>(0x00A6CDA0)(this, value);
    }

    string(fmtd, const char* format, ...) : string() {
        va_list arguments;
        va_start(arguments, format);
        assign_format(format, arguments);
        va_end(arguments);
    }

    string(i32, const char* format, ...) : string() {
        va_list arguments;
        va_start(arguments, format);
        assign_format(format, arguments);
        va_end(arguments);
    }

    ~string() {
        util::gimmie::fn<void(__thiscall*)(string*)>(0x00A6CEE0)(this);
    }

    void construct_mashed_class();
    void destruct_mashed_class();
    void unmash(mash_info_struct* mash_info,
                void*             containing_class_ptr,
                buffer_type       buffer);

    i32 get_mash_sizeof() const {
        return sizeof(*this);
    }

    string &operator=(const string &other) {
        return *util::gimmie::fn<string*(__thiscall*)(string*, const string*)>(0x00A6CBC0)(this, &other);
    }

    string &operator=(const char* value) {
        assert(value != nullptr);

        return *util::gimmie::fn<string*(__thiscall*)(string*, const char*)>(0x00A6CBE0)(this, value);
    }

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

    void copy(const char* source, i32 source_length = npos) {
        update_guts(source, source_length);
    }

    void copy(const string &source) {
        update_guts(source.m_data, source.m_size);
    }

    void update_guts(const char* source, i32 source_length = npos) {
        assert(source != nullptr);

        util::gimmie::fn<void(__thiscall*)(string*, const char*, size_t)>(0x00A6C8F0)(this, source, (size_t)source_length);
    }

    void append(const char* source, i32 source_length = npos) {
        assert(source != nullptr);

        util::gimmie::fn<void(__thiscall*)(string*, const char*, size_t)>(0x00A6C9B0)(this, source, (size_t)source_length);
    }

    void append(const string &source) {
        util::gimmie::fn<void*(__thiscall*)(string*, const string*)>(0x00A6CA70)(this, &source);
    }

    void append(char value) {
        util::gimmie::fn<void(__thiscall*)(string*, char)>(0x00A6CA50)(this, value);
    }

    string &operator+=(const string &source) {
        append(source);
        
        return *this;
    }

    string &operator+=(const char* source) {
        append(source);
        
        return *this;
    }

    string &operator+=(char value) {
        append(value);
        
        return *this;
    }

    void clear() {
        util::gimmie::fn<void(__thiscall*)(string*)>(0x00A6CE60)(this);
    }

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

    i32 compare(const char* other) const {
        assert(other != nullptr);

        return util::gimmie::fn<i32(__thiscall*)(const string*, char*)>(0x00A6CC00)(this, (char*)other);
    }

    bool is_equal(const char* other) const {
        return other != nullptr && compare(other) == 0;
    }

    i32 find(const char* substring, i32 start = 0) const {
        assert(substring != nullptr);

        return util::gimmie::fn<i32(__thiscall*)(const string*, char*, i32)>(0x00A6CAC0)(this, (char*)substring, start);
    }

    i32 find(pos_t start, char value) const {
        return util::gimmie::fn<i32(__thiscall*)(const string*, i32, char)>(0x00A6CAF0)(this, start.value, value);
    }

    i32 rfind(char value, i32 start = npos) const {
        return util::gimmie::fn<i32(__thiscall*)(const string*, char, i32)>(0x00A6CB20)(this, value, start);
    }

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

    string &to_upper() {
        util::gimmie::fn<string*(__thiscall*)(string*)>(0x00A6CB60)(this);

        return *this;
    }

    string &to_lower() {
        util::gimmie::fn<string*(__thiscall*)(string*)>(0x00A6CB90)(this);

        return *this;
    }

    string substr(i32 start, i32 count = npos) const {
        assert(start >= 0);
        assert(start <= m_size);

        i32 available = m_size - start;

        if (count == npos || count > available)
            count = available;

        if (count < 0)
            count = 0;

        string result;
        result.append(m_data + start, count);

        return result;
    }

    string slice(i32 start, i32 end) const {
        if (start < 0)
            start += m_size;

        if (end < 0)
            end += m_size;

        assert(start >= 0);
        assert(start <= end);
        assert(end <= m_size);

        return substr(start, end - start);
    }

    string &remove_leading(const char* characters) {
        assert(characters != nullptr);

        util::gimmie::fn<string*(__thiscall*)(
            string*,
            char*)>(0x00A6D240)(
                this,
                (char*)characters);

        return *this;
    }

    string &remove_trailing(const char* characters) {
        assert(characters != nullptr);

        util::gimmie::fn<string*(__thiscall*)(string*, char*)>(0x00A6D310)(this,(char*)characters);

        return *this;
    }

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

    static i32 &live_count() {
        return *(i32*)0x01126D20;
    }

    friend bool operator==(const string &left, const string &right) {
        return left.compare(right.c_str()) == 0;
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
        return !(left == right);
    }

    friend bool operator!=(const char* left, const string &right) {
        return !(left == right);
    }

    /*
        compare() has the engine's reversed strcmp-style sign:
        +1 means *this < argument; -1 means *this > argument
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

    friend string operator+(const string &left, const string &right) {
        string result(left);
        
        result += right;

        return result;
    }

    friend string operator+(const string &left, const char* right) {
        string result(left);
        
        result += right;

        return result;
    }

    friend string operator+(const char* left, const string &right) {
        string result(left);
        
        result += right;

        return result;
    }

private:

    void assign_format(const char* format, va_list arguments) {
        assert(format != nullptr);
        
        char buffer[0x10000];

        vsnprintf(buffer, sizeof(buffer), format, arguments);

        buffer[sizeof(buffer) - 1] = '\0';

        update_guts(buffer);
    }
};

    ASSERT_SIZEOF  (string,                0x0C);
    ASSERT_OFFSETOF(string, m_size,        0x00);
    ASSERT_OFFSETOF(string, m_data,        0x04);
    ASSERT_OFFSETOF(string, m_source_slab, 0x08);
}} // treyarch::mash