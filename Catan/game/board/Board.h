//
// Created by andja on 10.12.25..
//

#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <vector>
#include <memory>
#include <board/Edge.h>
#include "Tile.h"
#include <board/Node.h>
#include <types/TypeAliases.h>

// helper struct to specify board layout and pass parametars to create tiles
struct TileDef { int q, r; ResourceType res; int number; };

// Custom hash function for tuple
struct TupleHash {
    template <class T>
    void hash_combine(std::size_t& seed, T const& v) const {
        seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template <class Tuple>
    size_t operator()(const Tuple& tuple) const {
        size_t seed = 0;
        std::apply([&](const auto&... args) {
            (hash_combine(seed, args), ...);
        }, tuple);
        return seed;
    }
};



class Board {
private:
    // default catan setup
    static std::array<int,18> m_standardNumberOrder;    // number ordering specified in clockwise order, can be used with any tile tile type rearrangement of standard 3-4-5-4-3 catan map
    static const std::array<HexCoords,19> m_standardCoordinates; // coordinates of tiles in standard board, can be used to assign random or standard numbers
    static const std::vector<TileDef> m_basicMap;  // one default standard catan map with fixed numbering, tile position and tile types
    static const std::array<HexCoords,6> m_directionCoords; // Coordinate vectors for each direction, for adjacent tiles

    // TODO factory?
    std::vector<TileDef> generateRandomBoard();

    // TODO coors classes? dir in tile?
    static HexCoords directionToCoord(SideDirection dir) {return m_directionCoords[directionToIndex(dir)];}
    static int directionToIndex(SideDirection dir){return static_cast<int>(dir);}
    static int directionToIndex(PointDirection dir){return static_cast<int>(dir);}

    static void standardizeEdgeCoords(HexCoords& coords, int& index);
    static void standardizeNodeCoords(HexCoords& coords, int& index);

    void clearBoard();
    void initializeBoard();
    void connectBoardElements();

    // TODO separate class
    void saveBoard(const std::string &saveFilePath);
    std::vector<TileDef> loadSavedBoard(const std::string &loadFilePath);
    std::vector<TileDef> loadBoardFromTextFile(const std::string &loadFilePath);

    // board is owner of all its elements, raw pointers are used inside to connect elements for convinience and optimization
    std::vector<std::unique_ptr<Tile>> m_tiles;
    std::vector<std::unique_ptr<Node>> m_nodes;
    std::vector<std::unique_ptr<Edge>> m_edges;

    std::map<NodeCoords,Node*> m_nodesByCoord;
    std::map<EdgeCoords,Node*> m_edgesByCoord;
    std::map<HexCoords,Tile*> m_tilesByCoord;
    std::map<int, std::vector<Tile*>> m_tilesByNumber;

public:
    Board() { initializeBoard(); }

    std::vector<Tile*> getTilesWithNumber(int num);

    Tile* getTileAt(HexCoords coords);
    Node* getNodeAt(HexCoords coords, int index);
    Edge* getEdgeAt(HexCoords coords, int index);
    Node* getNodeById(NodeId nodeId) const;
    Edge* getEdgeById(EdgeId edgeId) const;
    Tile* getTileById(TileId tileId) const;

    Node* getNodeAtDir(HexCoords coords, PointDirection);
    Edge* getEdgeAtDir(HexCoords coords, SideDirection);
    Tile* getTileAtDir(HexCoords coords, SideDirection);

    std::vector<HexCoords> getBoardCords();
    const std::vector<std::unique_ptr<Tile>>& getTiles() const { return m_tiles; }

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
