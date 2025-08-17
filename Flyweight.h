#ifndef FLYWEIGHT_H
#define FLYWEIGHT_H

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <map>
#include <memory>
#include <string>

class Ball;

class BrickFlyweight {
public:
    int width, height;
    Fl_Color color;
    
    BrickFlyweight(int w, int h, Fl_Color c);
    void render(int x, int y) const;
};

class BrickFactory {
private:
    std::map<std::string, std::shared_ptr<BrickFlyweight>> flyweights;
    std::string getKey(int w, int h, Fl_Color c) const;
    
public:
    std::shared_ptr<BrickFlyweight> getFlyweight(int w, int h, Fl_Color c);
    size_t getFlyweightCount() const;
};

class Brick {
public:
    int x, y;
    bool active;
    std::shared_ptr<BrickFlyweight> type;

    Brick(int xpos, int ypos, std::shared_ptr<BrickFlyweight> fw);
    void draw() const;
    void reset(int nx, int ny, std::shared_ptr<BrickFlyweight> newType);
    bool collidesWithBall(const Ball& ball) const;
};

#endif 