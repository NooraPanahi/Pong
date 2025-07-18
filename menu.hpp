#ifndef MENU_H
#define MENU_H
#include "State.hpp"

class State ; 

class Menu{
    private : 
        State* current_state ; 
    public:
        Menu() ; 
        void SetState(State*) ; 
        void renderCurrentState(); 
        ~Menu();
};


#endif