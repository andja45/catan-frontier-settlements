#ifndef NODE_H
#define NODE_H

#include <types/NodeType.h>
#include <types/ResourceType.h>
#include <types/TypeAliases.h>

enum class Direction;

class Edge;
class Tile;

enum class NodeType;

class Node {
private:
    NodeType m_type=NodeType::None;
    PlayerId m_ownerId=-1;

    IncidentTiles m_incidentTiles{};
    IncidentEdges m_incidentEdges{};

    NodeId m_nodeId=-1;
    inline static int m_numOfNodes=0;

    //index is nodes id relative to tile
    HexCoords m_tileCoords = {-1,-1};
    NodeIndex m_nodeIndex=-1;

    bool m_hasTrade=false;
    ResourceType m_tradeResource=ResourceType::None;
public:
    Node(HexCoords coords, NodeIndex index,bool isPort=false, ResourceType portType=ResourceType::None) : Node(coords.first, coords.second, index, isPort, portType) {}
    Node(int q, int r, int i, bool isPort=false, ResourceType portType=ResourceType::None) : m_nodeIndex(i) {
        m_numOfNodes++; m_nodeId =m_numOfNodes;
        m_tileCoords={q,r};
        m_hasTrade=isPort; m_tradeResource=portType;
    }

    bool isEmpty() const { return m_type==NodeType::None; }

    bool hasTrade() const { return m_hasTrade; }
	bool is3for1Trade() const; //TODO
    ResourceType getTradeResource() const { return m_tradeResource; }

    NodeType getNodeType() const { return m_type; }
    PlayerId getOwner() const { return m_ownerId; }
    IncidentTiles getIncidentTiles() const { return m_incidentTiles; }
    IncidentEdges getIncidentEdges() const { return m_incidentEdges; }
    std::array<Node*,3> getIncidentNodes();

    HexCoords getTileCoords() const {return m_tileCoords;}
    NodeIndex getNodeIndex() const {return m_nodeIndex;}

    void setNodeType(const NodeType nodeType) { m_type = nodeType; }
    void setOwner(int ownerId) { m_ownerId = ownerId; m_type=NodeType::Settlement; }
    void upgradeToCity() {m_type=NodeType::City;}
    void setTrade(ResourceType tradeResource) {m_hasTrade=true;m_tradeResource=tradeResource;}

    void addAdjacentTile(Tile * tile){ static int i=0; m_incidentTiles.at(i++)=tile;}
    void addAdjacentEdge(Edge * edge){static int i=0; m_incidentEdges.at(i++)=edge;}

    friend bool operator==(const Node &lhs, const Node &rhs) {
        return lhs.m_nodeId == rhs.m_nodeId;
    }
    friend bool operator!=(const Node &lhs, const Node &rhs) {
        return !(lhs == rhs);
    }
	bool isSettlement() const { return getNodeType() == NodeType::Settlement; }
    bool isCity() const { return getNodeType() == NodeType::City; }
};

#endif // NODE_H