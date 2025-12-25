#ifndef NODE_H
#define NODE_H

#include "../Types/NodeType.h"
#include "../Types/ResourceType.h"
#include "../Types/TypeAliases.hpp"

class Player;

class Edge;
class Tile;

enum class NodeType;

class Node {
private:
    NodeType m_type=NodeType::None;
    Player* m_owner=nullptr;

    IncidentTiles m_incidentTiles{};
    IncidentEdges m_incidentEdges{};

    int m_nodeID=-1;
    HexCoords m_tileCoords = {-1,-1};
    int m_nodeIndex=-1;
    static int m_numOfNodes;

    bool m_hasTrade=false;
    ResourceType m_tradeResource=ResourceType::None;
public:
    Node(int q, int r, int i, bool isPort=false, ResourceType portType=ResourceType::None) : m_nodeIndex(i) {
        m_numOfNodes++; m_nodeID=m_numOfNodes;
        m_tileCoords={q,r};
        m_hasTrade=isPort; m_tradeResource=portType;
    }

    bool isEmpty() const { return m_type==NodeType::None; }
    bool hasTrade() const { return m_hasTrade; }
	bool is3for1Trade() const; //TODO
    ResourceType getTradeResource() const { return m_tradeResource; }

    NodeType getNodeType() const { return m_type; }
    Player* getOwner() const { return m_owner; }
    IncidentTiles getIncidentTiles() const { return m_incidentTiles; }
    IncidentEdges getIncidentEdges() const { return m_incidentEdges; }

    HexCoords getTileCoords() const {return m_tileCoords;}
    int getNodeIndex() const {return m_nodeIndex;}

    void setNodeType(const NodeType nodeType) { m_type = nodeType; }
    void setOwner(Player* owner) { m_owner = owner; m_type=NodeType::Settlement; }
    void upgradeToCity() {m_type=NodeType::City;}
    void setTrade(ResourceType tradeResource) {m_hasTrade=true;m_tradeResource=tradeResource;}

    void setIncidentTiles(const IncidentTiles &incidentTiles) { m_incidentTiles = incidentTiles; }
    void setIncidentEdges(const IncidentEdges &incidentEdges) { m_incidentEdges = incidentEdges; }

    void addAdjacentTile(Tile * tile, int corner);
    void addAdjacentEdge(Edge * edge, int corner);
};

#endif // NODE_H