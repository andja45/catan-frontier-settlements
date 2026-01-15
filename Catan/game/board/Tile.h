#ifndef TILE_H
#define TILE_H

#include <string>

#include <types/ResourceType.h>
#include <types/TypeAliases.h>
#include <board/Coords/AxialCoords.hpp>

enum class SideDirection;
enum class PointDirection;
enum class ResourceType;
class Tile;
class Node;

class Tile {
private:
    ResourceType m_type=ResourceType::None;
    int m_number=-1;
    bool m_robberOnTile=false;

    NeighbourNodes m_adjacentNodes{};

    TileId m_tileId=-1;
    TileCoords m_tileCoord;
    inline static int m_numOfTiles=0;

public:
    Tile(AxialCoords coords,ResourceType type, int number): Tile(coords.q(),coords.r(),type,number) {}
    Tile(int q, int r, ResourceType type, int number) : m_type(type), m_number(number), m_adjacentNodes() {
        m_numOfTiles++;
        m_tileId=m_numOfTiles;
        m_tileCoord={q,r};
    }

    void setId(TileId id) {m_tileId=id;}
    ResourceType getResourceType() const { return m_type; }
    int getNumber() const { return m_number; }

    bool isRobberOnTile() const { return m_robberOnTile; }
    void setRobber(bool isRobber){m_robberOnTile=isRobber;}

    TileCoords getTileCoord() const { return m_tileCoord; }
    TileId getTileId() const { return m_tileId; }

    NeighbourNodes getAdjacentNodes() const { return m_adjacentNodes; }
    void addAdjacentNode(Node* adjacentNode) { m_adjacentNodes.push_back(adjacentNode); }

    static std::string typeToString(ResourceType type);
    friend std::ostream& operator<<(std::ostream& os, const Tile& tile);
    std::string toString() const;

};
#endif // TILE_H
