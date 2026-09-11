#include "treyarch/game/frontend/ui_frontend.hh"

using namespace treyarch;

void ui_frontend::draw() {
    vtable->draw(this);
}

void ui_frontend::draw_startup() {
    vtable->draw_startup(this);
}

void ui_frontend::draw_quad_list() {
    vtable->draw_quad_list(this);
}

void ui_frontend::clear_quad_list() {
    quad_list_state_200 = 0;

    container::clear_legacy_list(&quad_list);
}
