#include "../base.hh"

/*
    fmt_string for engine errors?
*/

using callback_t = int(__cdecl*)(char*);

class hk_sub_9CC940 : public c_hook<hk_sub_9CC940, banana::hk_fn_sig::sub_9CC940_t> {
    
public:

    hk_sub_9CC940() : c_hook((void*)0x009CC940) {}

    static int __cdecl detour(const char* fmt, ...) {
        if (!fmt)
            return 0;

        char buffer[1024];

        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);

        buffer[sizeof(buffer) - 1] = '\0'; 

        // maybe one day we'll trip one?
        LOG.err(buffer);

        // qword_1115A28
        callback_t* callback = (callback_t*)0x01115A28;

        if (callback && *callback)
            return (*callback)(buffer);

        return 0;
    }

}; hk_sub_9CC940 hk {};