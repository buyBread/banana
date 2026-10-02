#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstring>

#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// inlined into every constructor
void mash::string::initialize() {
    m_data = &references::null_string_guts.get();
    m_size = 0;

    ++live_count();
}

// inlined into everything that drops the old characters
void mash::string::destroy_guts() {
    if (m_data == &references::null_string_guts.get())
        return;

    if (m_source_slab != references::string_source_slab.read())
        memory::heap::free(m_data);

    m_data = &references::null_string_guts.get();
}

i32 &mash::string::live_count() {
    return references::string_count.get();
}

// sub_A6CC70
mash::string::string() {
    initialize();
}

// sub_A6CE30
mash::string::string(const string &other) {
    initialize();
    update_guts(other.m_data);
}

// sub_A6CF60
mash::string::string(const string &other,
                           i32     start,
                           i32     count) {

    initialize();

    string source(other.m_data);

    if (count == npos || count > source.m_size - start)
        count = source.m_size - start;

    update_guts(source.m_data + start, count);
}

// sub_A6CE00
mash::string::string(const char* value) {
    initialize();

    if (value)
        update_guts(value);
}

// sub_A6CCE0
mash::string::string(fmtd, const char* format, ...) {
    char buffer[0x10000];

    va_list arguments;
    va_start(arguments, format);
    _vsnprintf(buffer, 0xFFFD, format, arguments);
    va_end(arguments);

    initialize();
    update_guts(buffer);
}

// sub_A6CD50
mash::string::string(i32 value) {
    char buffer[32];
    sprintf(buffer, "%d", value);

    initialize();
    update_guts(buffer);
}

// sub_A6CDA0
mash::string::string(f32 value) {
    char buffer[128];
    sprintf(buffer, "%0.3f", value);

    initialize();
    update_guts(buffer);
}

// sub_A6CEE0
mash::string::~string() {
    destroy_guts();

    --live_count();
}

// sub_A6CCA0
void mash::string::finalize(allocation_scope) {
    destroy_guts();

    --live_count();
}

// sub_A6CC90
void mash::string::construct_mashed_class() {
    ++live_count();
}

void mash::string::destruct_mashed_class() {
    this->~string();
}

// sub_A6CF20
void mash::string::unmash(mash_info_struct* mash_info,
                          void*,
                          buffer_type       buffer) {

    // length lives in the sideband stream; the actual bytes follow whichever stream owns the member
    mash_info->read_from_buffer(SHARED_BUFFER, m_size);

    if (m_size <= 0) {
        m_data        = &references::null_string_guts.get();
        m_source_slab = nullptr;

        return;
    }

    m_data = (char*)mash_info->read_from_buffer(buffer, m_size + 1, 1);

    // the next thing in either stream expects at least the usual dword alignment
    mash_info->align_buffer(buffer, 4);
    m_source_slab = references::string_source_slab.read();
}

// sub_A6CBC0
mash::string &mash::string::operator=(const string &other) {
    if (this != &other)
        update_guts(other.m_data, other.m_size);

    return *this;
}

// sub_A6CBE0
mash::string &mash::string::operator=(const char* value) {
    update_guts(value);

    return *this;
}

// sub_A6C8F0
void mash::string::update_guts(const char* source, i32 source_length) {
    if (source_length == npos)
        source_length = (i32)strlen(source);

    /* borrowed bytes always get swapped for a fresh allocation;
       m_source_slab is left alone though, so that allocation is never freed either... */
    if (source_length > m_size || m_source_slab == references::string_source_slab.read()) {
        destroy_guts();

        m_data = (char*)memory::heap::allocate(source_length + 1);
    }

    if (source_length <= 0) {
        destroy_guts();

        m_size = source_length;
        m_data = &references::null_string_guts.get();

        return;
    }

    memcpy(m_data, source, source_length);
    m_data[source_length] = '\0';
    m_size = source_length;
}

