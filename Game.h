#ifndef GAME_H
#define GAME_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <vector>
#include <memory>
#include "GameObjects.h"
#include "Flyweight.h"
#include "Memento.h"

class BallBricksGame : public Fl_Window {
private:
    std::unique_ptr<Ball> ball;
    std::unique_ptr<Paddle> paddle;
    std::vector<Brick> bricks;
    BrickFactory factory;
    std::unique_ptr<GameHistory> gameHistory;

    int score = 0;
    int playerLife = 2;
    bool isPaused = false;
    bool gameStarted = false;

    Fl_Box *scoreBox, *playerLifeBox, *historyBox;
    Fl_Button *lifeLineButton, *quitButton;
    
    // For new bricks addition and updating screen 60 times per second
    const double gameTickInterval = 0.016;
    const double brickAddInterval = 3.0;  

public:
    BallBricksGame(int width, int height, const char* title);
    
    void initializeBricks();
    void startGame();
    void draw() override;
    void gameUpdate();
    void handleCollisions();
    void resetBallAndPaddle();
    void gameOver();
    void updateScoreDisplay();
    void storeGameState();
    void restartGameWithLifeLine();
    void resetGame();
    void addRandomBrick();

    // static section
    static void gameTickCallback(void* userData);
    static void addRandomBrickCallback(void* userData);
    static void quitCallback(Fl_Widget*, void* userData);
    static void lifeLineCallback(Fl_Widget*, void* userData);
};

#endif 