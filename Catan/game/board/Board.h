//
// Created by andja on 10.12.25..
//

#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <vector>
#include <memory>
#include <unordered_map>
#include <board/Edge.h>
#include "Tile.h"
#include <board/Node.h>
#include <types/TypeAliases.h>

// helper struct to specify board layout and pass parameters to create tiles
struct TileDef { int q, r; ResourceType res; int number; };
struct PortDef { int q, r, i; TradeType tradeType;};

class Board {
private:
    void clearBoard();
    // TODO load ports, from file

    TileId m_robberTile = types::InvalidTileId;

    // board is owner of all its elements, raw pointers are used inside to connect elements for convinience and optimization
    // make sure element id corresponds to index in these vectors during creation
    std::vector<std::unique_ptr<Tile>> m_tiles;
    std::vector<std::unique_ptr<Node>> m_nodes;
    std::vector<std::unique_ptr<Edge>> m_edges;

    std::unordered_map<NodeCoords,Node*> m_nodesByCoord;
    std::unordered_map<EdgeCoords,Edge*> m_edgesByCoord;
    std::unordered_map<TileCoords,Tile*> m_tilesByCoord;

    std::unordered_map<int, std::vector<Tile*>> m_tilesByNumber;

    std::vector<NodeCoords> m_tradeCoords;
public:
    Board() = default;
    ~Board()=default;

    void initializeBoard(std::vector<TileDef> tileDefs); // constructs board from tiles
    void loadTrades(std::vector<PortDef> portDefs);
    void addTrade(NodeCoords nodeCoords, TradeType tradeType);
    void addTrade(EdgeCoords edgeCoords, TradeType tradeType); // adds trade to two nodes incident to edge

    std::vector<NodeCoords> getTradeCoords() const {return m_tradeCoords;}

    std::vector<Tile*> getTilesWithNumber(int num);

    TileId robberTileId() const {
        return m_robberTile;
    }

    Tile* getTileAt(TileCoords coords) const;
    Node* getNodeAt(NodeCoords) const;
    Edge* getEdgeAt(EdgeCoords) const;

    Node* getNodeById(NodeId nodeId) const;
    Edge* getEdgeById(EdgeId edgeId) const;
    Tile* getTileById(TileId tileId) const;

    const std::vector<std::unique_ptr<Tile>>& getTiles() const { return m_tiles; }
    const std::vector<std::unique_ptr<Node>>& getNodes() const { return m_nodes; }
    const std::vector<std::unique_ptr<Edge>>& getEdges() const { return m_edges; }

    std::vector<Edge*> getEdgesAdjacentToNode(NodeId nodeId) const;
    std::vector<Node*> getNodesAdjacentToEdge(EdgeId edgeId) const;
    std::vector<Tile*> getTilesAdjacentToNode(NodeId nodeId) const;
    std::vector<Node*> getNodesAdjacentToNode(NodeId edgeId) const;
    std::vector<Edge*> getIncidentEdges(EdgeId edgeId) const;
    std::vector<Edge*> getIncidentContinuousEdges(EdgeId edgeId) const;  // gets edges connected to given edge by nodes owned by same player
    Node* getNodeBetweenEdges(EdgeId edge1Id, EdgeId edge2Id) const;

    bool isEdgeFree(EdgeId edgeId) const;
    bool isNodeFree(NodeId nodeId) const;
    bool isRobberOnTile(TileId tileId) const { return getTileById(tileId)->isRobberOnTile(); }

    PlayerId getEdgeOwner(EdgeId edgeId) const;
    PlayerId getNodeOwner(NodeId nodeId) const;

    bool edgeTouchesPlayersBuilding(PlayerId playerId, EdgeId edgeId) const; // checks if edge is incident with node belonging to player with given player id
    bool edgeTouchesPlayersRoad(PlayerId playerId, EdgeId edgeId) const; // similar to above but for edges
    bool nodeTouchesPlayerRoad(PlayerId playerId, NodeId nodeId) const;
    bool tileTouchesPlayerBuilding(PlayerId playerId, TileId tileId) const;

    bool edgeTouchesNode(NodeId nodeId, EdgeId edgeId) const; // checks if edge is incident with given node
    bool nodeTouchesAnyBuilding(NodeId nodeId) const;

    void placeRoad(PlayerId playerId, EdgeId edgeId) const;
    void placeSettlement(PlayerId playerId, NodeId nodeId);
    void placeCity(PlayerId playerId, NodeId nodeId);
    void placeRobber(TileId tileId) { getTileById(m_robberTile)->setRobber(false);  getTileById(tileId)->setRobber(); m_robberTile=tileId; }

    bool isBuildingOwnedBy(PlayerId playerId, NodeId nodeId) const; // consider removing, use get node owner and check yourself
    bool isNodeSettlement(NodeId nodeId) const;

    std::vector<AxialCoords> getBoardCords() const;
    std::vector<TileDef> getTileDefs() const;
    std::vector<PortDef> getPortDefs() const;

    std::vector<EdgeId> edgeIds() const;
    std::vector<NodeId> nodeIds() const;
    std::vector<TileId> tileIds() const;
};


#endif //BOARD_H
