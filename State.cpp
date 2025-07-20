#include "State.hpp"
#include "menu.hpp"
#include <iostream>

State::State(const std::string& bgPath) {
    background = LoadTexture(bgPath.c_str());
}

State::~State(){
    UnloadTexture(background) ;
}

Texture2D State::get_background() const{
    return background;
}

StartState::StartState() 
    : State("../Assets/Background.png"), 
      startButton("../Assets/Startgame.png", {170, 300}),
      exitButton("../Assets/Exit.png", {170, 500}){
    sound = LoadSound("../Assets/Click.wav");    
      }

void StartState::displayState(Menu& menu)  {
        DrawTexture(get_background(), 0, 0, WHITE);

        Vector2 mouse = GetMousePosition();
        bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        startButton.Draw(mouse);
        exitButton.Draw(mouse);

        if (startButton.isPressed(mouse, click)) {
            PlaySound(sound) ; 
            menu.SetState(new NameInputState()); 
        }
        if (exitButton.isPressed(mouse, click)) {
            PlaySound(sound) ; 
            menu.SetState(new ExitState) ; 
        }
    }

StartState::~StartState(){
    UnloadSound(sound) ;
}

ExitState::ExitState() 
    : State("../Assets/Background.png"),
      YesText("Yes", {280, 400}, 30, DARKGRAY),
      NoText("No", {480, 400}, 30, DARKGRAY){
        sound = LoadSound("../Assets/Click.wav"); 
      }

void ExitState::displayState(Menu& menu) {
    DrawTexture(get_background(), 0, 0, WHITE);

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

    DrawText("Are you sure you want to exit?", 200, 200, 30, WHITE);

    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    YesText.Draw(mouse);
    NoText.Draw(mouse);

    if (YesText.isClicked(mouse, click)) {
        PlaySound(sound) ;
        WaitTime(0.5);
        CloseWindow();
    }

    if (NoText.isClicked(mouse, click)) {
        PlaySound(sound) ; 
        menu.SetState(new StartState());
    }
}

ExitState::~ExitState(){   
    UnloadSound(sound) ; 
}

NameInputState::NameInputState() : State("../Assets/Background.png") {
    box = TextBox({100, 150, 300, 40});
    continueButton = {300, 500, 200, 50};
    sound = LoadSound("../Assets/Click.wav"); 
}

