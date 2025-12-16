//
// Created by andja on 10.12.25..
//

#include "../../headers/Board.h"
#include <random>
#include <algorithm>
#include <iostream>

void Board::randomBoard(){
    std::vector<ResourceType> hexList = {ResourceType::Desert};
    for(int i = 0; i < 4; i++) {
        hexList.push_back(ResourceType::Wood);
        hexList.push_back(ResourceType::Wheat);
        hexList.push_back(ResourceType::Wool);
    }
    for(int i = 0; i < 3; i++) {
        hexList.push_back(ResourceType::Ore);
        hexList.push_back(ResourceType::Brick);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(hexList.begin(), hexList.end(), g);

    std::vector<int> numberOrder = {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11};

    int j = 0;
    for(int i = 0; i < hexList.size(); i++){
        if(hexList[i] == ResourceType::Desert) m_tiles.push_back(new Tile(hexList[i], 7));
        else m_tiles.push_back(new Tile(hexList[i], numberOrder[j++]));
    }
    for(auto tile : m_tiles) std::cout << tile->toString() << std::endl;
}
