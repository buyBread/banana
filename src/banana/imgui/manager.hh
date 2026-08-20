#pragma once

#include <vector>

#include "util/singleton.hh"

class i_gui;

class s_gui_manager : public util::singleton<s_gui_manager> {

    std::vector<i_gui*> m_GUIs;

public:

    void register_GUI(i_gui* GUI);
    void run();
};

class i_gui {

public:

    i_gui() {
        s_gui_manager::get().register_GUI(this);
    }

    virtual bool logic() = 0; // for non-immediate things
    virtual void draw()  = 0;
};

namespace banana { namespace imgui {
    inline auto &gui_manager = s_gui_manager::get();
}} // banana::imgui