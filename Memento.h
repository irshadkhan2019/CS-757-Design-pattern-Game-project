#ifndef MEMENTO_H
#define MEMENTO_H

#include <vector>
#include <memory>
#include <string>

class ScoreState {
private:
    int score;
    int lives;
public:
    ScoreState(int s, int l);
    int getScore() const;
    int getLives() const;
};

class GameHistory {
private:
    std::vector<std::unique_ptr<ScoreState>> scores;
public:
    void push(int score, int lives);
    ScoreState getLastScore() const;
    std::string getPastScores() const;
};

#endif 