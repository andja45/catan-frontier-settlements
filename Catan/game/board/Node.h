#ifndef NODE_H
#define NODE_H

#include <types/NodeType.h>
#include <types/ResourceType.h>
#include <types/TypeAliases.h>
#include <board/Coords/NodeCoords.hpp>

class Edge;
class Tile;

enum class NodeType;

class Node {
private:
    //TODO consider class
    NodeType m_type=NodeType::None; // is it empty settlement or city
    PlayerId m_ownerId=-1;  // if has building who owns it

    // for easier neighbour search
    IncidentTiles m_incidentTiles{};
    IncidentEdges m_incidentEdges{};

    NodeId m_nodeId=-1;
    inline static int m_numOfNodes=0;

    NodeCoords m_nodeCoords;

    bool m_hasTrade=false; //TODO consider class
    ResourceType m_tradeResource=ResourceType::None; // if has trade than this resolve which resource its for
public:
    Node(NodeCoords coords,bool isPort=false, ResourceType portType=ResourceType::None) : Node(coords.q(), coords.r(), coords.i(), isPort, portType) {}
    Node(int q, int r, int i, bool isPort=false, ResourceType portType=ResourceType::None)  {
        m_numOfNodes++; m_nodeId =m_numOfNodes;
        m_nodeCoords={q,r,i};
        m_hasTrade=isPort; m_tradeResource=portType;
    }

    bool isEmpty() const { return m_type==NodeType::None; }
    bool hasTrade() const { return m_hasTrade; }
	bool is3for1Trade() const;
    ResourceType getTradeResource() const { return m_tradeResource; }

    NodeType getNodeBuildingType() const { return m_type; }
    PlayerId getOwner() const { return m_ownerId; }
    IncidentTiles getIncidentTiles() const { return m_incidentTiles; }
    IncidentEdges getIncidentEdges() const { return m_incidentEdges; }

    NodeCoords getNodeCoords() const {return m_nodeCoords;}

    void setNodeBuildingType(const NodeType nodeType) { m_type = nodeType; }
    void setOwner(PlayerId ownerId) { m_ownerId = ownerId;}
    void setTrade(ResourceType tradeResource) {m_tradeResource=tradeResource;}

    void addAdjacentTile(Tile * tile){ m_incidentTiles.push_back(tile);}
    void addAdjacentEdge(Edge * edge){ m_incidentEdges.push_back(edge);}

    friend bool operator==(const Node &lhs, const Node &rhs) {
        return lhs.m_nodeId == rhs.m_nodeId;
    }
    friend bool operator!=(const Node &lhs, const Node &rhs) {
        return !(lhs == rhs);
    }

};

#endif // NODE_H