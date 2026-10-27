#include <cctype>

#include "treyarch/base/tl_fixed_string.hh"

using namespace treyarch;

// sub_40A6B0
tl_fixed_string::tl_fixed_string(const char* value) : hash(), text() {
    char* cursor = text;

    for (const char* source = value; *source; ++source) {
        char c = *source;

        c = isalpha(c) ? (char)tolower(c) : c;

        hash.source_hash_code += (hash.source_hash_code << 5) + (u32)c;

        if (cursor - text < 27)
            *cursor++ = c;
    }

    text[27] = '\0';
}

// sub_5B7480
bool tl_fixed_string::operator==(const tl_fixed_string &other) const {
    auto left  = (const u32*)this;
    auto right = (const u32*)&other;

    for (i32 i = 0; i < 8; i++) {
        if (left[i] != right[i])
            return false;
    }

    return true;
}
