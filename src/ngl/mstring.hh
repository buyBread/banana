#pragma once

#include <cassert>

#include "../util.hh"

struct from_mash_in_place_constructor {};

class mString {

public:

    struct fmtd { int32_t value; };

    struct pos_t {
        int32_t value;

        constexpr operator int32_t() const noexcept {
            return value;
        }
    };

    static constexpr int32_t npos       = -1;
    static constexpr size_t  max_length = 65535;

    /*
        0x00: current string length
        0x04: null-terminated character data
        0x08: allocation/mash scope marker

        scope of -15 means the character data belongs to a mashed image and must not be freed normally
    */

    int32_t m_size;
    char*   m_data;
    int32_t m_allocation_scope;

    inline static char* const null = (char*)0x01126D24;

    mString() noexcept: m_size(0), m_data(null), m_allocation_scope(0) {
        get_fn<void(__thiscall*)(mString*)>(0x00A6CC70)(this);
    }

    explicit mString(from_mash_in_place_constructor*) noexcept {
        get_fn<void(__thiscall*)(mString*)>(0x00A6CC90)(this);
    }

    mString(const mString& other) : m_size(0), m_data(null), m_allocation_scope(0) {
        get_fn<mString*(__thiscall*)(mString*, const mString*)>(0x00A6CE30)(this, &other);
    }

    mString(const char* value) : m_size(0), m_data(null), m_allocation_scope(0) {
        get_fn<mString*(__thiscall*)(mString*, const char*)>(0x00A6CE00)(this, value);
    }

    explicit mString(int32_t value) : m_size(0), m_data(null), m_allocation_scope(0) {
        get_fn<mString*(__thiscall*)(mString*, int32_t)>(0x00A6CD50)(this, value);
    }

    explicit mString(float value) : m_size(0), m_data(null), m_allocation_scope(0) {
        get_fn<mString*(__thiscall*)(mString*, float)>(0x00A6CDA0)(this, value);
    }

    mString(fmtd, const char* format, ...) : mString() {
        va_list arguments;
        va_start(arguments, format);
        assign_format(format, arguments);
        va_end(arguments);
    }

    mString(int32_t, const char* format, ...) : mString() {
        va_list arguments;
        va_start(arguments, format);
        assign_format(format, arguments);
        va_end(arguments);
    }

    ~mString() {
        get_fn<void(__thiscall*)(mString*)>(0x00A6CEE0)(this);
    }

    mString &operator=(const mString &other) {
        return *get_fn<mString*(__thiscall*)(mString*, const mString*)>(0x00A6CBC0)(this, &other);
    }

    mString &operator=(const char* value) {
        assert(value != nullptr);

        return *get_fn<mString*(__thiscall*)(mString*, const char*)>(0x00A6CBE0)(this, value);
    }

    [[nodiscard]] bool empty() const noexcept {
        return m_size == 0;
    }

    [[nodiscard]] int32_t size() const noexcept {
        return m_size;
    }

    [[nodiscard]] int32_t length() const noexcept {
        return m_size;
    }

    [[nodiscard]] const char* c_str() const noexcept {
        return m_data;
    }

    [[nodiscard]] char* data() noexcept {
        return m_data;
    }

    [[nodiscard]] const char* data() const noexcept {
        return m_data;
    }

    [[nodiscard]] char at(int32_t index) const {
        return (*this)[index];
    }

    [[nodiscard]] char operator[](int32_t index) const {
        assert(index >= 0);
        assert(index <= m_size);

        return m_data[index];
    }

    void copy(const char* source, int32_t source_length = npos) {
        update_guts(source, source_length);
    }

    void copy(const mString& source) {
        update_guts(source.m_data, source.m_size);
    }

    void update_guts(const char* source, int32_t source_length = npos) {
        assert(source != nullptr);

        get_fn<void(__thiscall*)(mString*, const char*, size_t)>(0x00A6C8F0)(this, source, (size_t)source_length);
    }

    void append(const char* source, int32_t source_length = npos) {
        assert(source != nullptr);

        get_fn<void(__thiscall*)(mString*, const char*, size_t)>(0x00A6C9B0)(this, source, (size_t)source_length);
    }

    void append(const mString& source) {
        get_fn<void*(__thiscall*)(mString*, const mString*)>(0x00A6CA70)(this, &source);
    }

    void append(char value) {
        get_fn<void(__thiscall*)(mString*, char)>(0x00A6CA50)(this, value);
    }

    mString& operator+=(const mString& source) {
        append(source);
        
        return *this;
    }

    mString& operator+=(const char* source) {
        append(source);
        
        return *this;
    }

    mString& operator+=(char value) {
        append(value);
        
        return *this;
    }

    void clear() {
        get_fn<void(__thiscall*)(mString*)>(0x00A6CE60)(this);
    }

    mString truncate(int32_t requested_size) {
        int32_t new_size = requested_size;

        if (new_size < 0)
            new_size = 0;

        if (new_size > m_size)
            new_size = m_size;

        m_data[new_size] = '\0';
        m_size = new_size;

        return *this;
    }

