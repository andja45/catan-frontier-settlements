//
// Created by andja on 10.12.25..
//

#include "../../headers/Board.h"
#include <random>
#include <algorithm>

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

    auto hexCoordinates = generateCoordinates();

    int j = 0;
    for(int i = 0; i < hexList.size(); i++){
        if(hexList[i] == ResourceType::Desert) m_tiles.push_back(new Tile(hexList[i], 7, hexCoordinates[i]));
        else m_tiles.push_back(new Tile(hexList[i], m_numberOrder[j++], hexCoordinates[i]));
    }
    for(auto tile : m_tiles) {
        m_tilesByCoord[tile->getTileCoord()] = tile;
    }
}

std::vector<std::tuple<int,int,int>> Board::generateCoordinates(){
    std::vector<std::tuple<int,int,int>> hexCoordinates = {
        // Center
        { 0,  0,  0},

        // Ring 1 (distance = 1)
        { 1, -1,  0},
        { 1,  0, -1},
        { 0,  1, -1},
        {-1,  1,  0},
        {-1,  0,  1},
        { 0, -1,  1},

        // Ring 2 (distance = 2)
        { 1, -2,  1},
        { 2, -2,  0},
        { 2, -1, -1},
        { 2,  0, -2},
        { 1,  1, -2},
        { 0,  2, -2},
        {-1,  2, -1},
        {-2,  2,  0},
        {-2,  1,  1},
        {-2,  0,  2},
        {-1, -1,  2},
        { 0, -2,  2}
    };
    std::reverse(hexCoordinates.begin(), hexCoordinates.end());
    return hexCoordinates;
}

Board::~Board(){
    for(Tile* tile : m_tiles)
        delete tile;
}

Tile* Board::getAdjacent(Tile* tile, std::tuple<int,int,int> dir){
    auto coord = tile->getTileCoord();
    std::get<0>(coord) += std::get<0>(dir);
    std::get<1>(coord) += std::get<1>(dir);
    std::get<2>(coord) += std::get<2>(dir);

    if(abs(std::get<0>(coord)) == 3) return new Tile();
    if(abs(std::get<1>(coord)) == 3) return new Tile();
    if(abs(std::get<2>(coord)) == 3) return new Tile();

    return m_tilesByCoord[coord];
}

std::tuple<int,int,int> Board::directionToTuple(Direction dir){
    switch (dir) {
    case Direction::UpLeft:
        return std::tuple<int,int,int>(-1, 0, 1);
    case Direction::UpRight:
        return std::tuple<int,int,int>(0, -1, 1);
    case Direction::Right:
        return std::tuple<int,int,int>(1, -1, 0);
    case Direction::DownRight:
        return std::tuple<int,int,int>(1, 0, -1);
    case Direction::DownLeft:
        return std::tuple<int,int,int>(0, 1, -1);
    case Direction::Left:
        return std::tuple<int,int,int>(-1, 1, 0);
    }
}
