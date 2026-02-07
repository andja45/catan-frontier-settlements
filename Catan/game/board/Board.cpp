//
// Created by andja on 10.12.25..
//

#include "Board.h"
#include "Node.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <set>
#include <unordered_set>
#include <vector>
#include <board/coords/AxialCoords.hpp>
#include <board/coords/NodeCoords.hpp>
#include <board/coords/EdgeCoords.hpp>

#include <iostream>

struct TileDef;


void Board::clearBoard() {
    m_tiles.clear();
    m_nodes.clear();
    m_edges.clear();
    m_tilesByCoord.clear();
    m_tilesByNumber.clear();
}


// initializes tiles edge nodes and connects them together
void Board::initializeBoard(std::vector<TileDef> tileMap) {
    m_robberTile=types::InvalidTileId;
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

        if (m_robberTile==types::InvalidTileId && res==ResourceType::Desert) {
            m_robberTile=id;
            rawTile->setRobber(true);
        }

        // make nodes
        auto nodeCoords = ax.getNodeCoords();
        auto edgeCoords = ax.getEdgeCoords();
        int numOfElements=nodeCoords.size();

        for (auto nc:nodeCoords) {
            int id=m_nodes.size();
            if (m_nodesByCoord.find(nc)!=m_nodesByCoord.end()) {
                continue; // node already exists
            }
            auto n = std::make_unique<Node>(id,nc);
            Node* rawNode = n.get();
            m_nodes.push_back(std::move(n));
            m_nodesByCoord[nc]=rawNode;
        }

        // make edges
        for (auto ec:edgeCoords) {
            int id=m_edges.size();
            if (m_edgesByCoord.find(ec)!=m_edgesByCoord.end()) {
                continue; // edge already exists
            }
            auto e = std::make_unique<Edge>(id,ec);
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
            rawTile->addAdjacentNode(node);
        }
    }
}

std::vector<Edge *> Board::getEdgesAdjacentToNode(NodeId nodeId) const {
    Node* node=getNodeById(nodeId);

    if (node == nullptr) {
        return {};
    }

    return std::vector(node->getIncidentEdges().begin(),node->getIncidentEdges().end());
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
    Edge* e1 = getEdgeById(edge1Id);
    Edge* e2 = getEdgeById(edge2Id);
    Node* a1 = e1->getStart();
    Node* b1 = e1->getEnd();
    Node* a2 = e2->getStart();
    Node* b2 = e2->getEnd();

    if (a1 == a2 || a1 == b2) return a1;
    if (b1 == a2 || b1 == b2) return b1;
    return nullptr; // not adjacent edges
}


std::vector<Edge *> Board::getIncidentContinuousEdges(EdgeId edgeId) const {
    Edge* edge=getEdgeById(edgeId);
    std::vector<Edge*> edges;

    if (edge == nullptr) {
        return {};
    }

    std::vector<Edge*> adjacentEdges1;
    if (edge->getStart() != nullptr) {
        if (edge->getStart()->isEmpty() || edge->getStart()->getOwner()==edge->getOwner())
            adjacentEdges1=std::vector<Edge*>(getEdgesAdjacentToNode(edge->getStart()->getNodeId()));
    }

    std::vector<Edge*> adjacentEdges2;
    if (edge->getEnd() != nullptr) {
        if (edge->getEnd()->isEmpty() || edge->getEnd()->getOwner()==edge->getOwner())
            adjacentEdges2=std::vector<Edge*>(getEdgesAdjacentToNode(edge->getEnd()->getNodeId()));
    }
    std::vector<Edge*> continuousEdges;
    adjacentEdges1.insert(adjacentEdges1.end(),adjacentEdges2.begin(),adjacentEdges2.end());

    for (Edge* e : adjacentEdges1) {
        if (e->getOwner()==edge->getOwner() && e->getEdgeId()!=edgeId)
            continuousEdges.push_back(e);
    }

    return continuousEdges;
}

std::vector<Edge *> Board::getIncidentEdges(EdgeId edgeId) const {
    Edge* edge=getEdgeById(edgeId);
    std::vector<Edge*> edges;

    std::vector<Edge*> adjacentEdges1(getEdgesAdjacentToNode(edge->getStart()->getNodeId()));
    std::vector<Edge*> adjacentEdges2(getEdgesAdjacentToNode(edge->getEnd()->getNodeId()));
    adjacentEdges1.insert(adjacentEdges1.end(),adjacentEdges2.begin(),adjacentEdges2.end());

    adjacentEdges1.erase(std::remove_if(adjacentEdges1.begin(), adjacentEdges1.end(), [edgeId](Edge* e){return e->getEdgeId()==edgeId;}), adjacentEdges1.end());

    return adjacentEdges1;
}

