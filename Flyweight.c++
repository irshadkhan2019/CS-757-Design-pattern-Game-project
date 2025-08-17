#include "Flyweight.h"
#include "GameObjects.h"
#include <string>

// BrickFlyweight
BrickFlyweight::BrickFlyweight(int w, int h, Fl_Color c)
    : width(w), height(h), color(c) {}

void BrickFlyweight::render(int x, int y) const {
    fl_color(color);
    fl_rectf(x, y, width, height);
    fl_color(FL_BLACK);
    fl_rect(x, y, width, height);
}

// BrickFactory 
std::string BrickFactory::getKey(int w, int h, Fl_Color c) const {
    return std::to_string(w) + "_" + std::to_string(h) + "_" + std::to_string((int)c);
}

std::shared_ptr<BrickFlyweight> BrickFactory::getFlyweight(int w, int h, Fl_Color c) {
    std::string key = getKey(w, h, c);
    auto it = flyweights.find(key);
    if (it == flyweights.end()) {
        flyweights[key] = std::make_shared<BrickFlyweight>(w, h, c);
    }
    return flyweights[key];
}

size_t BrickFactory::getFlyweightCount() const { 
    return flyweights.size(); 
}

// Brick 
Brick::Brick(int xpos, int ypos, std::shared_ptr<BrickFlyweight> fw)
    : x(xpos), y(ypos), active(true), type(fw) {}

void Brick::draw() const { 
    if (active && type) {
        type->render(x, y); 
    }
}

void Brick::reset(int nx, int ny, std::shared_ptr<BrickFlyweight> newType) { 
    x = nx; 
    y = ny; 
    active = true;
    type = newType;
}

bool Brick::collidesWithBall(const Ball& ball) const {
    return active && type &&
           (ball.x + ball.radius > x && ball.x - ball.radius < x + type->width) &&
           (ball.y + ball.radius > y && ball.y - ball.radius < y + type->height);
}