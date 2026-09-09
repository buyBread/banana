#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/string.hh"
#include "util/memory_reference.hh"

using namespace treyarch;

// non-empty strings remember which allocator slab owns the borrowed character buffer
static util::memory_reference<void*> string_source_slab { 0x00FBF24C };

void mash::string::construct_mashed_class() {
    ++live_count();
}

void mash::string::destruct_mashed_class() {
    this->~string();
}

void mash::string::unmash(mash_info_struct* mash_info,
                          void*,
                          buffer_type       buffer) {

    // length lives in the sideband stream; the actual bytes follow whichever stream owns the member
    mash_info->read_from_buffer(SHARED_BUFFER, m_size);

    if (m_size <= 0) {
        m_data        = null;
        m_source_slab = nullptr;
        
        return;
    }

    m_data = (char*)mash_info->read_from_buffer(buffer, m_size + 1, 1);

    // the next thing in either stream expects at least the usual dword alignment
    mash_info->align_buffer(buffer, 4);
    m_source_slab = string_source_slab.read();
}
