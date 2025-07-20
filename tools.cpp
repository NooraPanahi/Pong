#include "tools.hpp"
#include <cmath>

Paddle::Paddle(float x, float y) 
    : rect{x, y, 20, 100},
      speed{8.0f} {}

void Paddle::draw() const {
    DrawRectangleRec(rect, WHITE);
}

void Paddle::moveUp() {
    rect.y -= speed;
    if (rect.y < 0) rect.y = 0;
}

void Paddle::moveDown() {
    rect.y += speed;
    if (rect.y > GetScreenHeight() - rect.height)
        rect.y = GetScreenHeight() - rect.height;
}

void Paddle::aiMove(float ballY, float ballSpeedX) {
    float dynamicError = GetRandomValue(-errorRange, errorRange) * (1.0f + std::abs(ballSpeedX)/10.0f);
    
    float paddleCenter = rect.y + rect.height/2 + dynamicError;
    float reactionThreshold = 20.0f;
    
    if (GetRandomValue(0, 100) < 80 && GetTime() > lastReactionTime + 0.1f) {
        
        if (paddleCenter < ballY - reactionThreshold) {
            moveDown();
            lastReactionTime = GetTime();
        }
        else if (paddleCenter > ballY + reactionThreshold) {
            moveUp();
            lastReactionTime = GetTime();
        }
    }
}

void Paddle::reset() {
    rect.y = GetScreenHeight()/2 - rect.height/2;
}

void Paddle::setSpeed(float newSpeed) {
    speed = newSpeed;
}

Ball::Ball(float x, float y) 
    : position{x, y},
      velocity{5.0f, 5.0f},
      radius{10.0f},
      color{WHITE} {}

void Ball::draw() const {
    DrawCircleV(position, radius, color);
}

void Ball::update() {
    position.x += velocity.x;
    position.y += velocity.y;
    
    if (position.y <= radius || position.y >= GetScreenHeight() - radius) {
        bounceY();
    }
}

void Ball::bounceX() {
    velocity.x *= -1;
    velocity.x *= 1.05f; 
    velocity.y *= 1.05f;
}

void Ball::bounceY() {
    velocity.y *= -1;
}

bool Ball::checkCollision(Rectangle paddle) const {
    return CheckCollisionCircleRec(position, radius, paddle);
}

bool Ball::outOfBounds() const {
    return (position.x < -radius || position.x > GetScreenWidth() + radius);
}

void Ball::reset(float x, float y) {
    position = {x, y};
    velocity = {5.0f * (GetRandomValue(0, 1) ? 1 : -1), 
                5.0f * (GetRandomValue(0, 1) ? 1 : -1)};
}
void Ball::setVelocityY(float newY) {
    velocity.y = newY;
}

Vector2 Ball::getVelocity(){
    return velocity;
}
