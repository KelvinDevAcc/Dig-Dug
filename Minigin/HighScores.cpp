#include "HighScores.h"
#include <algorithm>
#include <ranges>

HighScores::HighScores() : fileName("highscores.txt") {
    loadScores();
}

void HighScores::loadScores() {
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        createDefaultScores();
        return;
    }

    highScores.clear(); 

    for (size_t i = 0; i < NUM_HIGH_SCORES; ++i) {
        std::array<char, PLAYER_NAME_SIZE> playerName;
        file.read(playerName.data(), PLAYER_NAME_SIZE);

        uint32_t score;
        file.read(reinterpret_cast<char*>(&score), sizeof(score));

        highScores.emplace_back(playerName, score);
    }

    std::ranges::sort(highScores, [](const auto& left, const auto& right) {
        return left.second > right.second;
        });

    // Trim the vector if it exceeds the maximum number of high scores
    if (highScores.size() > NUM_HIGH_SCORES) {
        highScores.resize(NUM_HIGH_SCORES);
    }
}

void HighScores::createDefaultScores() const
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Couldn't create highscores file");
    }

    for (const auto& score : DEFAULT_SCORES) {
        std::array<char, PLAYER_NAME_SIZE> playerName;
        std::copy_n(score.first, PLAYER_NAME_SIZE, playerName.begin());

        file.write(playerName.data(), PLAYER_NAME_SIZE);
        file.write(reinterpret_cast<const char*>(&score.second), sizeof(score.second));
    }
}

bool HighScores::isHighScore(uint32_t score) const {
    return score >= highScores.back().second;
}

int HighScores::highScorePosition(uint32_t score) const {
    int position = 1;
    for (const auto& highScore : highScores | std::views::values) {
        if (score > highScore)
            ++position;
        else
            break;
    }
    return position;
}

uint32_t HighScores::getTopScore() const {
    return highScores.front().second;
}

void HighScores::saveNewScore(const std::string& playerName, uint32_t newScore) {
    // Add the new score to the high scores
    highScores.emplace_back(std::array<char, PLAYER_NAME_SIZE>(), newScore);
    std::copy_n(playerName.begin(), std::min(playerName.size(), PLAYER_NAME_SIZE), highScores.back().first.begin());

    // Sort the high scores in descending order
    std::ranges::sort(highScores, [](const auto& left, const auto& right) {
        return left.second > right.second;
        });

    // Remove the lowest score if the number of high scores exceeds the limit
    if (highScores.size() > NUM_HIGH_SCORES) {
        highScores.pop_back();
    }

    // Write the high scores to the file
    std::ofstream file(fileName, std::ios::binary);
    for (const auto& score : highScores) {
        file.write(score.first.data(), PLAYER_NAME_SIZE);
        file.write(reinterpret_cast<const char*>(&score.second), sizeof(score.second));
    }
}