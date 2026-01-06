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

//TODO consider trade and houses saved here?
class Board {
private:
    void clearBoard();
    // TODO add ports, load ports, from file

    // board is owner of all its elements, raw pointers are used inside to connect elements for convinience and optimization
    // make sure element id corresponds to index in these vectors during creation
    std::vector<std::unique_ptr<Tile>> m_tiles;
    std::vector<std::unique_ptr<Node>> m_nodes;
    std::vector<std::unique_ptr<Edge>> m_edges;

    std::unordered_map<NodeCoords,Node*> m_nodesByCoord;
    std::unordered_map<EdgeCoords,Edge*> m_edgesByCoord;
    std::unordered_map<TileCoords,Tile*> m_tilesByCoord;

    std::unordered_map<int, std::vector<Tile*>> m_tilesByNumber;
public:
    Board() = default;
    ~Board()=default;

    void initializeBoard(std::vector<TileDef> tileDefs);

    std::vector<Tile*> getTilesWithNumber(int num);

    Tile* getTileAt(TileCoords coords);
    Node* getNodeAt(NodeCoords);
    Edge* getEdgeAt(EdgeCoords);

    Node* getNodeById(NodeId nodeId) const;
    Edge* getEdgeById(EdgeId edgeId) const;
    Tile* getTileById(TileId tileId) const;

    const std::vector<std::unique_ptr<Tile>>& getTiles() const { return m_tiles; }

    std::vector<Edge*> getAdjacentEdges(NodeId nodeId) const;
    std::vector<Edge*> getIncidentEdges(EdgeId edgeId) const;
    std::vector<Node*> getAdjacentNodes(EdgeId edgeId) const;
    std::vector<Tile*> getAdjacentTiles(NodeId nodeId) const;
    Node* getNodeBetweenEdges(EdgeId edge1Id, EdgeId edge2Id) const;
    std::vector<Edge*> getIncidentContinuous(EdgeId edgeId) const;

    bool isEdgeFree(EdgeId edgeId) const; // TODO implement
    bool isNodeFree(NodeId nodeId) const;
    bool edgeTouchesPlayerHouse(PlayerId playerId, EdgeId edgeId) const; // TODO implement | refer to GameModel -> canPlaceRoad | one side of edge is either settlement or city owned by this player
    bool edgeTouchesPlayerSettlement(NodeId settlementId, EdgeId edgeId) const; // TODO implement | one side of edge touches this settlement
    bool edgeTouchesPlayerRoad(PlayerId playerId, EdgeId edgeId) const;
    bool nodeTouchesAnySettlement(int nodeId) const; // TODO implement | refer to GameModel -> canPlaceSettlement
    bool nodeTouchesPlayerRoad(int playerId, int nodeId) const;


    void placeRoad(PlayerId playerId, EdgeId edgeId) const; // TODO implement | refer to GameModel -> placeRoad
    void placeSettlement(PlayerId playerId, NodeId nodeId); // TODO implement | refer to GameModel -> placeSettlement
    bool isSettlementOwnedBy(PlayerId playerId, NodeId nodeId) const; // TODO implement | first false if not settlement then false if owner isnt playerid
    void placeCity(PlayerId playerId, NodeId nodeId); // TODO implement | node upgradToCity (player has pointers so it will be registered)

};


#endif //BOARD_H
