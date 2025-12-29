//
// Created by andja on 10.12.25..
//

#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <vector>
#include <memory>
#include "Edge.h"
#include "Tile.h"
#include "Node.h"
#include "../../headers/Types/TypeAliases.h"

struct TileDef { int q, r; ResourceType res; int number; };


enum class PointDirection {
    Top,
    RightTop,
    RightBottom,
    Bottom,
    LeftBottom,
    LeftTop,
    End
};
enum class SideDirection {
    TopRight,
    Right,
    BottomRight,
    BottomLeft,
    Left,
    TopLeft,
    End
};


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
    static const std::array<int,18> m_standardNumberOrder;
    static const std::array<HexCoords,19> m_standardCoordinates;
    static const std::vector<TileDef> m_basicMap;
    static const std::array<HexCoords,6> m_directionCoords;

    static HexCoords directionToCoord(SideDirection dir) {return m_directionCoords[directionToIndex(dir)];}
    static int directionToIndex(SideDirection dir){return static_cast<int>(dir);}
    static int directionToIndex(PointDirection dir){return static_cast<int>(dir);}

    static void standardizeEdgeCoords(HexCoords& coords, int& index);
    static void standardizeNodeCoords(HexCoords& coords, int& index);

    void initializeBoard(std::vector<TileDef> tileMap);
    std::vector<TileDef> generateRandomBoard();

    std::map<HexCoords,Tile*> m_tilesByCoord;
    std::map<int, std::vector<Tile*>> m_tilesByNumber;

    std::vector<std::unique_ptr<Tile>> m_tiles;
    std::vector<std::unique_ptr<Node>> m_nodes;
    std::vector<std::unique_ptr<Edge>> m_edges;

public:
    Board() {  }

    std::vector<Tile*> getTilesWithNumber(int num);

    Tile* getTileAt(HexCoords coords);
    Node* getNodeAt(HexCoords coords, int index);
    Edge* getEdgeAt(HexCoords coords, int index);
    Node* getNodeById(int nodeId) const;
    Edge* getEdgeById(int edgeId) const;
    Tile* getTileById(int tileId) const;

    Node* getNodeAtDir(HexCoords coords, PointDirection);
    Edge* getEdgeAtDir(HexCoords coords, SideDirection);
    Tile* getTileAtDir(HexCoords coords, SideDirection);

    std::vector<HexCoords> getBoardCords();

};


#endif //BOARD_H
