#pragma once
#include <string>
#include <vector>

class LoadMap final {
public:
    LoadMap(const std::string& mapFilePath, const std::string& tunFilePath, const std::string& entityFilePath);
    ~LoadMap() = default;

    LoadMap(const LoadMap&) = delete;
    LoadMap& operator=(const LoadMap&) = delete;
    LoadMap(LoadMap&&) noexcept = default;
    LoadMap& operator=(LoadMap&&) noexcept = default;

    void printMaps() const;
    const std::vector<std::vector<char>>& getMap() const;
    const std::vector<std::vector<char>>& getTunMap() const;
    const std::vector<std::vector<char>>& getEntityMap() const;

private:
    void loadFromFile(const std::string& mapFilePath, const std::string& tunFilePath, const std::string& entityFilePath);
    static void printMap(const std::vector<std::vector<char>>& mapToPrint);

    std::vector<std::vector<char>> m_map;
    std::vector<std::vector<char>> m_tunnelMap;
    std::vector<std::vector<char>> m_entityMap;
};
