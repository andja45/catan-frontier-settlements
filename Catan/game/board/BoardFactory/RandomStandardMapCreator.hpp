//
// Created by matija on 1/5/26.
//

#ifndef CATAN_RANDOMSTANDARDSMALLCREATOR_HPP
#define CATAN_RANDOMSTANDARDSMALLCREATOR_HPP

#include <algorithm>
#include <random>
#include <board/Board.h>
#include <board/BoardFactory/AbstractBoardCreator.hpp>

class RandomStandardMapCreator : public AbstractBoardCreator{
private:
    // default catan setup
    static std::array<int,18> m_standardNumberOrder;    // number ordering specified in clockwise order, can be used with any tile tile type rearrangement of standard 3-4-5-4-3 catan map
    static const std::array<TileCoords,19> m_standardCoordinates; // coordinates of tiles in standard board, can be used to assign random or standard numbers
    static const std::vector<TileDef> m_basicMap;  // one default standard catan map with fixed numbering, tile position and tile types
    static const std::array<TileCoords,6> m_directionCoords; // Coordinate vectors for each direction, for adjacent tiles
    static std::vector<TileDef> generateRandomBoard();
    static void populatePorts(Board*);
public:
    std::unique_ptr<Board> getBoard() override;

};

#endif //CATAN_RANDOMSTANDARDSMALLCREATOR_HPP