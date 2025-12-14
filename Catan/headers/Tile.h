#ifndef TILE_H
#define TILE_H

#include <vector>
#include "ResourceType.h"

class Tile{
private:
    ResourceType _type;
    int _number;
    std::vector<Tile*> _adjacentTiles;
    std::pair<int,int> _tileCoord;
public:
    Tile();
    Tile(ResourceType type, int number) : _type(type), _number(number) {}
    ~Tile() = default;

    ResourceType getType() { return _type; }
    int getNumber() { return _number; }
    std::vector<Tile*> getAdjacentTiles() { return _adjacentTiles; }

    void setAdjacentTiles(std::vector<Tile*> adjacentTiles) { _adjacentTiles = adjacentTiles; }};

#endif // TILE_H
