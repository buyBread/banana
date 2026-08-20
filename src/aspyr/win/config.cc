#include "aspyr/win/config.hh"
#include "util/memory_reference.hh"

namespace aspyr { namespace win {
    using get_config_number_callback = double (__cdecl*)(const char*  name,
                                                               double fallback,
                                                               bool   write_default);
    namespace references {
        inline util::memory_reference<get_config_number_callback> get_config_number_callback { 0x00B79020 };
    } // references
}} // aspyr::win

double aspyr::win::get_config_number(const char*  name,
                                           double fallback,
                                           bool   write_default) {

    return references::get_config_number_callback.get()
        (name, fallback, write_default);
}
