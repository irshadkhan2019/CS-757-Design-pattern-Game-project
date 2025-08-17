#include "Game.h"
#include "GameConfig.h"
#include <FL/Fl.H>
#include <ctime>
#include <cstdlib>
#include <algorithm>

BallBricksGame::BallBricksGame(int width, int height, const char* title)
    : Fl_Window(width, height, title),
      ball(std::make_unique<Ball>(width/2, height/2, BALL_RADIUS, 2, -2)),
      paddle(std::make_unique<Paddle>(width/2 - PADDLE_WIDTH/2, height - PADDLE_HEIGHT - 10,
                               PADDLE_WIDTH, PADDLE_HEIGHT)),
      gameHistory(std::make_unique<GameHistory>()) {

    srand(static_cast<unsigned>(time(nullptr)));

    // UI Setup
    quitButton = new Fl_Button(width/2 - 50, height/2 + 30, 100, 40, "Quit");
    lifeLineButton = new Fl_Button(width/2 - 60, height/2 + 90, 120, 40, "Use Bonus Life");
    quitButton->hide(); 
    lifeLineButton->hide();
    quitButton->callback(quitCallback, this);
    lifeLineButton->callback(lifeLineCallback, this);

    scoreBox = new Fl_Box(10, 30, 100, 20, "Score: 0");
    scoreBox->align(FL_ALIGN_TOP_LEFT);
    playerLifeBox = new Fl_Box(120, 30, 100, 20, "Lives: 2");
    playerLifeBox->align(FL_ALIGN_TOP_LEFT);
    historyBox = new Fl_Box(250, 30, 400, 20, "Past Scores: None");
    historyBox->align(FL_ALIGN_TOP_LEFT);

    initializeBricks();
    startGame();
}

void BallBricksGame::initializeBricks() {
    auto greenBrick = factory.getFlyweight(BRICK_WIDTH, BRICK_HEIGHT, FL_GREEN);
    bricks.clear();
    
    int minY = WINDOW_HEIGHT * 0.1; // Skip top 10% for UI elements
    int maxY = WINDOW_HEIGHT * 0.6; // Keep bricks in upper-middle area
    
    for (int i = 0; i < NUM_BRICKS_X; i++) {
        for (int j = 0; j < NUM_BRICKS_Y; j++) {
            int bx = rand() % (WINDOW_WIDTH - BRICK_WIDTH);
            int by = minY + rand() % (maxY - minY);
            bricks.emplace_back(bx, by, greenBrick);
        }
    }
}

void BallBricksGame::startGame() {
    gameStarted = true;
    isPaused = false;
    Fl::add_timeout(gameTickInterval, gameTickCallback, this);
    Fl::add_timeout(brickAddInterval, addRandomBrickCallback, this);
}

void BallBricksGame::draw() {
    Fl_Window::draw();
    
    if (ball) ball->box->redraw();
    if (paddle) paddle->box->redraw();
    
    for (const auto& brick : bricks) {
        brick.draw();
    }
}

void BallBricksGame::gameUpdate() {
    if (isPaused || !gameStarted) return;

    ball->updatePosition();
    handleCollisions();
    
    int mouseX = Fl::event_x();
    paddle->move(mouseX - PADDLE_WIDTH/2);
    
    bool allBricksDestroyed = true;
    for (const auto& brick : bricks) {
        if (brick.active) {
            allBricksDestroyed = false;
            break;
        }
    }
    
    if (allBricksDestroyed) {
        score += 100;
        updateScoreDisplay();
        initializeBricks();
    }
}

void BallBricksGame::handleCollisions() {
    if (ball->x - ball->radius <= 0 || ball->x + ball->radius >= WINDOW_WIDTH) {
        ball->reverseX();
    }
    if (ball->y - ball->radius <= 0) {
        ball->reverseY();
    }

    if (paddle->collidesWithBall(*ball)) {
        ball->reverseY();
        int paddleCenter = paddle->x + paddle->width/2;
        int hitOffset = ball->x - paddleCenter;
        ball->xSpeed += hitOffset / 20;
    }

    for (auto& brick : bricks) {
        if (brick.collidesWithBall(*ball)) {
            brick.active = false;
            ball->ySpeed = -(abs(ball->ySpeed) + 1);
            score++;
            updateScoreDisplay();
            break;
        }
    }

    if (ball->y - ball->radius >= WINDOW_HEIGHT) {
        storeGameState();
        
        isPaused = true; 
        quitButton->show();
        if (playerLife > 0) {
            lifeLineButton->show();
        }
        
        updateScoreDisplay();
        Fl::remove_timeout(gameTickCallback, this);
        Fl::remove_timeout(addRandomBrickCallback, this);
    }
}

