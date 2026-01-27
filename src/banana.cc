#include <cassert>
#include <d3d9.h>

#include "banana.hh"
#include "hooks/manager.hh"
#include "ngl/macros.hh"
#include "ngl/renderer/d3d9.hh"
#include "ngl/renderer/profiler.hh"

namespace banana {
    void main() {
        core::init();
        core::spin();
        core::shutdown();
    }

namespace core {
    void init() {
        s_hook_manager::get().install_all();
        
        { using namespace NGL_RENDERER;
            profiler::framerate_panel.write(profiler::framerate::compact);

            // this should always be true
            assert((void*)d3d9_device.read() == (void*)store::d3d9_device);
        }
    }

    void spin() {
        while (running) {
            std::this_thread::sleep_for(44s);

            LOG.flush(); // periodically flush prints
        }
    }

    void shutdown() {
        banana::safe = false;
        banana::safe.notify_all();

        s_hook_manager::get().uninstall_all();

        LOG.flush();
        LOG.close();
    }
} // core

} // banana
