#ifndef TOOLS_H
#define TOOLS_H
#include <raylib.h>
#include <cmath>
class Paddle {
private:
    Rectangle rect;
    float speed;
    float lastReactionTime = 0;
    float errorRange = 15.0f;
    
public:
    Paddle(float x, float y);
    void draw() const;
    void moveUp();
    void moveDown();
    void aiMove(float ballY, float ballSpeedX);
    void reset();
    void setSpeed(float newSpeed);
    Rectangle getRect() const { return rect; }
    
    void setErrorRange(float range) { errorRange = range; }
};

class Ball {
private:
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color ; 
    
public:
    Ball(float x, float y);
    void draw() const;
    void update();
    void bounceX();
    void bounceY();
    void reset(float x, float y);
    bool checkCollision(Rectangle paddle) const ;
    bool outOfBounds() const;
    float getX() const { return position.x; }
    float getY() const { return position.y; }
    void setVelocityY(float newY) ;
    Vector2 getVelocity() ; 
};

#endif