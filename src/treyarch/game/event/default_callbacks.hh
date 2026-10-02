#pragma once

namespace treyarch {
    // app::app runs it once, right after the event manager and the other singletons exist
    void register_default_event_callbacks();
} // treyarch
