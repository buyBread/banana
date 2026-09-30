#include "aspyr/win/init.hh"
#include "util/memory_reference.hh"

namespace aspyr { namespace win {
    using init_callback               = i32  (__cdecl*)();
    using show_error_message_callback = void (__cdecl*)(i32 error);

    namespace references {
        inline util::memory_reference<init_callback>               init_callback               { 0x00B79004 };
        inline util::memory_reference<show_error_message_callback> show_error_message_callback { 0x00B79008 };
    } // references
}} // aspyr::win

i32 aspyr::win::init() {
    return references::init_callback.get()();
}

void aspyr::win::show_error_message(i32 error) {
    references::show_error_message_callback.get()(error);
}