void Board::addTrade(NodeCoords nodeCoords, TradeType tradeType) {
    Node* node=getNodeAt(nodeCoords);
    node->setTrade(tradeType);
    m_tradeCoords.push_back(nodeCoords);
}

void Board::addTrade(EdgeCoords edgeCoords, TradeType tradeType) {
     for (auto n:getEdgeAt(edgeCoords)->getNodes()) {
         addTrade(n->getCoords(),tradeType);
     }
}

void Board::loadTrades(std::vector<PortDef> portDefs) {
    for (const auto&[q, r, i, tradeType] : portDefs) {
        NodeCoords nc(q,r,i);
        addTrade(nc,static_cast<TradeType>(tradeType));
    }
}

std::vector<Node *> Board::getNodesAdjacentToEdge(EdgeId edgeId) const {
    return getEdgeById(edgeId)->getNodes();
}

std::vector<Tile *> Board::getTilesWithNumber(int num) {
    return m_tilesByNumber[num];
}

Tile * Board::getTileAt(TileCoords coords) const {
    if (m_tilesByCoord.find(coords)==m_tilesByCoord.end()) return nullptr;
    return m_tilesByCoord.at(coords);
}

Node * Board::getNodeAt(NodeCoords nc) const{
    if (m_nodesByCoord.find(nc)==m_nodesByCoord.end()) return nullptr;
    return m_nodesByCoord.at(nc);
}

Edge * Board::getEdgeAt(EdgeCoords ec) const {
    if (m_edgesByCoord.find(ec)==m_edgesByCoord.end()) return nullptr;
    return m_edgesByCoord.at(ec);
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


bool Board::edgeTouchesNode(NodeId nodeId, EdgeId edgeId) const{
    Edge* edge = this->getEdgeById(edgeId);

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
    for (Edge* e : getIncidentEdges(edgeId)) {
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

bool Board::tileTouchesPlayerBuilding(PlayerId playerId, TileId tileId) const {
    auto tile = this->getTileById(tileId);
    auto nodes=tile->getAdjacentNodes();
    for (auto n : nodes) {
        if (n->getOwner()==playerId)
            return true;
    }
    return false;
}

void Board::placeRoad(PlayerId playerId, EdgeId edgeId) const {
    Edge* edge = this->getEdgeById(edgeId);

    if (edge == nullptr) {
        return;
    }

    edge->setOwner(playerId);
}
void Board::placeSettlement(PlayerId playerId, NodeId nodeId) {
    Node* node = this->getNodeById(nodeId);
    if (node == nullptr) {
        return;
    }
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

bool Board::isNodeCity(NodeId nodeId) const {
    return getNodeById(nodeId)->isCity();
}

std::vector<AxialCoords> Board::getBoardCords() const{
    std::vector<AxialCoords> coords;
    for (auto&[coord, tile] : m_tilesByCoord) {
        coords.push_back(coord);
    }
    return coords;
}

std::vector<TileDef> Board::getTileDefs() const {
    std::vector<TileDef> tileDefs;
    for (const auto& tile : m_tiles) {
        TileDef tdef;
        AxialCoords coord = tile->getTileCoord();
        tdef.q = coord.q();
        tdef.r = coord.r();
        tdef.res = tile->getResourceType();
        tdef.number = tile->getNumber();
        tileDefs.push_back(tdef);
    }
    return tileDefs;
}

std::vector<PortDef> Board::getPortDefs() const {
    std::vector<PortDef> portDefs;
    for (const auto& tradeCoord : m_tradeCoords) {
        PortDef pdef;
        pdef.q = tradeCoord.axialCoords().q();
        pdef.r = tradeCoord.axialCoords().r();
        pdef.i = static_cast<int>(tradeCoord.direction());
        Node* node = getNodeAt(tradeCoord);
        pdef.tradeType = node->getTradeResource();
        portDefs.push_back(pdef);
    }
    return portDefs;
}

std::vector<EdgeId> Board::edgeIds() const {
    std::vector<EdgeId> edgeIds;
    for (const auto& edge : m_edges) {
        edgeIds.push_back(edge->getEdgeId());
    }
    return edgeIds;
}

std::vector<NodeId> Board::nodeIds() const {
    std::vector<NodeId> nodeIds;
    for (const auto& node : m_nodes) {
        nodeIds.push_back(node->getNodeId());
    }
    return nodeIds;
}

std::vector<TileId> Board::tileIds() const {
    std::vector<TileId> tileIds;
    for (const auto& tile : m_tiles) {
        tileIds.push_back(tile->getTileId());
    }
    return tileIds;
}
