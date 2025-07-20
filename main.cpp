#include <raylib.h>
#include "menu.hpp"
int main() {
    InitWindow(1000, 1000, "Menu");
    InitAudioDevice(); 
    SetTargetFPS(60);

    Menu menu;
    menu.SetState(new StartState());

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        menu.renderCurrentState();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
