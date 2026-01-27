#pragma once

#include "imgui.hh"
#include "manager.hh"

class c_gui : public i_gui {

public:
    c_gui() { s_gui_manager::get().register_GUI(this); }
};