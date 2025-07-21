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
            menu.SetState(new DifficultySelectionState(box.text)) ;
            PlaySound(sound) ; 
        }
        else 
            DrawText("Please fill all fields!", 300, 550, 20, RED);  
    }
}

NameInputState::~NameInputState(){
    UnloadSound(sound) ; 
}
DifficultySelectionState::DifficultySelectionState(const std::string& name)
    : State("../Assets/Background.png"), playerName(name),
      easy("EASY", {200, 300}, 40, GREEN),
      medium("MEDIUM", {200, 400}, 40, YELLOW),
      hard("HARD", {200, 500}, 40, RED),
      back("Back", {100, 900}, 30, WHITE)
{
    clickSound = LoadSound("../Assets/Click.wav");
}

DifficultySelectionState::~DifficultySelectionState() {
    UnloadSound(clickSound);
}

void DifficultySelectionState::displayState(Menu& menu) {
    DrawTexture(get_background(), 0, 0, WHITE);
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    easy.Draw(mouse);
    medium.Draw(mouse);
    hard.Draw(mouse);
    back.Draw(mouse);

    if (easy.isClicked(mouse, click)) {
        PlaySound(clickSound);
        menu.SetState(new GameState(playerName, GameState::EASY));
    }
    if (medium.isClicked(mouse, click)) {
        PlaySound(clickSound);
        menu.SetState(new GameState(playerName, GameState::MEDIUM));
    }
    if (hard.isClicked(mouse, click)) {
        PlaySound(clickSound);
        menu.SetState(new GameState(playerName, GameState::HARD));
    }
    if (back.isClicked(mouse, click)) {
        PlaySound(clickSound);
        menu.SetState(new NameInputState());
    }
}

GameState::GameState(std::string playername , Difficulty diff)
    : State("../Assets/background-in-game.jpg"),
      player(50, GetScreenHeight()/2 - 50),
      ai(GetScreenWidth() - 70, GetScreenHeight()/2 - 50),
      ball(GetScreenWidth()/2, GetScreenHeight()/2),
      playerScore(0), aiScore(0),
      pause {"pause" ,{80 , 850} , 30 , WHITE} ,
      playername(playername)
{
    setDifficulty(diff) ;
    hitSound = LoadSound("../Assets/Click.wav");
    font = LoadFont("../Assets/BitcountSingle_Cursive-ExtraLight.ttf");
    paused = false ; 
    
}

GameState::~GameState() {
    UnloadSound(hitSound);
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
    
    BeginDrawing();
    ClearBackground(BLACK); 
    Rectangle src = {0.0f , 0.0f , (float)get_background().width , (float)get_background().height};
    Rectangle dest = { 0.0f, 0.0f, float(GetScreenWidth()), float(GetScreenHeight()) };
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(get_background() , src , dest , origin, 0.0f , WHITE); 

    Vector2 mousePos = GetMousePosition() ; 
    bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    pause.Draw(mousePos) ;
    if(pause.isClicked(mousePos , mouseClicked)){
        paused = !paused ; 
    } 

    if(paused)
        DrawText("Game Paused" , 370 , 450 , 30 , WHITE) ;

    if(!paused){
        player.draw();
        ai.draw();
        ball.draw();
        handleInput();
        update();        
    }

    if (isGameOver()) {
        std::string winner = (playerScore > aiScore) ? playername : "AI";
        menu.SetState(new GameOverState(winner));
        return; 
    }

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

    std::string scoreText = playername + ": " + std::to_string(playerScore) ;
    std::string Aiscore = "AI: " + std::to_string(aiScore) ;  
    DrawTextEx(font, scoreText.c_str(), Vector2{ 50.0f, 20.0f }, 30, 2, GREEN);
    DrawTextEx(font, Aiscore.c_str(), Vector2{ static_cast<float>(GetScreenWidth()) - 150.0f, 20.0f }, 30, 2, RED);

}

bool GameState::isGameOver() const {
    return playerScore >= 5 || aiScore >= 5;
}

GameOverState::GameOverState(const std::string& winner)
    : State("../Assets/Background.png"),
      winnerText(winner + " WINS!"),
      backToMenu("Back to Menu", {300, 500}, 40, WHITE) {
    clickSound = LoadSound("../Assets/Click.wav");
}

GameOverState::~GameOverState() {
    UnloadSound(clickSound);
}

void GameOverState::displayState(Menu& menu) {
    DrawTexture(get_background(), 0, 0, WHITE);
    
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    
    Vector2 textSize = MeasureTextEx(GetFontDefault(), winnerText.c_str(), 60, 2);
    Vector2 textPos = {
        (GetScreenWidth() - textSize.x)/2,
        (GetScreenHeight() - textSize.y)/2 - 50
    };
    DrawTextEx(GetFontDefault(), winnerText.c_str(), textPos, 60, 2, GOLD);
    
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    backToMenu.Draw(mouse);
    if (backToMenu.isClicked(mouse, click)) {
        PlaySound(clickSound);
        menu.SetState(new StartState());
    }
}