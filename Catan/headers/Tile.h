#ifndef TILE_H
#define TILE_H

#include <string>
#include <tuple>
#include "ResourceType.h"


class Tile{
private:
    ResourceType m_type;
    int m_number;
    std::tuple<int,int,int> m_tileCoord;
public:
    Tile() : m_type(ResourceType::Sea) {}
    Tile(ResourceType type, int number, std::tuple<int,int,int> tileCoord) : m_type(type), m_number(number), m_tileCoord(tileCoord) {}
    ~Tile() = default;

    ResourceType getType() { return m_type; }
    int getNumber() { return m_number; }
    std::tuple<int,int,int> getTileCoord() { return m_tileCoord; }

    static std::string typeToString(ResourceType type);
    friend std::ostream& operator<<(std::ostream& os, const Tile& tile);
    std::string toString() const;
};

#endif // TILE_H