// sub_A6C9B0
void mash::string::append(const char* source, i32 source_length) {
    if (source_length == npos)
        source_length = (i32)strlen(source);

    if (!source_length)
        return;

    i32   new_size = m_size + source_length;
    char* joined   = (char*)memory::heap::allocate(new_size + 1);

    if (m_size > 0) {
        strncpy(joined, m_data, m_size);
        joined[m_size] = '\0';
    } else
        *joined = '\0';

    strncat(joined, source, source_length);

    destroy_guts();

    m_data = joined;
    m_size = new_size;
    m_data[new_size] = '\0';
}

// sub_A6CA50
void mash::string::append(char value) {
    char source[2] { value, '\0' };

    append(source);
}

// sub_A6CA70
mash::string &mash::string::operator+=(const string &source) {
    append(source.m_data);

    return *this;
}

// sub_A6CA90
mash::string &mash::string::operator+=(const char* source) {
    append(source);

    return *this;
}

// sub_A6CE60
void mash::string::clear() {
    if (!m_size)
        return;

    // a negative size survives clearing
    i32 new_size = m_size < 0 ? m_size : 0;

    m_data[new_size] = '\0';
    m_size = new_size;
}

// sub_A6CC00
i32 mash::string::compare(const char* other) const {
    i32 index = 0;

    for (; index < m_size; ++index) {
        char theirs = other[index];

        if (!theirs)
            return -1;

        char ours = m_data[index];

        if (theirs > ours)
            return 1;

        if (theirs < ours)
            return -1;
    }

    return other[index] != '\0';
}

// sub_A6CAC0
i32 mash::string::find(const char* substring, i32 start) const {
    const char* match = strstr(m_data + start, substring);

    return match ? (i32)(match - m_data) : npos;
}

// sub_A6CAF0
i32 mash::string::find(pos_t start, char value) const {
    for (const char* cursor = m_data + start; *cursor; ++cursor) {
        if (*cursor == value)
            return (i32)(cursor - m_data);
    }

    return npos;
}

// sub_A6CB20
i32 mash::string::rfind(char value, i32 start) const {
    if (!m_size)
        return npos;

    if (start == npos)
        start = m_size - 1;

    for (const char* cursor = m_data + start; cursor >= m_data; --cursor) {
        if (*cursor == value)
            return (i32)(cursor - m_data);
    }

    return npos;
}

// sub_A6CB60
mash::string &mash::string::to_upper() {
    const u8* table = &references::upper_case_table.get();

    for (char* cursor = m_data; *cursor; ++cursor) {
        char upper = (char)table[(u8)*cursor];

        if (upper)
            *cursor = upper;
    }

    return *this;
}

// sub_A6CB90
mash::string &mash::string::to_lower() {
    for (char* cursor = m_data; *cursor; ++cursor)
        *cursor = (char)tolower(*cursor);

    return *this;
}

// sub_A6D030
mash::string mash::string::substr(i32 start, i32 count) const {
    // no clamping, a npos count just runs to the terminator
    string result;
    result.append(m_data + start, count);

    return result;
}

// sub_A6D240
mash::string &mash::string::remove_leading(const char* characters) {
    i32 size  = m_size;
    i32 index = 0;

    for (; index < size && characters; ++index) {
        const char* match = characters;

        while (*match && *match != m_data[index])
            ++match;

        if (!*match)
            break;
    }

    string remaining = slice(index, size);
    update_guts(remaining.m_data, remaining.m_size);

    return *this;
}

// sub_A6D310
mash::string &mash::string::remove_trailing(const char* characters) {
    i32 index = m_size;

    for (; index > 0 && characters; --index) {
        const char* match = characters;

        while (*match && *match != m_data[index - 1])
            ++match;

        if (!*match)
            break;
    }

    string remaining = slice(0, index);
    update_guts(remaining.m_data, remaining.m_size);

    return *this;
}

// sub_A6D0A0
mash::string mash::operator+(const string &left, const string &right) {
    string result(left);
    result.append(right.m_data);

    return result;
}

// sub_A6D1C0
mash::string mash::operator+(const string &left, const char* right) {
    string result(left);
    result.append(right);

    return result;
}

// sub_A6D130
mash::string mash::operator+(const char* left, const string &right) {
    string result(left);
    result.append(right.m_data);

    return result;
}
