#pragma once

#include "util/types.hh"

namespace aspyr { namespace win {
    wchar_t* get_local_string(const wchar_t* key,
                              wchar_t*       buffer,
                              u32            size);
    wchar_t* get_title       (wchar_t* buffer,
                              u32      size);
}} // aspyr::win
