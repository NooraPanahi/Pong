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
            menu.SetState(new GameState()) ;
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
      player(50, GetScreenHeight()/2 - 50),
      ai(GetScreenWidth() - 70, GetScreenHeight()/2 - 50),
      ball(GetScreenWidth()/2, GetScreenHeight()/2),
      playerScore(0), aiScore(0), currentDiff(MEDIUM) {
    hitSound = LoadSound("../Assets/sfx/hit.wav");
    scoreSound = LoadSound("../Assets/sfx/score.wav");
    font = LoadFont("../Assets/BitcountSingle_Cursive-ExtraLight.ttf");
}

GameState::~GameState() {
    UnloadSound(hitSound);
    UnloadSound(scoreSound);
    UnloadFont(font);
}

void GameState::setDifficulty(Difficulty diff) {
    currentDiff = diff;
    switch (diff) {
        case EASY: ai.setSpeed(6.0f); ai.setErrorRange(40.0f); ai.setReactionDelay(0.05f); break;
        case MEDIUM: ai.setSpeed(8.0f); ai.setErrorRange(25.0f); ai.setReactionDelay(0.02f); break;
        case HARD: ai.setSpeed(10.0f); ai.setErrorRange(10.0f); ai.setReactionDelay(0.005f); break;
    }
}

void GameState::displayState(Menu& menu) {
    handleInput();
    update();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(get_background(), 0, 0, WHITE);
    player.draw();
    ai.draw();
    ball.draw();
    drawScores();
    EndDrawing();
}

void GameState::handleInput() {
    if (IsKeyDown(KEY_UP)) player.moveUp();
    if (IsKeyDown(KEY_DOWN)) player.moveDown();
}

void GameState::update() {
    ai.updateAI(ball);
    ball.update();
    checkCollision();

    if (ball.isOutOfBounds()) {
        if (ball.getX() < 0) aiScore++;
        else playerScore++;
        resetRound();
    }
}

void GameState::checkCollision() {
    if (ball.checkCollision(player.getRect()) || ball.checkCollision(ai.getRect())) {
        PlaySound(hitSound);
        ball.bounceX();
        float hitY = (ball.getY() - player.getY()) / player.getHeight();
        ball.setVelocityY(hitY * 10.0f);
    }
}

void GameState::resetRound() {
    player.reset();
    ai.reset();
    ball.reset();
}

void GameState::drawScores() const {
    DrawTextEx(font, TextFormat("Player: %d", playerScore), Vector2{ 50.0f, 20.0f }, 30, 2, GREEN);
    DrawTextEx(font, TextFormat("AI: %d", aiScore), Vector2{ static_cast<float>(GetScreenWidth()) - 150.0f, 20.0f }, 30, 2, RED);

    if (isGameOver()) {
        const char* winner = (playerScore > aiScore) ? "Player Wins!" : "AI Wins!";
        
        // محاسبه عرض متن با فونت فعلی
        Vector2 textSize = MeasureTextEx(font, winner, 40, 2);
        
        Vector2 position = {
            (GetScreenWidth() - textSize.x) / 2.0f,
            GetScreenHeight() / 2.0f
        };
        
        DrawTextEx(font, winner, position, 40, 2, YELLOW);
    }
}

bool GameState::isGameOver() const {
    return playerScore >= 5 || aiScore >= 5;
}
