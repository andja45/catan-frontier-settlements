#ifndef TILE_H
#define TILE_H

#include <array>
#include <vector>
#include "../Types/ResourceType.h"
#include "../Types/TypeAliases.hpp"
#include <string>
#include <tuple>
#include "../Types/ResourceType.h"

enum class ResourceType;
class Tile;
class Node;

class Tile {
private:
    ResourceType _type=ResourceType::None;
    int _number=-1;
    int _robber_on_tile=false;

    NeighbourTiles _adjacentTiles{};
    NeighbourNodes _adjacentNodes{};
    NeighbourEdges _adjacentEdges{};

    int _tileId=-1;
    HexCoords _tileCoord{-1,-1};
    static int _numOfTiles;

public:
    Tile() {_numOfTiles++; _tileId=_numOfTiles;}
    Tile(int q, int r, ResourceType type, int number) : _type(type), _number(number), _adjacentTiles(), _adjacentNodes() {
        _numOfTiles++;
        _tileId=_numOfTiles;
        _tileCoord={q,r};
    }

    ResourceType getType() const { return _type; }
    int getNumber() const { return _number; }
    bool isRobberOnTile() const { return _robber_on_tile; }
    NeighbourTiles getAdjacentTiles() const { return _adjacentTiles; }
    NeighbourNodes getAdjacentNodes() const { return _adjacentNodes; }
    HexCoords getTileCoord() const { return _tileCoord; }
    int getTileId() const { return _tileId; }
    Node* getNodeAt(int i) const {return _adjacentNodes[i];}
    Edge* getEdgeAt(int i) const {return _adjacentEdges[i];}

    void addAdjacentTile(Tile* adjacentTile, int index) { _adjacentTiles[index] = adjacentTile; }
    void addAdjacentNode(Node* adjacentNode, int index) { _adjacentNodes[index] = adjacentNode; }
    void addAdjacentEdge(Edge* adjacentEdge, int index) { _adjacentEdges[index] = adjacentEdge; }

    static std::string typeToString(ResourceType type);
    friend std::ostream& operator<<(std::ostream& os, const Tile& tile);
    std::string toString() const;

};
#endif // TILE_H
