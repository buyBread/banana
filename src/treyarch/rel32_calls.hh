#pragma once

#include "flags.hh"
#include "util/redirect_rel32.hh"
#include "util/macros/debug.hh"
#include "banana/logging.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/cutscene/cutscene_player.hh"
#include "treyarch/game/event/event_manager.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/input/input_mgr.hh"
#include "treyarch/ngl/ngl.hh"

namespace treyarch {
    inline void _redirect_event_manager() {
        banana::log.dbg("redirecting event_manager callsites");

        struct event_callsite {
            u32 address;
            u8  expected[5];
        };

        if (!util::redirect_rel32(0x0068643C, { 0xE8, 0x3F, 0xFD, 0xFF, 0xFF }, &event_manager::clear)                  ||
            !util::redirect_rel32(0x0068674A, { 0xE8, 0x11, 0xFB, 0xFF, 0xFF }, &event_manager::register_event_type)    ||
            !util::redirect_rel32(0x0068690A, { 0xE8, 0x51, 0xF9, 0xFF, 0xFF }, &event_manager::register_event_type)    ||
            !util::redirect_rel32(0x00429ADC, { 0xE8, 0x6F, 0xC8, 0x25, 0x00 }, &event_manager::garbage_collect)        ||
            !util::redirect_rel32(0x0042A833, { 0xE8, 0xE8, 0xBB, 0x25, 0x00 }, &event_manager::create_inst)            ||
            !util::redirect_rel32(0x006867DD, { 0xE8, 0x2E, 0xFF, 0xFF, 0xFF }, &event_manager::create_event_recipient) ||
            !util::redirect_rel32(0x0068686D, { 0xE8, 0x9E, 0xFE, 0xFF, 0xFF }, &event_manager::create_event_recipient))
            
            FATAL_BREAKPOINT();

        // ====== //

        using clear_script_callbacks_function = void(__cdecl*)
            (arch_base_vhandle, chuck::vm::script_executable*);

        clear_script_callbacks_function clear_script_callbacks = &event_manager::clear_script_callbacks;

        const event_callsite clear_script_callbacks_calls[] { { 0x00601889, { 0xE8, 0xD2, 0x32, 0x08, 0x00 } },
                                                              { 0x00825E71, { 0xE8, 0xEA, 0xEC, 0xE5, 0xFF } }, };

        for (const auto &callsite : clear_script_callbacks_calls) {
            if (!util::redirect_rel32(callsite.address, callsite.expected, clear_script_callbacks))
                FATAL_BREAKPOINT();
        }

        // ====== //

        using clear_script_callback_function = void(__cdecl*)
            (arch_base_vhandle, string_hash);

        clear_script_callback_function clear_script_callback = &event_manager::clear_script_callback;

        const event_callsite clear_script_function_calls[] { { 0x006018B2, { 0xE8, 0x29, 0x33, 0x08, 0x00 } },
                                                             { 0x008259FB, { 0xE8, 0xE0, 0xF1, 0xE5, 0xFF } }, };

        for (const auto &callsite : clear_script_function_calls) {
            if (!util::redirect_rel32(callsite.address, callsite.expected, clear_script_callback))
                FATAL_BREAKPOINT();
        }

        // ====== //

        using clear_script_id_function = void(__cdecl*)
            (arch_base_vhandle, u32);

        clear_script_id_function clear_script_id = &event_manager::clear_script_callback;

        const event_callsite clear_script_id_calls[] { { 0x006018C9, { 0xE8, 0xA2, 0x33, 0x08, 0x00 } },
                                                       { 0x00825A1E, { 0xE8, 0x4D, 0xF2, 0xE5, 0xFF } }, };

        for (const auto &callsite : clear_script_id_calls) {
            if (!util::redirect_rel32(callsite.address, callsite.expected, clear_script_id))
                FATAL_BREAKPOINT();
        }

        // ====== //

        const event_callsite remove_callback_calls[] { { 0x00601867, { 0xE8, 0x84, 0x34, 0x08, 0x00 } },
                                                       { 0x006630CD, { 0xE8, 0x1E, 0x1C, 0x02, 0x00 } },
                                                       { 0x006686FF, { 0xE8, 0xEC, 0xC5, 0x01, 0x00 } },
                                                       { 0x006698A7, { 0xE8, 0x44, 0xB4, 0x01, 0x00 } },
                                                       { 0x00684F3D, { 0xE8, 0xAE, 0xFD, 0xFF, 0xFF } },
                                                       { 0x00684F76, { 0xE8, 0x75, 0xFD, 0xFF, 0xFF } },
                                                       { 0x00698808, { 0xE8, 0xE3, 0xC4, 0xFE, 0xFF } },
                                                       { 0x00698845, { 0xE8, 0xA6, 0xC4, 0xFE, 0xFF } },
                                                       { 0x0076B4DA, { 0xE8, 0x11, 0x98, 0xF1, 0xFF } },
                                                       { 0x0076E64D, { 0xE8, 0x9E, 0x66, 0xF1, 0xFF } },
                                                       { 0x0076E674, { 0xE8, 0x77, 0x66, 0xF1, 0xFF } },
                                                       { 0x00920F3E, { 0xE8, 0xAD, 0x3D, 0xD6, 0xFF } },
                                                       { 0x00983703, { 0xE8, 0xE8, 0x15, 0xD0, 0xFF } },
                                                       { 0x00983743, { 0xE8, 0xA8, 0x15, 0xD0, 0xFF } },
        };

        for (const auto &callsite : remove_callback_calls) {
            if (!util::redirect_rel32(callsite.address, callsite.expected, &event_manager::remove_callback))
                FATAL_BREAKPOINT();
        }

        if (!util::redirect_rel32(0x00601842, { 0xE8, 0x59, 0x35, 0x08, 0x00 }, &event_manager::has_callbacks))
            FATAL_BREAKPOINT();

        // ====== //

        using raise_type_function = void(__cdecl*)
            (string_hash, arch_base_vhandle);

        raise_type_function raise_type = &event_manager::raise_event;

        const event_callsite raise_type_calls[] { { 0x005BFBC6, { 0xE8, 0xD5, 0x58, 0x0C, 0x00 } },
                                                  { 0x005C36D7, { 0xE8, 0xC4, 0x1D, 0x0C, 0x00 } },
                                                  { 0x005C371C, { 0xE8, 0x7F, 0x1D, 0x0C, 0x00 } },
                                                  { 0x00601762, { 0xE8, 0x39, 0x3D, 0x08, 0x00 } },
                                                  { 0x006048F4, { 0xE8, 0xA7, 0x0B, 0x08, 0x00 } },
                                                  { 0x00604C77, { 0xE8, 0x24, 0x08, 0x08, 0x00 } },
                                                  { 0x00610EBC, { 0xE8, 0xDF, 0x45, 0x07, 0x00 } },
                                                  { 0x00610F1D, { 0xE8, 0x7E, 0x45, 0x07, 0x00 } },
                                                  { 0x00610FD0, { 0xE8, 0xCB, 0x44, 0x07, 0x00 } },
                                                  { 0x00611026, { 0xE8, 0x75, 0x44, 0x07, 0x00 } },
                                                  { 0x00611410, { 0xE8, 0x8B, 0x40, 0x07, 0x00 } },
                                                  { 0x0061BAC8, { 0xE8, 0xD3, 0x99, 0x06, 0x00 } },
                                                  { 0x0063C1AD, { 0xE8, 0xEE, 0x92, 0x04, 0x00 } },
                                                  { 0x0063C2F0, { 0xE8, 0xAB, 0x91, 0x04, 0x00 } },
                                                  { 0x0063C564, { 0xE8, 0x37, 0x8F, 0x04, 0x00 } },
                                                  { 0x0064B0EF, { 0xE8, 0xAC, 0xA3, 0x03, 0x00 } },
                                                  { 0x00651C93, { 0xE8, 0x08, 0x38, 0x03, 0x00 } },
                                                  { 0x00652040, { 0xE8, 0x5B, 0x34, 0x03, 0x00 } },
                                                  { 0x0065258F, { 0xE8, 0x0C, 0x2F, 0x03, 0x00 } },
                                                  { 0x006525C8, { 0xE8, 0xD3, 0x2E, 0x03, 0x00 } },
                                                  { 0x0065736D, { 0xE8, 0x2E, 0xE1, 0x02, 0x00 } },
                                                  { 0x0065A3E9, { 0xE8, 0xB2, 0xB0, 0x02, 0x00 } },
                                                  { 0x00660A2B, { 0xE8, 0x70, 0x4A, 0x02, 0x00 } },
                                                  { 0x00660A5C, { 0xE8, 0x3F, 0x4A, 0x02, 0x00 } },
                                                  { 0x00660ADD, { 0xE8, 0xBE, 0x49, 0x02, 0x00 } },
                                                  { 0x00661E30, { 0xE8, 0x6B, 0x36, 0x02, 0x00 } },
                                                  { 0x0066236C, { 0xE8, 0x2F, 0x31, 0x02, 0x00 } },
                                                  { 0x00662421, { 0xE8, 0x7A, 0x30, 0x02, 0x00 } },
                                                  { 0x006632F9, { 0xE8, 0xA2, 0x21, 0x02, 0x00 } },
                                                  { 0x006637A1, { 0xE8, 0xFA, 0x1C, 0x02, 0x00 } },
                                                  { 0x00663EA3, { 0xE8, 0xF8, 0x15, 0x02, 0x00 } },
                                                  { 0x0066FA3C, { 0xE8, 0x5F, 0x5A, 0x01, 0x00 } },
                                                  { 0x0067067D, { 0xE8, 0x1E, 0x4E, 0x01, 0x00 } },
                                                  { 0x00677A2A, { 0xE8, 0x71, 0xDA, 0x00, 0x00 } },
                                                  { 0x00677BE9, { 0xE8, 0xB2, 0xD8, 0x00, 0x00 } },
                                                  { 0x00677C69, { 0xE8, 0x32, 0xD8, 0x00, 0x00 } },
                                                  { 0x0067EBD1, { 0xE8, 0xCA, 0x68, 0x00, 0x00 } },
                                                  { 0x00690A39, { 0xE8, 0x62, 0x4A, 0xFF, 0xFF } },
                                                  { 0x00690CA6, { 0xE8, 0xF5, 0x47, 0xFF, 0xFF } },
                                                  { 0x006A720E, { 0xE8, 0x8D, 0xE2, 0xFD, 0xFF } },
                                                  { 0x006A730E, { 0xE8, 0x8D, 0xE1, 0xFD, 0xFF } },
                                                  { 0x006BBAD2, { 0xE8, 0xC9, 0x99, 0xFC, 0xFF } },
                                                  { 0x006BF252, { 0xE8, 0x49, 0x62, 0xFC, 0xFF } },
                                                  { 0x006D170E, { 0xE8, 0x8D, 0x3D, 0xFB, 0xFF } },
                                                  { 0x006D1A56, { 0xE8, 0x45, 0x3A, 0xFB, 0xFF } },
                                                  { 0x006DE59E, { 0xE8, 0xFD, 0x6E, 0xFA, 0xFF } },
                                                  { 0x006DEA8D, { 0xE8, 0x0E, 0x6A, 0xFA, 0xFF } },
                                                  { 0x006DED50, { 0xE8, 0x4B, 0x67, 0xFA, 0xFF } },
                                                  { 0x006DEE3C, { 0xE8, 0x5F, 0x66, 0xFA, 0xFF } },
                                                  { 0x006ED4E7, { 0xE8, 0xB4, 0x7F, 0xF9, 0xFF } },
                                                  { 0x00843205, { 0xE8, 0x96, 0x22, 0xE4, 0xFF } },
                                                  { 0x00947DFE, { 0xE8, 0x9D, 0xD6, 0xD3, 0xFF } },
                                                  { 0x0094C443, { 0xE8, 0x58, 0x90, 0xD3, 0xFF } },
                                                  { 0x0094C45D, { 0xE8, 0x3E, 0x90, 0xD3, 0xFF } },
                                                  { 0x009511BA, { 0xE8, 0xE1, 0x42, 0xD3, 0xFF } },
                                                  { 0x0097BC28, { 0xE8, 0x73, 0x98, 0xD0, 0xFF } },
                                                  { 0x0097CA24, { 0xE8, 0x77, 0x8A, 0xD0, 0xFF } },
                                                  { 0x0097E440, { 0xE8, 0x5B, 0x70, 0xD0, 0xFF } },
                                                  { 0x0097E4FC, { 0xE8, 0x9F, 0x6F, 0xD0, 0xFF } },
                                                  { 0x0097E5E9, { 0xE8, 0xB2, 0x6E, 0xD0, 0xFF } },
                                                  { 0x0097EFF2, { 0xE8, 0xA9, 0x64, 0xD0, 0xFF } },
                                                  { 0x0097F3BB, { 0xE8, 0xE0, 0x60, 0xD0, 0xFF } },
                                                  { 0x0097F3F8, { 0xE8, 0xA3, 0x60, 0xD0, 0xFF } },
                                                  { 0x0097F455, { 0xE8, 0x46, 0x60, 0xD0, 0xFF } },
                                                  { 0x0098065E, { 0xE8, 0x3D, 0x4E, 0xD0, 0xFF } },
                                                  { 0x00984B45, { 0xE8, 0x56, 0x09, 0xD0, 0xFF } },
                                                  { 0x0098566F, { 0xE8, 0x2C, 0xFE, 0xCF, 0xFF } },
                                                  { 0x009862A6, { 0xE8, 0xF5, 0xF1, 0xCF, 0xFF } },
                                                  { 0x0098631B, { 0xE8, 0x80, 0xF1, 0xCF, 0xFF } },
                                                  { 0x0098638C, { 0xE8, 0x0F, 0xF1, 0xCF, 0xFF } },
                                                  { 0x009983AE, { 0xE8, 0xED, 0xD0, 0xCE, 0xFF } }, };

        for (const auto &callsite : raise_type_calls) {
            if (!util::redirect_rel32(callsite.address, callsite.expected, raise_type))
                FATAL_BREAKPOINT();
        }

        // ====== //

        using raise_object_function = void(__cdecl*)(event*, arch_base_vhandle);

        raise_object_function raise_object = &event_manager::raise_event;

        const event_callsite raise_object_calls[] { { 0x005B9E9A, { 0xE8, 0x81, 0xB6, 0x0C, 0x00 } },
                                                    { 0x005D68FD, { 0xE8, 0x1E, 0xEC, 0x0A, 0x00 } },
                                                    { 0x005D69C8, { 0xE8, 0x53, 0xEB, 0x0A, 0x00 } },
                                                    { 0x005D6A75, { 0xE8, 0xA6, 0xEA, 0x0A, 0x00 } },
                                                    { 0x00601779, { 0xE8, 0xA2, 0x3D, 0x08, 0x00 } },
                                                    { 0x0060A3C6, { 0xE8, 0x55, 0xB1, 0x07, 0x00 } },
                                                    { 0x0060A471, { 0xE8, 0xAA, 0xB0, 0x07, 0x00 } },
                                                    { 0x00610D44, { 0xE8, 0xD7, 0x47, 0x07, 0x00 } },
                                                    { 0x00610DD4, { 0xE8, 0x47, 0x47, 0x07, 0x00 } },
                                                    { 0x0061986B, { 0xE8, 0xB0, 0xBC, 0x06, 0x00 } },
                                                    { 0x0063C235, { 0xE8, 0xE6, 0x92, 0x04, 0x00 } },
                                                    { 0x0063C3D1, { 0xE8, 0x4A, 0x91, 0x04, 0x00 } },
                                                    { 0x0063C4E9, { 0xE8, 0x32, 0x90, 0x04, 0x00 } },
                                                    { 0x006573F5, { 0xE8, 0x26, 0xE1, 0x02, 0x00 } },
                                                    { 0x00675C64, { 0xE8, 0xB7, 0xF8, 0x00, 0x00 } },
                                                    { 0x007AC27E, { 0xE8, 0x9D, 0x92, 0xED, 0xFF } },
                                                    { 0x007AC40D, { 0xE8, 0x0E, 0x91, 0xED, 0xFF } },
                                                    { 0x00806348, { 0xE8, 0xD3, 0xF1, 0xE7, 0xFF } },
                                                    { 0x008431C8, { 0xE8, 0x53, 0x23, 0xE4, 0xFF } },
                                                    { 0x0092E75A, { 0xE8, 0xC1, 0x6D, 0xD5, 0xFF } },
                                                    { 0x0092F260, { 0xE8, 0xBB, 0x62, 0xD5, 0xFF } },
                                                    { 0x00948A7E, { 0xE8, 0x9D, 0xCA, 0xD3, 0xFF } },
                                                    { 0x0094BD0D, { 0xE8, 0x0E, 0x98, 0xD3, 0xFF } },
                                                    { 0x0095372B, { 0xE8, 0xF0, 0x1D, 0xD3, 0xFF } },
                                                    { 0x0097F789, { 0xE8, 0x92, 0x5D, 0xD0, 0xFF } }, };

        for (const auto &callsite : raise_object_calls) {
            if (!util::redirect_rel32(callsite.address, callsite.expected, raise_object))
                FATAL_BREAKPOINT();
        }

        // ====== //

        using add_code_function = u32(__cdecl*)
            (string_hash, arch_base_vhandle, code_event_callback_function, void*, bool);

        add_code_function add_code = &event_manager::add_callback;

        const event_callsite add_code_calls[] { { 0x006017CE, { 0xE8, 0xCD, 0x4F, 0x08, 0x00 } },
                                                { 0x0065C814, { 0xE8, 0x87, 0x9F, 0x02, 0x00 } },
                                                { 0x0065C860, { 0xE8, 0x3B, 0x9F, 0x02, 0x00 } },
                                                { 0x0065CA6C, { 0xE8, 0x2F, 0x9D, 0x02, 0x00 } },
                                                { 0x0065CAB2, { 0xE8, 0xE9, 0x9C, 0x02, 0x00 } },
                                                { 0x00667EC5, { 0xE8, 0xD6, 0xE8, 0x01, 0x00 } },
                                                { 0x00667F05, { 0xE8, 0x96, 0xE8, 0x01, 0x00 } },
                                                { 0x00669821, { 0xE8, 0x7A, 0xCF, 0x01, 0x00 } },
                                                { 0x0066DB0D, { 0xE8, 0x8E, 0x8C, 0x01, 0x00 } },
                                                { 0x00686994, { 0xE8, 0x07, 0xFE, 0xFF, 0xFF } },
                                                { 0x00717C68, { 0xE8, 0x33, 0xEB, 0xF6, 0xFF } },
                                                { 0x00717CA7, { 0xE8, 0xF4, 0xEA, 0xF6, 0xFF } },
                                                { 0x0076CDBB, { 0xE8, 0xE0, 0x99, 0xF1, 0xFF } },
                                                { 0x0076EC94, { 0xE8, 0x07, 0x7B, 0xF1, 0xFF } },
                                                { 0x0076ECC4, { 0xE8, 0xD7, 0x7A, 0xF1, 0xFF } },
                                                { 0x009848C6, { 0xE8, 0xD5, 0x1E, 0xD0, 0xFF } },
                                                { 0x00984907, { 0xE8, 0x94, 0x1E, 0xD0, 0xFF } }, };

        for (const auto &callsite : add_code_calls) {
            if (!util::redirect_rel32(callsite.address, callsite.expected, add_code))
                FATAL_BREAKPOINT();
        }

        // ====== //

        using add_script_function = u32(__cdecl*)
            (string_hash, arch_base_vhandle, chuck::vm::script_instance*, chuck::vm::script_function*, const void*, bool);

        add_script_function add_script = &event_manager::add_callback;

        const event_callsite add_script_calls[] { { 0x00601823, { 0xE8, 0x08, 0x50, 0x08, 0x00 } },
                                                  { 0x00825ABE, { 0xE8, 0x6D, 0x0D, 0xE6, 0xFF } }, };

        for (const auto &callsite : add_script_calls) {
            if (!util::redirect_rel32(callsite.address, callsite.expected, add_script))
                FATAL_BREAKPOINT();
        }

        // ====== //

        const event_callsite add_default_calls[] { { 0x00667079, { 0xE8, 0x52, 0xF8, 0x01, 0x00 } },
                                                   { 0x00667099, { 0xE8, 0x32, 0xF8, 0x01, 0x00 } },
                                                   { 0x006670B9, { 0xE8, 0x12, 0xF8, 0x01, 0x00 } },
                                                   { 0x006670D9, { 0xE8, 0xF2, 0xF7, 0x01, 0x00 } },
                                                   { 0x006670F9, { 0xE8, 0xD2, 0xF7, 0x01, 0x00 } },
                                                   { 0x00667119, { 0xE8, 0xB2, 0xF7, 0x01, 0x00 } },
                                                   { 0x00667139, { 0xE8, 0x92, 0xF7, 0x01, 0x00 } },
                                                   { 0x00667159, { 0xE8, 0x72, 0xF7, 0x01, 0x00 } },
                                                   { 0x00667179, { 0xE8, 0x52, 0xF7, 0x01, 0x00 } },
                                                   { 0x00667199, { 0xE8, 0x32, 0xF7, 0x01, 0x00 } },
                                                   { 0x006671B9, { 0xE8, 0x12, 0xF7, 0x01, 0x00 } },
                                                   { 0x006671D9, { 0xE8, 0xF2, 0xF6, 0x01, 0x00 } },
                                                   { 0x006671F9, { 0xE8, 0xD2, 0xF6, 0x01, 0x00 } },
                                                   { 0x00667219, { 0xE8, 0xB2, 0xF6, 0x01, 0x00 } },
                                                   { 0x00667239, { 0xE8, 0x92, 0xF6, 0x01, 0x00 } },
                                                   { 0x00667259, { 0xE8, 0x72, 0xF6, 0x01, 0x00 } },
                                                   { 0x00667279, { 0xE8, 0x52, 0xF6, 0x01, 0x00 } },
                                                   { 0x00667299, { 0xE8, 0x32, 0xF6, 0x01, 0x00 } },
                                                   { 0x006672B9, { 0xE8, 0x12, 0xF6, 0x01, 0x00 } },
                                                   { 0x006672D9, { 0xE8, 0xF2, 0xF5, 0x01, 0x00 } },
                                                   { 0x006672F9, { 0xE8, 0xD2, 0xF5, 0x01, 0x00 } },
                                                   { 0x00667319, { 0xE8, 0xB2, 0xF5, 0x01, 0x00 } },
                                                   { 0x00667339, { 0xE8, 0x92, 0xF5, 0x01, 0x00 } },
                                                   { 0x00667359, { 0xE8, 0x72, 0xF5, 0x01, 0x00 } },
                                                   { 0x00667379, { 0xE8, 0x52, 0xF5, 0x01, 0x00 } },
                                                   { 0x00667399, { 0xE8, 0x32, 0xF5, 0x01, 0x00 } },
                                                   { 0x006673B9, { 0xE8, 0x12, 0xF5, 0x01, 0x00 } },
                                                   { 0x006673D9, { 0xE8, 0xF2, 0xF4, 0x01, 0x00 } },
                                                   { 0x006673F9, { 0xE8, 0xD2, 0xF4, 0x01, 0x00 } },
                                                   { 0x00667419, { 0xE8, 0xB2, 0xF4, 0x01, 0x00 } },
                                                   { 0x00667439, { 0xE8, 0x92, 0xF4, 0x01, 0x00 } },
                                                   { 0x00667459, { 0xE8, 0x72, 0xF4, 0x01, 0x00 } },
                                                   { 0x0094B561, { 0xE8, 0x6A, 0xB3, 0xD3, 0xFF } }, };

        for (const auto &callsite : add_default_calls) {
            if (!util::redirect_rel32(callsite.address, callsite.expected, &event_manager::add_default_callback))
                FATAL_BREAKPOINT();
        }
    }

