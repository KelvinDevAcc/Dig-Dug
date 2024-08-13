#include "LoadMap.h"
#include <iostream>
#include <fstream>
#include "SceneHelpers.h"
LoadMap::LoadMap(const std::string& mapFilePath, const std::string& tunFilePath, const std::string& entityFilePath) {
    loadFromFile(mapFilePath, tunFilePath,entityFilePath);
}

void LoadMap::loadFromFile(const std::string& mapFilePath, const std::string& tunFilePath, const std::string& entityFilePath) {
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

    std::ifstream tunFile(tunFilePath);
    if (!tunFile.is_open()) {
        std::cerr << "Unable to open Tunnel file: " << tunFilePath << std::endl;
        return;
    }

    while (std::getline(tunFile, line)) {
        std::vector<char> row(line.begin(), line.end());
        m_tunnelMap.push_back(row);
    }
    tunFile.close();

    std::ifstream entityFile(entityFilePath);
    if (!entityFile.is_open()) {
        std::cerr << "Unable to open Tunnel file: " << tunFilePath << std::endl;
        return;
    }

    while (std::getline(entityFile, line)) {
        std::vector<char> row(line.begin(), line.end());
        m_entityMap.push_back(row);
    }
    entityFile.close();

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
    std::cout << "tunnel Map:" << std::endl;
    printMap(m_tunnelMap);
    std::cout << "entity Map:" << std::endl;
    printMap(m_entityMap);
}

const std::vector<std::vector<char>>& LoadMap::getMap() const {
    return m_map;
}

const std::vector<std::vector<char>>& LoadMap::getTunMap() const {
    return m_tunnelMap;
}

const std::vector<std::vector<char>>& LoadMap::getEntityMap() const {
    return m_entityMap;
}

