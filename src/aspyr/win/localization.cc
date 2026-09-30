#include "aspyr/win/localization.hh"
#include "util/memory_reference.hh"

namespace aspyr { namespace win {
    using get_local_string_callback = wchar_t* (__cdecl*)(const wchar_t* key,
                                                          wchar_t*       buffer,
                                                          u32            size);
    using get_title_callback        = wchar_t* (__cdecl*)(wchar_t* buffer,
                                                          u32      size);

    namespace references {
        inline util::memory_reference<get_local_string_callback> get_local_string_callback { 0x00B79010 };
        inline util::memory_reference<get_title_callback>        get_title_callback        { 0x00B79014 };
    } // references
}} // aspyr::win

wchar_t* aspyr::win::get_local_string(const wchar_t* key,
                                      wchar_t*       buffer,
                                      u32            size) {

    return references::get_local_string_callback.get()
        (key, buffer, size);
}

wchar_t* aspyr::win::get_title(wchar_t* buffer,
                               u32      size) {

    return references::get_title_callback.get()
        (buffer, size);
}
