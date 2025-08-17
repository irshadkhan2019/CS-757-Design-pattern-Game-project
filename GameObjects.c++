#include "GameObjects.h"
#include "GameConfig.h"
#include <algorithm>

// Ball related code
Ball::Ball(int startX, int startY, int r, int xs, int ys)
    : x(startX), y(startY), radius(r), xSpeed(xs), ySpeed(ys),
      box(new Fl_Box(x - r, y - r, 2*r, 2*r)) {
    box->box(FL_ROUND_UP_BOX);
    box->color(FL_RED);
}

void Ball::updatePosition() {
    x += xSpeed; 
    y += ySpeed;
    box->position(x - radius, y - radius);
}

void Ball::resetBallSpeed() { 
    xSpeed = 2; 
    ySpeed = -2;
}

void Ball::reverseX() { 
    xSpeed = -xSpeed; 
}

void Ball::reverseY() { 
    ySpeed = -ySpeed; 
}

void Ball::limitSpeed() {
}

// Paddle code
Paddle::Paddle(int sx, int sy, int w, int h)
    : x(sx), y(sy), width(w), height(h),
      box(new Fl_Box(x, y, w, h)) {
    box->box(FL_UP_BOX);
    box->color(FL_BLUE);
}

void Paddle::move(int newX) { 
    x = std::max(0, std::min(newX, WINDOW_WIDTH - width));
    box->position(x, y); 
}

bool Paddle::collidesWithBall(const Ball& ball) const {
    return (ball.x + ball.radius > x && ball.x - ball.radius < x + width) &&
           (ball.y + ball.radius > y && ball.y - ball.radius < y + height);
}