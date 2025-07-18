#include "button.hpp"

ClickableText::ClickableText(const std::string& txt, Vector2 pos, int size, Color col)
    : text(txt), position(pos), fontSize(size), color(col) {
    int width = MeasureText(text.c_str(), fontSize);
    bounds = { pos.x, pos.y, (float)width, (float)fontSize };
}

void ClickableText::Draw(Vector2 mousePos) {
    DrawText(text.c_str(), position.x, position.y, fontSize, color);
    if (CheckCollisionPointRec(mousePos, bounds)) {
        DrawRectangleLines(bounds.x, bounds.y, bounds.width, bounds.height, RED);
    }
}

bool ClickableText::isClicked(Vector2 mousePos, bool click) const {
    if(click && CheckCollisionPointRec(mousePos, bounds)){
        return true ; 
    }
    return false ; 
}
// ClickableText::~ClickableText(){
//     UnloadSound(sound) ; 
// }

Button::Button(const std::string& path, Vector2 position) {
    texture = LoadTexture(path.c_str());
    bounds = { position.x, position.y, (float)texture.width, (float)texture.height };
}
void Button::Draw(Vector2 mousePos){
    DrawTexture(texture, bounds.x, bounds.y, WHITE);
    if (CheckCollisionPointRec(mousePos, bounds)) {
        DrawRectangleLines(bounds.x, bounds.y, bounds.width, bounds.height, RED);
    }
}

bool Button::isPressed(Vector2 mousePos, bool click) {
    if(click && CheckCollisionPointRec(mousePos, bounds)){
        return true ; 
    }
    return false ; 
}

Button::~Button() {
    UnloadTexture(texture);
}
