#ifndef NODE_H
#define NODE_H

#include <types/NodeType.h>
#include <types/ResourceType.h>
#include <types/TypeAliases.h>
#include <board/coords/NodeCoords.hpp>

class Edge;
class Tile;

enum class NodeType;

class Node {
private:
    //TODO consider separate class, stored in board? player?
    NodeType m_type=NodeType::None; // is it empty, settlement or city
    PlayerId m_ownerId=-1;  // if it has building who owns it

    // for easier neighbour search
    IncidentTiles m_incidentTiles{};
    IncidentEdges m_incidentEdges{};

    NodeId m_nodeId=-1;

    NodeCoords m_coords;

    bool m_hasTrade=false; //TODO consider separate class, stored in board? player?
    TradeType m_tradeResource=TradeType::None; // if has trade than this resolve which resource its for
public:
    Node(NodeId id,NodeCoords nc,bool isPort=false, ResourceType portType=ResourceType::None) {
        m_nodeId=id;
        m_hasTrade=isPort;
        m_tradeResource=portType;
        m_coords=nc;
    }
    // maybe place building that takes type and owner, and upgrade to city?
    // is is is or manual check?
    bool isEmpty() const { return m_type==NodeType::None; }
    bool isCity() const { return m_type==NodeType::City; }
    bool isSettlement() const { return m_type==NodeType::Settlement; }
    bool hasTrade() const { return m_hasTrade; }
	bool is3for1Trade() const{return hasTrade() && getTradeResource() == TradeType::None;}
    bool isTradeFor(ResourceType resourceType) const { return m_tradeResource==resourceType; } // manual check via get trade resouce
    bool isTrade() const {return hasTrade();}

    ResourceType getTradeResource() const { return m_tradeResource; }

    NodeType getNodeBuildingType() const { return m_type; }
    PlayerId getOwner() const { return m_ownerId; }

    IncidentTiles getIncidentTiles() const { return m_incidentTiles; }
    IncidentEdges getIncidentEdges() const { return m_incidentEdges; }

    void setNodeBuildingType(const NodeType nodeType) { m_type = nodeType; }
    void setOwner(PlayerId ownerId) { m_ownerId = ownerId;}
    void setTrade(TradeType tradeResource) {m_tradeResource=tradeResource; m_hasTrade=true;}
    void setId(NodeId nodeId){m_nodeId=nodeId;}
    NodeId getNodeId() const {return m_nodeId;}

    void buildSettlement(PlayerId playerId) {m_ownerId=playerId; m_type=NodeType::Settlement;}
    void buildCity() {m_type=NodeType::City;}

    void addAdjacentTile(Tile * tile) {
        m_incidentTiles.push_back(tile);
    }
    void addAdjacentEdge(Edge * edge) {
            m_incidentEdges.insert(edge); // consider array with static integer
    }

    NodeCoords getCoords() const {return m_coords;}

    friend bool operator==(const Node &lhs, const Node &rhs) {
        return lhs.m_nodeId == rhs.m_nodeId;
    }
    friend bool operator!=(const Node &lhs, const Node &rhs) {
        return !(lhs == rhs);
    }
};

#endif // NODE_H