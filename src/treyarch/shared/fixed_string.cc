#include <cstdio>
#include <cstring>

#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/memory/heap.hh"
#include "util/memory_reference.hh"

using namespace treyarch;

namespace treyarch { namespace references {
    // sixteen "0x12345678" slots for names that only have a hash
    util::memory_reference<char> hash_texts      { 0x00FC6950 };
    util::memory_reference<u32>  hash_text_index { 0x00FC6A00 };
}} // treyarch::references

// sub_408E10
void fixed_string::set_text(const char* value) {
    char* copy = (char*)memory::heap::allocate(std::strlen(value) + 1);

    text = copy;

    for (; *value; ++value, ++copy)
        *copy = (u8)(*value - 'A') <= 25 ? *value + 32 : *value;

    *copy = 0;
}

// sub_4FABD0
const char* fixed_string::get_text() const {
    if (text)
        return text;

    u32   index = references::hash_text_index.read();
    char* slot  = &references::hash_texts.get() + 11 * index;

    std::sprintf(slot, "0x%08X", hash.source_hash_code);

    references::hash_text_index.write((index + 1) & 0x0F);

    return slot;
}

// sub_4FAB70
fixed_string &fixed_string::assign(const fixed_string &other) {
    memory::heap::free(text);

    hash = other.hash;

    if (other.text) {
        u32 size = std::strlen(other.text) + 1;

        text = (char*)memory::heap::allocate(size);
        std::memcpy(text, other.text, size);
    } else
        text = nullptr;

    return *this;
}