void NameInputState::displayState(Menu& menu) {

    DrawTexture(get_background(), 0, 0, WHITE);
    ClickableText BackToMenu = {"Back to menu" , {100,900} , 30 , RED} ; 

    Vector2 mousePos = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    BackToMenu.Draw(mousePos) ;

    if(BackToMenu.isClicked(mousePos , mouseClicked)){
        PlaySound(sound) ; 
        menu.SetState(new StartState) ;
    }
    
    if (mouseClicked) {
            box.active = false;
            
        if (CheckCollisionPointRec(mousePos, box.rect)) {
            box.active = true;
        }       
    }

        if (box.active) {
            int key = GetCharPressed();
            while (key > 0) {
                
                if ((isalpha(key) || isdigit(key) || key == ' ') && box.text.length() < 30) {
                    box.text += (char)key;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !box.text.empty()) {
                box.text.pop_back();
            }
        }

    DrawText("Enter Your Name:", 100, 100, 20, RED);
    
    DrawRectangleRec(box.rect, box.active ? LIGHTGRAY : GRAY);
    DrawRectangleLinesEx(box.rect, 2, box.active ? RED : DARKGRAY);
    
    if (!box.text.empty() || box.active) {
        DrawText(box.text.c_str(), box.rect.x + 10, box.rect.y + 10, 20, BLACK);
    }
    
    if (box.active && ((int)(GetTime()*2) % 2 == 0)) {
        int textWidth = MeasureText(box.text.c_str(), 20);
        DrawText("|", box.rect.x + 10 + textWidth, box.rect.y + 10, 20, BLACK);
    }


    bool allFilled = !box.text.empty() ;    
    DrawRectangleRec(continueButton, allFilled ? GREEN : GRAY);
    DrawText("Continue", continueButton.x + 50, continueButton.y + 15, 20, WHITE);
    
    if (mouseClicked && CheckCollisionPointRec(mousePos, continueButton)) {
        if (allFilled){
            menu.SetState(new GameState) ;
            PlaySound(sound) ; 
        }
        else 
            DrawText("Please fill all fields!", 300, 550, 20, RED);  
    }
}

NameInputState::~NameInputState(){
    UnloadSound(sound) ; 
}

GameState::GameState() 
    : State("../Assets/Background.png"),
      playerPaddle(50, GetScreenHeight()/2 - 50),
      aiPaddle(GetScreenWidth() - 70, GetScreenHeight()/2 - 50),
      gameBall(GetScreenWidth()/2, GetScreenHeight()/2) {
    setDifficulty(HARD); // یا EASY/HARD
    
    // hitSound = LoadSound("../Assets/sfx/hit.wav");
    // scoreSound = LoadSound("../Assets/sfx/score.wav");
    // gameFont = LoadFont("../Assets/fonts/game_font.ttf");

    // SetSoundVolume(hitSound, 0.7f);
    // SetSoundVolume(scoreSound, 0.5f);

}

GameState::~GameState() {
    UnloadSound(hitSound);
    UnloadSound(scoreSound);
    UnloadFont(gameFont);
}
void GameState::displayState(Menu& menu) {
    updatePhysics();
    
    BeginDrawing();
        ClearBackground(BLACK);
        
        DrawTexture(get_background(), 0, 0, WHITE);
        
        for (int i = 0; i < GetScreenHeight(); i += 20) {
            DrawRectangle(GetScreenWidth()/2 - 2, i, 4, 10, WHITE);
        }
        
        playerPaddle.draw();
        aiPaddle.draw();
        gameBall.draw();
        
        DrawTextEx(gameFont, 
                  TextFormat("PLAYER: %i", playerScore), 
                  {(float)GetScreenWidth()/4 - 100, 20}, 
                  30, 0, GREEN);
                  
        DrawTextEx(gameFont, 
                  TextFormat("AI: %i", aiScore), 
                  {3.0f*GetScreenWidth()/4 - 50, 20}, 
                  30, 0, RED);
        
         // دکمه بازگشت به منو

    EndDrawing();
}
void GameState::updatePhysics() {
    if (IsKeyDown(KEY_W)) playerPaddle.moveUp();
    if (IsKeyDown(KEY_S)) playerPaddle.moveDown();
    
    aiPaddle.aiMove(gameBall.getY(), gameBall.getVelocity().x);    
    gameBall.update();
    
    if (gameBall.checkCollision(playerPaddle.getRect()) || 
        gameBall.checkCollision(aiPaddle.getRect())) {
        PlaySound(hitSound);
        gameBall.bounceX();
    }
    if (abs(gameBall.getVelocity().x) > 8.0f) { 
        aiPaddle.setSpeed(4.0f);
    } else {
        aiPaddle.setSpeed(6.0f);
    }
    if (gameBall.outOfBounds()) {
        PlaySound(scoreSound);
        (gameBall.getX() < 0) ? aiScore++ : playerScore++;
        resetGame();
    }
}
void GameState::resetGame() {
    playerPaddle.reset();
    aiPaddle.reset();
    
    gameBall.reset(GetScreenWidth()/2, GetScreenHeight()/2);
    
    if (playerScore >= 5 || aiScore >= 5) { 
        WaitTime(1.5);
    } else {
        WaitTime(0.05);
    }
}

// تنظیم دشواری بازی:
void GameState::setDifficulty(Difficulty diff) {
    switch(diff) {
        case EASY:
            aiPaddle.setSpeed(5.0f);
            aiPaddle.setErrorRange(25.0f);
            break;
        case MEDIUM:
            aiPaddle.setSpeed(7.0f);
            aiPaddle.setErrorRange(15.0f);
            break;
        case HARD:
            aiPaddle.setSpeed(10.0f);
            aiPaddle.setErrorRange(5.0f);
            break;
    }
}