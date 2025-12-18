#ifndef NODE_H
#define NODE_H

#include "../Types/NodeType.h"
#include "../Types/ResourceType.h"
#include "../Types/TypeAliases.hpp"

enum class Direction;
class Player;

class Edge;
class Tile;

enum class NodeType;

class Node {
private:
    NodeType m_type=NodeType::None;
    int m_ownerId=-1;

    IncidentTiles m_incidentTiles{};
    IncidentEdges m_incidentEdges{};

    int m_nodeID=-1;
    static int m_numOfNodes;

    //index is nodes id relative to tile
    HexCoords m_tileCoords = {-1,-1};
    int m_nodeIndex=-1;

    bool m_hasTrade=false;
    ResourceType m_tradeResource=ResourceType::None;
public:
    Node(HexCoords coords, int index,bool isPort=false, ResourceType portType=ResourceType::None) : Node(coords.first, coords.second, index, isPort, portType) {}
    Node(int q, int r, int i, bool isPort=false, ResourceType portType=ResourceType::None) : m_nodeIndex(i) {
        m_numOfNodes++; m_nodeID =m_numOfNodes;
        m_tileCoords={q,r};
        m_hasTrade=isPort; m_tradeResource=portType;
    }

    bool isEmpty() const { return m_type==NodeType::None; }

    bool hasTrade() const { return m_hasTrade; }
    ResourceType getTradeResource() const { return m_tradeResource; }

    NodeType getNodeType() const { return m_type; }
    int getOwnerId() const { return m_ownerId; }

    IncidentTiles getIncidentTiles() const { return m_incidentTiles; }
    IncidentEdges getIncidentEdges() const { return m_incidentEdges; }

    HexCoords getTileCoords() const {return m_tileCoords;}
    int getNodeIndex() const {return m_nodeIndex;}

    void setNodeType(const NodeType nodeType) { m_type = nodeType; }
    void setOwnerId(int owner) { m_ownerId = owner; m_type=NodeType::Settlement; }
    void upgradeToCity() {m_type=NodeType::City;}
    void setTrade(ResourceType tradeResource) {m_hasTrade=true;m_tradeResource=tradeResource;}

    void addAdjacentTile(Tile * tile){ static int i=0; m_incidentTiles.at(i++)=tile;}
    void addAdjacentEdge(Edge * edge){static int i=0; m_incidentEdges.at(i++)=edge;}

    friend bool operator==(const Node &lhs, const Node &rhs) {
        return lhs.m_nodeID == rhs.m_nodeID;
    }
    friend bool operator!=(const Node &lhs, const Node &rhs) {
        return !(lhs == rhs);
    }
};

#endif // NODE_H