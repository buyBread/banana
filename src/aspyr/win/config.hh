#pragma once

namespace aspyr { namespace win {
    double get_config_number(const char* name,
                             double      fallback,
                             bool        write_default);
}} // aspyr::win
