#ifndef TOOLS_H
#define TOOLS_H
#include <raylib.h>
#include <cmath>

class Ball {
public:
    Ball(float x, float y);
    void draw() const;
    void update();
    void reset();
    bool checkCollision(Rectangle paddle) const;
    bool isOutOfBounds() const;
    void bounceX();
    void bounceY();
    Vector2 getPosition() const;
    float getX() const;
    float getY() const;
    Vector2 getVelocity() const;
    void setVelocityY(float newY);
    void checkWallCollision();

private:
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
};

class Paddle {
public:
    Paddle(float x, float y);
    void draw() const;
    void moveUp();
    void moveDown();
    void reset();
    void updateAI(const Ball& ball);

    Rectangle getRect() const;
    float getY() const;
    float getHeight() const;

    void setSpeed(float newSpeed);
    void setReactionDelay(float delay);
    void setErrorRange(float range);

private:
    Rectangle rect;
    float speed;
    float errorRange;
    float reactionDelay;
    double lastReactionTime;

    bool shouldReactToBall(const Ball& ball) const;
    float calculateTargetY(const Ball& ball) const;
    void moveTowards(float targetY);
};


#endif