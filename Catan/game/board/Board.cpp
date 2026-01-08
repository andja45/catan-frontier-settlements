//
// Created by andja on 10.12.25..
//

#include "Board.h"
#include "Node.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <queue>
#include <unordered_set>
#include <vector>
#include <board/Coords/AxialCoords.hpp>
#include <board/Coords/NodeCoords.hpp>
#include <board/Coords/EdgeCoords.hpp>

struct TileDef;


void Board::clearBoard() {
    m_tiles.clear();
    m_nodes.clear();
    m_edges.clear();
    m_tilesByCoord.clear();
    m_tilesByNumber.clear();
}


// initializes tiles edge nodes and connects them together
void Board::initializeBoard(std::vector<TileDef> tileMap) { //TODO ROBBER AND PORTS, break into parts?
    clearBoard();

    for (const auto&[q, r, res, number] : tileMap) {
        // make tile for each tiledef
        AxialCoords ax(q,r);

        auto t = std::make_unique<Tile>(q, r, res, number);
        Tile* rawTile = t.get();
        int id=m_tiles.size();
        t->setId(id);
        m_tiles.push_back(std::move(t));
        m_tilesByCoord[ax]=rawTile;

        if (number >= 2 && number <= 12)
            m_tilesByNumber[number].push_back(rawTile);

        // make nodes
        auto nodeCoords = ax.getNodeCoords();
        auto edgeCoords = ax.getEdgeCoords();
        int numOfElements=nodeCoords.size();

        for (auto nc:nodeCoords) {
            int id=m_nodes.size();
            auto n = std::make_unique<Node>(id);
            Node* rawNode = n.get();
            m_nodes.push_back(std::move(n));
            m_nodesByCoord[nc]=rawNode;
        }

        // make edges
        for (auto ec:edgeCoords) {
            int id=m_edges.size();
            auto e = std::make_unique<Edge>(id);
            Edge* rawEdge = e.get();
            m_edges.push_back(std::move(e));
            m_edgesByCoord[ec]=rawEdge;
        }

        // connect elements
        for (int i=0; i<numOfElements; i++) {
            NodeCoords nc=nodeCoords[i];
            NodeCoords nnc=nodeCoords[(i+1)%numOfElements];
            EdgeCoords ec=edgeCoords[i];

            Edge* edge=getEdgeAt(ec);
            Node* node=getNodeAt(nc);
            Node* nextNode=getNodeAt(nnc);

            edge->setNodes(node,nextNode);
            node->addAdjacentEdge(edge);
            nextNode->addAdjacentEdge(edge);

            node->addAdjacentTile(rawTile);
        }

    }

}

std::vector<Edge *> Board::getEdgesAdjacentToNode(NodeId nodeId) const {
    Node* node=getNodeById(nodeId);
    return node->getIncidentEdges();
}
std::vector<Tile *> Board::getTilesAdjacentToNode(NodeId nodeId) const {
    return getNodeById(nodeId)->getIncidentTiles();
}

std::vector<Node *> Board::getNodesAdjacentToNode(NodeId nodeId) const {
    Node* node=getNodeById(nodeId);
    auto edges=getEdgesAdjacentToNode(nodeId);
    std::vector<Node*> nodes;
    for (Edge* e : edges) {
        nodes.push_back(e->getStart());
        nodes.push_back(e->getEnd());
    }
    nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
    return nodes;
}

Node * Board::getNodeBetweenEdges(EdgeId edge1Id, EdgeId edge2Id) const {
    Edge* edge1=getEdgeById(edge1Id);
    Edge* edge2=getEdgeById(edge2Id);
    return edge1->getStart()==edge2->getEnd()?edge1->getStart():edge1->getEnd();
}

// remove in future, use get incidentEdges and filter by get node between?
std::vector<Edge *> Board::getIncidentContinuousEdges(EdgeId edgeId) const {
    Edge* edge=getEdgeById(edgeId);
    std::vector<Edge*> edges;

    std::vector<Edge*> adjacentEdges1;
    if (edge->getStart()->getOwner()==edge->getOwner())
       adjacentEdges1=std::vector<Edge*>(edge->getStart()->getIncidentEdges());

    std::vector<Edge*> adjacentEdges2;
    if (edge->getEnd()->getOwner()==edge->getOwner())
        adjacentEdges2=std::vector<Edge*>(edge->getEnd()->getIncidentEdges());

    adjacentEdges1.insert(adjacentEdges1.end(),adjacentEdges2.begin(),adjacentEdges2.end());
    adjacentEdges1.erase(std::remove_if(adjacentEdges1.begin(), adjacentEdges1.end(), [edgeId](Edge* e){return e->getEdgeId()==edgeId;}), adjacentEdges1.end());

    return adjacentEdges1;
}

std::vector<Edge *> Board::getIncidentEdges(EdgeId edgeId) const {
    Edge* edge=getEdgeById(edgeId);
    std::vector<Edge*> edges;

    std::vector<Edge*> adjacentEdges1(edge->getStart()->getIncidentEdges());
    std::vector<Edge*> adjacentEdges2(edge->getEnd()->getIncidentEdges());
    adjacentEdges1.insert(adjacentEdges1.end(),adjacentEdges2.begin(),adjacentEdges2.end());

    adjacentEdges1.erase(std::remove_if(adjacentEdges1.begin(), adjacentEdges1.end(), [edgeId](Edge* e){return e->getEdgeId()==edgeId;}), adjacentEdges1.end());

    return adjacentEdges1;
}

