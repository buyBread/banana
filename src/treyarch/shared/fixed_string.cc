#include <cstring>

#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_408E10
void fixed_string::set_text(const char* value) {
    char* copy = (char*)memory::heap::allocate(std::strlen(value) + 1);

    text = copy;

    for (; *value; ++value, ++copy)
        *copy = (u8)(*value - 'A') <= 25 ? *value + 32 : *value;

    *copy = 0;
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
