/*
    this isn't actually the entry.
    DllMain should be used for when you need to do something hyper-early.
*/

#include <windows.h>

#include "flags.hh"
#include "util/redirect_rel32.hh"
#include "util/macros/debug.hh"
#include "banana/logging.hh"
#include "banana/core.hh"

#if REL32_GAME_CALLS
    #include "treyarch/app/app.hh"
    #include "treyarch/game/cutscene/cutscene_player.hh"
    #include "treyarch/game/game.hh"
    #include "treyarch/game/input/input_mgr.hh"
    #include "treyarch/ngl/ngl.hh"
#endif

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) { 
        DisableThreadLibraryCalls(module);

        if (!banana::vibe_check())
            return FALSE;

#if REL32_GAME_CALLS
        banana::log.dbg("DllMain: redirecting nglInit (WinMain)");
        
        if (!util::redirect_rel32(0x009CC222, { 0xE8, 0xA9, 0x9A, 0x01, 0x00 }, &treyarch::ngl::init))
            FATAL_BREAKPOINT();

        banana::log.dbg("DllMain: redirecting nglSetBufferSize callsites (WinMain)");

        if (!util::redirect_rel32(0x009CC2D2, { 0xE8, 0x29, 0xC6, 0x01, 0x00 }, &treyarch::ngl::set_buffer_size) ||
            !util::redirect_rel32(0x009CC311, { 0xE8, 0xEA, 0xC5, 0x01, 0x00 }, &treyarch::ngl::set_buffer_size) ||
            !util::redirect_rel32(0x009CC321, { 0xE8, 0xDA, 0xC5, 0x01, 0x00 }, &treyarch::ngl::set_buffer_size) ||
            !util::redirect_rel32(0x009CC334, { 0xE8, 0xC7, 0xC5, 0x01, 0x00 }, &treyarch::ngl::set_buffer_size))
            
            FATAL_BREAKPOINT();

        banana::log.dbg("DllMain: redirecting app::tick callsites (WinMain)");

        if (!util::redirect_rel32(0x009CC44C, { 0xE8, 0x3F, 0xD6, 0xA5, 0xFF }, &treyarch::app::tick) ||
            !util::redirect_rel32(0x009CC477, { 0xE8, 0x14, 0xD6, 0xA5, 0xFF }, &treyarch::app::tick))

            FATAL_BREAKPOINT();

        banana::log.dbg("DllMain: redirecting input_mgr::poll_devices (movie_manager)");

        if (!util::redirect_rel32(0x006ABCEB, { 0xE8, 0xE0, 0xD5, 0x2A, 0x00 }, &treyarch::input_mgr::poll_devices))
            FATAL_BREAKPOINT();

        banana::log.dbg("DllMain: redirecting cut_scene_player::is_playing callsites");

        if (!util::redirect_rel32(0x004E0869, { 0xE8, 0xB2, 0x47, 0x32, 0x00 }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x004ECC8B, { 0xE8, 0x90, 0x83, 0x31, 0x00 }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x005D663B, { 0xE8, 0xE0, 0xE9, 0x22, 0x00 }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0061F718, { 0xE8, 0x03, 0x59, 0x1E, 0x00 }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0061F9B7, { 0xE8, 0x64, 0x56, 0x1E, 0x00 }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x006567E1, { 0xE8, 0x3A, 0xE8, 0x1A, 0x00 }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0065A96D, { 0xE8, 0xAE, 0xA6, 0x1A, 0x00 }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x006CC8A3, { 0xE8, 0x78, 0x87, 0x13, 0x00 }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x00715183, { 0xE8, 0x98, 0xFE, 0x0E, 0x00 }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x007A1C93, { 0xE8, 0x88, 0x33, 0x06, 0x00 }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x007A1DEA, { 0xE8, 0x31, 0x32, 0x06, 0x00 }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x007A1E45, { 0xE8, 0xD6, 0x31, 0x06, 0x00 }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x00824EA6, { 0xE8, 0x75, 0x01, 0xFE, 0xFF }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x00842487, { 0xE8, 0x94, 0x2B, 0xFC, 0xFF }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x00876618, { 0xE8, 0x03, 0xEA, 0xF8, 0xFF }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0090453E, { 0xE8, 0xDD, 0x0A, 0xF0, 0xFF }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0090A9C4, { 0xE8, 0x57, 0xA6, 0xEF, 0xFF }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x00948907, { 0xE8, 0x14, 0xC7, 0xEB, 0xFF }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x00950AD7, { 0xE8, 0x44, 0x45, 0xEB, 0xFF }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0095111B, { 0xE8, 0x00, 0x3F, 0xEB, 0xFF }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x009691F6, { 0xE8, 0x25, 0xBE, 0xE9, 0xFF }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x009772FF, { 0xE8, 0x1C, 0xDD, 0xE8, 0xFF }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x0097773A, { 0xE8, 0xE1, 0xD8, 0xE8, 0xFF }, &treyarch::cutscene_player::is_playing) ||
            !util::redirect_rel32(0x009778E9, { 0xE8, 0x32, 0xD7, 0xE8, 0xFF }, &treyarch::cutscene_player::is_playing))

            FATAL_BREAKPOINT();

        banana::log.dbg("DllMain: redirecting nglPresent (movie_manager)");

        if (!util::redirect_rel32(0x006ABC12, { 0xE8, 0xE9, 0xE8, 0x32, 0x00 }, &treyarch::ngl::present))
            FATAL_BREAKPOINT();

        banana::log.dbg("DllMain: redirecting game::clear_screen callsites");

        if (!util::redirect_rel32(0x0076BC14, { 0xE8, 0xF7, 0xF3, 0x20, 0x00 }, &treyarch::game::clear_screen) ||
            !util::redirect_rel32(0x0097B495, { 0xE8, 0x76, 0xFB, 0xFF, 0xFF }, &treyarch::game::clear_screen))

            FATAL_BREAKPOINT();
#endif
    }

    return TRUE;
}