void BallBricksGame::resetBallAndPaddle() {
    ball->x = WINDOW_WIDTH/2;
    ball->y = WINDOW_HEIGHT/2;
    ball->resetBallSpeed();
    paddle->move(WINDOW_WIDTH/2 - PADDLE_WIDTH/2);
}

void BallBricksGame::gameOver() {
    isPaused = true;
    gameStarted = false;
    quitButton->show();
    if (playerLife > 0) {
        lifeLineButton->show();
    }
    
    Fl::remove_timeout(gameTickCallback, this);
    Fl::remove_timeout(addRandomBrickCallback, this);
}

void BallBricksGame::updateScoreDisplay() {
    scoreBox->copy_label(("Score: " + std::to_string(score)).c_str());
    playerLifeBox->copy_label(("Lives: " + std::to_string(playerLife)).c_str());
    
    std::string pastScores = gameHistory->getPastScores();
    std::string historyText = "Past Scores: " + (pastScores.empty() ? "None" : pastScores);
    historyBox->copy_label(historyText.c_str());
    
    scoreBox->redraw();
    playerLifeBox->redraw();
    historyBox->redraw();
}

void BallBricksGame::storeGameState() { 
    gameHistory->push(score, playerLife); 
}

void BallBricksGame::restartGameWithLifeLine() {
    if (playerLife <= 0) return;
    
    ScoreState lastState = gameHistory->getLastScore();
    int bestScore = std::max(lastState.getScore(), score);
    
    score = bestScore;
    playerLife--;
    
    resetBallAndPaddle();
    quitButton->hide(); 
    lifeLineButton->hide();
    isPaused = false;
    updateScoreDisplay();
    
    Fl::add_timeout(gameTickInterval, gameTickCallback, this);
    Fl::add_timeout(brickAddInterval, addRandomBrickCallback, this);
}

void BallBricksGame::resetGame() {
    storeGameState();
    score = 0;
    playerLife = 2;
    resetBallAndPaddle();
    initializeBricks();
    quitButton->hide(); 
    lifeLineButton->hide();
    updateScoreDisplay();
    startGame();
}

void BallBricksGame::addRandomBrick() {
    auto yellowBrick = factory.getFlyweight(BRICK_WIDTH, BRICK_HEIGHT, FL_YELLOW);
    
    for (auto& brick : bricks) {
        if (!brick.active) {
            int bx = rand() % (WINDOW_WIDTH - BRICK_WIDTH);
            int by = WINDOW_HEIGHT * 0.1 + rand() % (int)(WINDOW_HEIGHT * 0.5);
            brick.reset(bx, by, yellowBrick);
            return;
        }
    }
}

// Static callbacks
void BallBricksGame::gameTickCallback(void* userData) {
    BallBricksGame* game = static_cast<BallBricksGame*>(userData);
    game->gameUpdate();
    game->redraw();
    Fl::repeat_timeout(game->gameTickInterval, gameTickCallback, userData);
}

void BallBricksGame::addRandomBrickCallback(void* userData) {
    BallBricksGame* game = static_cast<BallBricksGame*>(userData);
    game->addRandomBrick();
    Fl::repeat_timeout(game->brickAddInterval, addRandomBrickCallback, userData);
}

void BallBricksGame::quitCallback(Fl_Widget*, void* userData) {
    BallBricksGame* game = static_cast<BallBricksGame*>(userData);
    game->hide();
}

void BallBricksGame::lifeLineCallback(Fl_Widget*, void* userData) {
    BallBricksGame* game = static_cast<BallBricksGame*>(userData);
    game->restartGameWithLifeLine();
}