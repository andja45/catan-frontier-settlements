#ifndef NODE_H
#define NODE_H
#include <vector>

#include "../Types/NodeType.h"

class Edge;
class Player;
class Tile;

class Node{
private:
    NodeType m_type;
    Player* m_owner;
    std::vector<Tile*> m_incidentTiles;
    std::vector<Edge*> m_incidentEdges;
    int m_nodeID;
public:
    Node();
    ~Node() = default;

    NodeType getNodeType() const { return m_type; }
    Player* getOwner() const { return m_owner; }
    std::vector<Tile*> getIncidentTiles() const { return m_incidentTiles; }
    std::vector<Edge*> getIncidentEdges() const { return m_incidentEdges; }

    void setNodeType(NodeType nodeType) { m_type = nodeType; }
    void setOwner(Player* owner) { m_owner = owner; }
    void setIncidentTiles(std::vector<Tile*> incidentTiles) { m_incidentTiles = incidentTiles; }
    void setIncidentEdges(std::vector<Edge*> incidentEdges) { m_incidentEdges = incidentEdges; }
};

#endif // NODE_H
