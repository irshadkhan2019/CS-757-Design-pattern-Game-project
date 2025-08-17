#include <FL/Fl.H>
#include "Game.h"
#include "GameConfig.h"
#include "Performance.h"

int main() {
    BallBricksGame game(WINDOW_WIDTH, WINDOW_HEIGHT, "Ball and Bricks Game With Flyweight & Memento");
    game.show();

    Fl::add_timeout(2.0, monitorCallback, nullptr);
    return Fl::run();
}