    [[nodiscard]] int32_t compare(const char* other) const {
        assert(other != nullptr);

        return get_fn<int32_t(__thiscall*)(const mString*, char*)>(0x00A6CC00)(this, (char*)other);
    }

    [[nodiscard]] bool is_equal(const char* other) const {
        return other != nullptr && compare(other) == 0;
    }

    [[nodiscard]] int32_t find(const char* substring, int32_t start = 0) const {
        assert(substring != nullptr);

        return get_fn<int32_t(__thiscall*)(const mString*, char*, int32_t)>(0x00A6CAC0)(this, (char*)substring, start);
    }

    [[nodiscard]] int32_t find(pos_t start, char value) const {
        return get_fn<int32_t(__thiscall*)(const mString*, int32_t, char)>(0x00A6CAF0)(this, start.value, value);
    }

    [[nodiscard]] int32_t rfind(char value, int32_t start = npos) const {
        return get_fn<int32_t(__thiscall*)(const mString*, char, int32_t)>(0x00A6CB20)(this, value, start);
    }

    [[nodiscard]] int32_t rfind(const char* substring) const {
        assert(substring != nullptr);

        int32_t substring_length =
            (int32_t)strlen(substring);

        if (substring_length > m_size)
            return npos;

        for (int32_t index = m_size - substring_length;index >= 0; --index) {
            if (memcmp(m_data + index, substring, substring_length) == 0)
                return index;
        }

        return npos;
    }

    mString& to_upper() {
        get_fn<mString*(__thiscall*)(mString*)>(0x00A6CB60)(this);

        return *this;
    }

    mString& to_lower() {
        get_fn<mString*(__thiscall*)(mString*)>(0x00A6CB90)(this);

        return *this;
    }

    [[nodiscard]] mString substr(int32_t start, int32_t count = npos) const {
        assert(start >= 0);
        assert(start <= m_size);

        int32_t available = m_size - start;

        if (count == npos || count > available)
            count = available;

        if (count < 0)
            count = 0;

        mString result;
        result.append(m_data + start, count);

        return result;
    }

    [[nodiscard]] mString slice(int32_t start, int32_t end) const {
        if (start < 0)
            start += m_size;

        if (end < 0)
            end += m_size;

        assert(start >= 0);
        assert(start <= end);
        assert(end <= m_size);

        return substr(start, end - start);
    }

    mString& remove_leading(const char* characters) {
        assert(characters != nullptr);

        get_fn<mString*(__thiscall*)(
            mString*,
            char*)>(0x00A6D240)(
                this,
                (char*)characters);

        return *this;
    }

    mString& remove_trailing(const char* characters) {
        assert(characters != nullptr);

        get_fn<mString*(__thiscall*)(mString*, char*)>(0x00A6D310)(this,(char*)characters);

        return *this;
    }

    mString &remove_surrounding_whitespace() {
        remove_leading(" \n\t\r");
        remove_trailing(" \n\t\r");

        return *this;
    }

    [[nodiscard]] int32_t to_int() const {
        return atoi(m_data);
    }

    [[nodiscard]] double to_float() const {
        return atof(m_data);
    }

    [[nodiscard]] static mString from_int(int32_t value) {
        return mString(value);
    }

    [[nodiscard]] static mString from_float( float value) {
        return mString(value);
    }

    [[nodiscard]] static int32_t& live_count() {
        return *(int32_t*)0x01126D20;
    }

    friend bool operator==(const mString& left, const mString& right) {
        return left.compare(right.c_str()) == 0;
    }

    friend bool operator!=(const mString& left, const mString& right) {
        return !(left == right);
    }

    friend bool operator==(const mString& left, const char* right) {
        return left.is_equal(right);
    }

    friend bool operator==(const char* left, const mString& right) {
        return right.is_equal(left);
    }

    friend bool operator!=(const mString& left, const char* right) {
        return !(left == right);
    }

    friend bool operator!=(const char* left, const mString& right) {
        return !(left == right);
    }

    /*
        compare() has the engine's reversed strcmp-style sign:
        +1 means *this < argument; -1 means *this > argument
    */

    friend bool operator<(const mString& left, const mString& right) {
        return left.compare(right.c_str()) == 1;
    }

    friend bool operator>(const mString& left, const mString& right) {
        return left.compare(right.c_str()) == -1;
    }

    friend bool operator<=(const mString& left, const mString& right) {
        return !(left > right);
    }

    friend bool operator>=(const mString& left, const mString& right) {
        return !(left < right);
    }

    friend mString operator+(const mString& left, const mString& right) {
        mString result(left);
        
        result += right;

        return result;
    }

    friend mString operator+(const mString& left, const char* right) {
        mString result(left);
        
        result += right;

        return result;
    }

    friend mString operator+(const char* left, const mString& right) {
        mString result(left);
        
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

static_assert(sizeof(mString) == 0x0C, ASSERT_FAIL_SANITY);

static_assert(offsetof(mString, m_size)             == 0x00, ASSERT_FAIL_SANITY);
static_assert(offsetof(mString, m_data)             == 0x04, ASSERT_FAIL_SANITY);
static_assert(offsetof(mString, m_allocation_scope) == 0x08, ASSERT_FAIL_SANITY);

typedef mString mash_string;