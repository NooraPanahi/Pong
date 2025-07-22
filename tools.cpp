#include "tools.hpp"
#include <cmath>


Paddle::Paddle(float x, float y): rect{ x, y, 20, 100 }, speed{ 8.0f }, errorRange{ 15.0f }, reactionDelay{ 0.1f }, lastReactionTime{ 0.0 } {}

void Paddle::draw() const {
    DrawRectangleRec(rect, WHITE);
}

void Paddle::moveUp() {
    rect.y -= speed;
    if (rect.y < 0) rect.y = 0;
}

void Paddle::moveDown() {
    rect.y += speed;
    if (rect.y > GetScreenHeight() - rect.height) rect.y = GetScreenHeight() - rect.height;
}

void Paddle::reset() {
    rect.y = GetScreenHeight() / 2 - rect.height / 2;
}

void Paddle::updateAI(const Ball& ball) {
    float targetY = calculateTargetY(ball);
    moveTowards(targetY);
}


bool Paddle::shouldReactToBall(const Ball& ball) const {
    return GetTime() - lastReactionTime >= reactionDelay;
}

float Paddle::calculateTargetY(const Ball& ball) const {
    float predictionError = GetRandomValue(-errorRange, errorRange);
    float predictedY = ball.getY() + predictionError;

    if (predictedY < 0) {
        predictedY = 0;
    } else if (predictedY > GetScreenHeight() - rect.height) {
        predictedY = GetScreenHeight() - rect.height;
    }

    return predictedY;
}


void Paddle::moveTowards(float targetY) {
    float paddleCenter = rect.y + rect.height / 2;
    float threshold = 15.0f;
    if (paddleCenter < targetY - threshold) moveDown();
    else if (paddleCenter > targetY + threshold) moveUp();
    lastReactionTime = GetTime();
}

Rectangle Paddle::getRect() const { return rect; }
float Paddle::getY() const { return rect.y; }
float Paddle::getHeight() const { return rect.height; }
void Paddle::setSpeed(float newSpeed) { speed = newSpeed; }
void Paddle::setReactionDelay(float delay) { reactionDelay = delay; }
void Paddle::setErrorRange(float range) { errorRange = range; }

Ball::Ball(float x, float y): position{ x, y }, velocity{ 5.0f, 5.0f }, radius{ 10.0f }, color{ WHITE } {}

void Ball::draw() const {
    DrawCircleV(position, radius, color);
}

void Ball::update() {
    position.x += velocity.x;
    position.y += velocity.y;
    checkWallCollision();
}
void Ball::reset() {
    position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    float dirX = (GetRandomValue(0, 1) == 0) ? -1.0f : 1.0f;
    float dirY = (GetRandomValue(0, 1) == 0) ? -1.0f : 1.0f;
    velocity = { 5.0f * dirX, 5.0f * dirY };
}

bool Ball::checkCollision(Rectangle paddle) const {
    return CheckCollisionCircleRec(position, radius, paddle);
}

bool Ball::isOutOfBounds() const {
    return position.x < 0 || position.x > GetScreenWidth();
}

void Ball::bounceX() {
    velocity.x *= -1;
}

void Ball::bounceY() {
    velocity.y *= -1;
}

Vector2 Ball::getPosition() const { return position; }
float Ball::getX() const { return position.x; }
float Ball::getY() const { return position.y; }
Vector2 Ball::getVelocity() const { return velocity; }
void Ball::setVelocityY(float newY) { velocity.y = newY; }

void Ball::checkWallCollision() {
    if (position.y <= 0 || position.y >= GetScreenHeight()) bounceY();
}
