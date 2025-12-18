#ifndef TILE_H
#define TILE_H

#include <array>
#include <vector>
#include "../Types/ResourceType.h"
#include "../Types/TypeAliases.h"
#include <string>
#include <tuple>
#include "../Types/ResourceType.h"

enum class ResourceType;
class Tile;
class Node;

class Tile {
private:
    ResourceType m_type=ResourceType::None;
    int m_number=-1;
    int m_robber_on_tile=false;

    NeighbourTiles m_adjacentTiles{};
    NeighbourNodes m_adjacentNodes{};
    NeighbourEdges m_adjacentEdges{};

    int m_tileId=-1;
    HexCoords m_tileCoord{-1,-1};
    static int m_numOfTiles;

public:
    Tile() {m_numOfTiles++; m_tileId=m_numOfTiles;}
    Tile(int q, int r, ResourceType type, int number) : m_type(type), m_number(number), m_adjacentTiles(), m_adjacentNodes() {
        m_numOfTiles++;
        m_tileId=m_numOfTiles;
        m_tileCoord={q,r};
    }

    ResourceType getType() const { return m_type; }
    int getNumber() const { return m_number; }
    bool isRobberOnTile() const { return m_robber_on_tile; }
    NeighbourTiles getAdjacentTiles() const { return m_adjacentTiles; }
    NeighbourNodes getAdjacentNodes() const { return m_adjacentNodes; }
    HexCoords getTileCoord() const { return m_tileCoord; }
    int getTileId() const { return m_tileId; }
    Node* getNodeAt(int i) const {return m_adjacentNodes[i];}
    Edge* getEdgeAt(int i) const {return m_adjacentEdges[i];}

    void addAdjacentTile(Tile* adjacentTile, int index) { m_adjacentTiles[index] = adjacentTile; }
    void addAdjacentNode(Node* adjacentNode, int index) { m_adjacentNodes[index] = adjacentNode; }
    void addAdjacentEdge(Edge* adjacentEdge, int index) { m_adjacentEdges[index] = adjacentEdge; }

    static std::string typeToString(ResourceType type);
    friend std::ostream& operator<<(std::ostream& os, const Tile& tile);
    std::string toString() const;

};
#endif // TILE_H
