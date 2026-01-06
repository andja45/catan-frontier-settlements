//
// Created by matija on 1/5/26.
//

#include "RandomStandarsExtendedCreator.hpp"


//
// Created by matija on 1/5/26.
//

#include "RandomStandardSmallCreator.hpp"
#include <board/Board.h>

struct TileDef;

// generates standard catan map with random resource distribution
std::vector<TileDef> RandomStandarsExtendedCreator::generateRandomBoard(){
    // list of axial coordinates of tiles, given in spiral order for number assignment purposes
    static const std::array<TileCoords,30> standardExtendedCoords = {
        {
        // ring 1
        {-1,-3},
        {0,-3},
        {1,-3},
        {1,-2},
        {2,-1},
        {2, 0},
        {2, 1},
        {1, 2},
        {0, 3},
        {-1, 3},
        {-1, 2},
        {-2, 1},
        {-3, 0},
        {-2,-1},
        {-2,-2},
        {-1,-2},
        {0,-2},
        {1,-2},

        // ring 2
        {-1,-1},
        {0,-1},
        {1,-1},
        {1, 0},
        {0, 1},
        {-1, 1},
        {-2, 0},
        {-2,-1},

        {-1, 0},
        {0, 0},
        {0, 1},
        {-1, 1}
        }
    };
    const static std::vector<int> extendedNumbers = {
            // Ring 1
            10,
            8,
            9,
            6,
            10,
            5,
            8,
            4,
            2,
            2,
            5,
            6,
            3,
            11,
            3,
            4,
            11,
            12,

           // Ring 2
            3,
            9,
            11,
            10,
            8,
            4,
            9,
            3,

           // Ring 3
            10,
            5,
            0,
            0
    }; // numbers in spiral order, order is fixed to insure balance

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

std::unique_ptr<Board> RandomStandarsExtendedCreator::getBoard() {
    auto board=std::make_unique<Board>();
    auto tiles = generateRandomBoard();
    board->initializeBoard(tiles);
    return std::move(board);
}
