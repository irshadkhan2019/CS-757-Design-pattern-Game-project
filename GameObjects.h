#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <FL/Fl.H>
#include <FL/Fl_Box.H>

class Ball {
public:
    int x, y, radius;
    int xSpeed, ySpeed;
    Fl_Box* box;
    static const int MAX_SPEED = 5;

    Ball(int startX, int startY, int r, int xs, int ys);
    void updatePosition();
    void resetBallSpeed();
    void reverseX();
    void reverseY();
    void limitSpeed();
};

class Paddle {
public:
    int x, y, width, height;
    Fl_Box* box;

    Paddle(int sx, int sy, int w, int h);
    void move(int newX);
    bool collidesWithBall(const Ball& ball) const;
};

#endif 