#ifndef TILE_H
#define TILE_H

#include <array>
#include <vector>
#include "../Types/ResourceType.h"
#include "../Types/TypeAliases.hpp"
#include <string>
#include <tuple>
#include "../Types/ResourceType.h"

enum class SideDirection;
enum class PointDirection;
enum class ResourceType;
class Tile;
class Node;

class Tile {
private:
    ResourceType m_type=ResourceType::None;
    int m_number=-1;
    int m_robber_on_tile=false;

    NeighbourNodes m_adjacentNodes{};
    NeighbourEdges m_adjacentEdges{};

    int m_tileId=-1;
    HexCoords m_tileCoord{-1,-1};
    inline static int m_numOfTiles=0;


public:
    Tile(int q, int r, ResourceType type, int number) : m_type(type), m_number(number), m_adjacentNodes() {
        m_numOfTiles++;
        m_tileId=m_numOfTiles;
        m_tileCoord={q,r};
    }

    ResourceType getType() const { return m_type; }
    int getNumber() const { return m_number; }
    bool isRobberOnTile() const { return m_robber_on_tile; }

    HexCoords getTileCoord() const { return m_tileCoord; }
    int getTileId() const { return m_tileId; }

    NeighbourNodes getAdjacentNodes() const { return m_adjacentNodes; }

    Node* getNodeAt(int i) const {return m_adjacentNodes[i];}
    Edge* getEdgeAt(int i) const {return m_adjacentEdges[i];}

    Node* getNodeAtDir(PointDirection dir) const { return getNodeAt(static_cast<int>(dir));}
    Edge* getEdgeAtDir(SideDirection dir) const {return getEdgeAt(static_cast<int>(dir));}

    void setAdjacentNode(Node* adjacentNode, int index) { m_adjacentNodes[index] = adjacentNode; }
    void setAdjacentEdge(Edge* adjacentEdge, int index) { m_adjacentEdges[index] = adjacentEdge; }

    void setAdjacentNodeDir(Node* adjacentNode, PointDirection dir){ setAdjacentNode(adjacentNode, static_cast<int>(dir)); }
    void setAdjacentEdgeDir(Edge* adjacentEdge, SideDirection dir){ setAdjacentEdge(adjacentEdge, static_cast<int>(dir)); }

    static std::string typeToString(ResourceType type);
    friend std::ostream& operator<<(std::ostream& os, const Tile& tile);
    std::string toString() const;

};
#endif // TILE_H
