#ifndef NODE_H
#define NODE_H
#include <vector>

#include "NodeType.h"

class Edge;
class Player;
class Tile;

class Node{
private:
    NodeType _type;
    Player* _owner;
    std::vector<Tile*> _incidentTiles;
    std::vector<Edge*> _incidentEdges;
    int _nodeID;
public:
    Node();
    ~Node() = default;

    NodeType getNodeType() const { return _type; }
    Player* getOwner() const { return _owner; }
    std::vector<Tile*> getIncidentTiles() const { return _incidentTiles; }
    std::vector<Edge*> getIncidentEdges() const { return _incidentEdges; }

    void setNodeType(NodeType nodeType) { _type = nodeType; }
    void setOwner(Player* owner) { _owner = owner; }
    void setIncidentTiles(std::vector<Tile*> incidentTiles) { _incidentTiles = incidentTiles; }
    void setIncidentEdges(std::vector<Edge*> incidentEdges) { _incidentEdges = incidentEdges; }
};

#endif // NODE_H
