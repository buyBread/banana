#include <atomic>

#include "../base.hh"

/*
    script loading callback
*/

class hk_sub_825E00 : public c_hook<hk_sub_825E00, hk_fn_sig::sub_825E00_t> {

    static inline std::atomic<uint32_t> reason_counts[16] {};

    static std::string get_script_name(void* executable) {
        if (!executable)
            return "<none>";

        uint32_t length = *(uint32_t*)((uint8_t*)executable + 0x04);
        const char* name = *(const char**)((uint8_t*)executable + 0x08);

        if (!name || !length || length > 0x400)
            return "<unknown>";

        return std::string(name, length);
    }

public:

    hk_sub_825E00() : c_hook((void*)0x00825E00) {}

    static void* __cdecl detour(int reason, void* executable, int context) {
        bool valid_reason = reason >= 0 && reason < 16;
        uint32_t count = valid_reason ? ++reason_counts[reason] : 0;
        bool lifecycle = reason >= 0 && reason <= 3;
        bool sample = count <= 4 || (count && !(count & (count - 1)));

        if (!valid_reason || lifecycle || sample)
            HK_MSG("reason: {} | count: {} | executable: 0x{:08x} | script: {} | context: 0x{:08x}",
                reason, count,
                (uint32_t)executable,
                get_script_name(executable),
                (uint32_t)context);

        return m_original(reason, executable, context);
    }

};

// hk_sub_825E00 hk {};