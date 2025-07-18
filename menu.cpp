// menu.cpp
#include "menu.hpp"

Menu::Menu() : current_state(nullptr) {}

Menu::~Menu() {
    if (current_state) delete current_state;
}

void Menu::SetState(State* s) {
    if (current_state) delete current_state;
    current_state = s;
}

void Menu::renderCurrentState() {
    if (current_state) {
        current_state->displayState(*this);
    } else {
        DrawText("No State Selected!", 100, 100, 20, RED);
    }
}
