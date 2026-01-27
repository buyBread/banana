#pragma once

#include <vector>

#include "../singleton.hh"

class i_gui {

public:

    virtual bool logic() = 0; // for non-immediate things
    virtual void draw()  = 0;
};

class s_gui_manager : public c_singleton<s_gui_manager> {

    std::vector<i_gui*> m_GUIs;

public:

    void register_GUI(i_gui* GUI);
    void render();
};