#ifndef TILE_H
#define TILE_H

#include <vector>
#include <string>
#include <tuple>
#include "ResourceType.h"

class Tile{
private:
    ResourceType m_type;
    int m_number;
    std::vector<Tile*> m_adjacentTiles;
    std::tuple<int,int,int> m_tileCoord;
public:
    Tile();
    Tile(ResourceType type, int number) : m_type(type), m_number(number) {}
    Tile(ResourceType type, int number, std::tuple<int,int,int> tileCoord) : m_type(type), m_number(number), m_tileCoord(tileCoord) {}
    ~Tile() = default;

    ResourceType getType() { return m_type; }
    int getNumber() { return m_number; }
    std::vector<Tile*> getAdjacentTiles() { return m_adjacentTiles; }

    void setAdjacentTiles(std::vector<Tile*> adjacentTiles) { m_adjacentTiles = adjacentTiles; }

    static std::string typeToString(ResourceType type);
    std::string toString();
};

#endif // TILE_H
