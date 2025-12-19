//
// Created by andja on 10.12.25..
//

#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <vector>
#include <memory>
struct TileDef;

enum class Direction {
    UpLeft,
    UpRight,
    Right,
    DownRight,
    DownLeft,
    Left
};

#include "Edge.h"
#include "Tile.h"
#include "Node.h"
#include "../../headers/Types/TypeAliases.h"

// Custom hash function for std::tuple
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

    std::vector<int> m_standardNumberOrder= {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11} ;
    static const std::vector<TileDef> basicMap;
    static HexCoords directionToCoord(Direction dir);
    static std::vector<HexCoords> generateCoordinates();

    static void standardizeCoords(HexCoords& coords, int& index);
    void initializeStandardBoard(std::vector<TileDef> tileMap);

    Tile* getAdjacent(Tile* tile, HexCoords dir);
    void randomBoard();

    std::map<HexCoords,Tile*> m_tilesByCoord;
    std::map<int, std::vector<Tile*>> m_tilesByNumber;

    std::vector<std::unique_ptr<Tile>> m_tiles;
    std::vector<std::unique_ptr<Node>> m_nodes;
    std::vector<std::unique_ptr<Edge>> m_edges;

public:
    Board() {  }
    ~Board();

    std::vector<Tile*> getTilesWithNumber(int num);
    Tile* getTileAt(HexCoords coords);
    Node* getNodeAt(HexCoords coords, int index);
    Edge* getEdgeAt(HexCoords coords, int index);
    Node* getNodeById(int nodeId) const;
    Edge* getEdgeById(int edgeId) const;
    Tile* getTileById(int tileId) const;

    Tile* getAdjacent(Tile* tile, Direction dir) { return getAdjacent(tile, directionToCoord(dir)); }
};


#endif //BOARD_H
