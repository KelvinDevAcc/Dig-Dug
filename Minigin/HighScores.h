#pragma once
#include <fstream>
#include <vector>
#include <array>
#include "Singleton.h"

class HighScores final : public dae::Singleton<HighScores>
{
public:

    static constexpr size_t NUM_HIGH_SCORES = 5;
    static constexpr size_t PLAYER_NAME_SIZE = 20;

    HighScores();
    ~HighScores() override = default;

    HighScores(const HighScores&) = delete;
    HighScores& operator=(const HighScores&) = delete;
    HighScores(HighScores&&) noexcept = delete;
    HighScores& operator=(HighScores&&) noexcept = delete;

    void loadScores();
    void createDefaultScores() const;
    bool isHighScore(uint32_t score) const;
    int highScorePosition(uint32_t score) const;
    uint32_t getTopScore() const;
    void saveNewScore(const std::string& playerName, uint32_t newScore);

    const std::vector<std::pair<std::array<char, PLAYER_NAME_SIZE>, uint32_t>>& getHighScores() const {
        return highScores;
    }

private:
    std::string fileName;
    std::vector<std::pair<std::array<char, PLAYER_NAME_SIZE>, uint32_t>> highScores;
    static constexpr std::array<std::pair<const char*, uint32_t>, NUM_HIGH_SCORES> DEFAULT_SCORES = {
        std::make_pair("Player1", 0),
        std::make_pair("Player2", 0),
        std::make_pair("Player3", 200),
        std::make_pair("Player4", 0),
        std::make_pair("Player5", 100)
    };
};
