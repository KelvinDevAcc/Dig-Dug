#pragma once
#include <string>
#include <vector>


namespace game
{
	class Player;
}

class LoadMap final {
public:
    LoadMap(const std::string& mapFilePath, const std::string& ingFilePath);
    ~LoadMap() = default;
    
    LoadMap(const LoadMap&) = delete; 
    LoadMap& operator=(const LoadMap&) = delete; 
    LoadMap(LoadMap&&) noexcept = default; 
    LoadMap& operator=(LoadMap&&) noexcept = default; 

    void printMaps() const;
    const std::vector<std::vector<char>>& getMap() const;
    const std::vector<std::vector<char>>& getIngMap() const;


private:
    void loadFromFile(const std::string& mapFilePath, const std::string& ingFilePath);
    static void printMap(const std::vector<std::vector<char>>& mapToPrint);

    std::vector<std::vector<char>> m_map;
    std::vector<std::vector<char>> m_ingMap;
};