    inline void redirect_game_calls() {
        banana::log.dbg("redirecting nglInit (WinMain)");
        
        if (!util::redirect_rel32(0x009CC222, { 0xE8, 0xA9, 0x9A, 0x01, 0x00 }, &ngl::init))
            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting nglSetBufferSize callsites (WinMain)");

        if (!util::redirect_rel32(0x009CC2D2, { 0xE8, 0x29, 0xC6, 0x01, 0x00 }, &ngl::set_buffer_size) ||
            !util::redirect_rel32(0x009CC311, { 0xE8, 0xEA, 0xC5, 0x01, 0x00 }, &ngl::set_buffer_size) ||
            !util::redirect_rel32(0x009CC321, { 0xE8, 0xDA, 0xC5, 0x01, 0x00 }, &ngl::set_buffer_size) ||
            !util::redirect_rel32(0x009CC334, { 0xE8, 0xC7, 0xC5, 0x01, 0x00 }, &ngl::set_buffer_size))
            
            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting app::tick callsites (WinMain)");

        if (!util::redirect_rel32(0x009CC44C, { 0xE8, 0x3F, 0xD6, 0xA5, 0xFF }, &app::tick) ||
            !util::redirect_rel32(0x009CC477, { 0xE8, 0x14, 0xD6, 0xA5, 0xFF }, &app::tick))

            FATAL_BREAKPOINT();



        banana::log.dbg("DllMain: redirecting input_mgr::poll_devices (movie_manager)");

        if (!util::redirect_rel32(0x006ABCEB, { 0xE8, 0xE0, 0xD5, 0x2A, 0x00 }, &input_mgr::poll_devices))
            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting cutscene_player::is_playing callsites");

        if (!util::redirect_rel32(0x004E0869, { 0xE8, 0xB2, 0x47, 0x32, 0x00 }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x004ECC8B, { 0xE8, 0x90, 0x83, 0x31, 0x00 }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x005D663B, { 0xE8, 0xE0, 0xE9, 0x22, 0x00 }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0061F718, { 0xE8, 0x03, 0x59, 0x1E, 0x00 }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0061F9B7, { 0xE8, 0x64, 0x56, 0x1E, 0x00 }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x006567E1, { 0xE8, 0x3A, 0xE8, 0x1A, 0x00 }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0065A96D, { 0xE8, 0xAE, 0xA6, 0x1A, 0x00 }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x006CC8A3, { 0xE8, 0x78, 0x87, 0x13, 0x00 }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x00715183, { 0xE8, 0x98, 0xFE, 0x0E, 0x00 }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x007A1C93, { 0xE8, 0x88, 0x33, 0x06, 0x00 }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x007A1DEA, { 0xE8, 0x31, 0x32, 0x06, 0x00 }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x007A1E45, { 0xE8, 0xD6, 0x31, 0x06, 0x00 }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x00824EA6, { 0xE8, 0x75, 0x01, 0xFE, 0xFF }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x00842487, { 0xE8, 0x94, 0x2B, 0xFC, 0xFF }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x00876618, { 0xE8, 0x03, 0xEA, 0xF8, 0xFF }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0090453E, { 0xE8, 0xDD, 0x0A, 0xF0, 0xFF }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0090A9C4, { 0xE8, 0x57, 0xA6, 0xEF, 0xFF }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x00948907, { 0xE8, 0x14, 0xC7, 0xEB, 0xFF }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x00950AD7, { 0xE8, 0x44, 0x45, 0xEB, 0xFF }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0095111B, { 0xE8, 0x00, 0x3F, 0xEB, 0xFF }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x009691F6, { 0xE8, 0x25, 0xBE, 0xE9, 0xFF }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x009772FF, { 0xE8, 0x1C, 0xDD, 0xE8, 0xFF }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0097773A, { 0xE8, 0xE1, 0xD8, 0xE8, 0xFF }, &cutscene_player::is_playing) ||
            !util::redirect_rel32(0x009778E9, { 0xE8, 0x32, 0xD7, 0xE8, 0xFF }, &cutscene_player::is_playing))

            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting nglPresent (movie_manager)");

        if (!util::redirect_rel32(0x006ABC12, { 0xE8, 0xE9, 0xE8, 0x32, 0x00 }, &ngl::present))
            FATAL_BREAKPOINT();

        banana::log.dbg("redirecting game::clear_screen callsites");

        if (!util::redirect_rel32(0x0076BC14, { 0xE8, 0xF7, 0xF3, 0x20, 0x00 }, &game::clear_screen) ||
            !util::redirect_rel32(0x0097B495, { 0xE8, 0x76, 0xFB, 0xFF, 0xFF }, &game::clear_screen))

            FATAL_BREAKPOINT();

        _redirect_event_manager();
    }
} // treyarch
