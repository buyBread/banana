#include <bit>
#include <cstdlib>
#include <float.h>
#include <process.h>
#include <string>
#include <time.h>

#include "retail.hh"
#include "aspyr/win/config.hh"
#include "aspyr/win/init.hh"
#include "aspyr/win/localization.hh"
#include "treyarch/WinMain.hh"
#include "treyarch/windows_app.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/post_process/post_process.hh"
#include "treyarch/game/shadow/shadow.hh"
#include "treyarch/game/wds/camera/references.hh"
#include "treyarch/nfl/nfl.hh"
#include "treyarch/ngl/display.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/d3d9/display.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/shared/timing/hires_clock.hh"

namespace treyarch {
    namespace references {
        inline util::memory_reference<u8>        video_confirmation_running { 0x00F4E295 };
        inline util::memory_reference<u64>       log_callback_table         { 0x00F4E2B0 };
        inline util::memory_reference<HINSTANCE> application_instance       { 0x011136FC };
        inline util::memory_reference<void*>     nfl_work_space             { 0x01113700 };
        inline util::memory_reference<u32>       nfl_work_space_size        { 0x01113704 };
    } // references
} // treyarch

using namespace treyarch;

// sub_9CBB80
int WINAPI treyarch::WinMain(HINSTANCE instance, HINSTANCE, LPSTR command_line, int show_command) {
    retail::sub_429C40(command_line);

    const i32 aspyr_error = aspyr::win::init();

    if (aspyr_error != 13) {
        aspyr::win::show_error_message(aspyr_error);

        return 0;
    }

    if (!references::pack_mode.read() && aspyr::win::get_config_number("Version", 0.0, false) != 7.0) {
        wchar_t message[200];
        wchar_t caption[100];
        aspyr::win::get_local_string(L"LauncherPleaseRun", message, 200);
        aspyr::win::get_title(caption, 100);
        MessageBoxW(nullptr, message, caption, 0);

        return 0;
    }

    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    references::processor_count.write((u8)system_info.dwNumberOfProcessors);

    const auto configured_processors = (u8)aspyr::win::get_config_number("MaxCpuCount", 0.0, false);

    if (configured_processors) {
        const auto processor_count = configured_processors < references::processor_count.read() ?
            configured_processors : references::processor_count.read();

        references::processor_count.write(processor_count);

        SetProcessAffinityMask(GetCurrentProcess(), (1u << (processor_count & 31)) - 1);
    }

    char filename[264];
    GetModuleFileNameA(nullptr, filename, 0x104);
    std::string executable_directory(filename);
    executable_directory.resize(executable_directory.rfind('\\') + 1);

    if (!references::pack_mode.read()) {
        SetCurrentDirectoryA(executable_directory.c_str());
        std::string game_root = executable_directory + "..\\..\\";
        SetEnvironmentVariableA("AE_GAME_ROOT_DOS", game_root.c_str());
        std::string environment_entry = "AE_GAME_ROOT_DOS=" + game_root;
        _putenv(environment_entry.c_str());
    }

    _controlfp(_PC_24, _MCW_PC);
    u16 control_word;
    retail::sub_9C8B30(&control_word);

    references::application_instance.write(instance);

    windows_app* application = (windows_app*)memory::heap::allocate(sizeof(windows_app));

    if (application) {
        application->vtable = &references::windows_app_vtable.get();
        application->window = nullptr;
    }

    references::application.write(application);

    mash::string root;
    mash::string image_directory;

    if (references::pack_mode.read()) {
        root = std::getenv("AE_GAME_ROOT_DOS");

        mash::string joined(root);
        joined.append("image\\pc\\");
        image_directory = joined;
    } else {
        root = executable_directory.c_str();
        image_directory = executable_directory.c_str();
    }

    references::image_root.get() = image_directory.c_str();
    references::data_root.get() = image_directory.c_str();

    mash::string shared_config("game_shared.ini");
    retail::sub_7DF700(shared_config.data());

    if (references::pack_mode.read()) {
        references::image_root.get() = root.c_str();

        mash::string data_directory(root);
        data_directory.append("data\\");
        references::data_root.get() = data_directory.c_str();
    }

    HWND window = (HWND)retail::sub_9C8A20(instance);
    references::application.read()->window = window;
    retail::sub_9CC900((i64*)&references::log_callback_table.get());
    retail::sub_4291C0();

    const nfl::init_params nfl_params { references::pack_mode.read() ? 1024 : 128, 64, 64, 3, 1 };
    references::nfl_work_space_size.write(nfl::init(&nfl_params));
    references::nfl_work_space.write(memory::heap::allocate(references::nfl_work_space_size.read()));
    nfl::start(references::nfl_work_space.read());

    std::srand(_time32(nullptr));

    if (!references::pack_mode.read()) {
        ngl::d3d9::set_windowed(aspyr::win::get_config_number("Windowed", 0.0, false) != 0.0);

        const f64 desired_width  = aspyr::win::get_config_number("VideoDesiredW", 0.0, false);
        const f64 desired_height = aspyr::win::get_config_number("VideoDesiredH", 0.0, false);
        f64       width          = aspyr::win::get_config_number("VideoW", 800.0, false);
        f64       height         = aspyr::win::get_config_number("VideoH", 600.0, false);
        bool      mode_changed   = false;

        if (desired_width != 0.0 && desired_height != 0.0 &&
            (width != desired_width || height != desired_height)) {

            mode_changed = true;
            width = desired_width;
            height = desired_height;
        }

        aspyr::win::set_config_number("VideoDesiredW", 0.0);
        aspyr::win::set_config_number("VideoDesiredH", 0.0);

        ngl::d3d9::set_display_mode((i32)width, (i32)height, width / height > 1.366666666666666);

        ngl::d3d9::references::particle_depth_texture_requested.write(aspyr::win::get_config_number("VideoParticles", 0.0, false) != 0.0);
        shadow::references::active.write(aspyr::win::get_config_number("VideoShadows", 0.0, false) != 0.0);
        post_process::references::active.write(aspyr::win::get_config_number("VideoPostProcFx", 0.0, false) != 0.0);

        ngl::init(window);

        if (mode_changed) {
            wchar_t message[200];
            wchar_t caption[100];
            aspyr::win::get_local_string(L"GameVideoModeChanged", message, 200);
            aspyr::win::get_title(caption, 100);
            _beginthread(retail::sub_9C8C00, 0, nullptr);

            if (MessageBoxW(window, message, caption, MB_OKCANCEL) != IDOK) {
                references::video_confirmation_running.write(0);
                retail::sub_9C8B90();
            }

            references::video_confirmation_running.write(0);
            aspyr::win::set_config_number("VideoW", width);
            aspyr::win::set_config_number("VideoH", height);
        }

        ShowCursor(FALSE);
        ngl::set_buffer_size(ngl::buffer_platform_work, 0x10, true, true);
        retail::sub_A374C0(16000);
        retail::sub_A374D0(48000);
        retail::sub_A26530();
        ngl::references::resource_callback.write((ngl::resource_callback)retail::sub_76A960);
        ngl::set_frame_lock(references::default_frame_lock.read());
        ngl::set_buffer_size(ngl::buffer_list_work, 0xC00000, true, true);
        ngl::set_buffer_size(ngl::buffer_scratch_index, 0x3FFFE, true, true);
        ngl::set_buffer_size(ngl::buffer_scratch_vertex, 0x400000, true, true);

        references::camera_aspect_ratio.write(ngl::is_display_widescreen() ?
            std::bit_cast<f32>(u32 { 0x3F100000 }) :
            std::bit_cast<f32>(u32 { 0x3F400000 }));
    }

    retail::sub_9D1800();
    retail::sub_631D30();
    retail::sub_5B4A70(nullptr);
    app::create_inst();

    if (references::pack_mode.read())
        retail::sub_9CC8F0();
    else {
        timing::get_cpu_cycle();

        bool was_active = true;
        MSG message;

        while (!references::game.read()->i_quit) {
            while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
                if (message.message == WM_QUIT) {
                    references::game.read()->i_quit = 1;

                    break;
                }

                TranslateMessage(&message);
                DispatchMessageA(&message);

                if (references::game.read()->i_quit)
                    break;
            }

            if (references::game.read()->i_quit)
                break;

            if (references::window_active.read()) {
                if (!was_active) {
                    was_active = true;
                    retail::sub_9012B0(0);
                }

                hires_clock_t frame_clock;
                timing::get_cpu_cycle();

                app::get().tick();
                app::get().get_game()->frame_timing.total_delta = frame_clock.elapsed();
            } else {
                if (was_active) {
                    was_active = false;
                    retail::sub_9012B0(1);
                    app::get().tick();
                }

                Sleep(250);
            }
        }
    }

    retail::sub_9C8B90();

    return 0;
}