void Board::addTrade(NodeCoords nodeCoords, TradeType tradeType) {
    Node* node=getNodeAt(nodeCoords);
    node->setTrade(tradeType);
}

std::vector<Node *> Board::getNodesAdjacentToEdge(EdgeId edgeId) const {
    return getEdgeById(edgeId)->getNodes();
}

std::vector<Tile *> Board::getTilesWithNumber(int num) {
    return m_tilesByNumber[num];
}

Tile * Board::getTileAt(TileCoords coords) {
    return m_tilesByCoord[coords];
}

Node * Board::getNodeAt(NodeCoords nc) {
    return m_nodesByCoord[nc];
}

Edge * Board::getEdgeAt(EdgeCoords ec) {
    return m_edgesByCoord[ec];
}

Node * Board::getNodeById(NodeId nodeId) const {
    if (m_nodes[nodeId]->getNodeId()==nodeId) { // we will make sure ids correspond to indices during creation
        return m_nodes[nodeId].get();
    }
    for (const auto &node : m_nodes) { // this is for safety if someone creates board incorrectly
        if (node->getNodeId() == nodeId) return node.get();
    }
    return nullptr;
}

Edge * Board::getEdgeById(EdgeId edgeId) const {
    if (m_edges[edgeId]->getEdgeId()==edgeId) {
        return m_edges[edgeId].get();
    }
    for (const auto &edge : m_edges) {
        if (edge->getEdgeId() == edgeId) return edge.get();
    }
    return m_edges[edgeId].get();
}

Tile * Board::getTileById(TileId tileId) const {
    if (m_tiles[tileId]->getTileId()==tileId) {
        return m_tiles[tileId].get();
    }
    for (const auto &tile : m_tiles) {
        if (tile->getTileId() == tileId) return tile.get();
    }
    return nullptr;
}


bool Board::isEdgeFree(EdgeId edgeId) const {
    Edge* edge = this->Board::getEdgeById(edgeId);
    return !edge->isOccupied();
}
bool Board::isNodeFree(NodeId nodeId) const {
    Node * node = this->getNodeById(nodeId);
    return node->isEmpty();
}

PlayerId Board::getEdgeOwner(EdgeId edgeId) const {
    Edge* edge = this->getEdgeById(edgeId);
    return edge->getOwner();
}
PlayerId Board::getNodeOwner(NodeId nodeId) const {
    Node* node = this->getNodeById(nodeId);
    return node->getOwner();
}

// TODO clean these functions
// TODO make uniform interface for edges and nodes!!

bool Board::edgeTouchesNode(NodeId nodeId, EdgeId edgeId) const{
    Edge* edge = this->getEdgeById(nodeId);

    for (Node* n : edge->getNodes()) {
        if (!n) {
		     continue;
	    }
        if (n->getNodeId() == nodeId) {
            return true;
	    }
    }
    return false;
}
bool Board::edgeTouchesPlayersBuilding(PlayerId playerId, EdgeId edgeId) const {
    Edge* edge = this->getEdgeById(edgeId);

    for (Node* n : edge->getNodes()) {
        if (!n) {
		     continue;
	    }
        if (n->getOwner() == playerId) {
            return true;
	    }
    }
    return false;
}
bool Board::edgeTouchesPlayersRoad(PlayerId playerId, EdgeId edgeId) const{
    Edge* edge = this->getEdgeById(edgeId);

    for (Edge* e : getEdgesAdjacentToNode(edgeId)) {
        if (!e) {
		    continue;
	    }
        if (e->getOwner() == playerId) {
		    return true;
	    }
    }
    return false;
}
bool Board::nodeTouchesAnyBuilding(int nodeId) const {
    Node* node = this->getNodeById(nodeId);

     for (Node* adj : getNodesAdjacentToNode(nodeId)) {
        if (!adj) {
		    continue;
	    }
        if (!adj->isEmpty()) {
            return true;
	    }
    }

    return false;
}
bool Board::nodeTouchesPlayerRoad(int playerId, int nodeId) const{
    Node* node = this->getNodeById(nodeId);

	for (Edge* e : node->getIncidentEdges()) {
        if (e && e->getOwner() == playerId)
            return true;
    }
	return false;
}

void Board::placeRoad(PlayerId playerId, EdgeId edgeId) const {
    Edge* edge = this->getEdgeById(edgeId);

    edge->setOwner(playerId);
}
void Board::placeSettlement(PlayerId playerId, NodeId nodeId) {
    Node* node = this->getNodeById(nodeId);
    node->setOwner(playerId);
    node->setNodeBuildingType(NodeType::Settlement);
}
void Board::placeCity(PlayerId playerId, NodeId nodeId){
	Node * node = this->getNodeById(nodeId);

	if(node->getOwner() == playerId) {
		node->setNodeBuildingType(NodeType::City);
	 }
}

bool Board::isBuildingOwnedBy(PlayerId playerId, NodeId nodeId) const {
    Node * node = this->getNodeById(nodeId);

    if(!node->isEmpty() && node->getOwner() == playerId) {
	    return true;
    }
    return false;
}

bool Board::isNodeSettlement(NodeId nodeId) const {
    return getNodeById(nodeId)->isSettlement();
}


std::vector<AxialCoords> Board::getBoardCords() {
    std::vector<AxialCoords> coords;
    for (auto&[coord, tile] : m_tilesByCoord) {
        coords.push_back(coord);
    }
    return coords;
}
