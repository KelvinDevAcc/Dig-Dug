#include "LoadMap.h"
#include <iostream>
#include <fstream>
#include "SceneHelpers.h"

LoadMap::LoadMap(const std::string& mapFilePath, const std::string& ingFilePath) {
    loadFromFile(mapFilePath, ingFilePath);
}

void LoadMap::loadFromFile(const std::string& mapFilePath, const std::string& ingFilePath) {
    std::ifstream mapFile(mapFilePath);
    if (!mapFile.is_open()) {
        std::cerr << "Unable to open map file: " << mapFilePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(mapFile, line)) {
        std::vector<char> row(line.begin(), line.end());
        m_map.push_back(row);
    }
    mapFile.close();

    std::ifstream ingFile(ingFilePath);
    if (!ingFile.is_open()) {
        std::cerr << "Unable to open ingredient file: " << ingFilePath << std::endl;
        return;
    }

    while (std::getline(ingFile, line)) {
        std::vector<char> row(line.begin(), line.end());
        m_ingMap.push_back(row);
    }
    ingFile.close();
}

void LoadMap::printMap(const std::vector<std::vector<char>>& mapToPrint)
{
    for (const auto& row : mapToPrint) {
        for (const char tile : row) {
            std::cout << tile;
        }
        std::cout << std::endl;
    }
}

void LoadMap::printMaps() const {
    std::cout << "Map:" << std::endl;
    printMap(m_map);
    std::cout << "Ingredients Map:" << std::endl;
    printMap(m_ingMap);
}

const std::vector<std::vector<char>>& LoadMap::getMap() const {
    return m_map;
}

const std::vector<std::vector<char>>& LoadMap::getIngMap() const {
    return m_ingMap;
}






