//
// Created by matija on 1/5/26.
//

#include "RandomStandardExtendedCreator.hpp"


//
// Created by matija on 1/5/26.
//

#include "RandomStandardSmallCreator.hpp"
#include <board/Board.h>

struct TileDef;

// generates standard catan map with random resource distribution
std::vector<TileDef> RandomStandardExtendedCreator::generateRandomBoard(){
    // list of axial coordinates of tiles, given in spiral order for number assignment purposes

    static const std::array<TileCoords,30> standardExtendedCoords = {{
        // outer ring
        {0, -3}, {1, -3}, {2, -3},
        {2, -2},
        {2, -1},
        {2, 0},
        {1, 1}, {0, 2},
        {-1, 3}, {-2, 3}, {-3, 3},
        {-3, 2},
        {-3, 1},
        {-3, 0},
        {-2, -1}, {-1, -2},
        // middle ring
        {0, -2}, {1, -2},
        {1, -1},
        {1, 0},
        {0, 1},
        {-1, 2}, {-2, 2},
        {-2, 1}, {-2, 0},
        {-1,-1},
        // center ring
        {-1, 0}, {0, 0},
        {0, -1}, {-1, 1}

    }};


    // numbers in spiral order, order is fixed to insure balance
    const static std::vector<int> extendedNumbers = {
            2, 5, 4, 6, 3, 9, 8, 11, 11, 10,
            6, 3, 8, 4, 8, 10, 11, 12, 10, 5,
            4, 9, 5, 9, 12, 3, 2, 6
        };

    // populating and shuffling resources
    std::vector<TileDef> r;
    std::vector<ResourceType> hexList = {ResourceType::Desert,ResourceType::Desert};
    for(int i = 0; i < 4+2; i++) {
        hexList.push_back(ResourceType::Wood);
        hexList.push_back(ResourceType::Wheat);
        hexList.push_back(ResourceType::Wool);
    }
    for(int i = 0; i < 3+2; i++) {
        hexList.push_back(ResourceType::Ore);
        hexList.push_back(ResourceType::Brick);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(hexList.begin(), hexList.end(), g);

    // we take coord numbers and resoruces and merge them
    int j = 0;
    for(int i = 0; i < hexList.size(); i++){
        if(hexList[i] == ResourceType::Desert) {  // desert skips number
            r.push_back({standardExtendedCoords[i].q(),standardExtendedCoords[i].r(), hexList[i], 7});
        }
        else r.push_back({standardExtendedCoords[i].q(),standardExtendedCoords[i].r(), hexList[i], extendedNumbers[j++]});
    }
    return r;
}

std::unique_ptr<Board> RandomStandardExtendedCreator::getBoard() {
    auto board=std::make_unique<Board>();
    auto tiles = generateRandomBoard();
    board->initializeBoard(tiles);
    return std::move(board);
}
