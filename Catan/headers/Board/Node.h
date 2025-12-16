#ifndef NODE_H
#define NODE_H
#include <vector>

#include "../Types/NodeType.h"
#include "../Types/ResourceType.h"
#include "../Types/TypeAliases.hpp"

class Player;
enum class NodeType;

class Node{
private:
    NodeType _type=NodeType::None;
    Player* _owner=nullptr;

    IncidentTiles _incidentTiles{};
    IncidentEdges _incidentEdges{};

    int _nodeID=-1;
    HexCoords _tileCoords = {-1,-1};
    int _nodeIndex=-1;
    static int _numOfNodes;

    bool _hasTrade=false;
    ResourceType _tradeResource=ResourceType::None;
public:
    Node(int q, int r, int i, bool isPort=false, ResourceType portType=ResourceType::None) : _nodeIndex(i) {
        _numOfNodes++; _nodeID=_numOfNodes;
        _tileCoords={q,r};
        _hasTrade=isPort; _tradeResource=portType;
    }

    bool isEmpty() const { return _type==NodeType::None; }
    bool hasTrade() const { return _hasTrade; }
    ResourceType getTradeResource() const { return _tradeResource; }

    NodeType getNodeType() const { return _type; }
    Player* getOwner() const { return _owner; }
    IncidentTiles getIncidentTiles() const { return _incidentTiles; }
    IncidentEdges getIncidentEdges() const { return _incidentEdges; }

    HexCoords getTileCoords() const {return _tileCoords;}
    int getNodeIndex() const {return _nodeIndex;}

    void setNodeType(const NodeType nodeType) { _type = nodeType; }
    void setOwner(Player* owner) { _owner = owner; _type=NodeType::Settlement; }
    void upgradeToCity() {_type=NodeType::City;}
    void setTrade(ResourceType tradeResource) {_hasTrade=true;_tradeResource=tradeResource;}

    void setIncidentTiles(const IncidentTiles &incidentTiles) { _incidentTiles = incidentTiles; }
    void setIncidentEdges(const IncidentEdges &incidentEdges) { _incidentEdges = incidentEdges; }

    void addAdjacentTile(Tile * tile, int corner);
    void addAdjacentEdge(Edge * edge, int corner);
};

#endif // NODE_H
