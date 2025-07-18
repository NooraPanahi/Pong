#ifndef STATE_H
#define STATE_H
#include <raylib.h>
#include <iostream>
#include "button.hpp"
#include "menu.hpp"

class Menu ;

class State{
    private:
        Texture2D background ; 
    public :
        State(const std::string&) ; 
        Texture2D get_background() const ; 
        virtual void displayState(Menu&) = 0 ; 
        virtual ~State() ; 
};

class StartState : public State {
    private:
        Button startButton ; 
        Button exitButton ; 
        Sound sound ; 
    public:
        StartState() ; 
        void displayState(Menu&) override ;
        ~StartState() ;  
};

class ExitState : public State{
    private:
        ClickableText YesText;
        ClickableText NoText;
        Sound sound ; 
    public:
        ExitState();
        void displayState(Menu&) override;
        ~ExitState() ; 
} ; 

class NameInputState : public State {
    private:
        TextBox box;
        Rectangle continueButton;
        Sound sound ; 
    public:
        NameInputState();
        void displayState(Menu&) override;
        ~NameInputState() ; 
};

#endif