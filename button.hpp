#ifndef BUTTON_H
#define BUTTON_H

#include <raylib.h>
#include <string>

class ClickableText {
private:
    std::string text;
    Vector2 position;
    int fontSize;
    Color color;
    Rectangle bounds;
public:
    ClickableText(const std::string&, Vector2, int, Color);
    void Draw(Vector2 mousePos);
    bool isClicked(Vector2 mousePos, bool click) const;
};


class Button {
    private:
        Texture2D texture ; 
        Rectangle bounds;
    public:
        Button(const std::string& path, Vector2 position);
        void Draw(Vector2 mousePos);
        bool isPressed(Vector2 mousePos, bool click);
        ~Button();
};

struct TextBox {
    Rectangle rect;
    std::string text;
    bool active;
    TextBox(): rect({0,0,0,0}), text(""), active(false) {} ;

    TextBox(Rectangle r) : 
        rect(r), text(""), active(false) {}
};
#endif