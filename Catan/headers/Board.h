//
// Created by andja on 10.12.25..
//

#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <vector>
#include "Tile.h"

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
    std::vector<Tile*> m_tiles;
    std::map<std::tuple<int,int,int>, Tile*> m_tilesByCoord;
    std::vector<int> m_numberOrder = {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11};
    void randomBoard();
    static std::vector<std::tuple<int,int,int>> generateCoordinates();
    Tile* findAdjacent(Tile* tile, std::tuple<int,int,int> dir);
public:
    Board() { randomBoard(); }
    ~Board();
    std::vector<Tile*> getTiles() { return m_tiles; };
    std::map<std::tuple<int,int,int>, Tile*> getTilesByCoord() { return m_tilesByCoord; };
    Tile* upLeft(Tile* tile) { return findAdjacent(tile, std::tuple<int,int,int>(-1, 0, 1)); }
    Tile* upRight(Tile* tile) { return findAdjacent(tile, std::tuple<int,int,int>(0, -1, 1)); }
    Tile* right(Tile* tile) { return findAdjacent(tile, std::tuple<int,int,int>(1, -1, 0)); }
    Tile* downRight(Tile* tile) { return findAdjacent(tile, std::tuple<int,int,int>(1, 0, -1)); }
    Tile* downLeft(Tile* tile) { return findAdjacent(tile, std::tuple<int,int,int>(0, 1, -1)); }
    Tile* left(Tile* tile) { return findAdjacent(tile, std::tuple<int,int,int>(-1, 1, 0)); }
};


#endif //BOARD_H
