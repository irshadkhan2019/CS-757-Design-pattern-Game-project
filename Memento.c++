#include "Memento.h"
#include <memory>

// ScoreState implementation
ScoreState::ScoreState(int s, int l) : score(s), lives(l) {}

int ScoreState::getScore() const { 
    return score; 
}

int ScoreState::getLives() const { 
    return lives; 
}

// GameHistory implementation
void GameHistory::push(int score, int lives) { 
    scores.push_back(std::make_unique<ScoreState>(score, lives)); 
}

ScoreState GameHistory::getLastScore() const {
    if (!scores.empty()) 
        return *scores.back();
    return ScoreState(0, 2);
}

std::string GameHistory::getPastScores() const {
    std::string result;
    for (const auto& score : scores) {
        result += std::to_string(score->getScore()) + " ";
    }
    return result;
}